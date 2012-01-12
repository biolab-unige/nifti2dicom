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


#include "n2dSeries.h"
#include "n2dToolsDate.h"
#include <gdcmUIDGenerator.h>

#ifdef DEBUG
 #include "n2dToolsMetaDataDictionary.h"
#endif

namespace n2d {

//BEGIN DICOM tags
const std::string seriesinstanceuidtag   ( "0020|000e" );
const std::string seriesnumbertag        ( "0020|0011" );
const std::string seriesdescriptiontag   ( "0008|103e" );
const std::string seriesdatetag          ( "0008|0021" );
const std::string seriestimetag          ( "0008|0031" );
const std::string frameofreferenceuidtag ( "0020|0052" );
//END DICOM tags


//BEGIN Default values
const std::string defaultseriesnumber      ( "1" ); // DateTimeStr()?
const std::string defaultseriesdescription ( "Postprocessing" );
//END Default values


bool Series::Update()
{

    #ifdef DEBUG
        std::cout << "Series - BEGIN:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif

    if(m_SeriesArgs.useoriginalseries)
    {
        CopySeriesTags(m_ImportedDict, m_Dict);

    #ifdef DEBUG
        std::cout << "Series - After CopySeriesTags:" << std::endl<< std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif
    }



//BEGIN (0020,000e) Series Instance UID
    if (!m_SeriesArgs.seriesinstanceuid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, seriesinstanceuidtag, m_SeriesArgs.seriesinstanceuid);
    else
    {
        std::string seriesInstanceUID;
        itk::ExposeMetaData<std::string>(m_Dict, seriesinstanceuidtag, seriesInstanceUID);
        if(seriesInstanceUID.empty() && !m_SeriesArgs.useoriginalseries)
        {
            gdcm::UIDGenerator uid;
            seriesInstanceUID = uid.Generate();
        }
        itk::EncapsulateMetaData<std::string>(m_Dict, seriesinstanceuidtag, seriesInstanceUID);
    }
//END (0020,000e) Series Instance UID



//BEGIN (0020,0011) Series Number
//TODO Use DateTime instead of "1"?
    if (!m_SeriesArgs.seriesnumber.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, seriesnumbertag, m_SeriesArgs.seriesnumber);
    else
    {
        std::string seriesNumber;
        itk::ExposeMetaData<std::string>(m_Dict, seriesnumbertag, seriesNumber);
        if(seriesNumber.empty() && !m_SeriesArgs.useoriginalseries)
            seriesNumber=defaultseriesnumber;
        itk::EncapsulateMetaData<std::string>(m_Dict, seriesnumbertag, seriesNumber);
    }
//END (0020,0011) Series Number




//BEGIN (0008,103e) Series Description
    if (!m_SeriesArgs.seriesdescription.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, seriesdescriptiontag, m_SeriesArgs.seriesdescription);
    else
    {
        std::string seriesDescription;
        itk::ExposeMetaData<std::string>(m_Dict, seriesdescriptiontag, seriesDescription);
        if(seriesDescription.empty() && !m_SeriesArgs.useoriginalseries)
            seriesDescription=defaultseriesdescription;
        itk::EncapsulateMetaData<std::string>(m_Dict, seriesdescriptiontag, seriesDescription);
    }
//END (0008,103e) Series Description



//BEGIN (0008,0021) Series Date
    if (!m_SeriesArgs.seriesdate.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, seriesdatetag, m_SeriesArgs.seriesdate);
    else
    {
        std::string seriesDate;
        itk::ExposeMetaData<std::string>(m_Dict, seriesdatetag, seriesDate);
        if(seriesDate.empty() && !m_SeriesArgs.useoriginalseries)
            seriesDate = tools::Date::DateStr();
        itk::EncapsulateMetaData<std::string>(m_Dict, seriesdatetag, seriesDate);
    }
//END (0008,0021) Series Date




//BEGIN (0008,0031) Series Time
    if (!m_SeriesArgs.seriestime.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, seriestimetag, m_SeriesArgs.seriestime);
    else
    {
        std::string seriesTime;
        itk::ExposeMetaData<std::string>(m_Dict, seriestimetag, seriesTime);
        if(seriesTime.empty() && !m_SeriesArgs.useoriginalseries)
            seriesTime = tools::Date::TimeStr();
        itk::EncapsulateMetaData<std::string>(m_Dict, seriestimetag, seriesTime);
    }
//END (0008,0031) Series Time




//BEGIN (0020,0052) Frame of Reference UID
    gdcm::UIDGenerator uid;
    std::string frameOfReferenceUID = uid.Generate();
    itk::EncapsulateMetaData<std::string>(m_Dict, frameofreferenceuidtag, frameOfReferenceUID);
//END (0020,0052) Frame of Reference UID





#ifdef DEBUG
    std::cout << "Series - END:" << std::endl<< std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;
#endif

    return true;
}



void Series::CopySeriesTags (const DictionaryType &fromDict, DictionaryType &toDict)
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
            if ( !tagkey.compare( 0, 9, seriesinstanceuidtag ) ||
                 !tagkey.compare( 0, 9, seriesnumbertag      ) ||
                 !tagkey.compare( 0, 9, seriesdescriptiontag ) ||
                 !tagkey.compare( 0, 9, seriesdatetag        ) ||
                 !tagkey.compare( 0, 9, seriestimetag        )
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
