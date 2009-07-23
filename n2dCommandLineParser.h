//    This file is part of Nifti2Dicom.
//    Copyright (C) 2008,2009 Daniele E, Domenichelli
//
//    Nifti2Dicom is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Nifti2Dicom is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

// $Id$


#ifndef NIFTI2DICOMCOMMANDLINEPARSER_H
#define NIFTI2DICOMCOMMANDLINEPARSER_H

#include <string>

//TODO Test nel CMakeLists per trovare TCLAP
#include <tclap/CmdLine.h>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

//BEGIN nifti2dicomCommandLineParser.h

/*!
 * \struct nifti2dicomArgs
 *
 * \brief Contains all data read from command line.
 */
typedef struct nifti2dicomArgs
{
  std::string inputfile;
  std::string outputdirectory;
  std::string Format;

  bool rescale;

  std::string dicomheaderfile;

  std::string accessionnumber;
  bool yes;

  std::string studydate;
  std::string seriesdate;
  std::string modality;
  std::string manufacturer;
  std::string istitutionname;
  std::string referphysicianname;
  std::string studydescription;
  std::string seriesdescription;
  std::string patientname;
  std::string patientid;
  std::string patientdob;
  std::string patientsex;
  std::string patientage;
  std::string studyistanceuid;
  std::string seriesistanceuid;
  std::string seriesnumber;
  std::string acquisitionnumber;

} nifti2dicomArgs;


static nifti2dicomArgs args;



/*!
 * \class nifti2dicomCommandLineParser
 *
 * \brief Parses command line and fills nifti2dicomArgs
 */
class nifti2dicomCommandLineParser
{
public:
  nifti2dicomCommandLineParser();
  ~nifti2dicomCommandLineParser();

  void parse(int argc, char* argv[]);


private:
  TCLAP::CmdLine cmd;

  std::string suffix;
  std::string prefix;
  int digits; //TODO

  bool AccessionNumberWarning(bool hasDicomHeaderFile);
};

//END nifti2dicomCommandLineParser.h


// -------------------------------------------------------------------------------






#endif // NIFTI2DICOMCOMMANDLINEPARSER_H
