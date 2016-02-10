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


#include "n2dInstance.h"
#include "n2dToolsMetaDataDictionary.h"
#include <iomanip>
#include <vector>
#include <sstream>


namespace n2d {
//BEGIN DICOM tags
const std::string instancenumbertag ( "0020|0013" );
const std::string slicethicknesstag ( "0018|0050" );

//END DICOM tags


//BEGIN Default values
const std::string defaultmanufacturer ( "BIOLAB" );
//END Default values



bool Instance::Update(void)
{

    #ifdef DEBUG
        std::cout << "Instance - BEGIN:" << std::endl << std::endl;
        tools::PrintDictionary(m_Dict);
        std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
    #endif



//BEGIN Image info
    unsigned int nbSlices = (m_Image->GetLargestPossibleRegion().GetSize())[2];
    std::vector<n2d::SeriesWriterType::DictionaryRawPointer> dictionaryRaw(nbSlices);

    ImageType::PointType position;
    ImageType::SpacingType spacing = m_Image->GetSpacing();
    ImageType::DirectionType direction = m_Image->GetDirection();
    ImageType::IndexType index;
//END Image info


    std::ostringstream value;
    value << std::dec << std::setprecision(15);

    for (unsigned int i=0; i<nbSlices; i++)
    {
    //BEGIN MetaDataDictionary copy
        dictionaryRaw[i] = new SeriesWriterType::DictionaryType;
        n2d::tools::CopyDictionary(m_Dict, *dictionaryRaw[i]);
    //END MetaDataDictionary copy



    //BEGIN (0020,0013) Instance Number
        value.str("");
        value << i + 1;
        itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i], instancenumbertag, value.str());
    //END (0020,0013) Instance Number



    //BEGIN (0018,0050) Slice Thickness
    value.str("");
    value << spacing[2];
    itk::EncapsulateMetaData<std::string>(*dictionaryRaw[i], slicethicknesstag  , value.str());
    //END (0018,0050) Slice Thickness



//WARNING In futuro questa parte potrebbe essere inutile
//BEGIN ITK Tags

    //BEGIN ITK_NumberOfDimensions
        itk::EncapsulateMetaData<unsigned int>(*dictionaryRaw[i], "ITK_NumberOfDimensions", 3);
    //END ITK_NumberOfDimensions


    //BEGIN ITK_Origin
        index[0] = 0;
        index[1] = 0;
        index[2] = i;
        m_Image->TransformIndexToPhysicalPoint(index, position);
        typedef itk::Array< double > DoubleArrayType;
        DoubleArrayType originArray(3);
        for(int j = 0; j<3; j++)
            originArray[j]=position[j];
        itk::EncapsulateMetaData<DoubleArrayType>(*dictionaryRaw[i], "ITK_Origin", originArray);
    //END ITK_Origin


    //BEGIN ITK_Spacing
        DoubleArrayType spacingArray(3);
        for(int j = 0; j<3; j++)
            spacingArray[j]=spacing[j];
        itk::EncapsulateMetaData<DoubleArrayType>(*dictionaryRaw[i], "ITK_Spacing", spacingArray);
    //END ITK_Spacing


    //BEGIN ITK_ZDirection
        typedef itk::Matrix< double, 3, 3 > DoubleMatrixType;
        DoubleMatrixType directionMatrix;
        for(int j = 0; j<3; j++)
            for(int k = 0; k<3; k++)
                directionMatrix[k][j]=direction[j][k];
         itk::EncapsulateMetaData<DoubleMatrixType>(*dictionaryRaw[i], "ITK_ZDirection", directionMatrix);
    //END ITK_ZDirection

//END ITK Tags

        m_DictionaryArray.push_back(dictionaryRaw[i]);
    }

#ifdef DEBUG
    std::cout << "Instance - END:" << std::endl << std::endl;
    for (unsigned int i=0; i<nbSlices; i++)
    {
        std::cout << " * m_DictionaryArray[" << i << "]" << std::endl;
        tools::PrintDictionary(*m_DictionaryArray[i]);
        std::cout << "---" << std::endl;
    }
    std::cout << "-------------------------------------------------------------------" << std::endl;
#endif

    return true;
}


} // namespace n2d
