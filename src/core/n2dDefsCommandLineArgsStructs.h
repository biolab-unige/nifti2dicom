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


#ifndef N2DDEFSCOMMANDLINEARGSSTRUCTS_H
#define N2DDEFSCOMMANDLINEARGSSTRUCTS_H

#include <string>
#include <map>

namespace n2d {


//BEGIN struct n2d::AccessionNumberArgs
/*!
 * \brief Contains all arguments read from command line related to filters.
 *
 * The following tags are read:
 *
 * \li (0008,0050) Accession Number
 */
typedef struct AccessionNumberArgs
{
    std::string accessionnumber;
    bool yes;
} AccessionNumberArgs;
//END struct n2d::AccessionNumberArgs



//BEGIN struct n2d::DicomHeaderArgs
/*!
 * \brief Contains all arguments read from command line related to DICOM header.
 *
 */
typedef struct DicomHeaderArgs
{
    std::string dicomheaderfile;
} DicomHeaderArgsArgs;
//END struct n2d::DicomHeaderArgs



//BEGIN struct n2d::DicomClassArgs
/*!
 * \brief Contains all arguments read from command line related to DICOM class/modality, etc.
 *
 * The following tags are read:
 *
 * \li (0002,0010) Transfer Syntax UID
 * \li (0008,0016) SOP Class UID
 * \li (0008,0060) Modality
 * \li (0008,0008) Image Type
 * \li (0018,1030) Protocol Name
 *
 * \note (0002,0010) Transfer Syntax UID cannot be set by GDCM
 * \todo otherdicomtags
 */
typedef struct DicomClassArgs
{
//    std::string transfersyntaxuid;
    std::string modality;
    std::string sopclassuid;
    std::string imagetype;
    std::string protocolname;
//    std::map<std::string, std::string> otherdicomtags; //TODO
} DicomClassArgs;
//END struct n2d::DicomClassArgs



//BEGIN struct n2d::OtherDicomTagsArgs
/*!
 * \brief Contains all arguments read from command line related to other dicom tags.
 *
 * The following tags are read:
 *
 * \li (0008,0070) Manufacturer
 * \li (0008,1090) Manufacturer's Model Name
 * \li (0008,0080) Institution Name
 * \li (0008,0090) Referring Physician's Name
 *
 * \todo othertags
 */
typedef struct OtherDicomTagsArgs
{
    std::string manufacturer;
    std::string manufacturersmodelname;
    std::string institutionname;
    std::string referringphysiciansname;
//    std::map<std::string, std::string> othertags; //TODO
} OtherDicomTagsArgs;
//END struct n2d::OtherDicomTagsArgs



//BEGIN struct n2d::PatientArgs
/*!
 * \brief Contains all arguments read from command line related to patient's vital statistics.
 *
 * The following tags are read:
 *
 * \li (0010,0010) Patient's Name
 * \li (0010,0020) Patient ID
 * \li (0010,0030) Patient's Birth Date
 * \li (0010,0040) Patient's Sex
 * \li (0010,1010) Patient's Age
 * \li (0010,1030) Patient's Weight
 * \li (0010|****)
 *
 * \todo otherpatienttags
 */
typedef struct PatientArgs
{
    std::string patientname;
    std::string patientid;
    std::string patientdob;
    std::string patientsex;
    std::string patientage;
    std::string patientweight;
} PatientArgs;
//END struct n2d::PatientArgs



//BEGIN struct n2d::StudyArgs
/*!
 * \brief Contains all arguments read from command line related to DICOM study.
 *
 * The following tags are read:
 *
 * \li (0020|000d) Study Instance UID
 * \li (0020|0010) Study ID
 * \li (0008|1030) Study Description
 * \li (0008|0020) Study Date
 * \li (0008|0030) Study Time
 */
typedef struct StudyArgs
{
    std::string studyinstanceuid;
    std::string studyid;
    std::string studydescription;
    std::string studydate;
    std::string studytime;
    bool donotuseoriginalstudy;
} StudyArgs;
//END struct n2d::StudyArgs



//BEGIN struct n2d::SeriesArgs
/*!
 * \brief Contains all arguments read from command line related to DICOM series.
 *
 * The following tags are read:
 *
 * \li (0020,000e) Series Instance UID
 * \li (0020,0011) Series Number
 * \li (0008,103e) Series Description
 * \li (0008,0021) Series Date
 * \li (0008,0031) Series Time
 */
typedef struct SeriesArgs
{
    std::string seriesinstanceuid;
    std::string seriesnumber;
    std::string seriesdescription;
    std::string seriesdate;
    std::string seriestime;
    bool useoriginalseries;
} SeriesArgs;
//END struct n2d::SeriesArgs



//BEGIN struct n2d::AcquisitionArgs
/*!
 * \brief Contains all arguments read from command line related to DICOM acquisition.
 *
 * The following tags are read:
 *
 * \li (0020,0012) Acquisition Number
 * \li (0008,0022) Acquisition Date
 * \li (0008,0032) Acquisition Time
 */
typedef struct AcquisitionArgs
{
    std::string acquisitionnumber;
    std::string acquisitiondate;
    std::string acquisitiontime;
} AcquisitionArgs;
//END struct n2d::AcquisitionArgs



//BEGIN struct n2d::InputArgs
/*!
 * \brief Contains all arguments read from command line related to input.
 *
 */
typedef struct InputArgs
{
    std::string inputfile;
} InputArgs;
//END struct n2d::InputArgs



//BEGIN struct n2d::FiltersArgs
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
//END struct n2d::FiltersArgs



//BEGIN struct n2d::InstanceArgs
/*!
 * \brief Contains all arguments read from command line related to DICOM instance.
 *
 * \todo otherinstancetags
 */
typedef struct InstanceArgs
{
    std::map<std::string, std::string> otherinstancetags; //TODO
} InstanceArgs;
//END struct n2d::InstanceArgs



//BEGIN struct n2d::OutputArgs
/*!
 * \brief Contains all arguments read from command line related to output.
 *
 */
typedef struct OutputArgs
{
    std::string outputdirectory;
    std::string suffix;
    std::string prefix;
    int digits;
} OutputArgs;
//END struct n2d::OutputArgs


} // namespace n2d

#endif // N2DDEFSCOMMANDLINEARGSSTRUCTS_H
