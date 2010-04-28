#  This file is part of Nifti2Dicom, is an open source converter from
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008, 2009 Gabriele Arnulfo
#
#  Nifti2Dicom is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Nifti2Dicom is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Nifti2Dicom.  If not, see <http://www.gnu.org/licenses/>.

# $Id: CMakeLists.txt 251 2010-03-01 15:11:05Z gabri $


set(nifti2dicom_gui_SOURCES wizard.cpp
			    init.cpp)

set(nifti2dicom_gui_NO_INSTALL_HEADERS ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h\
					 wizard.h
					 init.h)


# nifti2dicom target
add_executable(noodle ${nifti2dicom_gui_SOURCES} ${QT_LIBRARIES})
