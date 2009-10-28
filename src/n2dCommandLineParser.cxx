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
#include "n2dVersion.h"


namespace n2d {


bool CommandLineParser::Parse(int argc, char* argv[])
{
    std::ostringstream version;
    version << GetVersion() << std::endl;
    version << std::endl<< "Using:" << std::endl;
    version << "\t" << "ITK    " << GetITKVersion() << std::endl;
    version << "\t" << "GDCM   " << GetGDCMVersion() << std::endl;
    version << "\t" << "TCLAP  " << GetTCLAPVersion() << std::endl;
    version << std::endl << GetCopyright();

    try
    {
        TCLAP::CmdLine cmd("Converts NIfTI1 images to DICOM", ' ', version.str());
//BEGIN Command line arguments declaration

    //BEGIN Accession number command line arguments

        // -----------------------------------------------------------------------------
        // Accession number
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> accessionnumberArg ( "a", "accessionnumber",
                "(0008,0050) Accession Number",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // Accession number warning
        // -----------------------------------------------------------------------------

        TCLAP::SwitchArg yesSwitch ( "y", "yes",
                "Do not prompt for Accession Number Warning",
                cmd,
                false);

    //END Accession number command line arguments



    //BEGIN DICOM header command line arguments

        // -----------------------------------------------------------------------------
        // Dicom header to import
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> dicomheaderfileArg ( "d", "dicomheaderfile",
                "File containing DICOM header to import",
                false, "",
                "string",
                cmd);

    //END DICOM header command line arguments



    //BEGIN DICOM class/modality command line arguments


/*
        // -----------------------------------------------------------------------------
        // (0002,0010) Transfer Syntax UID
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> transfersyntaxuidArg  ( "", "transfersyntaxuid",
                "(0002,0010) Transfer Syntax UID",
                false, "",
                "string",
                cmd);
*/
        // -----------------------------------------------------------------------------
        // (0008,0016) SOP Class UID
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> sopclassuidArg ( "", "sopclassuid",
                "(0008,0016) SOP Class UID",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0060) Modality
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> modalityArg ( "", "modality",
                "(0008,0060) Modality",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0008) Image Type
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> imagetypeArg ( "", "imagetype",
                "(0008,0008) Image Type",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0018,1030) Protocol Name
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> protocolnameArg ( "", "protocolname",
                "(0018,1030) Protocol Name",
                false, "",
                "string",
                cmd);

    //END DICOM class/modality command line arguments



    //BEGIN Other Dicom Tags command line arguments

        // -----------------------------------------------------------------------------
        // (0008,0070) Manufacturer
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> manufacturerArg ( "", "manufacturer",
                "(0008,0070) Manufacturer",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,1090) Manufacturer's Model Name
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> manufacturersmodelnameArg ( "", "manufacturersmodelname",
                "(0008,1090) Manufacturer's Model Name",
                false, "",
                "string",
                cmd);


        // -----------------------------------------------------------------------------
        // (0008,0080) Institution Name
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> institutionnameArg ( "", "institutionname",
                "(0008,0080) Institution Name",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0090) Referring Physician's Name
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> referringphysiciansnameArg ( "", "referringphysiciansname",
                "(0008,0090) Referring Physician's Name",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // Other Arguments
        // -----------------------------------------------------------------------------
        //TODO

    //END Other Dicom Tags command line arguments



    //BEGIN Patient command line arguments

        // -----------------------------------------------------------------------------
        // (0010,0010) Patient Name
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> patientnameArg ( "", "patientname",
                "(0010,0010) Patient Name",
                false, "",
                "COGNOME NOME",
                cmd);

        // -----------------------------------------------------------------------------
        // (0010,0020) Patient ID
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> patientidArg ( "", "patientid",
                "(0010,0020) Patient ID",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0010,0030) Patient Date of Birth
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> patientdobArg ( "", "patientdob",
                "(0010,0030) Patient Date of Birth",
                false, "",
                "AAAAMMGG",
                cmd);

        // -----------------------------------------------------------------------------
        // (0010,0040) Patient Sex
        // -----------------------------------------------------------------------------

        std::vector<std::string> patientsexVec;
        patientsexVec.push_back("M");
        patientsexVec.push_back("F");
        patientsexVec.push_back("O");
        TCLAP::ValuesConstraint<std::string> patientsexCon(patientsexVec);
        TCLAP::ValueArg<std::string> patientsexArg ( "", "patientsex",
                "(0010,0040) Patient Sex",
                false, "",
                &patientsexCon,
                cmd);

        // -----------------------------------------------------------------------------
        // (0010,1010) Patient Age
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> patientageArg ( "", "patientage",
                "(0010,1010) Patient Age",
                false, "",
                "###Y/M",
                cmd);

        // -----------------------------------------------------------------------------
        // (0010,1030) Patient Weight
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> patientweightArg ( "", "patientweight",
                "(0010,1030) Patient Weight",
                false, "",
                "WW",
                cmd);

    //END Patient command line arguments



    //BEGIN Study command line arguments

        // -----------------------------------------------------------------------------
        // (0020,000d) Study Instance UID
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> studyinstanceuidArg ( "", "studyinstanceuid",
                "(0020,000d) Study Instance UID",
                false, "",
                "string",
                cmd);


        // -----------------------------------------------------------------------------
        // (0020,0010) Study ID
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> studyidArg ( "", "studyid",
                "(0020,0010) Study ID",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,1030) Study Description
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> studydescriptionArg ( "", "studydescription",
                "(0008,1030) Study Description",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0020) Study Date
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> studydateArg ( "", "studydate",
                "(0008,0020) Study Date",
                false, "",
                "YYYYMMDD",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0030) Study Time
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> studytimeArg ( "", "studytime",
                "(0008,0030) Study Time",
                false, "",
                "hhmmss",
                cmd);

        // -----------------------------------------------------------------------------
        // Do not use original study
        // -----------------------------------------------------------------------------

        TCLAP::SwitchArg donotUseOriginalstudySwitch ( "", "donotuseoriginalstudy",
                "Do not use original study",
                cmd,
                false);

    //END Study command line arguments



    //BEGIN Series command line arguments

        // -----------------------------------------------------------------------------
        // (0020,000e) Series Instance ID
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> seriesinstanceuidArg ( "", "seriesinstanceuid",
                "(0020,000e) Series Instance UID",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0020,0011) Series Number
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> seriesnumberArg ( "", "seriesnumber",
                "(0020,0011) Series Number",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,103e) Series Description
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> seriesdescriptionArg ( "", "seriesdescription",
                "(0008,103e) Series Description",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0021) Series Date
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> seriesdateArg ( "", "seriesdate",
                "(0008,0021) Series Date",
                false, "",
                "AAAAMMGG",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0031) Series Time
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> seriestimeArg ( "", "seriestime",
                "(0008,0031) Series Time",
                false, "",
                "AAAAMMGG",
                cmd);

        // -----------------------------------------------------------------------------
        // Use original series
        // -----------------------------------------------------------------------------

        TCLAP::SwitchArg useOriginalseriesSwitch ( "", "useoriginalseries",
                "Use original series",
                cmd,
                false);
    //END Series command line arguments



    //BEGIN Acquisition command line arguments

        // -----------------------------------------------------------------------------
        // (0020,0012) Acquisition Number
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string>acquisitionnumberArg ( "", "acquisitionnumber",
                "(0020,0012) Acquisition Number",
                false, "",
                "string",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0022) Acquisition Date
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> acquisitiondateArg ( "", "acquisitiondate",
                "(0008,0022) Acquisition Date",
                false, "",
                "AAAAMMGG",
                cmd);

        // -----------------------------------------------------------------------------
        // (0008,0032) Acquisition Time
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> acquisitiontimeArg ( "", "acquisitiontime",
                "(0008,0032) Acquisition Time",
                false, "",
                "AAAAMMGG",
                cmd);

    //END Acquisition command line arguments



    //BEGIN Instance command line arguments

    //END Instance command line arguments



    //BEGIN Input command line arguments

        // -----------------------------------------------------------------------------
        // NIFTI Input file
        // -----------------------------------------------------------------------------

        TCLAP::ValueArg<std::string> inputArg ( "i", "inputfile",
                "Input NIFTI 1 file",
                true, "",
                "string",
                cmd);

    //END Input command line arguments



    //BEGIN Filters command line arguments
        // -----------------------------------------------------------------------------
        // Rescale image
        // -----------------------------------------------------------------------------

        TCLAP::SwitchArg rescaleSwitch ( "r", "rescale",
                "Rescale image before exporting",
                cmd,
                false);
    //END Filters command line arguments



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
                "N2D_", "string",
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


//END Command line arguments declaration




//BEGIN Command line arguments parsing
        cmd.parse( argc, argv );
//END Command line arguments parsing



//BEGIN Populating structs

        //BEGIN Accession number command line arguments
        accessionNumberArgs.yes = yesSwitch.getValue();
        if (accessionnumberArg.isSet())
            accessionNumberArgs.accessionnumber = accessionnumberArg.getValue();
        //END Accession number command line arguments



        //BEGIN DICOM header command line arguments
        if (dicomheaderfileArg.isSet())
            dicomHeaderArgs.dicomheaderfile = dicomheaderfileArg.getValue();
        //END DICOM header command line arguments



        //BEGIN DICOM class command line arguments
//        dicomClassArgs.transfersyntaxuid = transfersyntaxuidArg.getValue();
        dicomClassArgs.modality = modalityArg.getValue();
        dicomClassArgs.sopclassuid = sopclassuidArg.getValue();
        dicomClassArgs.imagetype = imagetypeArg.getValue();
        dicomClassArgs.protocolname = protocolnameArg.getValue();
        //END DICOM class command line arguments



        //BEGIN Other DICOM tags command line arguments
        otherDicomTagsArgs.manufacturer            = manufacturerArg.getValue();
        otherDicomTagsArgs.manufacturersmodelname  = manufacturersmodelnameArg.getValue();
        otherDicomTagsArgs.institutionname         = institutionnameArg.getValue();
        otherDicomTagsArgs.referringphysiciansname = referringphysiciansnameArg.getValue();
        //END Other DICOM tags command line arguments



        //BEGIN Patient command line arguments
        patientArgs.patientname        = patientnameArg.getValue();
        patientArgs.patientid          = patientidArg.getValue();
        patientArgs.patientdob         = patientdobArg.getValue();
        patientArgs.patientsex         = patientsexArg.getValue();
        patientArgs.patientage         = patientageArg.getValue();
        patientArgs.patientweight      = patientweightArg.getValue();
        //END Patient command line arguments



        //BEGIN Study command line arguments
        studyArgs.studyinstanceuid      = studyinstanceuidArg.getValue();
        studyArgs.studyid               = studyidArg.getValue();
        studyArgs.studydescription      = studydescriptionArg.getValue();
        studyArgs.studydate             = studydateArg.getValue();
        studyArgs.studytime             = studytimeArg.getValue();
        studyArgs.donotuseoriginalstudy = donotUseOriginalstudySwitch.getValue();
        //END Study command line arguments



        //BEGIN Series command line arguments
        seriesArgs.seriesinstanceuid  = seriesinstanceuidArg.getValue();
        seriesArgs.seriesnumber       = seriesnumberArg.getValue();
        seriesArgs.seriesdescription  = seriesdescriptionArg.getValue();
        seriesArgs.seriesdate         = seriesdateArg.getValue();
        seriesArgs.seriestime         = seriestimeArg.getValue();
        seriesArgs.useoriginalseries  = useOriginalseriesSwitch.getValue();
        //END Series command line arguments



        //BEGIN Acquisition command line arguments
        acquisitionArgs.acquisitionnumber  = acquisitionnumberArg.getValue();
        acquisitionArgs.acquisitiondate    = acquisitiondateArg.getValue();
        acquisitionArgs.acquisitiontime    = acquisitiontimeArg.getValue();
        //END Acquisition command line arguments



        //BEGIN Instance command line arguments

        //END Instance command line arguments



        //BEGIN Input command line arguments
        inputArgs.inputfile       = inputArg.getValue();
        //END Input command line arguments



        //BEGIN Filters command line arguments
        filtersArgs.rescale         = rescaleSwitch.getValue();
        //END Filters command line arguments



        //BEGIN Output command line arguments
        outputArgs.outputdirectory = outputArg.getValue();
        outputArgs.prefix          = prefixArg.getValue();
        outputArgs.suffix          = suffixArg.getValue();
        outputArgs.digits          = digitsArg.getValue();
        //END Output command line arguments

//END Populating structs
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return false;
    }

    #ifdef DEBUG
    DebugPrint();
    #endif //DEBUG

    return true;
}








void CommandLineParser::DebugPrint( void )
{
    std::cout << "-----------------------------------------" << std::endl;

//BEGIN Accession number
    std::cout << "Accession Number:" << std::endl;
    std::cout << "  (0008|0050) Accession Number            = " << accessionNumberArgs.accessionnumber << std::endl;
    std::cout << "              yes                         = " << accessionNumberArgs.yes << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Accession number

//BEGIN DICOM header
    std::cout << "DICOM Header:" << std::endl;
    std::cout << "              dicomheaderfile             = " << dicomHeaderArgs.dicomheaderfile << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END DICOM header

//BEGIN DICOM class/modality
    std::cout << "DICOM class/modality:" << std::endl;
//    std::cout << "  (0002,0010) Transfer Syntax UID         = " << dicomClassArgs.transfersyntaxuid << std::endl;
    std::cout << "  (0008,0016) SOP Class UID               = " << dicomClassArgs.sopclassuid       << std::endl;
    std::cout << "  (0008,0060) Modality                    = " << dicomClassArgs.modality          << std::endl;
    std::cout << "  (0008,0008) Image Type                  = " << dicomClassArgs.imagetype         << std::endl;
    std::cout << "  (0018,1030) Protocol Name               = " << dicomClassArgs.protocolname      << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END DICOM class/modality

//BEGIN Other
    std::cout << "Other:" << std::endl;
    std::cout << "  (0008|0070) Manufacturer                = " << otherDicomTagsArgs.manufacturer            << std::endl;
    std::cout << "  (0008,1090) Manufacturer's Model Name   = " << otherDicomTagsArgs.manufacturer            << std::endl;
    std::cout << "  (0008|0080) Istitution Name             = " << otherDicomTagsArgs.institutionname         << std::endl;
    std::cout << "  (0008|0090) Referring Physician's Name  = " << otherDicomTagsArgs.referringphysiciansname << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Other

//BEGIN Patient
    std::cout << "Patient:" << std::endl;
    std::cout << "  (0010|0010) Patient Name                = " << patientArgs.patientname   << std::endl;
    std::cout << "  (0010|0020) Patient ID                  = " << patientArgs.patientid     << std::endl;
    std::cout << "  (0010|0030) Patient Date of Birth       = " << patientArgs.patientdob    << std::endl;
    std::cout << "  (0010|0040) Patient Sex                 = " << patientArgs.patientsex    << std::endl;
    std::cout << "  (0010|1010) Patient Age                 = " << patientArgs.patientage    << std::endl;
    std::cout << "  (0010|1030) Patient Weight              = " << patientArgs.patientweight << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Patient

//BEGIN Study
    std::cout << "Study:" << std::endl;
    std::cout << "  (0020|000d) Study Instance UID          = " << studyArgs.studyinstanceuid << std::endl;
    std::cout << "  (0020|0010) Study ID                    = " << studyArgs.studyid          << std::endl;
    std::cout << "  (0008|1030) Study Description           = " << studyArgs.studydescription << std::endl;
    std::cout << "  (0008|0020) Study Date                  = " << studyArgs.studydate        << std::endl;
    std::cout << "  (0008|0020) Study Time                  = " << studyArgs.studytime        << std::endl;
    std::cout << "              donotuseoriginalstudy       = " << studyArgs.donotuseoriginalstudy << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Study

//BEGIN Series
    std::cout << "Series:" << std::endl;
    std::cout << "  (0020|000e) Series Instance UID         = " << seriesArgs.seriesinstanceuid << std::endl;
    std::cout << "  (0020|0011) Series Number               = " << seriesArgs.seriesnumber      << std::endl;
    std::cout << "  (0008|103e) Series Description          = " << seriesArgs.seriesdescription << std::endl;
    std::cout << "  (0008|0021) Series Date                 = " << seriesArgs.seriesdate        << std::endl;
    std::cout << "  (0008|0031) Series Time                 = " << seriesArgs.seriestime        << std::endl;
    std::cout << "              useoriginalseries           = " << seriesArgs.useoriginalseries << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Series

//BEGIN Acquisition
    std::cout << "Acqisition:" << std::endl;
    std::cout << "  (0020|0012) Acquisition Number          = " << acquisitionArgs.acquisitionnumber << std::endl;
    std::cout << "  (0008,0022) Acquisition Date            = " << acquisitionArgs.acquisitiondate   << std::endl;
    std::cout << "  (0008,0032) Acquisition Time            = " << acquisitionArgs.acquisitiontime   << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Acquisition

//BEGIN Input
    std::cout << "Input:" << std::endl;
    std::cout << "              inputfile                   = " << inputArgs.inputfile << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Input

//BEGIN Filter
    std::cout << "Filter:" << std::endl;
    std::cout << "              rescale                     = " << filtersArgs.rescale         << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Filter

//BEGIN Instance
    std::cout << "Instance:" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Instance

//BEGIN Output
    std::cout << "Output:" << std::endl;
    std::cout << "              outputdirectory             = " << outputArgs.outputdirectory << std::endl;
    std::cout << "              suffix                      = " << outputArgs.suffix << std::endl;
    std::cout << "              prefix                      = " << outputArgs.prefix << std::endl;
    std::cout << "              digits                      = " << outputArgs.digits << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
//END Output
}

} // namespace n2d
