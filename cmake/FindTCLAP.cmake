# Try to find the TCLAP library.
# Once done this will define the following variables:
#
# TCLAP_FOUND         - System has TCLAP
# TCLAP_INCLUDE_DIRS  - TCLAP include directory
# TCLAP_VERSION       - TCLAP version
#
# It uses pkg-config when it is available, otherwise it tries to locate
# TCLAP headers using the TCLAP_ROOT environment variable.

#  This file is part of Nifti2Dicom, is an open source converter from
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008, 2009, 2010, 2014 Daniele E. Domenichelli <ddomenichelli@drdanz.it>
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


include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
  if(TCLAP_FIND_VERSION)
    if(TCLAP_FIND_VERSION_EXACT)
      pkg_check_modules(_PC_TCLAP QUIET tclap=${TCLAP_FIND_VERSION})
    else()
      pkg_check_modules(_PC_TCLAP QUIET tclap>=${TCLAP_FIND_VERSION})
    endif()
  else()
    pkg_check_modules(_PC_TCLAP QUIET tclap)
  endif()

  if(_PC_TCLAP_FOUND)
    set(TCLAP_INCLUDE_DIRS ${_PC_TCLAP_INCLUDE_DIRS})
    set(TCLAP_VERSION ${_PC_TCLAP_VERSION})
    find_package_handle_standard_args(TCLAP DEFAULT_MSG TCLAP_VERSION)
    return()
  endif()
endif()

find_path(TCLAP_INCLUDEDIR "tclap/Arg.h"
          HINTS /usr
                /usr/local
                ENV TCLAP_ROOT
          PATH_SUFFIXES include)
mark_as_advanced(TCLAP_INCLUDEDIR)

set(TCLAP_INCLUDE_DIRS ${TCLAP_INCLUDEDIR})
set(TCLAP_VERSION "TCLAP_VERSION-NOTFOUND")
find_package_handle_standard_args(TCLAP DEFAULT_MSG TCLAP_INCLUDE_DIRS)
