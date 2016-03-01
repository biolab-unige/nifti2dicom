//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <ddomenichelli@drdanz.it>
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


#include "n2dStudy.h"
#include "n2dToolsDate.h"
#include <gdcmUIDGenerator.h>


#ifdef DEBUG
 #include "n2dToolsMetaDataDictionary.h"
#endif


namespace n2d {

//BEGIN DICOM tags
const std::string studyinstanceuidtag ( "0020|000d" );
const std::string studyidtag          ( "0020|0010" );
const std::string studydescriptiontag ( "0008|1030" );
const std::string studydatetag        ( "0008|0020" );
const std::string studytimetag        ( "0008|0030" );
//END DICOM tags


//BEGIN Default values
const std::string defaultstudydescription ( "Postprocessing" );
//END Default values


// To keep the new series in the same study as the original we need
// to keep the same study UID. But we need new series and frame of
// reference UID's.
bool Study::Update()
{
    #ifdef DEBUG
        std::cout << "Study - BEGIN:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif

    if(!m_StudyArgs.donotuseoriginalstudy)
    {
        CopyStudyTags(m_ImportedDict, m_Dict);

    #ifdef DEBUG
        std::cout << "Study - After CopyStudyTags:" << std::endl<< std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif
    }



//BEGIN (0020,000d) Study Instance UID
    if (!m_StudyArgs.studyinstanceuid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, studyinstanceuidtag, m_StudyArgs.studyinstanceuid );
    else
    {
        std::string studyInstanceUID;
        itk::ExposeMetaData<std::string>(m_Dict, studyinstanceuidtag, studyInstanceUID);
        if(studyInstanceUID.empty() && m_StudyArgs.donotuseoriginalstudy)
        {
            gdcm::UIDGenerator uid;
            studyInstanceUID = uid.Generate();
        }
        itk::EncapsulateMetaData<std::string>( m_Dict, studyinstanceuidtag, studyInstanceUID );
    }
//END (0020,000d) Study Instance UID



//BEGIN (0020,0010) Study ID
    if (!m_StudyArgs.studyid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, studyidtag, m_StudyArgs.studyid);
    else
    {
        std::string studyID;
        itk::ExposeMetaData<std::string>( m_Dict, studyidtag, studyID );
        if(studyID.empty() && m_StudyArgs.donotuseoriginalstudy)
            studyID = tools::Date::DateTimeStr();
        itk::EncapsulateMetaData<std::string>( m_Dict, studyidtag, studyID );
    }
//END (0020,0010) Study ID



//BEGIN (0008,1030) Study Description
    if (!m_StudyArgs.studydescription.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, studydescriptiontag, m_StudyArgs.studydescription);
    else
    {
        std::string studyDescription;
        itk::ExposeMetaData<std::string>(m_Dict, studydescriptiontag, studyDescription );
        if(studyDescription.empty() && m_StudyArgs.donotuseoriginalstudy)
            studyDescription = defaultstudydescription;
        itk::EncapsulateMetaData<std::string>(m_Dict, studydescriptiontag, studyDescription);
    }
//END (0008,1030) Study Description



//BEGIN (0008,0020) Study Date
    if (!m_StudyArgs.studydate.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, studydatetag, m_StudyArgs.studydate);
    else
    {
        std::string studyDate;
        itk::ExposeMetaData<std::string>(m_Dict, studydatetag, studyDate);
        if(studyDate.empty() && m_StudyArgs.donotuseoriginalstudy)
            studyDate = tools::Date::TimeStr();
        itk::EncapsulateMetaData<std::string>(m_Dict, studydatetag, studyDate);
    }
//END (0008,0020) Study Date



//BEGIN (0008,0030) Study Time
    if (!m_StudyArgs.studytime.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, studytimetag, m_StudyArgs.studytime);
    else
    {
        std::string studyTime;
        itk::ExposeMetaData<std::string>(m_Dict, studytimetag, studyTime);
        if(studyTime.empty() && m_StudyArgs.donotuseoriginalstudy)
            studyTime = tools::Date::DateStr();
        itk::EncapsulateMetaData<std::string>(m_Dict, studytimetag, studyTime);
    }
//END (0008,0030) Study Time




#ifdef DEBUG
    std::cout << "Study - END:" << std::endl<< std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;
#endif

    return true;
}




void Study::CopyStudyTags (const DictionaryType &fromDict, DictionaryType &toDict)
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
            if ( !tagkey.compare( 0, 9, studyinstanceuidtag ) ||
                 !tagkey.compare( 0, 9, studyidtag          ) ||
                 !tagkey.compare( 0, 9, studydescriptiontag ) ||
                 !tagkey.compare( 0, 9, studydatetag        ) ||
                 !tagkey.compare( 0, 9, studytimetag        )
               )
            {
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

