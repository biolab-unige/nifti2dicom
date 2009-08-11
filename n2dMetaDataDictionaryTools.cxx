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


#include "n2dMetaDataDictionaryTools.h"

#include <string>
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>

#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"



namespace n2d {

void CopyDictionary (const DictionaryType &fromDict, DictionaryType &toDict)
{
    DictionaryType::ConstIterator itr = fromDict.Begin();
    DictionaryType::ConstIterator end = fromDict.End();

    while ( itr != end )
    {
        itk::MetaDataObjectBase::Pointer  entry = itr->second;

        MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>( entry.GetPointer() ) ;
        if ( entryvalue )
        {
            std::string tagkey   = itr->first;
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            itk::EncapsulateMetaData<std::string>(toDict, tagkey, tagvalue);
        }
        ++itr;
    }
}


void PrintDictionary (const itk::MetaDataDictionary &Dict)
{
    DictionaryType::ConstIterator itr = Dict.Begin();
    DictionaryType::ConstIterator end = Dict.End();

    while ( itr != end )
    {
        itk::MetaDataObjectBase::Pointer  entry = itr->second;

        MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>( entry.GetPointer() ) ;
        if ( entryvalue )
        {
            std::string tagkey   = itr->first;
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();

            std::cout << "(" << tagkey << ") " << tagvalue << std::endl;
        }
        ++itr;
    }
}



bool ReadDICOMTags(std::string file, itk::MetaDataDictionary &dict)
{
    DICOMReaderType::Pointer reader = DICOMReaderType::New();
    DICOMImageIOType::Pointer dicomIO = DICOMImageIOType::New();
    reader->SetImageIO( dicomIO );
    reader->SetFileName( file );

    try
    {
       // std::cout << "Reading DICOM Tags... " << std::flush;
        reader->Update();
       // std::cout << "DONE" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << message << std::endl;
        return false;
    }

    CopyDictionary( reader->GetMetaDataDictionary(), dict );
    return true;
}


} // namespace n2d