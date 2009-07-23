cmake_minimum_required(VERSION 2.6.2)

find_package(ITK)
if(ITK_FOUND)
	include(${ITK_USE_FILE})
else(ITK_FOUND)
	message(FATAL_ERROR "Itk not found")
endif(ITK_FOUND)

#TODO Inserire controllo libreria TCLAP (command line parsing)
set(nifti2dicom_SOURCES nifti2dicom.cxx
                        n2dCommandLineParser.cxx
)
set(nifti2dicom_NO_INSTALL_HEADERS n2dCommandLineParser.h)


add_executable(nifti2dicom ${nifti2dicom_SOURCES})
target_link_libraries(nifti2dicom ITKIO ITKCommon)
install(TARGETS nifti2dicom RUNTIME DESTINATION bin)

