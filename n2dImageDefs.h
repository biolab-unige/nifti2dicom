//    This file is part of Nifti2Dicom.
//    Copyright (C) 2008,2009 Daniele E. Domenichelli
//
//    Nifti2Dicom is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Nifti2Dicom is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

// $Id$

#ifndef N2DIMAGEDEFS_H
#define N2DIMAGEDEFS_H

#include <itkImage.h>


// Base definitions
const int Dimension = 3;
const int DICOMDimension = 2;

typedef unsigned int PixelType;
typedef signed short DICOMPixelType;

typedef itk::Image <PixelType, Dimension> ImageType;
typedef itk::Image<DICOMPixelType, Dimension> DICOM3DImageType;
typedef itk::Image<DICOMPixelType, DICOMDimension> DICOMImageType;

#endif // N2DIMAGEDEFS_H
