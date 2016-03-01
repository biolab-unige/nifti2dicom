//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <ddomenichelli@drdanz.it>
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


#ifndef N2DOTHERDICOMTAGS_H
#define N2DOTHERDICOMTAGS_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"


namespace n2d {

//BEGIN class n2d::OtherDicomTags
/*!
 * \brief A class that handles DICOM tags related to Other DICOM Tags
 *
 * The following tags are handled by this class:
 *
 * \li (0008,0070) Manufacturer
 * \li (0008,1090) Manufacturer's Model Name
 * \li (0008,0080) Institution Name
 * \li (0008,0090) Referring Physician's Name
 */
class OtherDicomTags
{
public:
    OtherDicomTags(const OtherDicomTagsArgs& otherDicomTagsArgs, DictionaryType& dict) :
            m_OtherDicomTagsArgs(otherDicomTagsArgs),
            m_Dict(dict)
    {
    }

    ~OtherDicomTags() {}

    bool Update( void );

private:
    const OtherDicomTagsArgs& m_OtherDicomTagsArgs;
    DictionaryType& m_Dict;
};
//END class n2d::OtherDicomTags

} // namespace n2d

#endif // N2DOTHERDICOMTAGS_H
