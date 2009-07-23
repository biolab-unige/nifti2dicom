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


#include "n2dCommandLineParser.h"


nifti2dicomCommandLineParser::nifti2dicomCommandLineParser()
try //WARNING Non è detto che tutti i compilatori lo riconoscano
 : cmd("Converts NIFTI 1 images to DICOM", ' ', "0.2 experimental pre-alpha\n\t\t\t\tby Dado, BioLab")
{
}
catch (TCLAP::ArgException &e)
{
  std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
//    throw std::exception;
}



nifti2dicomCommandLineParser::~nifti2dicomCommandLineParser()
{
}



void nifti2dicomCommandLineParser::parse(int argc, char* argv[])
{
  try
  {
  // -----------------------------------------------------------------------------
  // NIFTI Input file
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> inputArg ( "i", "inputfile",
                                                "Input NIFTI 1 file",
                                                true,
                                                "", "string",
                                                cmd);

  // -----------------------------------------------------------------------------
  // DICOM Output directory
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> outputArg ( "o", "outputdirectory",
                                                 "Output dicom directory",
                                                 true,
                                                 "", "string",
                                                 cmd);

  // -----------------------------------------------------------------------------
  // DICOM File names prefix
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> prefixArg ( "p", "prefix",
                                                 "Prefix of dicom file names",
                                                 false,
                                                 "", "string",
                                                 cmd);

  // -----------------------------------------------------------------------------
  // DICOM File names suffix
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> suffixArg ( "s", "suffix",
                                                 "Suffix of dicom file names",
                                                 false,
                                                 ".dcm", "string",
                                                 cmd);

  // -----------------------------------------------------------------------------
  // DICOM Number of digits in filename
  // -----------------------------------------------------------------------------
  // TODO Number of digits in filename

  TCLAP::ValueArg<int> digitsArg ( "#", "digits",
                                         "Number of digits in dicom file names",
                                         false,
                                         3, "int",
                                         cmd);

  // -----------------------------------------------------------------------------
  // Rescale image
  // -----------------------------------------------------------------------------

  TCLAP::SwitchArg rescaleSwitch ( "r", "rescale",
                                         "Set Rescale = ON (default = OFF)",
                                         cmd,
                                         false);

  // -----------------------------------------------------------------------------
  // Dicom header to import
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> dicomheaderfileArg ( "d", "dicomheaderfile",
                                                          "File containing DICOM header to import",
                                                          false,
                                                          "", "string",
                                                          cmd);

 // -----------------------------------------------------------------------------
 // Accession number
 // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> accessionnumberArg ( "a", "accessionnumber",
                                                          "(0008,0050) Accession Number (default = ?)",
                                                          false,
                                                          "", "string",
                                                          cmd);

  // -----------------------------------------------------------------------------
  // Accession number warning
  // -----------------------------------------------------------------------------

  TCLAP::SwitchArg yesSwitch ( "y", "yes",
                                     "Do not prompt for Accession Number Warning",
                                     cmd,
                                     false);

  // -----------------------------------------------------------------------------
  // Study Date
  // -----------------------------------------------------------------------------

  //TODO Creare un tipo DATE che prenda AAAAMMGG con overload dell'operatore << e che possa avere come default today
  TCLAP::ValueArg<std::string> studydateArg ( "", "studydate",
                                                    "(0008,0020) Study Date (default = today)",
                                                    false,
                                                    "", "AAAAMMGG",
                                                    cmd);

  // -----------------------------------------------------------------------------
  // Series Date
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> seriesdateArg ( "", "seriesdate",
                                                     "(0008,0021) Series Date (default = today)",
                                                     false,
                                                     "",
                                                     "AAAAMMGG",
                                                     cmd);

  // -----------------------------------------------------------------------------
  // Modality
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> modalityArg ( "", "modality",
                                                   "(0008,0060) Modality (default = \"SC\" screen capture)",
                                                   false,
                                                   "SC",
                                                   "string",
                                                   cmd);

  // -----------------------------------------------------------------------------
  // Manufacturer
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> manufacturerArg ( "", "manufacturer",
                                                       "(0008,0070) Manufacturer (default = BIOLABNIGUARDA)",
                                                       false,
                                                       "BIOLABNIGUARDA",
                                                       "string",
                                                       cmd);

  // -----------------------------------------------------------------------------
  // Istitution Name
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> istitutionnameArg ( "", "istitutionname",
                                                         "(0008,0080) Istitution Name (default = OSP. NIGUARDA)",
                                                         false,
                                                         "",
                                                         "string",
                                                         cmd);

  // -----------------------------------------------------------------------------
  // Refer Physician Name
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> referphysiciannameArg ( "", "referphysicianname",
                                                             "(0008,0090) Refer Physician Name",
                                                             false,
                                                             "",
                                                             "string",
                                                             cmd);

  // -----------------------------------------------------------------------------
  // Study Description
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> studydescriptionArg ( "", "studydescription",
                                                           "(0008,1030) Study Description (Postprocessing)",
                                                           false,
                                                           "",
                                                           "string",
                                                           cmd);

  // -----------------------------------------------------------------------------
  // Series Description
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> seriesdescriptionArg ( "", "seriesdescription",
                                                            "(0008,103e) Series Description (editabile con elenco fisso?)",
                                                            false,
                                                            "",
                                                            "string",
                                                            cmd);

  // -----------------------------------------------------------------------------
  // Patient Name
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> patientnameArg ( "", "patientname",
                                                      "(0010,0010) Patient Name",
                                                      false,
                                                      "",
                                                      "COGNOME NOME",
                                                      cmd);

  // -----------------------------------------------------------------------------
  // Patient ID
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> patientidArg ( "", "patientid",
                                                    "(0010,0020) Patient ID",
                                                    false,
                                                    "",
                                                    "string",
                                                    cmd);

  // -----------------------------------------------------------------------------
  // Patient Date of Birth
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> patientdobArg ( "", "patientdob",
                                                     "(0010,0030) Patient Date of Birth",
                                                     false,
                                                     "",
                                                     "AAAAMMGG",
                                                     cmd);

  // -----------------------------------------------------------------------------
  // Patient Sex
  // -----------------------------------------------------------------------------

  std::vector<std::string> patientsexVec;
  patientsexVec.push_back("M");
  patientsexVec.push_back("F");
  patientsexVec.push_back("O");
  TCLAP::ValuesConstraint<std::string> patientsexCon(patientsexVec);
  TCLAP::ValueArg<std::string> patientsexArg ( "", "patientsex",
                                                     "(0010,0040) Patient Sex",
                                                     false,
                                                     "",
                                                     &patientsexCon,
                                                     cmd);

  // -----------------------------------------------------------------------------
  // Patient Age
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> patientageArg ( "", "patientage",
                                                     "(0010,1010) Patient Age - Calcolato?",
                                                     false,
                                                     "",
                                                     "(000)y/m?",
                                                     cmd);

  // -----------------------------------------------------------------------------
  // Study Istance ID
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> studyistanceuidArg ( "", "studyistanceuid",
                                                          "(0020,000d) Study Istance UID (da controllare)",
                                                          false,
                                                          "",
                                                          "string",
                                                          cmd);

  // -----------------------------------------------------------------------------
  // Series Istance ID
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> seriesistanceuidArg ( "", "seriesistanceuid",
                                                           "(0020,000e) Series Istance UID (da controllare)",
                                                           false,
                                                           "",
                                                           "string",
                                                           cmd);

  // -----------------------------------------------------------------------------
  // Series Number
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string> seriesnumberArg ( "", "seriesnumber",
                                                       "(0020,0011) Series Number (da editare oppure vuoto)",
                                                       false,
                                                       "",
                                                       "string",
                                                       cmd);

  // -----------------------------------------------------------------------------
  // Acquisition Number
  // -----------------------------------------------------------------------------

  TCLAP::ValueArg<std::string>acquisitionnumberArg ( "", "acquisitionnumber",
                                                           "(0020,0012) Acquisition Number (da editare oppure vuoto)",
                                                           false,
                                                           "",
                                                           "string",
                                                           cmd);

  // -----------------------------------------------------------------------------







    cmd.parse( argc, argv );

    args.inputfile       = inputArg.getValue();
    args.outputdirectory = outputArg.getValue();
    prefix                = prefixArg.getValue();
    suffix                = suffixArg.getValue();
    digits                = digitsArg.getValue(); // TODO Digits

// TODO   togliere -> std::string("/")
    args.Format = args.outputdirectory + std::string("/")+ prefix + std::string("%04d") + suffix;

    args.rescale         = rescaleSwitch.getValue();



    if (dicomheaderfileArg.isSet())
    {
      args.dicomheaderfile = dicomheaderfileArg.getValue();
    }

    args.yes = yesSwitch.getValue();
    if (accessionnumberArg.isSet())
    {
      args.accessionnumber = accessionnumberArg.getValue();
    }
    else if (!args.yes)
    {
      //Stampa un warning minaccioso
      if(!AccessionNumberWarning(dicomheaderfileArg.isSet()))
        exit(1);
        //return;
        //EXIT_FAILURE;
        //throw?
    }


    args.studydate          = studydateArg.getValue();
    args.seriesdate         = seriesdateArg.getValue();
    args.modality           = modalityArg.getValue();
    args.manufacturer       = manufacturerArg.getValue();
    args.istitutionname     = istitutionnameArg.getValue();
    args.referphysicianname = referphysiciannameArg.getValue();
    args.studydescription   = studydescriptionArg.getValue();
    args.seriesdescription  = seriesdescriptionArg.getValue();
    args.patientname        = patientnameArg.getValue();
    args.patientid          = patientidArg.getValue();
    args.patientdob         = patientdobArg.getValue();
    args.patientsex         = patientsexArg.getValue();
    args.patientage         = patientageArg.getValue();
    args.studyistanceuid    = studyistanceuidArg.getValue();
    args.seriesistanceuid   = seriesistanceuidArg.getValue();
    args.seriesnumber       = seriesnumberArg.getValue();
    args.acquisitionnumber  = acquisitionnumberArg.getValue();

#ifdef DEBUG
    std::cout << "inputfile       = " << args.inputfile       << std::endl;
    std::cout << "outputdirectory = " << args.outputdirectory << std::endl;
    std::cout << "---------------   " << std::endl;
    std::cout << "suffix          = " << suffix << std::endl;
    std::cout << "prefix          = " << prefix << std::endl;
    std::cout << "digits          = " << digits << std::endl;
    std::cout << "Format          = " << args.Format << std::endl;
    std::cout << "---------------   " << std::endl;
    std::cout << "rescale         = " << args.rescale         << std::endl;
    std::cout << "---------------   " << std::endl;
    std::cout << "dicomheaderfile = " << args.dicomheaderfile << std::endl;
    std::cout << "---------------   " << std::endl;
    std::cout << "(0008|0050) Accession Number      (" << accessionnumberArg.isSet()    << ") = " << args.accessionnumber    << std::endl;
    std::cout << "             yes                  (" << yesSwitch.isSet()             << ") = " << args.yes                << std::endl;
    std::cout << "---------------   " << std::endl;
    std::cout << "(0008|0020) Study Date            (" << studydateArg.isSet()          << ") = " << args.studydate          << std::endl;
    std::cout << "(0008|0021) Series Date           (" << seriesdateArg.isSet()         << ") = " << args.seriesdate         << std::endl;
    std::cout << "(0008|0060) Modality              (" << modalityArg.isSet()           << ") = " << args.modality           << std::endl;
    std::cout << "(0008|0070) Manufacturer          (" << manufacturerArg.isSet()       << ") = " << args.manufacturer       << std::endl;
    std::cout << "(0008|0080) Istitution Name       (" << istitutionnameArg.isSet()     << ") = " << args.istitutionname     << std::endl;
    std::cout << "(0008|0090) Refer Physician Name  (" << referphysiciannameArg.isSet() << ") = " << args.referphysicianname << std::endl;
    std::cout << "(0008|1030) Study Description     (" << studydescriptionArg.isSet()   << ") = " << args.studydescription   << std::endl;
    std::cout << "(0008|103e) Series Description    (" << seriesdescriptionArg.isSet()  << ") = " << args.seriesdescription  << std::endl;
    std::cout << "(0010|0010) Patient Name          (" << patientnameArg.isSet()        << ") = " << args.patientname        << std::endl;
    std::cout << "(0010|0020) Patient ID            (" << patientidArg.isSet()          << ") = " << args.patientid          << std::endl;
    std::cout << "(0010|0030) Patient Date of Birth (" << patientdobArg.isSet()         << ") = " << args.patientdob         << std::endl;
    std::cout << "(0010|0040) Patient Sex           (" << patientsexArg.isSet()         << ") = " << args.patientsex         << std::endl;
    std::cout << "(0010|1010) Patient Age           (" << patientageArg.isSet()         << ") = " << args.patientage         << std::endl;
    std::cout << "(0020|000d) Study Istance         (" << studyistanceuidArg.isSet()    << ") = " << args.studyistanceuid    << std::endl;
    std::cout << "(0020|000e) Series Istance        (" << seriesistanceuidArg.isSet()   << ") = " << args.seriesistanceuid   << std::endl;
    std::cout << "(0020|0011) Series Number         (" << seriesnumberArg.isSet()       << ") = " << args.seriesnumber       << std::endl;
    std::cout << "(0020|0012) Aquisition Number     (" << acquisitionnumberArg.isSet()  << ") = " << args.acquisitionnumber  << std::endl;
#endif // DEBUG
  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
//    throw std::exception;
  }



}





/*!
 * \brief Stampa un warning minaccioso e aspetta che l'utente inserisca YES o NO
 */
bool nifti2dicomCommandLineParser::AccessionNumberWarning(bool hasDicomHeaderFile)
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
  if (hasDicomHeaderFile)
  {
    std::cout << " " << "\033[1;37;41m" << "  |       accession number will be read from dicomheaderfile       |  " << "\033[0m" << " " << std::endl;
    std::cout << " " << "\033[1;37;41m" << "  |                  (this could be a bad idea!)                   |  " << "\033[0m" << " " << std::endl;
  }
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
