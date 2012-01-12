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


#ifndef N2DACQUISITION_H
#define N2DACQUISITION_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"


namespace n2d {

//BEGIN class n2d::Acquisition
/*!
 * \brief A class that handles DICOM tags related to Acquisition
 *
 * The following tags are handled by this class:
 *
 * \li (0020,0012) Acquisition Number
 * \li (0008,0022) Acquisition Date
 * \li (0008,0032) Acquisition Time
 */
class Acquisition
{
public:
    Acquisition(const AcquisitionArgs& acquisitionArgs, DictionaryType& dict) :
            m_AcquisitionArgs(acquisitionArgs),
            m_Dict(dict)
    {
    }

    ~Acquisition() {}

    bool Update( void );

private:
    const AcquisitionArgs& m_AcquisitionArgs;
    DictionaryType& m_Dict;
};
//END class n2d::Acquisition

} // namespace n2d

#endif // N2DACQUISITION_H