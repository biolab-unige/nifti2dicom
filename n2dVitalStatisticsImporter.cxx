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


#include "n2dVitalStatisticsImporter.h"

#include "n2dDefsImage.h"
#include "n2dMetaDataDictionaryTools.h"



namespace n2d
{


bool VitalStatisticsImporter::Import()
{
    if (!m_VitalStatisticsArgs.dicomheaderfile.empty())
    {
        if (!n2d::ReadDICOMTags(m_VitalStatisticsArgs.dicomheaderfile, m_Dict))
            return false;
    }

    if (!m_VitalStatisticsArgs.patientname.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0010|0010", m_VitalStatisticsArgs.patientname);
    if (!m_VitalStatisticsArgs.patientid.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0010|0020", m_VitalStatisticsArgs.patientid);
    if (!m_VitalStatisticsArgs.patientdob.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0010|0030", m_VitalStatisticsArgs.patientdob);
    if (!m_VitalStatisticsArgs.patientsex.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0010|0040", m_VitalStatisticsArgs.patientsex);
    if (!m_VitalStatisticsArgs.patientage.empty())
        itk::EncapsulateMetaData<std::string>( m_Dict, "0010|1010", m_VitalStatisticsArgs.patientage);

    return true;
}


}

