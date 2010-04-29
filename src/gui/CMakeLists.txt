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
			    qnifti2dicom.cpp)

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
target_link_libraries(qnifti2dicom ITKIO ITKCommon ${QT_LIBRARIES} QVTK vtkGraphics vtkHybrid vtkImaging vtkFiltering vtkCommon vtkRendering vtkIO ITKBasicFilters)

install(TARGETS qnifti2dicom RUNTIME DESTINATION bin)
