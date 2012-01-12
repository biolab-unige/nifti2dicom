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


#ifndef N2DHEADERIMPORTER_H
#define N2DHEADERIMPORTER_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsMetadata.h"




namespace n2d {

//BEGIN class n2d::HeaderImporter
/*!
 * \brief Reads a DICOM file and imports its header
 *
 */

class HeaderImporter
{
public:
    HeaderImporter(const DicomHeaderArgs& dicomHeaderArgs, DictionaryType& dict) :
            m_DicomHeaderArgs(dicomHeaderArgs),
            m_Dictionary(dict)
    {
    }

    ~HeaderImporter() {}

    bool Import( void );


/*!
 * \brief Get internal DICOM tags dictionary
 *
 * \return Internal DICOM tags dictionary.
 */
    inline const n2d::DictionaryType& getMetaDataDictionary(void) const { return m_Dictionary; }


private:
    bool ReadDICOMTags( std::string file );

    const DicomHeaderArgs& m_DicomHeaderArgs; //!< Input Arguments.
    n2d::DictionaryType &m_Dictionary;
};
//END class n2d::HeaderImporter


} // namespace n2d


#endif // #ifndef N2DHEADERIMPORTER_H
