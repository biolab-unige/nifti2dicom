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


#include "n2dVersion.h"
#include "Nifti2DicomConfig.h"
#include <sstream>


namespace n2d {


int GetMajorVersion()
{
    return Nifti2Dicom_MAJOR_VERSION;
}

int GetMinorVersion()
{
    return Nifti2Dicom_MINOR_VERSION;
}

int GetPatchVersion()
{
    return Nifti2Dicom_PATCH_VERSION;
}

std::string GetInternalVersion()
{
    return Nifti2Dicom_INTERNAL_VERSION;
}

std::string GetNameAndInternalVersion()
{
    std::ostringstream ost;
    ost << "nifti2dicom " << Nifti2Dicom_INTERNAL_VERSION;
    return ost.str();
}

std::string GetVersion()
{
    return Nifti2Dicom_VERSION;
}

std::string GetNameAndVersion()
{
    std::ostringstream ost;
    ost << "nifti2dicom " << Nifti2Dicom_VERSION;
    return ost.str();
}

std::string GetITKVersion()
{
    return ITK_VERSION;
}

std::string GetGDCMVersion()
{
    return GDCM_VERSION;
}

std::string GetTCLAPVersion()
{
    return TCLAP_VERSION;
}

std::string GetCopyright()
{
    return "Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli";
}


} //namespace n2d
