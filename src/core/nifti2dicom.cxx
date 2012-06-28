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


/*
Some notes: [PROBABLY OUTDATED]
  1. seriesdescription: the fixed list must have those fields:
    1. fMRI;
    2. DTI-ft;
    3. Image Fusion;
    4. Other/postprocessing.
  2. accessionNumber: by default the field must be set manually by the user.
     If a user sets a reference header instead, it must have 2 options:
     1. (default) set manually
     2. copy it from the DICOM header, BUT it must prompt a warning to inform
        the user that there will be 2 or more images with the same Accession
        Number
  3. [FIXED] studyinstanceUID e seriesnumber: the filter completes them
     automatically (those are univocal fields, created using date and time of
     the exam). we have to understand how they are created to check if those
     fields are set correctly.
  4. [FIXED] patientName: at the moment it is set to "GDCM",therefore it must be
     changed by them manually.
  5. [FIXED] sometimes (not for all the images) a gantrytilt is added (even when
     there should not be). patches/ITK_Direction4.patch is a patch for ITK that
     should fix this problem.
     This problem was reported upstream (see
     https://itk.icts.uiowa.edu/jira/browse/ITK-281 )
*/


/*
More notes:
  Steps performed:
 ( -1. Declarations of the common objects  )
 (  0. Command line parsing  )
    1. Check accession number
    2. Import DICOM header
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
    n2d::PixelType inputPixelType;
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
        inputPixelType = inputImporter.getPixelType();
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
        n2d::InputFilter inputFilter(parser.filtersArgs, inputImage, inputPixelType, dictionary);
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
