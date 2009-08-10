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


#include "n2dUIDGenerator.h"
#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"
#include "n2dMetaDataDictionaryTools.h"

//#include <itksys/SystemTools.hxx>
#include <gdcm/src/gdcmFile.h>
#include <gdcm/src/gdcmUtil.h>

namespace n2d
{

bool UIDGenerator::Generate()
{
    if (!m_UIDArgs.studyistanceuid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0020|000d", m_UIDArgs.studyistanceuid);
    if (!m_UIDArgs.seriesistanceuid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0020|000e", m_UIDArgs.seriesistanceuid);
    if (!m_UIDArgs.seriesnumber.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0020|0011", m_UIDArgs.seriesnumber);
    if (!m_UIDArgs.acquisitionnumber.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0020|0012", m_UIDArgs.acquisitionnumber);




        // To keep the new series in the same study as the original we need
        // to keep the same study UID. But we need new series and frame of
        // reference UID's.





    std::string seriesUID = gdcm::Util::CreateUniqueUID( m_DicomIO->GetUIDPrefix());
    std::string frameOfReferenceUID = gdcm::Util::CreateUniqueUID( m_DicomIO->GetUIDPrefix());
    std::string studyUID;
    std::string sopClassUID;
    itk::ExposeMetaData<std::string>(m_Dict, "0020|000d", studyUID);
    itk::ExposeMetaData<std::string>(m_Dict, "0008|0016", sopClassUID);

    itk::EncapsulateMetaData<std::string>(m_Dict, "0020|000d", studyUID);
    itk::EncapsulateMetaData<std::string>(m_Dict, "0020|000e", seriesUID);
    itk::EncapsulateMetaData<std::string>(m_Dict, "0020|0052", frameOfReferenceUID);
    std::string sopInstanceUID = gdcm::Util::CreateUniqueUID(m_DicomIO->GetUIDPrefix());
    itk::EncapsulateMetaData<std::string>(m_Dict, "0008|0018", sopInstanceUID);
    itk::EncapsulateMetaData<std::string>(m_Dict, "0002|0003", sopInstanceUID);




    return true;
}

} // namespace n2d

