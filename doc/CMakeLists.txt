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
find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    message(STATUS "To build documentation run the 'doc' target")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
add_custom_target(doc ${DOXYGEN_EXECUTABLE} "Doxyfile")
else (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    message(STATUS "You will need both doxygen and graphviz 'dot' to build documentation")
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)



# Build manpage
find_file(HELP2MAN help2man HINTS /bin /usr/bin /usr/local/bin)
find_file(GZIP gzip HINTS /bin /usr/bin /usr/local/bin)
find_file(SED sed HINTS /bin /usr/bin /usr/local/bin)

if(HELP2MAN)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
                       DEPENDS nifti2dicom ${CMAKE_CURRENT_SOURCE_DIR}/man/nifti2dicom.help2man.include
                       COMMAND ${HELP2MAN}
                               ${CMAKE_BINARY_DIR}/src/core/nifti2dicom
                               --version-string=${Nifti2Dicom_VERSION}
                               --no-info
                               --include=${CMAKE_CURRENT_SOURCE_DIR}/man/nifti2dicom.help2man.include
                               --output=${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
                       COMMAND ${SED} -i "s#${CMAKE_BINARY_DIR}/src/core/##g" ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
    )
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
                       DEPENDS qnifti2dicom ${CMAKE_CURRENT_SOURCE_DIR}/man/qnifti2dicom.help2man.include
                       COMMAND ${HELP2MAN}
                               ${CMAKE_BINARY_DIR}/src/gui/qnifti2dicom
                               --version-string=${Nifti2Dicom_VERSION}
                               --no-info
                               --include=${CMAKE_CURRENT_SOURCE_DIR}/man/qnifti2dicom.help2man.include
                               --output=${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
                       COMMAND ${SED} -i "s#${CMAKE_BINARY_DIR}/src/gui/##g" ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
    )
    add_custom_target(regenerate_man SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
                                             ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
    )
else(HELP2MAN)
    message(STATUS "To re-generate the man pages you need help2man")
endif(HELP2MAN)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/man1)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1.gz
                   COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1 ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1
                   COMMAND ${GZIP} -9 -f ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/nifti2dicom.1
)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1.gz
                   COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1 ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1
                   COMMAND ${GZIP} -9 -f ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/man/man1/qnifti2dicom.1
)

add_custom_target(manpages ALL SOURCES ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1.gz ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1.gz)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/man/man1/nifti2dicom.1.gz DESTINATION share/man/man1)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/man/man1/qnifti2dicom.1.gz DESTINATION share/man/man1)
