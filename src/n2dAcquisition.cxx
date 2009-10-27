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


#include "n2dAcquisition.h"
#include "n2dToolsDate.h"


#ifdef DEBUG
 #include "n2dToolsMetaDataDictionary.h"
#endif


namespace n2d {

//BEGIN DICOM tags
const std::string acquisitionnumbertag ( "0020|0012" );
const std::string acquisitiondatetag   ( "0008|0022" );
const std::string acquisitiontimetag   ( "0008|0032" );
//END DICOM tags


//BEGIN Default values
const std::string defaultacquisitionnumber ( "1" );
//END Default values


bool Acquisition::Update()
{

    #ifdef DEBUG
        std::cout << "Acquisition - BEGIN:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif



//BEGIN (0020,0012) Acquisition Number
//TODO chech if acquisition number in dicom header is "1" ?
    if (!m_AcquisitionArgs.acquisitionnumber.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, acquisitionnumbertag, m_AcquisitionArgs.acquisitionnumber);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, acquisitionnumbertag, defaultacquisitionnumber);
//END (0020,0012) Acquisition Number



//BEGIN (0008,0022) Acquisition Date
    if (!m_AcquisitionArgs.acquisitiondate.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, acquisitiondatetag, m_AcquisitionArgs.acquisitiondate);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, acquisitiondatetag, tools::Date::DateStr());
//END (0008,0022) Acquisition Date



//BEGIN (0008,0032) Acquisition Time
    if (!m_AcquisitionArgs.acquisitiontime.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, acquisitiontimetag, m_AcquisitionArgs.acquisitiontime);
    else
        itk::EncapsulateMetaData<std::string>( m_Dict, acquisitiontimetag, tools::Date::TimeStr());
//END (0008,0032) Acquisition Time


#ifdef DEBUG
    std::cout << "Acquisition - END:" << std::endl<< std::endl;
    tools::PrintDictionary(m_Dict);
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;
#endif

    return true;
}

} // namespace n2d
