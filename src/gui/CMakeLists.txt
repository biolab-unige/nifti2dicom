#  This file is part of Nifti2Dicom, is an open source converter from
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008, 2009, 2010 Gabriele Arnulfo
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
			    init.cpp
			    customize.cpp
			    finalize.cpp
				itkImageToVTKImageFilter.txx
				qnifti2dicom.cpp
				)

set(nifti2dicom_SOURCES ../core/n2dVersion.cxx
                        ../core/n2dToolsMetaDataDictionary.cxx
                        ../core/n2dToolsDate.cxx
                        ../core/n2dCommandLineParser.cxx
                        ../core/n2dAccessionNumberValidator.cxx
                        ../core/n2dHeaderImporter.cxx
                        ../core/n2dDicomClass.cxx
                        ../core/n2dOtherDicomTags.cxx
                        ../core/n2dPatient.cxx
                        ../core/n2dStudy.cxx
                        ../core/n2dSeries.cxx
                        ../core/n2dAcquisition.cxx
                        ../core/n2dInputImporter.cxx
                        ../core/n2dInputFilter.cxx
                        ../core/n2dInstance.cxx
                        ../core/n2dOutputExporter.cxx
)

set(nifti2dicom_NO_INSTALL_HEADERS ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h
                                   ../core/n2dVersion.h
                                   ../core/n2dDefsCommandLineArgsStructs.h
                                   ../core/n2dDefsImage.h
                                   ../core/n2dDefsMetadata.h
                                   ../core/n2dToolsMetaDataDictionary.h
                                   ../core/n2dToolsDate.h
                                   ../core/n2dCommandLineParser.h
                                   ../core/n2dAccessionNumberValidator.h
                                   ../core/n2dHeaderImporter.h
                                   ../core/n2dDicomClass.h
                                   ../core/n2dOtherDicomTags.h
                                   ../core/n2dPatient.h
                                   ../core/n2dStudy.h
                                   ../core/n2dSeries.h
                                   ../core/n2dAcquisition.h
                                   ../core/n2dInputImporter.h
                                   ../core/n2dInputFilter.h
                                   ../core/n2dInstance.h
                                   ../core/n2dOutputExporter.h
)
set(nifti2dicom_gui_NO_INSTALL_HEADERS ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h
					 wizard.h 
					 init.h
					 customize.h
					 finalize.h)

set(nifti2dicom_gui_MOC_HDR wizard.h init.h customize.h finalize.h)
set(nifti2dicom_gui_UIS init.ui) # customize.ui finalize.ui)
QT4_WRAP_UI(nifti2dicom_gui_UIS_HDR ${nifti2dicom_gui_UIS})
QT4_WRAP_CPP(nifti2dicom_gui_MOC_SRCS ${nifti2dicom_gui_MOC_HDR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# nifti2dicom target
add_executable(qnifti2dicom ${nifti2dicom_gui_SOURCES} ${nifti2dicom_SOURCES} ${nifti2dicom_gui_MOC_SRCS} ${nifti2dicom_gui_UIS_HDR})
target_link_libraries(qnifti2dicom ITKBasicFilters ITKIO ITKCommon ${QT_LIBRARIES} QVTK vtkGraphics vtkHybrid vtkImaging vtkFiltering vtkCommon vtkRendering vtkIO )

install(TARGETS qnifti2dicom RUNTIME DESTINATION bin)
