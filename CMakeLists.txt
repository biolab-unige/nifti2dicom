cmake_minimum_required(VERSION 2.6)

find_package(ITK)
if(ITK_FOUND)
	include(${ITK_USE_FILE})
else(ITK_FOUND)
	message(FATAL_ERROR "Itk not found")
endif(ITK_FOUND)

add_executable(nifti2dicom nifti2dicom.cxx)
target_link_libraries(nifti2dicom ITKIO ITKCommon )
install(TARGETS nifti2dicom RUNTIME DESTINATION bin)

