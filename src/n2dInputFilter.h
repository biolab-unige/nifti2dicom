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

// $Id$


#ifndef N2DINPUTFILTER_H
#define N2DINPUTFILTER_H

#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsCommandLineArgsStructs.h"


namespace n2d {

//BEGIN class n2d::InputFilter
/*!
 * \brief Filters the image
 *
 * Also handles:
 *
 * \li (0020,0020) Patient Orientation/
 */
class InputFilter
{
public:
    InputFilter(const FiltersArgs& filtersArgs, ImageType::ConstPointer inputImage, DictionaryType& dict) :
            m_FiltersArgs(filtersArgs),
            m_InputImage(inputImage),
            m_Dict(dict)
    {
    }

    ~InputFilter() {}

    bool Filter( void );

/*!
 * \brief Get filtered image.
 *
 * \return Internal image
 * \sa m_FilteredImage
 */
    inline DICOM3DImageType::ConstPointer getFilteredImage(void) const { return m_FilteredImage; }


private:
    const FiltersArgs& m_FiltersArgs;
    ImageType::ConstPointer m_InputImage;
    DICOM3DImageType::ConstPointer m_FilteredImage;
    DictionaryType& m_Dict;

};
//END class n2d::InputFilter

} // namespace n2d



#endif // N2DINPUTFILTER_H
