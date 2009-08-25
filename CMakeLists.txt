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



if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebugInfo")
  message(STATUS "DEBUG ENABLED")
  add_definitions("-DDEBUG")
else()
  message(STATUS "DEBUG NOT ENABLED")
endif()



set(nifti2dicom_SOURCES src/nifti2dicom.cxx
                        src/n2dMetaDataDictionaryTools.cxx
                        src/n2dCommandLineParser.cxx
                        src/n2dInputImporter.cxx
                        src/n2dInputFilter.cxx
                        src/n2dVitalStatisticsImporter.cxx
                        src/n2dDicomTagsImporter.cxx
                        src/n2dUIDGenerator.cxx
                        src/n2dSlicer.cxx
                        src/n2dAccessionNumberValidator.cxx
                        src/n2dOutputExporter.cxx
)

set(nifti2dicom_NO_INSTALL_HEADERS Nifti2DicomConfig.h
                                   src/n2dDefsCommandLineArgsStructs.h
                                   src/n2dDefsImage.h
                                   src/n2dDefsMetadata.h
                                   src/n2dMetaDataDictionaryTools.h
                                   src/n2dCommandLineParser.h
                                   src/n2dInputImporter.h
                                   src/n2dInputFilter.h
                                   src/n2dVitalStatisticsImporter.h
                                   src/n2dDicomTagsImporter.h
                                   src/n2dUIDGenerator.h
                                   src/n2dSlicer.h
                                   src/n2dAccessionNumberValidator.h
                                   src/n2dOutputExporter.h
)


add_executable(nifti2dicom ${nifti2dicom_SOURCES})
target_link_libraries(nifti2dicom ITKIO ITKCommon)
install(TARGETS nifti2dicom RUNTIME DESTINATION bin)


# make dist target

set(CPACK_PACKAGE_VERSION_MAJOR ${Nifti2Dicom_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${Nifti2Dicom_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${Nifti2Dicom_PATCH_VERSION})
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES
  "~$;/build/;/.svn/;/.kdev4/;\\\\.kdev4;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

