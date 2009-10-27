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
  3. studyinstanceUID e seriesnumber: il filtro li completa già da solo (sono campi univoci
     creati a partire dalla data dell'esame, dall'ora) bisogna capire come vengono creati
     per controllare l'effettiva correttezza di tali campi.
  4. patientName: allo stato attuale di default viene settato a "GDCM" quindi loro lo
     devono cambiare a mano.
  5. alcune volte, non per tutte le immagini, viene inserito un gantrytilt anche quando
     non c'è, quindi bisogna capire come funziona il calcolo del gantrytilt e verificare
     la correttezza delle informazioni relative salvate nell'header.
*/


/*
Altre note:
  Ordine delle operazioni:
 ( -1. Dichiarazioni degli oggetti comuni  )
 (  0. Lettura della command line  )
    1. Controllo dell'accession number
    2. Importazione dell'header
    3. Class/Modality/Transfer Syntax
    4. Other DICOM tags
    5. Patient
    6. Study
    7. Series
    8. Acquisition
    9. Image import
   10. Image filters
   11. Instance (Reslicing)
   12. Output
*/


#include <iostream>

#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"

#include "n2dCommandLineParser.h"

#include "n2dAccessionNumberValidator.h"
#include "n2dHeaderImporter.h"
#include "n2dDicomClass.h"
#include "n2dOtherDicomTags.h"
#include "n2dPatient.h"
#include "n2dStudy.h"
#include "n2dSeries.h"
#include "n2dAcquisition.h"
#include "n2dInputImporter.h"
#include "n2dInputFilter.h"
#include "n2dInstance.h"
#include "n2dOutputExporter.h"

#include "n2dToolsMetaDataDictionary.h"


int main(int argc, char* argv[])
{

//BEGIN Common objects declaration
    n2d::CommandLineParser parser;
    n2d::ImageType::ConstPointer inputImage;
    n2d::DICOM3DImageType::ConstPointer filteredImage;
    n2d::DictionaryType dictionary, importedDictionary;
    n2d::DictionaryArrayType dictionaryArray;

    n2d::DICOMImageIOType::Pointer dicomIO = n2d::DICOMImageIOType::New();
    dicomIO->KeepOriginalUIDOn(); // Preserve the original DICOM UID of the input files
    dicomIO->UseCompressionOff();
//END Common objects declaration



//BEGIN Command line parsing
    try
    {
        if (!parser.Parse(argc,argv))
        {
            std::cerr << "ERROR in \"Command line parsing\"." << std::endl;
            exit(1);
        }
    }
    catch (...)
    {
        std::cerr << "ERROR in \"Command line parsing\"." << std::endl;
        exit(101);
    }
//END Command line parsing




//BEGIN DICOM accession number validation
    try
    {
        n2d::AccessionNumberValidator accessionNumberValidator(parser.accessionNumberArgs, dictionary);
        if (!accessionNumberValidator.Validate())
        {
            std::cerr << "ERROR in \"DICOM accession number validation\"." << std::endl;
            exit(2);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"DICOM accession number validation\"." << std::endl;
        exit(102);
    }
//END DICOM accession number validation



//BEGIN DICOM header import
    try
    {
        n2d::HeaderImporter headerImporter(parser.dicomHeaderArgs, importedDictionary);
        if (!headerImporter.Import())
        {
            std::cerr << "ERROR in \"DICOM header import\"." << std::endl;
            exit(3);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"DICOM header import\"." << std::endl;
        exit(103);
    }
//END DICOM header import




//BEGIN DICOM Class
    try
    {
        n2d::DicomClass dicomClass(parser.dicomClassArgs, importedDictionary, dictionary);
        if (!dicomClass.Update())
        {
            std::cerr << "ERROR in \"DICOM Class\"." << std::endl;
            exit(4);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"DICOM Class\"." << std::endl;
        exit(104);
    }
//END DICOM Class



//BEGIN Other DICOM Tags
    try
    {
        n2d::OtherDicomTags otherDicomTags(parser.otherDicomTagsArgs, dictionary);
        if (!otherDicomTags.Update())
        {
            std::cerr << "ERROR in \"Other DICOM Tags\"." << std::endl;
            exit(5);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Other DICOM Tags\"." << std::endl;
        exit(105);
    }
//END Other DICOM Tags



//BEGIN Patient
    try
    {
        n2d::Patient patient(parser.patientArgs, importedDictionary, dictionary);
        if (!patient.Update())
        {
            std::cerr << "ERROR in \"Patient\"." << std::endl;
            exit(6);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Patient\"." << std::endl;
        exit(106);
    }
//END Patient



//BEGIN Study
    try
    {
        n2d::Study study(parser.studyArgs, importedDictionary, dictionary);
        if (!study.Update())
        {
            std::cerr << "ERROR in \"Study\"." << std::endl;
            exit(7);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Study\"." << std::endl;
        exit(107);
    }
//END Study



//BEGIN Series
    try
    {
        n2d::Series series(parser.seriesArgs, importedDictionary, dictionary);
        if (!series.Update())
        {
            std::cerr << "ERROR in \"Series\"." << std::endl;
            exit(8);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Series\"." << std::endl;
        exit(108);
    }
//END Series



//BEGIN Acquisition
    try
    {
        n2d::Acquisition acquisition(parser.acquisitionArgs, dictionary);
        if (!acquisition.Update())
        {
            std::cerr << "ERROR in \"Acquisition\"." << std::endl;
            exit(9);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Acquisition\"." << std::endl;
        exit(109);
    }
//END Acquisition



//BEGIN Input image import
    try
    {
        n2d::InputImporter inputImporter(parser.inputArgs);
        if (!inputImporter.Import())
        {
            std::cerr << "ERROR in \"Input image import\"." << std::endl;
            exit(10);
        }
        inputImage = inputImporter.getImportedImage();
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Input image import\"." << std::endl;
        exit(110);
    }
//END Input image import



//BEGIN Input filtering
    try
    {
        n2d::InputFilter inputFilter(parser.filtersArgs, inputImage, dictionary);
        if (!inputFilter.Filter())
        {
            std::cerr << "ERROR in \"Input filtering\"." << std::endl;
            exit(11);
        }
        filteredImage = inputFilter.getFilteredImage();
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Input filtering\"." << std::endl;
        exit(111);
    }
//END Input filtering



//BEGIN Instance
    try
    {
        n2d::Instance instance(parser.instanceArgs, filteredImage, dictionary, dictionaryArray);
        if (!instance.Update())
        {
            std::cerr << "ERROR in \"Instance\"." << std::endl;
            exit(12);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Instance\"." << std::endl;
        exit(112);
    }
//END Instance



//BEGIN Output
    try
    {

#ifndef DONT_USE_ARRAY
        n2d::OutputExporter outputExporter(parser.outputArgs, filteredImage, dictionaryArray, dicomIO);
#else // DONT_USE_ARRAY
        n2d::OutputExporter outputExporter(parser.outputArgs, filteredImage, dictionary, dicomIO);
#endif // DONT_USE_ARRAY

        if (!outputExporter.Export())
        {
            std::cerr << "ERROR in \"Output\"." << std::endl;
            exit(13);
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Output\"." << std::endl;
        exit(113);
    }
//END Output

    return EXIT_SUCCESS;
}
