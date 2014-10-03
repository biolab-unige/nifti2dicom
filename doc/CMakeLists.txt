#  This file is part of Nifti2Dicom, is an open source converter from
#  3D NIfTI images to 2D DICOM series.
#
#  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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



# Build documentation if doxygen and graphviz are found
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    message(STATUS "To build documentation run the 'doc' target")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} "Doxyfile")
endif()



# Build manpage
option(ENABLE_REGENERATE_MAN "Enable regenerate_man target" FALSE)
mark_as_advanced(ENABLE_REGENERATE_MAN)

if(ENABLE_REGENERATE_MAN)

    # Find help2man
    find_file(HELP2MAN_EXECUTABLE help2man HINTS /bin /usr/bin /usr/local/bin)
    if(NOT HELP2MAN_EXECUTABLE)
        message(FATAL_ERROR "To enable the regenerate_man target you need to install help2man")
    endif(NOT HELP2MAN_EXECUTABLE)

    # Find sed
    find_file(SED_EXECUTABLE sed HINTS /bin /usr/bin /usr/local/bin)
    if(NOT SED_EXECUTABLE)
        message(FATAL_ERROR "To enable the regenerate_man target you need to install sed")
    endif(NOT SED_EXECUTABLE)

    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
                       DEPENDS nifti2dicom ${CMAKE_CURRENT_SOURCE_DIR}/man/nifti2dicom.help2man.include
                       COMMAND ${HELP2MAN_EXECUTABLE}
                               ${CMAKE_BINARY_DIR}/src/core/nifti2dicom
                               --version-string=${Nifti2Dicom_VERSION}
                               --no-info
                               --include=${CMAKE_CURRENT_SOURCE_DIR}/man/nifti2dicom.help2man.include
                               --output=${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
                       COMMAND ${SED_EXECUTABLE} -i "s#${CMAKE_BINARY_DIR}/src/core/##g" ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
    )
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
                       DEPENDS qnifti2dicom ${CMAKE_CURRENT_SOURCE_DIR}/man/qnifti2dicom.help2man.include
                       COMMAND ${HELP2MAN_EXECUTABLE}
                               ${CMAKE_BINARY_DIR}/src/gui/qnifti2dicom
                               --version-string=${Nifti2Dicom_VERSION}
                               --no-info
                               --include=${CMAKE_CURRENT_SOURCE_DIR}/man/qnifti2dicom.help2man.include
                               --output=${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
                       COMMAND ${SED_EXECUTABLE} -i "s#${CMAKE_BINARY_DIR}/src/gui/##g" ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
    )
    add_custom_target(regenerate_man SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
                                             ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
    )
endif(ENABLE_REGENERATE_MAN)

find_file(GZIP_EXECUTABLE gzip HINTS /bin /usr/bin /usr/local/bin)

if(GZIP_EXECUTABLE)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/man1)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1.gz
                       COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1 ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1
                       COMMAND ${GZIP_EXECUTABLE} -9 -f ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
    )
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1.gz
                       COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1 ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1
                       COMMAND ${GZIP_EXECUTABLE} -9 -f ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
    )

    add_custom_target(manpages ALL SOURCES ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1.gz
                                           ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1.gz)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1.gz DESTINATION share/man/man1)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1.gz DESTINATION share/man/man1)
endif()
