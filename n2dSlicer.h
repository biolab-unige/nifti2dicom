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


#ifndef N2DSLICER_H
#define N2DSLICER_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"

namespace n2d {

class Slicer
{
public:
    Slicer(const ResliceArgs& resliceArgs, ImageType::ConstPointer image, const DictionaryType& dict, DictionaryArrayType& dictionaryArray) :
            m_ResliceArgs(resliceArgs),
            m_Image(image),
            m_Dict(dict),
            m_DictionaryArray(dictionaryArray)
    {
    }
    ~Slicer() {}

    bool Reslice(void);
    
private:
    const ResliceArgs& m_ResliceArgs;
    ImageType::ConstPointer m_Image;
    const DictionaryType& m_Dict;
    DictionaryArrayType& m_DictionaryArray;


}; // class Slicer

} // namespace n2d

#endif // N2DSLICER_H