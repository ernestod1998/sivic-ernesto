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


#ifndef SVK_IMAGE_READER_FACTORY_H
#define SVK_IMAGE_READER_FACTORY_H


#include <vtkObjectFactory.h>
#include <vtkImageReader2Factory.h>

#include <svkIdfVolumeReader.h>
#include <svkDcmMriVolumeReader.h>
#include <svkDcmMrsVolumeReader.h>
#include <svkDdfVolumeReader.h>
#include <svkFdfVolumeReader.h>
#include <svkVarianFidReader.h>
#include <svkSdbmVolumeReader.h>
#include <svkSiemensRdaReader.h>
#include <svkGEPFileReader.h>
#include <svkGESigna5XReader.h>
#include <svkGESignaLX2Reader.h>
#include <svkGEPostageStampReader.h>


namespace svk {


using namespace std;


/*! 
 *  Factory pattern Base class extended from vtkFramework:  
 */

class svkImageReaderFactory : public vtkImageReader2Factory 
{

    public:

        static svkImageReaderFactory* New();
        vtkTypeRevisionMacro(svkImageReaderFactory, vtkImageReader2Factory);

        //  Methods:
        svkImageReader2*    CreateImageReader2( const char* path );


    protected:

        svkImageReaderFactory();
        ~svkImageReaderFactory();


    private:

        //  Members:
        svkDcmMriVolumeReader*      dcmMriVolReader;
        svkDcmMrsVolumeReader*      dcmMrsVolReader;
        svkIdfVolumeReader*         idfVolReader;
        svkDdfVolumeReader*         ddfVolReader;
        svkFdfVolumeReader*         fdfVolReader;
        svkVarianFidReader*         fidVolReader;
        svkSdbmVolumeReader*        sdbmVolReader;
        svkSiemensRdaReader*        rdaVolReader;
        svkGEPFileReader*           gePFileReader;
        svkGESigna5XReader*         geSigna5XReader;
        svkGESignaLX2Reader*        geSignaLX2Reader;
        svkGEPostageStampReader*    gePostageStampReader;

};


}   //svk


#endif //SVK_IMAGE_READER_FACTORY_H

