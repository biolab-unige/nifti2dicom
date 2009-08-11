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


#include "n2dSlicer.h"

#include "n2dMetaDataDictionaryTools.h"
#include "n2dDefsIO.h"


namespace n2d {


bool Slicer::Reslice(void)
{
std::cout << "Go go! " << __LINE__ << std::endl;
    std::cout << m_Image;
    unsigned int nbSlices = (m_Image->GetLargestPossibleRegion().GetSize())[2];
std::cout << "nbSlices = " << nbSlices << std::endl;
std::cout << "Go go! " << __LINE__ << std::endl;
    n2d::SeriesWriterType::DictionaryRawPointer dictionaryRaw[ nbSlices ];
std::cout << "Go go! " << __LINE__ << std::endl;

    n2d::PrintDictionary( m_Dict );

    for (unsigned int i=0; i<nbSlices; i++)
    {
        dictionaryRaw[i] = new SeriesWriterType::DictionaryType;
        n2d::CopyDictionary(m_Dict, *dictionaryRaw[i]);

std::cout << "Go go! " << __LINE__ << std::endl;


        // Image Position Patient: This is calculated by computing the
        // physical coordinate of the first pixel in each slice.
        ImageType::PointType position;
        ImageType::SpacingType spacing = m_Image->GetSpacing();
        ImageType::IndexType index;
std::cout << "Go go! " << __LINE__ << std::endl;

        index[0] = 0;
        index[1] = 0;
        index[2] = i;
        m_Image->TransformIndexToPhysicalPoint(index, position);

        itksys_ios::ostringstream value;

        // Image Number
        value.str("");
        value << i + 1;
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0020|0013",value.str());


        value.str("");
        value << position[0] << "\\" << position[1] << "\\" << position[2];
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0020|0032", value.str());

        // Slice Location: For now, we store the z component of the Image
        // Position Patient.
        value.str("");
        value << position[2];
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0020|1041", value.str());
std::cout << "Go go! " << __LINE__ << std::endl;

        // Slice Thickness: For now, we store the z spacing
        value.str("");
        value << spacing[2];
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0018|0050", value.str());

        // Spacing Between Slices
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0018|0088", value.str());

        m_DictionaryArray.push_back(dictionaryRaw[i]);
    }
std::cout << "Go go! " << __LINE__ << std::endl;
    return true;
}

} // namespace n2d