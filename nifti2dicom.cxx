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


#include <iostream>

#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"
#include "n2dMetaDataDictionaryTools.h"
#include "n2dCommandLineParser.h"
#include "n2dInputImporter.h"
#include "n2dInputFilter.h"
#include "n2dVitalStatisticsImporter.h"
#include "n2dDicomTagsImporter.h"
#include "n2dUIDGenerator.h"
#include "n2dSlicer.h"
#include "n2dOutputExporter.h"


/*
Alcune note:
  1. seriesdescription: l'elenco fisso deve avere questi campi:
    1. fMRI;
    2. DTI-ft;
    3. Image Fusion;
    4. Other/postprocessing.
  2. accessionNumberb: di default il campo deve essere compilato a mano dall'utente; se
     invece viene data un header di riferimento, per la copia dei dati anagrafici ALLORA
     si prevede la doppia opzione:
     1. (default) da inserire a mano;
     2. copia dall'header DICOM ma dà un warning per avvertire che ci saranno due/o più
        immagini con lo stesso Accession Number.
  3. studyistanceUID e seriesnumber: il filtro li completa già da solo (sono campi univoci
     creati a partire dalla data dell'esame, dall'ora) bisogna capire come vengono creati
     per controllare l'effettiva correttezza di tali campi.
  4. patientName: allo stato attuale di default viene settato a "GDCM" quindi loro lo
     devono cambiare a mano.
  5. alcune volte, non per tutte le immagini, viene inserito un gantrytilt anche quando
     non c'è, quindi bisogna capire come funziona il calcolo del gantrytilt e verificare
     la correttezza delle informazioni relative salvate nell'header.
*/





int main(int argc, char* argv[])
{
//BEGIN Common objects declaration
    n2d::CommandLineParser parser;
    n2d::ImageType::ConstPointer inputImage;
    n2d::DICOM3DImageType::ConstPointer filteredImage;
    n2d::DictionaryType dictionary;
    n2d::DictionaryArrayType dictionaryArray;

    n2d::DICOMImageIOType::Pointer dicomIO = n2d::DICOMImageIOType::New();
//    dicomIO->KeepOriginalUIDOn(); // Preserve the original DICOM UID of the input files
//END Common objects declaration




//BEGIN Command line parsing
    try
    {
        parser.parse(argc,argv);
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Command line parsing\"." << std::endl;
        exit(1);
    }
//END Command line parsing




//BEGIN Input image import

    try
    {
        n2d::InputImporter inputImporter(parser.inputArgs);
        if (inputImporter.Import())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;

        inputImage = inputImporter.getImportedImage();
        dictionary = inputImporter.getMetaDataDictionary();
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Input image import\"." << std::endl;
        exit(2);
    }

//END Input image import




//BEGIN Input filtering
    try
    {
        n2d::InputFilter inputFilter(parser.filtersArgs, inputImage);
        if (inputFilter.Filter())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;
        filteredImage = inputFilter.getFilteredImage();
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Input filtering\"." << std::endl;
        exit(3);
    }    
//END Input filtering




//BEGIN Vital statistics import
    try
    {
        n2d::VitalStatisticsImporter vitalStatisticsImporter(parser.vitalStatisticsArgs, dictionary);
        if (vitalStatisticsImporter.Import())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Vital statistics import\"." << std::endl;
        exit(4);
    }    
//END Vital statistics import




//BEGIN DICOM tags import
    try
    {
        n2d::DicomTagsImporter dicomTagsImporter(parser.dicomTagsArgs, dictionary);
        if (dicomTagsImporter.Import())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;
    }
    catch (...)
    {
        std::cerr << "ERROR in \"DICOM tags import\"." << std::endl;
        exit(5);
    }    
//END DICOM tags import




//BEGIN DICOM UID generation
    try
    {
        n2d::UIDGenerator uidGenerator(parser.uidArgs, dictionary, dicomIO);
        if (uidGenerator.Generate())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;
    }
    catch (...)
    {
        std::cerr << "ERROR in \"DICOM UID generation\"." << std::endl;
        exit(6);
    }    
//END DICOM UID generation



//BEGIN Slicer
    try
    {
        n2d::Slicer slicer(parser.resliceArgs, filteredImage, dictionary, dictionaryArray);
        if (slicer.Reslice())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Slicer\"." << std::endl;
        exit(7);
    }
//END Slicer




//BEGIN Output
    try
    {
        n2d::OutputExporter outputExporter(parser.outputArgs, filteredImage, dictionaryArray, dicomIO);
        if (outputExporter.Export())
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - OK" << std::endl;
        else
            std::cout << "TODO: " << __FILE__ << ":" << __LINE__ << " - FAIL" << std::endl;
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Output\"." << std::endl;
        exit(8);
    }


//END Output

    return EXIT_SUCCESS;
}






