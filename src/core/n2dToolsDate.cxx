//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli <ddomenichelli@drdanz.it>
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


#include "n2dToolsDate.h"

namespace n2d {
namespace tools {


bool Date::ready = false;
std::tm Date::s_Time;


void Date::Init()
{
    std::time_t theTime;
    std::time( &theTime );   // get the calendar time
    std::tm *t = std::localtime( &theTime );  // convert to local
    s_Time = *t; // copy the structure
    ready = true;
}



std::string Date::DateTimeStr()
{
    if (!ready)
        Init();

    char currdatetime_bfr[15];
    std::strftime (currdatetime_bfr, 15, "%Y%m%d%H%M%S", &s_Time);
    std::string currdatetime(currdatetime_bfr);
    return currdatetime;
}



std::string Date::DateStr()
{
    if (!ready)
        Init();

    char currdate_bfr[15];
    std::strftime (currdate_bfr, 9, "%Y%m%d", &s_Time);
    std::string currdate(currdate_bfr);
    return currdate;
}



std::string Date::TimeStr()
{
    if (!ready)
        Init();

    char currtime_bfr[15];
    std::strftime (currtime_bfr, 7, "%H%M%S", &s_Time);
    std::string currtime(currtime_bfr);
    return currtime;
}


} // namespace tools
} // namespace n2d
