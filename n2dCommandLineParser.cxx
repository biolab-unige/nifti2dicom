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


#include "n2dCommandLineParser.h"
#include "Nifti2DicomConfig.h"



#ifdef DEBUG
#include <iostream>
#endif // DEBUG


namespace n2d
{

//WARNING Non è detto che tutti i compilatori riconoscano il try nell'inizializzazione
CommandLineParser::CommandLineParser()
try
:
    cmd("Converts NIfTI1 images to DICOM", ' ', std::string(Nifti2Dicom_VERSION) + "\n\t\tCopyright (C) 2008,2009 Daniele E. Domenichelli, BioLab, DIST, University of Genoa")
{
}
catch (TCLAP::ArgException &e)
{
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
 //Se si rilancia qua l'eccezione va gestita nel main. meglio lanciare l'eccezione nel parse()
//    throw std::exception;
}



CommandLineParser::~CommandLineParser()
{
}



bool CommandLineParser::parse(int argc, char* argv[])
{
    try
    {


//BEGIN Command line arguments declaration

        //BEGIN Input command line arguments
        // -----------------------------------------------------------------------------
        // NIFTI Input file
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> inputArg ( "i", "inputfile",
                                                "Input NIFTI 1 file",
                                                true,
                                                "", "string",
                                                cmd);
        //END Input command line arguments



        //BEGIN Output command line arguments
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
                                         4, "int",
                                         cmd);
        //END Output command line arguments




        //BEGIN Filters command line arguments
        // -----------------------------------------------------------------------------
        // Rescale image
        // -----------------------------------------------------------------------------

        TCLAP::SwitchArg rescaleSwitch ( "r", "rescale",
                                         "Set Rescale = ON (default = OFF)",
                                         cmd,
                                         false);
        //END Filters command line arguments



        //BEGIN Accession number command line arguments
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
        //END Accession number command line arguments


        //BEGIN Dicom Tags command line arguments
        // -----------------------------------------------------------------------------
        // Dicom header to import
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> dicomheaderfileArg ( "d", "dicomheaderfile",
                "File containing DICOM header to import",
                false,
                "", "string",
                cmd);

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
        // Other Arguments
        // -----------------------------------------------------------------------------
        //TODO

        //END Dicom Tags command line arguments

//END Command line arguments declaration




//BEGIN Command line arguments parsing
        cmd.parse( argc, argv );


        //BEGIN Input command line arguments
        inputArgs.inputfile       = inputArg.getValue();
        //END Input command line arguments



        //BEGIN Output command line arguments
        outputArgs.outputdirectory = outputArg.getValue();
        outputArgs.prefix          = prefixArg.getValue();
        outputArgs.suffix          = suffixArg.getValue();
        outputArgs.digits          = digitsArg.getValue(); // TODO Digits
        //END Output command line arguments



        //BEGIN Vital statistics command line arguments
        if (dicomheaderfileArg.isSet())
            vitalStatisticsArgs.dicomheaderfile = dicomheaderfileArg.getValue();
        vitalStatisticsArgs.patientname        = patientnameArg.getValue();
        vitalStatisticsArgs.patientid          = patientidArg.getValue();
        vitalStatisticsArgs.patientdob         = patientdobArg.getValue();
        vitalStatisticsArgs.patientsex         = patientsexArg.getValue();
        vitalStatisticsArgs.patientage         = patientageArg.getValue();
        //END Vital statistics command line arguments



        //BEGIN Filters command line arguments
        filtersArgs.rescale         = rescaleSwitch.getValue();
        //END Filters command line arguments



        //BEGIN Accession number command line arguments
        accessionNumberArgs.yes = yesSwitch.getValue();
        if (accessionnumberArg.isSet())
            accessionNumberArgs.accessionnumber = accessionnumberArg.getValue();
        //END Accession number command line arguments



        //BEGIN Dicom Tags command line arguments
        dicomTagsArgs.studydate          = studydateArg.getValue();
        dicomTagsArgs.seriesdate         = seriesdateArg.getValue();
        dicomTagsArgs.modality           = modalityArg.getValue();
        dicomTagsArgs.manufacturer       = manufacturerArg.getValue();
        dicomTagsArgs.istitutionname     = istitutionnameArg.getValue();
        dicomTagsArgs.referphysicianname = referphysiciannameArg.getValue();
        dicomTagsArgs.studydescription   = studydescriptionArg.getValue();
        dicomTagsArgs.seriesdescription  = seriesdescriptionArg.getValue();
        //END Dicom Tags command line arguments

        //BEGIN UID command line arguments
        uidArgs.studyistanceuid    = studyistanceuidArg.getValue();
        uidArgs.seriesistanceuid   = seriesistanceuidArg.getValue();
        uidArgs.seriesnumber       = seriesnumberArg.getValue();
        uidArgs.acquisitionnumber  = acquisitionnumberArg.getValue();
        //END UID command line arguments

//END Command line arguments parsing



#ifdef DEBUG
        std::cout << "inputfile       = " << inputArgs.inputfile       << std::endl;
        std::cout << "outputdirectory = " << outputArgs.outputdirectory << std::endl;
        std::cout << "---------------   " << std::endl;
        std::cout << "suffix          = " << outputArgs.suffix << std::endl;
        std::cout << "prefix          = " << outputArgs.prefix << std::endl;
        std::cout << "digits          = " << outputArgs.digits << std::endl;
        std::cout << "Format          = " << outputArgs.Format << std::endl;
        std::cout << "---------------   " << std::endl;
        std::cout << "rescale         = " << filtersArgs.rescale         << std::endl;
        std::cout << "---------------   " << std::endl;
        std::cout << "(0008|0050) Accession Number      (" << accessionnumberArg.isSet()    << ") = " << accessionNumberArgs.accessionnumber    << std::endl;
        std::cout << "             yes                  (" << yesSwitch.isSet()             << ") = " << accessionNumberArgs.yes                << std::endl;
        std::cout << "---------------   " << std::endl;
        std::cout << "dicomheaderfile = " << args.dicomheaderfile << std::endl;
        std::cout << "---------------   " << std::endl;
        std::cout << "(0008|0020) Study Date            (" << studydateArg.isSet()          << ") = " << dicomTagsArgs.studydate          << std::endl;
        std::cout << "(0008|0021) Series Date           (" << seriesdateArg.isSet()         << ") = " << dicomTagsArgs.seriesdate         << std::endl;
        std::cout << "(0008|0060) Modality              (" << modalityArg.isSet()           << ") = " << dicomTagsArgs.modality           << std::endl;
        std::cout << "(0008|0070) Manufacturer          (" << manufacturerArg.isSet()       << ") = " << dicomTagsArgs.manufacturer       << std::endl;
        std::cout << "(0008|0080) Istitution Name       (" << istitutionnameArg.isSet()     << ") = " << dicomTagsArgs.istitutionname     << std::endl;
        std::cout << "(0008|0090) Refer Physician Name  (" << referphysiciannameArg.isSet() << ") = " << dicomTagsArgs.referphysicianname << std::endl;
        std::cout << "(0008|1030) Study Description     (" << studydescriptionArg.isSet()   << ") = " << dicomTagsArgs.studydescription   << std::endl;
        std::cout << "(0008|103e) Series Description    (" << seriesdescriptionArg.isSet()  << ") = " << dicomTagsArgs.seriesdescription  << std::endl;
        std::cout << "(0010|0010) Patient Name          (" << patientnameArg.isSet()        << ") = " << dicomTagsArgs.patientname        << std::endl;
        std::cout << "(0010|0020) Patient ID            (" << patientidArg.isSet()          << ") = " << dicomTagsArgs.patientid          << std::endl;
        std::cout << "(0010|0030) Patient Date of Birth (" << patientdobArg.isSet()         << ") = " << dicomTagsArgs.patientdob         << std::endl;
        std::cout << "(0010|0040) Patient Sex           (" << patientsexArg.isSet()         << ") = " << dicomTagsArgs.patientsex         << std::endl;
        std::cout << "(0010|1010) Patient Age           (" << patientageArg.isSet()         << ") = " << dicomTagsArgs.patientage         << std::endl;
        std::cout << "(0020|000d) Study Istance         (" << studyistanceuidArg.isSet()    << ") = " << dicomTagsArgs.studyistanceuid    << std::endl;
        std::cout << "(0020|000e) Series Istance        (" << seriesistanceuidArg.isSet()   << ") = " << dicomTagsArgs.seriesistanceuid   << std::endl;
        std::cout << "(0020|0011) Series Number         (" << seriesnumberArg.isSet()       << ") = " << dicomTagsArgs.seriesnumber       << std::endl;
        std::cout << "(0020|0012) Aquisition Number     (" << acquisitionnumberArg.isSet()  << ") = " << dicomTagsArgs.acquisitionnumber  << std::endl;
#endif // DEBUG
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return false;
    }
    return true;
}




} // namespace n2d
