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
#include <iomanip>

namespace n2d {


bool Slicer::Reslice(void)
{
    unsigned int nbSlices = (m_Image->GetLargestPossibleRegion().GetSize())[2];
    n2d::SeriesWriterType::DictionaryRawPointer dictionaryRaw[ nbSlices ];


    ImageType::PointType position;
    ImageType::SpacingType spacing = m_Image->GetSpacing();
    ImageType::DirectionType direction = m_Image->GetDirection();
    ImageType::IndexType index;

    ImageType::PointType start_position;
    index[0] = 0;
    index[1] = 0;
    index[2] = 0;
    m_Image->TransformIndexToPhysicalPoint(index, start_position);

    itksys_ios::ostringstream value;
    value << std::dec << std::setprecision(15);

    value.str("");
    value << spacing[0] << "\\" << spacing[1];
    itk::EncapsulateMetaData<std::string>(m_Dict,"0028|0030", value.str().substr(0,16));

    for (unsigned int i=0; i<nbSlices; i++)
    {
        dictionaryRaw[i] = new SeriesWriterType::DictionaryType;
        n2d::CopyDictionary(m_Dict, *dictionaryRaw[i]);

        // Image Position Patient: This is calculated by computing the
        // physical coordinate of the first pixel in each slice.
        index[0] = 0;
        index[1] = 0;
        index[2] = i;
        m_Image->TransformIndexToPhysicalPoint(index, position);


        // Image Number
        value.str("");
        value << i + 1;
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0020|0013",value.str());


/* GDCM2 lo setta automaticamente corretto
        value.str("");
        value << position[0] << "\\" << position[1] << "\\" << position[2];
//        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0020|0032", value.str());
*/


        // Slice Location: For now, we store the z component of the Image
        // Position Patient.
        value.str("");
        value << position[2] - start_position[2];
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0020|1041", value.str().substr(0,16));

        // Slice Thickness: For now, we store the z spacing
        value.str("");
        value << sqrt( pow( spacing[0],2) + pow(spacing[1],2) + pow(spacing[2],2) );
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0018|0050", value.str().substr(0,16));

        // Spacing Between Slices
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i],"0018|0088", value.str().substr(0,16));


        itk::EncapsulateMetaData<unsigned int>(*dictionaryRaw[i], "ITK_NumberOfDimensions", 3);

        typedef itk::Array< double > DoubleArrayType;
        DoubleArrayType originArray(3);
        for(int j = 0; j<3; j++)
            originArray[j]=position[j];
        itk::EncapsulateMetaData<DoubleArrayType>(*dictionaryRaw[i], "ITK_Origin", originArray);

        DoubleArrayType spacingArray(3);
        for(int j = 0; j<3; j++)
            spacingArray[j]=spacing[j];
         itk::EncapsulateMetaData<DoubleArrayType>(*dictionaryRaw[i], "ITK_Spacing", spacingArray);

        typedef itk::Matrix< double, 3, 3 > DoubleMatrixType;
        DoubleMatrixType directionMatrix;
        for(int j = 0; j<3; j++)
            for(int k = 0; k<3; k++)
                directionMatrix[j][k]=direction[j][k];
         itk::EncapsulateMetaData<DoubleMatrixType>(*dictionaryRaw[i], "ITK_Direction", directionMatrix);


        m_DictionaryArray.push_back(dictionaryRaw[i]);

    }
//#define DEBUG
#ifdef DEBUG
    for (unsigned int i=0; i<nbSlices; i++)
    {
        std::cout << "m_DictionaryArray[" << i << "]" << std::endl;
        PrintDictionary(*m_DictionaryArray[i]);
        std::cout << "-------------------------------------------------------------------" << std::endl;
    }
#endif
    return true;
}

} // namespace n2d
