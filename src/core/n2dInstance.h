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


#ifndef N2DINSTANCE_H
#define N2DINSTANCE_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsImage.h"

namespace n2d {


//BEGIN class n2d::Instance
/*!
 * \brief A class that handles DICOM tags related to Series
 *
 * The following tags are handled by this class:
 *
 * \li (0020,0013) Instance Number
 * \li (0018|0050) Slice Thickness
 *
 * Also handles:
 *
 * \li ITK_NumberOfDimensions
 * \li ITK_Origin
 * \li ITK_Spacing
 * \li ITK_ZDirection
 *
 * These "ITK_" tags are used by itkGDCMImageIO to know 3D information of a 2D slice but maybe in the future they'll be handled by itkSeriesWriter.
 * \note In future this class could be unuseful because handled by ITK + GDCM2 or maybe ITK will set correctly ITK_ tags.
 * \note ITK_ZDirection is not supported by ITK at the moment, a patch was submitted to support it.
 */
class Instance
{
public:
    Instance(const InstanceArgs& instanceArgs, DICOM3DImageType::ConstPointer image, DictionaryType& dict, DictionaryArrayType& dictionaryArray) :
            m_InstanceArgs(instanceArgs),
            m_Image(image),
            m_Dict(dict),
            m_DictionaryArray(dictionaryArray)
    {
    }
    ~Instance() {}

    bool Update( void );

private:
    const InstanceArgs& m_InstanceArgs;
    DICOM3DImageType::ConstPointer m_Image;
    DictionaryType& m_Dict;
    DictionaryArrayType& m_DictionaryArray;


};
//END class n2d::Instance

} // namespace n2d

#endif // N2DINSTANCE_H
