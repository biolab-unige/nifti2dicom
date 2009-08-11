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


#ifndef N2DOUTPUTEXPORTER_H
#define N2DOUTPUTEXPORTER_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"


namespace n2d {

//BEGIN class n2d::OutputExporter
/*!
 * \brief [...]
 *
 */
class OutputExporter
{
public:
    OutputExporter(const OutputArgs& outputArgs, DICOM3DImageType::ConstPointer image, DictionaryArrayType& dictionaryArray, DICOMImageIOType::Pointer dicomIO) :
            m_OutputArgs(outputArgs),
            m_Image(image),
            m_DictionaryArray(dictionaryArray),
            m_DicomIO(dicomIO)
    {
    }
    ~OutputExporter() {}

    bool Export(void);
private:
    const OutputArgs& m_OutputArgs;
    DICOM3DImageType::ConstPointer m_Image;
    DictionaryArrayType& m_DictionaryArray;
    DICOMImageIOType::Pointer m_DicomIO;
};
//END class n2d::OutputExporter


}

#endif // N2DOUTPUTEXPORTER_H
