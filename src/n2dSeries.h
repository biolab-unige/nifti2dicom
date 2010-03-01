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


#ifndef N2DSERIES_H
#define N2DSERIES_H


#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"

namespace n2d {

//BEGIN class n2d::Series
/*!
 * \brief A class that handles DICOM tags related to Series
 *
 * The following tags are handled by this class:
 *
 * \li (0020,000e) Series Instance UID
 * \li (0020,0011) Series Number
 * \li (0008,103e) Series Description
 * \li (0008,0021) Series Date
 * \li (0008,0031) Series Time
 *
 * Also handles:
 *
 * \li (0020,0052) Frame of Reference UID
 */
class Series
{
public:
    Series(const SeriesArgs& seriesArgs, const DictionaryType& importedDict, DictionaryType& dict) :
            m_SeriesArgs(seriesArgs),
            m_ImportedDict(importedDict),
            m_Dict(dict)
    {
    }

    ~Series() {}

    bool Update( void );

private:
    static void CopySeriesTags(const itk::MetaDataDictionary &fromDict, itk::MetaDataDictionary &toDict);

    const SeriesArgs& m_SeriesArgs;
    const DictionaryType& m_ImportedDict;
    DictionaryType& m_Dict;
};
//END class n2d::Study

} // namespace n2d







#endif // #ifndef N2DSERIES_H
