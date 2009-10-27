//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008,2009 Daniele E. Domenichelli
//
//  Nifti2Dicom is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Nifti2Dicom is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Nifti2Dicom.  If not, see <http://www.gnu.org/licenses/>.

// $Id$


#include "n2dHeaderImporter.h"
#include "n2dDefsIO.h"
#include "n2dToolsMetaDataDictionary.h"

namespace n2d {

bool HeaderImporter::Import( void )
{

    if (!m_DicomHeaderArgs.dicomheaderfile.empty())
    {
        if (!ReadDICOMTags(m_DicomHeaderArgs.dicomheaderfile))
            return false;
    }
    return true;
}



bool HeaderImporter::ReadDICOMTags(std::string file)
{
    DICOMReaderType::Pointer reader = DICOMReaderType::New();
    DICOMImageIOType::Pointer dicomIO = DICOMImageIOType::New();
    reader->SetImageIO( dicomIO );
    reader->SetFileName( file );

    try
    {
        std::cout << " * \033[1;34mReading DICOM Header\033[0m... " << std::endl;
        reader->Update();
        std::cout << " * \033[1;34mReading DICOM Header\033[0m... \033[1;32mDONE\033[0m" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << " * \033[1;34mReading DICOM Header\033[0m... \033[1;31mFAIL\033[0m" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << message << std::endl;
        return false;
    }

    tools::CopyDictionary( reader->GetMetaDataDictionary(), m_Dictionary );
    return true;
}


} // namespace n2d
