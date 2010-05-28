#  This file is part of Nifti2Dicom, is an open source converter from
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli
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
add_subdirectory(core)
if(N2D_BUILD_GUI)
#Search for installed Qt4 
  FIND_PACKAGE(Qt4 REQUIRED)
  IF(QT_FOUND)
    INCLUDE(${QT_USE_FILE})
  ELSE(QT_FOUND)
    MESSAGE(FATAL ERROR "  Qt4 not found")
  ENDIF(QT_FOUND)

  find_package(VTK REQUIRED)
   if(VTK_FOUND)
    include(${VTK_USE_FILE})
   endif(VTK_FOUND)
   add_subdirectory(gui)

endif(N2D_BUILD_GUI)
