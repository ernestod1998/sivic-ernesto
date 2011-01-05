/*
 *  Copyright © 2009-2011 The Regents of the University of California.
 *  All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *  •   Redistributions of source code must retain the above copyright notice, 
 *      this list of conditions and the following disclaimer.
 *  •   Redistributions in binary form must reproduce the above copyright notice, 
 *      this list of conditions and the following disclaimer in the documentation 
 *      and/or other materials provided with the distribution.
 *  •   None of the names of any campus of the University of California, the name 
 *      "The Regents of the University of California," or the names of any of its 
 *      contributors may be used to endorse or promote products derived from this 
 *      software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 *  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 *  OF SUCH DAMAGE.
 */



/*
 *  $URL$
 *  $Rev$
 *  $Author$
 *  $Date$
 *
 *  Authors:
 *      Jason C. Crane, Ph.D.
 *      Beck Olson
 */



#include <svkCoilCombine.h>


using namespace svk;


vtkCxxRevisionMacro(svkCoilCombine, "$Rev$");
vtkStandardNewMacro(svkCoilCombine);


svkCoilCombine::svkCoilCombine()
{

#if VTK_DEBUG_ON
    this->DebugOn();
#endif

    vtkDebugMacro(<<this->GetClassName() << "::" << this->GetClassName() << "()");
}


svkCoilCombine::~svkCoilCombine()
{
}


/*! 
 *
 */
int svkCoilCombine::RequestInformation( vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector )
{
    return 1;
}


/*! 
 *
 */
int svkCoilCombine::RequestData( vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector )
{

    //  Iterate through spectral data from all cells.  Eventually for performance I should do this by visible
    svkImageData* data = this->GetImageDataInput(0); 

    int numFrequencyPoints = data->GetCellData()->GetNumberOfTuples();
    int numComponents = data->GetCellData()->GetNumberOfComponents();
    int numChannels  = data->GetDcmHeader()->GetNumberOfCoils();
    int numTimePts = data->GetDcmHeader()->GetNumberOfTimePoints();

    int numVoxels[3]; 
    data->GetNumberOfVoxels(numVoxels); 

    // For each voxel, add data from individual voxels:  
    float cmplxPt0[2];
    float cmplxPtN[2];
    for (int timePt = 0; timePt < numTimePts; timePt++) {
        for (int z = 0; z < numVoxels[2]; z++) {
            for (int y = 0; y < numVoxels[1]; y++) {
                for (int x = 0; x < numVoxels[0]; x++) {

                    vtkFloatArray* spectrum0 = static_cast<vtkFloatArray*>(
                        svkMrsImageData::SafeDownCast(data)->GetSpectrum( x, y, z, timePt, 0) );

                    for( int channel = 1; channel < numChannels; channel++ ) { 

                        vtkFloatArray* spectrumN = static_cast<vtkFloatArray*>(
                                            svkMrsImageData::SafeDownCast(data)->GetSpectrum( x, y, z, timePt, channel ) );
    
                        for (int i = 0; i < numFrequencyPoints; i++) {

                            spectrum0->GetTupleValue(i, cmplxPt0);
                            spectrumN->GetTupleValue(i, cmplxPtN);

                            cmplxPt0[0] += cmplxPtN[0]; 
                            cmplxPt0[1] += cmplxPtN[1]; 

                            spectrum0->SetTuple( i, cmplxPt0);
                        }

                    }
                }
            }
        }
    }

    //  Redimension data set:
    this->RedimensionData( data ); 

    //  Trigger observer update via modified event:
    this->GetInput()->Modified();
    this->GetInput()->Update();
    return 1; 
} 


/*!
 *  Remove coil dimension from data set:
 */
void svkCoilCombine::RedimensionData( svkImageData* data )
{
    int numCoils = data->GetDcmHeader()->GetNumberOfCoils();
    int numTimePts = data->GetDcmHeader()->GetNumberOfTimePoints();

    int numVoxels[3]; 
    data->GetNumberOfVoxels(numVoxels); 

    //  Remove all arrays with coil number > 0
    for (int coilNum = 1; coilNum < numCoils; coilNum++) {
        for (int timePt = 0; timePt < numTimePts; timePt++) {
            for (int z = 0; z < numVoxels[2]; z++) {
                for (int y = 0; y < numVoxels[1]; y++) {
                    for (int x = 0; x < numVoxels[0]; x++) {
                        char arrayName[30];
                        sprintf(arrayName, "%d %d %d %d %d", x, y, z, timePt, coilNum);
                        data->GetCellData()->RemoveArray( arrayName );
                    }     
                }
            }
        }
    }

    double origin[3]; 
    data->GetDcmHeader()->GetOrigin( origin, 0 ); 

    double voxelSpacing[3];  
    data->GetDcmHeader()->GetPixelSpacing( voxelSpacing );

    double dcos[3][3]; 
    data->GetDcmHeader()->GetDataDcos( dcos ); 

    data->GetDcmHeader()->InitPerFrameFunctionalGroupSequence( 
        origin,
        voxelSpacing,
        dcos, 
        data->GetDcmHeader()->GetNumberOfSlices(), 
        numTimePts, 
        1 
    );

    data->GetDcmHeader()->PrintDcmHeader( ); 
    
}


/*!
 *
 */
int svkCoilCombine::FillInputPortInformation( int vtkNotUsed(port), vtkInformation* info )
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "svkMrsImageData"); 
    return 1;
}


