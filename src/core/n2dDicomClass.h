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


#ifndef N2DDICOMCLASS_H
#define N2DDICOMCLASS_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"


namespace n2d {

//BEGIN class n2d::DicomClass
/*!
 * \brief A class that handles DICOM tags related to Dicom Class, Modality, etc.
 *
 * The following tags are handled by this class:
 *
 * \li (0002,0010) Transfer Syntax UID
 * \li (0008,0016) SOP Class UID
 * \li (0008,0060) Modality
 * \li (0008,0008) Image Type
 * \li (0018,1030) Protocol Name
 */
class DicomClass
{
public:
    DicomClass(const DicomClassArgs& dicomClassArgs, const DictionaryType& importedDict, DictionaryType& dict) :
            m_DicomClassArgs(dicomClassArgs),
            m_ImportedDict(importedDict),
            m_Dict(dict)
    {
    }

    ~DicomClass() {}

    bool Update( void );

private:
    static void CopyDicomClassTags(const itk::MetaDataDictionary &fromDict, itk::MetaDataDictionary &toDict);

    const DicomClassArgs& m_DicomClassArgs;
    const DictionaryType& m_ImportedDict;
    DictionaryType& m_Dict;
};
//END class n2d::DicomClass

} // namespace n2d

#endif // N2DDICOMCLASS_H