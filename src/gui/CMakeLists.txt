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
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/data/icons/qnifti2dicom.icns")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/data/icons/qnifti2dicom.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(qnifti2dicom_RCS "${CMAKE_SOURCE_DIR}/data/icons/qnifti2dicom.icns")
endif()

if(Nifti2Dicom_QT_VERSION EQUAL 5)
    qt5_wrap_cpp(qnifti2dicom_MOC_SRCS ${qnifti2dicom_MOC_HDR})
    qt5_add_resources(qnifti2dicom_QRC_SRCS ${qnifti2dicom_QRC_RCC})
else()
    qt4_wrap_cpp(qnifti2dicom_MOC_SRCS ${qnifti2dicom_MOC_HDR})
    qt4_add_resources(qnifti2dicom_QRC_SRCS ${qnifti2dicom_QRC_RCC})
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../core
                    ${QT_INCLUDE_DIR}
                    ${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR})

add_definitions(${QT_DEFINITIONS})

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

install(TARGETS qnifti2dicom
        COMPONENT Runtime
        RUNTIME DESTINATION bin
        BUNDLE DESTINATION .)

if(WIN32)
    set(APP "\${CMAKE_INSTALL_PREFIX}/bin/qnifti2dicom.exe")
elseif(APPLE)
    set(APP "\${CMAKE_INSTALL_PREFIX}/qnifti2dicom.app")
else()
    set(APP "\${CMAKE_INSTALL_PREFIX}/bin/qnifti2dicom")
endif()

set(DIRS ${QT_LIBRARY_DIR} ${QT_BINARY_DIR})

if(TARGET ITKCommon)
    get_property(ITK_IMPORTED_CONFIGURATIONS TARGET ITKCommon PROPERTY IMPORTED_CONFIGURATIONS)
    foreach(conf ${ITK_IMPORTED_CONFIGURATIONS})
        get_property(dir TARGET ITKCommon PROPERTY IMPORTED_LOCATION_${conf})
        get_filename_component(dir "${dir}" PATH)
        if(IS_DIRECTORY "${dir}")
            list(APPEND DIRS ${dir})
        endif()
    endforeach()
endif()

if(TARGET vtkCommonCore)
    get_property(VTK_IMPORTED_CONFIGURATIONS TARGET vtkCommonCore PROPERTY IMPORTED_CONFIGURATIONS)
    foreach(conf ${VTK_IMPORTED_CONFIGURATIONS})
        get_property(dir TARGET vtkCommonCore PROPERTY IMPORTED_LOCATION_${conf})
        get_filename_component(dir "${dir}" PATH)
        if(IS_DIRECTORY "${dir}")
            list(APPEND DIRS ${dir})
        endif()
    endforeach()
endif()

if(WIN32 OR APPLE)
    install(CODE "include(BundleUtilities)\n  fixup_bundle(\"${APP}\" \"\" \"${DIRS}\")"
            COMPONENT Runtime)
endif()
