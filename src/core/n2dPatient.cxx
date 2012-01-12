//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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


#include "n2dPatient.h"
#include "n2dDefsImage.h"

#ifdef DEBUG
 #include "n2dToolsMetaDataDictionary.h"
#endif

namespace n2d {

//BEGIN DICOM tags
const std::string patientnametag   ( "0010|0010" );
const std::string patientidtag     ( "0010|0020" );
const std::string patientdobtag    ( "0010|0030" );
const std::string patientsextag    ( "0010|0040" );
const std::string patientagetag    ( "0010|1010" );
const std::string patientweighttag ( "0010|1030" );
//END DICOM tags

//BEGIN Default values
const std::string defaultpatientmeta   ( "meta" );
const std::string defaultpatientid     ( "id" );
const std::string defaultpatientdob    ( "19900101" );
const std::string defaultpatientsex    ( "" );
const std::string defaultpatientage    ( "" );
const std::string defaultpatientweight ( "" );
const std::string defaultpatientname   ( "N2D_PATIENT" );

//END Default values

bool Patient::Update()
{
#ifdef DEBUG
    std::cout << "Patient - BEGIN:" << std::endl << std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
#endif

    CopyPatientTags(m_ImportedDict, m_Dict);

#ifdef DEBUG
    std::cout << "Patient - After CopyPatientTags:" << std::endl<< std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
#endif

//BEGIN (0010,0010) Patient's Name
    if (!m_PatientArgs.patientname.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, patientnametag, m_PatientArgs.patientname);
   	else
    {
        std::string patientName;
        itk::ExposeMetaData<std::string>(m_Dict, patientnametag, patientName);
        if(patientName.empty())
        	itk::EncapsulateMetaData<std::string>( m_Dict, patientnametag, defaultpatientname);
    }
//END (0010,0010) Patient's Name


//BEGIN (0010,0020) Patient ID
    if (!m_PatientArgs.patientid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, patientidtag, m_PatientArgs.patientid);
	else
    {
        std::string patientId;
        itk::ExposeMetaData<std::string>(m_Dict, patientidtag, patientId);
        if(patientId.empty())
        	itk::EncapsulateMetaData<std::string>( m_Dict, patientidtag, defaultpatientid);
    }
//END (0010,0020) Patient ID


//BEGIN (0010,0030) Patient's Birth Date
    if (!m_PatientArgs.patientdob.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, patientdobtag, m_PatientArgs.patientdob);
 	else
    {
        std::string patientDob;
        itk::ExposeMetaData<std::string>(m_Dict, patientdobtag, patientDob);
        if(patientDob.empty())
        	itk::EncapsulateMetaData<std::string>( m_Dict, patientdobtag, defaultpatientdob);
    }
//END (0010,0030) Patient's Birth Date


//BEGIN (0010,0040) Patient's Sex
    if (!m_PatientArgs.patientsex.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, patientsextag, m_PatientArgs.patientsex);
	else
    {
        std::string patientSex;
        itk::ExposeMetaData<std::string>(m_Dict, patientsextag, patientSex);
        if(patientSex.empty())
        	itk::EncapsulateMetaData<std::string>( m_Dict, patientsextag, defaultpatientsex);
    }
//END (0010,0040) Patient's Sex


//BEGIN (0010,1010) Patient's Age
    if (!m_PatientArgs.patientage.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, patientagetag, m_PatientArgs.patientage);
	else
    {
        std::string patientAge;
        itk::ExposeMetaData<std::string>(m_Dict, patientagetag, patientAge);
        if(patientAge.empty())
        	itk::EncapsulateMetaData<std::string>( m_Dict, patientagetag, defaultpatientage);
    }
//END (0010,1010) Patient's Age


//BEGIN (0010,1030) Patient's Weight
    if (!m_PatientArgs.patientweight.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, patientweighttag, m_PatientArgs.patientweight);
	else
    {
        std::string patientWeight;
        itk::ExposeMetaData<std::string>(m_Dict, patientweighttag, patientWeight);
        if(patientWeight.empty())
        	itk::EncapsulateMetaData<std::string>( m_Dict, patientweighttag, defaultpatientweight);
   } 
//END (0010,1030) Patient's Weight

#ifdef DEBUG
    std::cout << "Patient - END:" << std::endl<< std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;
#endif

    return true;
}




void Patient::CopyPatientTags (const DictionaryType &fromDict, DictionaryType &toDict)
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
            if (!tagkey.compare(0, 4, "0010") )
            {
//TODO (0010,21c0) Pregnancy Status causes a warning with dciovfy
                std::string tagvalue = entryvalue->GetMetaDataObjectValue();
#ifdef DEBUG
                std::cout << "Copying " << tagkey << "  " << tagvalue<< std::endl;
#endif //DEBUG
                itk::EncapsulateMetaData<std::string>(toDict, tagkey, tagvalue);
            }
        }
        ++itr;
    }
}

} // namespace n2d

