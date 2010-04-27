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


#ifndef N2DDEFSIMAGE_H
#define N2DDEFSIMAGE_H

#include <itkImage.h>
#include <itkImageBase.h>
#include <itkImageIOBase.h>


namespace n2d {

// Base definitions
const int Dimension = 3;
const int DICOMDimension = 2;

typedef itk::ImageIOBase::IOComponentType    PixelType;	
typedef signed short DICOMPixelType;

typedef itk::ImageBase< Dimension> ImageType;
typedef itk::Image<DICOMPixelType, Dimension> DICOM3DImageType;
typedef itk::Image<DICOMPixelType, DICOMDimension> DICOMImageType;

} // namespace n2d

#endif // N2DDEFSIMAGE_H
