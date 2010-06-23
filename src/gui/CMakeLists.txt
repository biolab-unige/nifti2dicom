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


IF( APPLE )
	set( PROGNAME qnifti2dicom )
	set( MACOSX_BUNDLE_NAME qnifti2dicom)
    SET( MACOSX_BUNDLE_ICON_FILE Brain.icns )
    SET( MACOSX_BUNDLE_SHORT_VERSION_STRING ${Nifti2Dicom_VERSION} )
    SET( MACOSX_BUNDLE_VERSION ${Nifti2Dicom_VERSION})
    SET( MACOSX_BUNDLE_LONG_VERSION_STRING Version ${Nifti2Dicom_INTERNAL_VERSION} )
    SET( CMAKE_OSX_ARCHITECTURES i386 ) #Comment out if not universal binary
ELSE( APPLE )
ENDIF( APPLE )

set(nifti2dicom_gui_SOURCES wizard.cpp
			    init.cpp
			    customize.cpp
			    finalize.cpp
				itkImageToVTKImageFilter.txx
				qnifti2dicom.cpp
				vtkKWImageIO.cxx
				vtkKWImage.cxx
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



set(nifti2dicom_gui_NO_INSTALL_HEADERS ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h
					 wizard.h 
					 init.h
					 customize.h
					 finalize.h)

set(nifti2dicom_gui_MOC_HDR wizard.h init.h customize.h finalize.h)
set(nifti2dicom_gui_QRC_RCC resources.qrc)

QT4_WRAP_CPP(nifti2dicom_gui_MOC_SRCS ${nifti2dicom_gui_MOC_HDR})
QT4_ADD_RESOURCES(nifti2dicom_gui_QRC_SRCS ${nifti2dicom_gui_QRC_RCC})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core)


IF( APPLE )
    ADD_EXECUTABLE( ${PROGNAME} MACOSX_BUNDLE ${nifti2dicom_gui_SOURCES} 
											  ${nifti2dicom_SOURCES} 
											  ${nifti2dicom_gui_MOC_SRCS} 
											  ${nifti2dicom_gui_UIS_HDR} 
											  ${nifti2dicom_gui_QRC_SRCS}) 

    set( CONTENTS_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents)

	ADD_CUSTOM_COMMAND( TARGET ${PROGNAME} POST_BUILD
	  #COMMAND mkdir ARGS ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents
      COMMAND mkdir ARGS ${CONTENTS_DIR}/Frameworks
	  COMMAND mkdir ARGS ${CONTENTS_DIR}/Resources
      COMMAND cp ARGS /Users/gabri/icons/${MACOSX_BUNDLE_ICON_FILE} ${CONTENTS_DIR}/Resources/
	  COMMAND cp ARGS /Developer/Applications/Qt/Qt\ Creator.app/Contents/Frameworks/libQtCore.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/
	  COMMAND cp ARGS /Developer/Applications/Qt/Qt\ Creator.app/Contents/Frameworks/libQtGui.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/
	  COMMAND cp ARGS /Developer/Applications/Qt/Qt\ Creator.app/Contents/Frameworks/libQtNetwork.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/
	  COMMAND cp ARGS /Developer/Applications/Qt/Qt\ Creator.app/Contents/Frameworks/libQtSql.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/
	  COMMAND ln ARGS -s libQtCore.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtCore.4.6.dylib   
	  COMMAND ln ARGS -s libQtCore.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtCore.4.dylib   
	  COMMAND ln ARGS -s libQtCore.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtCore.dylib
	  COMMAND ln ARGS -s libQtGui.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtGui.4.6.dylib
	  COMMAND ln ARGS -s libQtGui.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtGui.4.dylib
	  COMMAND ln ARGS -s libQtGui.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtGui.dylib
	  COMMAND ln ARGS -s libQtSql.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtSql.4.6.dylib
	  COMMAND ln ARGS -s libQtSql.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtSql.4.dylib
	  COMMAND ln ARGS -s libQtSql.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtSql.dylib
	  COMMAND ln ARGS -s libQtNetwork.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtNetwork.4.6.dylib
	  COMMAND ln ARGS -s libQtNetwork.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtNetwork.4.dylib
	  COMMAND ln ARGS -s libQtNetwork.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtNetwork.dylib
	  COMMAND install_name_tool ARGS -id @executable_path/../Frameworks/libQtCore.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtCore.4.6.2.dylib
	  COMMAND install_name_tool ARGS -id @executable_path/../Frameworks/libQtGui.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtGui.4.6.2.dylib
	  COMMAND install_name_tool ARGS -id @executable_path/../Frameworks/libQtNetwork.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtNetwork.4.6.2.dylib
	  COMMAND install_name_tool ARGS -id @executable_path/../Frameworks/libQtSql.4.6.2.dylib ${CONTENTS_DIR}/Frameworks/libQtSql.4.6.2.dylib
	  COMMAND install_name_tool ARGS -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/libQtGui.4.6.2.dylib ${CONTENTS_DIR}/MacOS/qnifti2dicom
	  COMMAND install_name_tool ARGS -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/libQtCore.4.6.2.dylib ${CONTENTS_DIR}/MacOS/qnifti2dicom
	  COMMAND install_name_tool ARGS -change QtSql.framework/Versions/4/QtSql @executable_path/../Frameworks/libQtSql.4.6.2.dylib ${CONTENTS_DIR}/MacOS/qnifti2dicom
	  COMMAND install_name_tool ARGS -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/libQtNetwork.4.6.2.dylib ${CONTENTS_DIR}/MacOS/qnifti2dicom
	  )

  ELSE( APPLE )
	add_executable(qnifti2dicom ${nifti2dicom_gui_SOURCES} ${nifti2dicom_SOURCES} ${nifti2dicom_gui_MOC_SRCS} ${nifti2dicom_gui_UIS_HDR} ${nifti2dicom_gui_QRC_SRCS}) 
  ENDIF( APPLE )


# nifti2dicom target
target_link_libraries(qnifti2dicom ITKBasicFilters ITKIO ITKCommon ${QT_LIBRARIES} QVTK vtkGraphics vtkHybrid vtkImaging vtkFiltering vtkCommon vtkRendering vtkIO )

if(APPLE)
else(APPLE)
	install(TARGETS qnifti2dicom RUNTIME DESTINATION bin)
endif(APPLE)
