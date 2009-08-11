//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008,2009 Daniele E. Domenichelli
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


#ifndef N2DDEFSCOMMANDLINEARGSSTRUCTS_H
#define N2DDEFSCOMMANDLINEARGSSTRUCTS_H

#include <string>
#include <map>

namespace n2d {


//BEGIN struct n2d::CommandLineParser::InputArgs
/*!
 * \brief Contains all arguments read from command line related to input.
 *
 */
typedef struct InputArgs
{
    std::string inputfile;
} InputArgs;
//END struct n2d::CommandLineParser::InputArgs



//BEGIN struct n2d::CommandLineParser::OutputArgs
/*!
 * \brief Contains all arguments read from command line related to output.
 *
 * \todo digits in Format string
 */
typedef struct OutputArgs
{
    std::string outputdirectory;
    std::string suffix;
    std::string prefix;
    int digits; //TODO
} OutputArgs;
//END struct n2d::CommandLineParser::OutputArgs



//BEGIN struct n2d::CommandLineParser::VitalStatisticsArgs
/*!
 * \brief Contains all arguments read from command line related to patient's vital statistics.
 *
 * \todo othervitalstatistics
 */
typedef struct VitalStatisticsArgs
{

    std::string dicomheaderfile;

    std::string patientname;
    std::string patientid;
    std::string patientdob;
    std::string patientsex;
    std::string patientage;

    std::map<std::string, std::string> othervitalstatistics; //TODO

} VitalStatisticsArgs;
//END struct n2d::CommandLineParser::VitalStatisticsArgs



//BEGIN struct n2d::CommandLineParser::DicomTagsArgs
/*!
 * \brief Contains all arguments read from command line related to dicom tags.
 *
 * \todo othertags
 */
typedef struct DicomTagsArgs
{
    std::string studydate;
    std::string seriesdate;
    std::string modality;
    std::string manufacturer;
    std::string istitutionname;
    std::string referphysicianname;
    std::string studydescription;
    std::string seriesdescription;

    std::map<std::string, std::string> othertags; //TODO

} DicomTagsArgs;
//END struct n2d::CommandLineParser::DicomTagsArgs



//BEGIN struct n2d::CommandLineParser::UIDArgs
/*!
 * \brief Contains all arguments read from command line related to dicom uid(s).
 *
 * \todo otheruids
 */
typedef struct UIDArgs
{
    std::string studyistanceuid;
    std::string seriesistanceuid;
    std::string seriesnumber;
    std::string acquisitionnumber;

//    std::map<std::string, std::string> otheruids; //TODO

} UIDArgs;
//END struct n2d::CommandLineParser::UIDArgs



//BEGIN struct n2d::CommandLineParser::ResliceArgs
/*!
 * \brief Contains all arguments read from command line related to reslicer.
 *
 * \todo otherresliceargs
 */
typedef struct ResliceArgs
{
    std::map<std::string, std::string> otherresliceargs; //TODO

} ResliceArgs;
//END struct n2d::CommandLineParser::ResliceArgs



//BEGIN struct n2d::CommandLineParser::AccessionNumberArgs
/*!
 * \brief Contains all arguments read from command line related to filters.
 * 
 */
typedef struct AccessionNumberArgs
{
    std::string accessionnumber;
    bool yes;
} AccessionNumberArgs;
//END struct n2d::CommandLineParser::AccessionNumberArgs



//BEGIN struct n2d::CommandLineParser::FiltersArgs
/*!
 * \brief Contains all arguments read from command line related to filters.
 *
 * \todo orientation
 */
typedef struct FiltersArgs
{
//    std::string orientation; //TODO
    bool rescale;
} FiltersArgs;
//END struct n2d::CommandLineParser::FiltersArgs



} // namespace n2d

#endif // N2DDEFSCOMMANDLINEARGSSTRUCTS_H
