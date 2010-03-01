//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli
//
//  Nifti2Dicom is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Nifti2Dicom is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Nifti2Dicom.  If not, see <http://www.gnu.org/licenses/>.

// $Id$


#include "n2dAccessionNumberValidator.h"

#include <iostream>


namespace n2d {


//BEGIN DICOM tags
const std::string accessionnumbertag("0008|0050");
//END DICOM tags


bool AccessionNumberValidator::Validate(void)
{
        if (!m_AccessionNumberArgs.accessionnumber.empty())
        {
            itk::EncapsulateMetaData<std::string>( m_Dict, accessionnumbertag, m_AccessionNumberArgs.accessionnumber);
        }
        else if (!m_AccessionNumberArgs.yes)
        {
            //Stampa un warning minaccioso
            if (!AccessionNumberWarning())
                return false;
        }
        return true;
}



/*!
 * \brief Stampa un warning minaccioso e aspetta che l'utente inserisca YES o NO
 */
bool AccessionNumberValidator::AccessionNumberWarning()
{
    std::cout << std::endl;
    std::cout << " " << "\033[1;37;41m" << "                                                                      " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "   ----------------------------------------------------------------   " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                                                                |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                            WARNING                             |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                                                                |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                                                                |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                   ACCESSION NUMBER NOT SET!                    |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                                                                |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                                                                |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |          Use -a or --accessionnumber to set manually           |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                                                                |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "   ----------------------------------------------------------------   " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "                                                                      " << "\033[0m" << " " << std::endl;
    std::cout << std::endl << "Do you wish to continue? " << std::flush;
    std::string cont;

    do
    {
        std::cout << "(type \"YES\" or \"NO\") " << std::flush;
        std::cin >> cont;
        if (cont == "NO" || cont == "no" || cont == "No" || cont == "nO")
            return false;
    }
    while (cont != "YES" && cont != "yes" && cont != "Yes" && cont != "yEs" && cont != "yeS" && cont != "YEs" && cont != "YeS" && cont != "yES");

    return true;

}

} // namespace n2d
