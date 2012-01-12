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


set(nifti2dicom_SOURCES nifti2dicom.cxx
                        n2dVersion.cxx
                        n2dToolsMetaDataDictionary.cxx
                        n2dToolsDate.cxx
                        n2dCommandLineParser.cxx
                        n2dAccessionNumberValidator.cxx
                        n2dHeaderImporter.cxx
                        n2dDicomClass.cxx
                        n2dOtherDicomTags.cxx
                        n2dPatient.cxx
                        n2dStudy.cxx
                        n2dSeries.cxx
                        n2dAcquisition.cxx
                        n2dInputImporter.cxx
                        n2dInputFilter.cxx
                        n2dInstance.cxx
                        n2dOutputExporter.cxx
)

set(nifti2dicom_NO_INSTALL_HEADERS ${CMAKE_BINARY_DIR}/Nifti2DicomConfig.h
                                   n2dVersion.h
                                   n2dDefsCommandLineArgsStructs.h
                                   n2dDefsImage.h
                                   n2dDefsMetadata.h
                                   n2dToolsMetaDataDictionary.h
                                   n2dToolsDate.h
                                   n2dCommandLineParser.h
                                   n2dAccessionNumberValidator.h
                                   n2dHeaderImporter.h
                                   n2dDicomClass.h
                                   n2dOtherDicomTags.h
                                   n2dPatient.h
                                   n2dStudy.h
                                   n2dSeries.h
                                   n2dAcquisition.h
                                   n2dInputImporter.h
                                   n2dInputFilter.h
                                   n2dInstance.h
                                   n2dOutputExporter.h
)


# nifti2dicom target
add_executable(nifti2dicom ${nifti2dicom_SOURCES})
target_link_libraries(nifti2dicom ITKIO ITKCommon)
install(TARGETS nifti2dicom RUNTIME DESTINATION bin)
