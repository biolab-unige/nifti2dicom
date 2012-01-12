//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli
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


#include "n2dOtherDicomTags.h"

#ifdef DEBUG
 #include "n2dToolsMetaDataDictionary.h"
#endif

namespace n2d {


//BEGIN DICOM tags
const std::string manufacturertag            ( "0008|0070" );
const std::string manufacturersmodelnametag  ( "0008|1090" );
const std::string institutionnametag         ( "0008|0080" );
const std::string referringphysiciansnametag ( "0008|0090" );
//END DICOM tags


//BEGIN Default values
const std::string defaultmanufacturer           ( "BIOLAB" );
const std::string defaultmanufacturersmodelname ( "nifti2dicom" );
const std::string defaultinstitutionname        ( "OSP. NIGUARDA"  );
//END Default values



bool OtherDicomTags::Update()
{
    #ifdef DEBUG
        std::cout << "Other DICOM Tags - BEGIN:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif

//BEGIN (0008,0070) Manufacturer
    if (!m_OtherDicomTagsArgs.manufacturer.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, manufacturertag, m_OtherDicomTagsArgs.manufacturer);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, manufacturertag, defaultmanufacturer);
//END (0008,0070) Manufacturer



//BEGIN (0008,1090) Manufacturer's Model Name
    if (!m_OtherDicomTagsArgs.manufacturersmodelname.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, manufacturersmodelnametag, m_OtherDicomTagsArgs.manufacturersmodelname);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, manufacturersmodelnametag, defaultmanufacturersmodelname);
//END (0008,1090) Manufacturer's Model Name



//BEGIN (0008,0080) Institution Name
    if (!m_OtherDicomTagsArgs.institutionname.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, institutionnametag, m_OtherDicomTagsArgs.institutionname);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, institutionnametag, defaultinstitutionname);
//END (0008,0080) Institution Name




//BEGIN (0008,0090) Referring Physician's Name
        if (!m_OtherDicomTagsArgs.referringphysiciansname.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, referringphysiciansnametag, m_OtherDicomTagsArgs.referringphysiciansname);
//END (0008,0090) Referring Physician's Name


    #ifdef DEBUG
        std::cout << "Other DICOM Tags - END:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif

    return true;
}

} // namespace n2d