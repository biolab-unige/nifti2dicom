#  This file is part of Nifti2Dicom, is an open source converter from 
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008,2009 Daniele E. Domenichelli
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

# $Id$


cmake_minimum_required(VERSION 2.6.2)
project(Nifti2Dicom)


set(Nifti2Dicom_MAJOR_VERSION "0")
set(Nifti2Dicom_MINOR_VERSION "2")
set(Nifti2Dicom_PATCH_VERSION "80")

set(Nifti2Dicom_VERSION "${Nifti2Dicom_MAJOR_VERSION}.${Nifti2Dicom_MINOR_VERSION}.${Nifti2Dicom_PATCH_VERSION}")


find_package(ITK)
if(ITK_FOUND)
    include(${ITK_USE_FILE})
else(ITK_FOUND)
    message(FATAL_ERROR "Itk not found")
endif(ITK_FOUND)
#TODO Inserire controllo libreria TCLAP (command line parsing)


configure_file(${CMAKE_SOURCE_DIR}/Nifti2DicomConfig.h.cmake ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h)

include_directories(${CMAKE_BINARY_DIR})

#TODO Inserire -DDEBUG


set(nifti2dicom_SOURCES nifti2dicom.cxx
                        n2dMetaDataDictionaryTools.cxx
                        n2dCommandLineParser.cxx
                        n2dVitalStatisticsImporter.cxx
                        n2dDicomTagsImporter.cxx
                        n2dInputImporter.cxx
                        n2dInputFilter.cxx
                        n2dOutputExporter.cxx
                        n2dUIDGenerator.cxx
                        n2dAccessionNumberValidator.cxx
)

set(nifti2dicom_NO_INSTALL_HEADERS Nifti2DicomConfig.h
                                   n2dImageDefs.h
                                   n2dMetaDataDictionaryTools.h
                                   n2dCommandLineArgsStructs.h
                                   n2dCommandLineParser.h
                                   n2dVitalStatisticsImporter.h
                                   n2dDicomTagsImporter.h
                                   n2dInputImporter.h
                                   n2dInputFilter.h
                                   n2dOutputExporter.h
                                   n2dUIDGenerator.h
                                   n2dAccessionNumberValidator.h
)


add_executable(nifti2dicom ${nifti2dicom_SOURCES})
target_link_libraries(nifti2dicom ITKIO ITKCommon)
install(TARGETS nifti2dicom RUNTIME DESTINATION bin)

