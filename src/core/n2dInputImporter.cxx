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


#include "n2dInputImporter.h"


namespace n2d {

bool InputImporter::Import( void )
{
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO( m_InputArgs.inputfile.c_str(), itk::ImageIOFactory::ReadMode );

    if(!imageIO)
    {
        std::cerr << "No ImageIO was found Not a valid Nifti" << std::endl;
        return false;
    }

    imageIO->SetFileName(m_InputArgs.inputfile);
    imageIO->ReadImageInformation();

    if(imageIO->GetPixelType() != itk::ImageIOBase::SCALAR)
    {
        std::cerr << "Only images of type SCALAR are supported." << std::endl;
        return false;
    }

    if(imageIO->GetNumberOfDimensions() != 3)
    {
        std::cerr << "Cannot open a " << imageIO->GetNumberOfDimensions() << "D image. Only 3D images are supported." << std::endl;
        return false;
    }
    m_pixelType = imageIO->GetComponentType();

    bool ret = false;
    switch(m_pixelType)
    {
        case itk::ImageIOBase::UCHAR:
        {
            ret = InternalRead<unsigned char>();
            break;
        }
        case itk::ImageIOBase::CHAR:
        {
            ret = InternalRead<char>();
            break;
        }
        case itk::ImageIOBase::USHORT:
        {
            ret = InternalRead<unsigned short>();
            break;
        }
        case itk::ImageIOBase::SHORT:
        {
            ret = InternalRead<short>();
            break;
        }
        case itk::ImageIOBase::UINT:
        {
            ret = InternalRead<unsigned int>();
            break;
        }
        case itk::ImageIOBase::INT:
        {
            ret = InternalRead<int>();
            break;
        }
        case itk::ImageIOBase::ULONG:
        {
            ret = InternalRead<unsigned long>();
            break;
        }
        case itk::ImageIOBase::LONG:
        {
            ret = InternalRead<long>();
            break;
        }
        case itk::ImageIOBase::FLOAT:
        {
            ret = InternalRead<float>();
            break;
        }
        case itk::ImageIOBase::DOUBLE:
        {
            ret = InternalRead<double>();
            break;
        }
        default:
        {
            std::cerr<<"ERROR: Unknown pixel type"<<std::endl;
            return false;
        }
    }

   return ret;
}


template<class TPixel> bool InputImporter::InternalRead( )
{
    typedef itk::Image<TPixel, Dimension>           InputImageType;
    typedef itk::ImageFileReader<InputImageType>    ReaderType;

    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( m_InputArgs.inputfile );
    try
    {
        std::cout << " * \033[1;34mReading input image\033[0m... " << std::endl;
        reader->Update();
        std::cout << " * \033[1;34mReading input image\033[0m... \033[1;32mDONE\033[0m" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << " * \033[1;34mReading input image\033[0m... \033[1;31mFAIL\033[0m" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << message << std::endl;
        return false;
    }
    m_ImportedImage = reader->GetOutput();
    m_dictionary    = &(reader->GetMetaDataDictionary());

    return true;
}

}
