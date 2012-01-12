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


#ifndef N2DPATIENT_H
#define N2DPATIENT_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"


namespace n2d {

//BEGIN class n2d::Patient
/*!
 * \brief A class that handles DICOM tags related to Patient
 *
 * The following tags are handled by this class:
 *
 * \li (0010,0010) Patient's Name
 * \li (0010,0020) Patient ID
 * \li (0010,0030) Patient's Birth Date
 * \li (0010,0040) Patient's Sex
 * \li (0010,1010) Patient's Age
 * \li (0010,1030) Patient's Weight
 * \li (0010|****)
 */
class Patient
{
public:
    Patient(const PatientArgs patientArgs, const DictionaryType& importedDict, DictionaryType& dict) :
            m_PatientArgs(patientArgs),
            m_ImportedDict(importedDict),
            m_Dict(dict)
    {
    }
    ~Patient() {}

    bool Update( void );

private:
    static void CopyPatientTags(const itk::MetaDataDictionary &fromDict, itk::MetaDataDictionary &toDict);

    const PatientArgs m_PatientArgs;
    const DictionaryType& m_ImportedDict;
    DictionaryType& m_Dict;
};
//END class n2d::Patient

}

#endif // N2DPATIENT_H
