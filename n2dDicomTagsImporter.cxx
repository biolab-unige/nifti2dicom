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


#include "n2dDicomTagsImporter.h"
#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"

namespace n2d
{

bool DicomTagsImporter::Import()
{
        if (!m_DicomTagsArgs.studydate.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|0020", m_DicomTagsArgs.studydate);
        if (!m_DicomTagsArgs.seriesdate.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|0021", m_DicomTagsArgs.seriesdate);
        if (!m_DicomTagsArgs.modality.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|0060", m_DicomTagsArgs.modality);
        if (!m_DicomTagsArgs.manufacturer.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|0070", m_DicomTagsArgs.manufacturer);
        if (!m_DicomTagsArgs.istitutionname.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|0080", m_DicomTagsArgs.istitutionname);
        if (!m_DicomTagsArgs.referphysicianname.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|0090", m_DicomTagsArgs.referphysicianname);
        if (!m_DicomTagsArgs.studydescription.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|1030", m_DicomTagsArgs.studydescription);
        if (!m_DicomTagsArgs.seriesdescription.empty())
            itk::EncapsulateMetaData<std::string>( m_Dict, "0008|103e", m_DicomTagsArgs.seriesdescription);

        //TODO settare correttamente BITS ALLOCATED (0028,0100)/ BITS STORED (0028,0101) / HIGH BIT (0028,0102)
        // (pare impossibile con itk+gdcm)
//        // Bits Allocated
//          itk::EncapsulateMetaData<std::string>( inputDict, "0028|0100", "16");
//        // Bits Stored
//          itk::EncapsulateMetaData<std::string>( inputDict, "0028|0101", "12");
//        // High Bit
//          itk::EncapsulateMetaData<std::string>( inputDict, "0028|0102", "11");

    //TODO Controllare che non siano settati Rescale - Slope - Windows
    //     o che siano settati, ma correttamente
    // Al momento sembra che non si possa http://www.itk.org/Bug/view.php?id=3223
    // (si può sempre non copiarli nel CopyDictionary)

#ifdef DEBUG
    n2d::PrintDictionary( m_Dict );
#endif // DEBUG

    return true;
}

} // namespace n2d