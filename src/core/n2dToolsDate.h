//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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


#ifndef N2DTOOLSDATE_H
#define N2DTOOLSDATE_H

#include <string>
#include <ctime>

namespace n2d {
namespace tools {

//BEGIN class n2d::tools::Date
/*!
 * \brief A fake class to handle date and time strings
 *
 */
class Date
{
public:
    static std::string DateTimeStr();
    static std::string DateStr();
    static std::string TimeStr();

private:

// Not implemented
    Date();
    ~Date();
    Date(Date&);

    static void Init();

    static std::tm s_Time;
    static bool ready;
};
//END class n2d::tools::Date


} // namespace tools
} // namespace n2d

#endif // #ifndef N2DTOOLSDATE_H



