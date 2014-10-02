#  This file is part of Nifti2Dicom, is an open source converter from
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2010 Gabriele Arnulfo <gabriele.arnulfo@dist.unige.it>
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


set(qnifti2dicom_SOURCES wizard.cpp
                         init.cpp
                         customize.cpp
                         finalize.cpp
                         itkImageToVTKImageFilter.txx
                         qnifti2dicom.cpp
                         vtkKWImageIO.cxx
                         vtkKWImage.cxx)

set(qnifti2dicom_HEADERS ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h
                         wizard.h
                         init.h
                         customize.h
                         finalize.h)

set(qnifti2dicom_MOC_HDR wizard.h
                         init.h
                         customize.h
                         finalize.h)

set(qnifti2dicom_QRC_RCC resources.qrc)

if(MSVC)
    set(qnifti2dicom_RCS qnifti2dicom.rc)
endif()

qt4_wrap_cpp(qnifti2dicom_MOC_SRCS ${qnifti2dicom_MOC_HDR})
qt4_add_resources(qnifti2dicom_QRC_SRCS ${qnifti2dicom_QRC_RCC})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../core
                    ${QT_INCLUDE_DIR}
                    ${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR})

add_executable(qnifti2dicom WIN32 MACOSX_BUNDLE
                            ${qnifti2dicom_SOURCES}
                            ${qnifti2dicom_HEADERS}
                            ${qnifti2dicom_MOC_SRCS}
                            ${qnifti2dicom_UIS_HDR}
                            ${qnifti2dicom_QRC_SRCS}
                            ${qnifti2dicom_RCS})

# qnifti2dicom target
target_link_libraries(qnifti2dicom nifti2dicom_core
                                   ${ITK_LIBRARIES}
                                   ${VTK_LIBRARIES}
                                   ${QT_QTCORE_LIBRARY}
                                   ${QT_QTGUI_LIBRARY})

if(MSVC AND CMAKE_VERSION VERSION_LESS 2.8.11)
    target_link_libraries(qnifti2dicom ${QT_QTMAIN_LIBRARY})
endif()

if(VTK_VERSION VERSION_LESS 6.0.0)
    # In VTK 5 QVTK is not in ${VTK_LIBRARIES} variable
    target_link_libraries(qnifti2dicom QVTK)
endif()

install(TARGETS qnifti2dicom RUNTIME DESTINATION bin)
