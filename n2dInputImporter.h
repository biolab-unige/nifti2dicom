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


#ifndef N2DINPUTIMPORTER_H
#define N2DINPUTIMPORTER_H

#include "n2dCommandLineArgsStructs.h"
#include <itkMetaDataDictionary.h>

namespace n2d
{

class InputImporter
{
public:
    InputImporter(InputArgs inputArgs, itk::MetaDataDictionary& dict);
    ~InputImporter();

    bool Import(void);

private:
    bool ReadDICOMTags(std::string file, itk::MetaDataDictionary &dict);

    InputArgs m_InputArgs;
    itk::MetaDataDictionary& m_Dict;
};

}

#endif // N2DVITALSTATISTICSIMPORTER_H
