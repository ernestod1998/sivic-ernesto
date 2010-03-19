/*
 *  Copyright © 2009-2010 The Regents of the University of California.
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


#ifndef SVK_SECONDARY_CAPTURE_FORMATTER_H
#define SVK_SECONDARY_CAPTURE_FORMATTER_H


#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkGraphicsFactory.h>
#include <vtkImagingFactory.h>
#include <vtkImageWriter.h>
#include <vtkRenderLargeImage.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageConstantPad.h>

#include <svkDataModel.h>
#include <svkImageWriter.h>
#include <svkImageData.h>
#include <svkPlotGridViewController.h>
#include <svkOverlayViewController.h>
#include <svkDetailedPlotViewController.h>
#include <svkMultiWindowToImageFilter.h>


class vtkSivicController;

namespace svk {

/*! 
 *
 */
class svkSecondaryCaptureFormatter : public vtkObject
{

    public:

        // vtk type revision macro
        vtkTypeRevisionMacro( svkSecondaryCaptureFormatter,vtkObject );
   
        static svkSecondaryCaptureFormatter* New();  

        svkSecondaryCaptureFormatter();
        ~svkSecondaryCaptureFormatter();

        enum {
            ALL_SLICES = 0,
            CURRENT_SLICE
        } OutputOption;

        enum {
            LIGHT_ON_DARK = 0,
            DARK_ON_LIGHT 
        } ColorSchema;
    
        enum {
            SPECTRA_CAPTURE = 0,
            IMAGE_CAPTURE
        } CaptureType;

        typedef enum {
            LANDSCAPE = 0,
            PORTRAIT
        } CaptureAspect;

    
        void SetSivicController( vtkSivicController* sivicController );
        void SetPlotController( svkPlotGridViewController* plotController );
        void SetOverlayController( svkOverlayViewController* overlayController );
        void SetDetailedPlotController( svkDetailedPlotViewController* detailedplotController );
        void SetModel( svkDataModel* );    
        void SetAspect( CaptureAspect aspect);
        void SetOrientation( svkDcmHeader::Orientation orientation );
        void WriteSpectraCapture( vtkImageWriter* writer, string fileNameString, int outputOption, svkImageData* outputImage, bool print );
        void WriteCombinedCapture( vtkImageWriter* writer, string fileNameString, int outputOption, svkImageData* outputImage, bool print );
        void WriteImageCapture( vtkImageWriter* writer, string fileNameString, int outputOption, svkImageData* outputImage, bool print, int instanceNumber = 0 );
        void PopulateInfoText( vtkTextActor* specText1, vtkTextActor* specText2, vtkTextActor* imageText );

    protected:

        vtkSivicController*            sivicController;
        svkPlotGridViewController*     plotController;
        svkDetailedPlotViewController* detailedPlotController;
        svkOverlayViewController*      overlayController;
        svkDataModel*                  model;
        CaptureAspect                  aspect;

    private:
    
        svkDcmHeader::Orientation      orientation;
        vtkRenderWindow*               captureWindow;
        int imageSize[2];
        int spectraHalfSize[2];

};


}   //svk


#endif //SVK_SECONDARY_CAPTURE_FORMATTER_H