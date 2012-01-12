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


#include "n2dDicomClass.h"
#include "n2dVersion.h"

#ifdef DEBUG
 #include "n2dToolsMetaDataDictionary.h"
#endif


namespace n2d {

//BEGIN DICOM tags
const std::string transfersyntaxuidtag ( "0002|0010 ");
const std::string sopclassuidtag       ( "0008|0016" );
const std::string modalitytag          ( "0008|0060" );
const std::string imagetypetag         ( "0008|0008" );
const std::string protocolnametag      ( "0018|1030" );
//END DICOM tags


//BEGIN Default values
const std::string defaulttransfersyntaxuid ( "1.2.840.10008.1.2" );
const std::string defaultsopclassuid       ( "1.2.840.10008.5.1.4.1.1.4" );
const std::string defaultmodality          ( "MR" );
const std::string defaultimagetype         ( "DERIVED\\SECONDARY" );
const std::string defaultprotocolname      ( GetNameAndInternalVersion() );
//END Default values



bool DicomClass::Update()
{
    #ifdef DEBUG
        std::cout << "DICOM Class - BEGIN:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif

    CopyDicomClassTags(m_ImportedDict, m_Dict);

    #ifdef DEBUG
        std::cout << "DICOM Class - After CopyDicomClassTags:" << std::endl<< std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif



/*
//BEGIN (0002,0010) Transfer Syntax UID
    if (!m_DicomClassArgs.transfersyntaxuid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, transfersyntaxuidtag, m_DicomClassArgs.transfersyntaxuid);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, transfersyntaxuidtag, defaulttransfersyntaxuid);
//END  (0002,0010) Transfer Syntax UID
*/


//BEGIN (0008,0016) SOP Class UID
    if (!m_DicomClassArgs.sopclassuid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, sopclassuidtag, m_DicomClassArgs.sopclassuid);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, sopclassuidtag, defaultsopclassuid);
//END (0008,0016) SOP Class UID



//BEGIN (0008,0060) Modality
    if (!m_DicomClassArgs.modality.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, modalitytag, m_DicomClassArgs.modality);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, modalitytag, defaultmodality);
//END (0008,0060) Modality



//BEGIN (0008,0008) Image Type
    if (!m_DicomClassArgs.imagetype.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, imagetypetag, m_DicomClassArgs.imagetype );
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, imagetypetag, defaultimagetype);
//END (0008,0008) Image Type



//BEGIN (0018,1030) Protocol Name
    if (!m_DicomClassArgs.protocolname.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, protocolnametag, m_DicomClassArgs.protocolname );
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, protocolnametag, defaultprotocolname);
//END (0018,1030) Protocol Name



#ifdef DEBUG
    std::cout << "DICOM Class - END:" << std::endl<< std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;
#endif

    return true;
}




void DicomClass::CopyDicomClassTags (const DictionaryType &fromDict, DictionaryType &toDict)
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
            if ( !tagkey.compare( 0, 9, modalitytag    ) ||
                 !tagkey.compare( 0, 9, sopclassuidtag )
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
