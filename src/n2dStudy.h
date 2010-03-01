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


#ifndef N2DSTUDY_H
#define N2DSTUDY_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"


namespace n2d {

//BEGIN class n2d::Study
/*!
 * \brief A class that handles DICOM tags related to Study
 *
 * The following tags are handled by this class:
 *
 * \li (0020|000d) Study Instance UID
 * \li (0020|0010) Study ID
 * \li (0008|1030) Study Description
 * \li (0008|0020) Study Date
 * \li (0008|0030) Study Time
 */
class Study
{
public:
    Study(const StudyArgs& studyArgs, const DictionaryType& importedDict, DictionaryType& dict) :
            m_StudyArgs(studyArgs),
            m_ImportedDict(importedDict),
            m_Dict(dict)
    {
    }

    ~Study() {}

    bool Update( void );

private:
    static void CopyStudyTags(const itk::MetaDataDictionary &fromDict, itk::MetaDataDictionary &toDict);

    const StudyArgs& m_StudyArgs;
    const DictionaryType& m_ImportedDict;
    DictionaryType& m_Dict;
};
//END class n2d::Study

} // namespace n2d

#endif // N2DSTUDY_H