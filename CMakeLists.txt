#  This file is part of Nifti2Dicom, is an open source converter from 
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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


cmake_minimum_required(VERSION 2.6.4)

if(NOT CMAKE_VERSION VERSION_LESS 2.8.11)
    cmake_policy(SET CMP0020 NEW) # Remove when CMake >=2.8.11
endif()

project(Nifti2Dicom)

set(Nifti2Dicom_MAJOR_VERSION "0")
set(Nifti2Dicom_MINOR_VERSION "4")
set(Nifti2Dicom_PATCH_VERSION "9")
set(Nifti2Dicom_VERSION "0.4.9")
set(Nifti2Dicom_INTERNAL_VERSION "${Nifti2Dicom_MAJOR_VERSION}.${Nifti2Dicom_MINOR_VERSION}.${Nifti2Dicom_PATCH_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#Check TCLAP Library [Templatized C++ Command Line Parser] (1.2.0)
find_package(TCLAP 1.2.0 REQUIRED)
include_directories(${TCLAP_INCLUDE_DIRS})

# Check ITK Library [Insight ToolKit] (3.17.0)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#Check Qt4 Library [Qt4] (4.4.0)
set(QT_USE_IMPORTED_TARGETS TRUE)
find_package(Qt4 4.4.0 REQUIRED)

#Check VTK Library [Visualization ToolKit]
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#TODO Check QVTK

configure_file(${CMAKE_SOURCE_DIR}/Nifti2DicomConfig.h.cmake ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h)

include_directories(${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DDEBUG")

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS
                  -D_SCL_SECURE_NO_WARNINGS)
endif()

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(data)
