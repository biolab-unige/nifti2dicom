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


#ifndef N2DCOMMANDLINEPARSER_H
#define N2DCOMMANDLINEPARSER_H

#include <string>
#include <map>

//TODO Test nel CMakeLists per trovare TCLAP
#include <tclap/CmdLine.h>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


namespace n2d
{





//BEGIN class n2d::CommandLineParser

/*!
 * \class CommandLineParser
 *
 * \brief Parses command line and fills nifti2dicomArgs
 */
class CommandLineParser
{
public:
  CommandLineParser();
  ~CommandLineParser();

  void parse(int argc, char* argv[]);



  //BEGIN struct n2d::CommandLineParser::DicomTagsArgs
  /*!
  * \struct DicomTagsArgs
  *
  * \brief Contains all arguments read from command line related to dicom tags.
  */
  typedef struct DicomTagsArgs
  {

    std::string dicomheaderfile;

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

    std::map<std::string, std::string> othertags; //TODO

  } DicomTagsArgs;
  //END struct n2d::CommandLineParser::DicomTagsArgs





  //BEGIN struct n2d::CommandLineParser::InputArgs
  /*!
  * \struct InputArgs
  *
  * \brief Contains all arguments read from command line related to input.
  */
  typedef struct InputArgs
  {
    std::string inputfile;
  } InputArgs;
  //END struct n2d::CommandLineParser::InputArgs





  //BEGIN struct n2d::CommandLineParser::OutputArgs
  /*!
  * \struct OutputArgs
  *
  * \brief Contains all arguments read from command line related to output.
  */
  typedef struct OutputArgs
  {
    std::string outputdirectory;
    std::string Format;
    std::string suffix;
    std::string prefix;
    int digits; //TODO
  } OutputArgs;
  //END struct n2d::CommandLineParser::OutputArgs





  //BEGIN struct n2d::CommandLineParser::FiltersArgs
  /*!
  * \struct FiltersArgs
  *
  * \brief Contains all arguments read from command line related to filters.
  */
  typedef struct FiltersArgs
  {
    bool rescale;
  } FiltersArgs;
  //END struct n2d::CommandLineParser::FiltersArgs




  //BEGIN struct n2d::CommandLineParser::AccessionNumberArgs
  /*!
  * \struct AccessionNumberArgs
  *
  * \brief Contains all arguments read from command line related to filters.
  */
  typedef struct AccessionNumberArgs
  {
    std::string accessionnumber;
    bool yes;
  } AccessionNumberArgs;
  //END struct n2d::CommandLineParser::AccessionNumberArgs



  DicomTagsArgs       dicomTagsArgs;
  InputArgs           inputArgs;
  OutputArgs          outputArgs;
  FiltersArgs         filtersArgs;
  AccessionNumberArgs accessionNumberArgs;


private:
  TCLAP::CmdLine cmd;

  bool AccessionNumberWarning(bool hasDicomHeaderFile);
};
//END class n2d::CommandLineParser


} // namespace n2d





#endif // N2DCOMMANDLINEPARSER_H
