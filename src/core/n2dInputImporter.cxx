//  This file is part of Nifti2Dicom, is an open source converter from 
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2008, 2009, 2010 Daniele E. Domenichelli
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


#include "n2dInputImporter.h"


namespace n2d {

bool InputImporter::Import( void )
{
      itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO( m_InputArgs.inputfile.c_str(), itk::ImageIOFactory::ReadMode );

   if( !imageIO) 
   {
   	std::cerr<<"No ImageIO was found Not a valid Nifti"<<std::endl;
	return false;
   }

   imageIO->SetFileName( m_InputArgs.inputfile);
   imageIO->ReadImageInformation();

   m_pixelType = imageIO->GetComponentType();
   bool ret=false;
   std::cout<<imageIO<<std::endl;
   
   switch(m_pixelType)
   {
   	case itk::ImageIOBase::UCHAR:
	{	
		ret=CreateReaderAndRead<unsigned char>();
		break;
	}
   	case itk::ImageIOBase::CHAR:
	{	
		ret=CreateReaderAndRead<char>();
		break;
	}
   	case itk::ImageIOBase::USHORT:
	{	
		ret=CreateReaderAndRead<unsigned short>();
		break;
	}
   	case itk::ImageIOBase::SHORT:
	{	
		ret=CreateReaderAndRead<short>();
		break;
	}
   	case itk::ImageIOBase::UINT:
	{	
		ret=CreateReaderAndRead<unsigned int>();
		break;
	}
   	case itk::ImageIOBase::INT:
	{	
		ret=CreateReaderAndRead<int>();
		break;
	}
   	case itk::ImageIOBase::ULONG:
	{	
		ret=CreateReaderAndRead<unsigned long>();
		break;
	}
   	case itk::ImageIOBase::LONG:
	{	
		ret=CreateReaderAndRead<long>();
		break;
	}
   	case itk::ImageIOBase::FLOAT:
	{	
		ret=CreateReaderAndRead<float>();
		break;
	}
   	case itk::ImageIOBase::DOUBLE:
	{	
		ret=CreateReaderAndRead<double>();
		break;
	}
	default:
	{
		std::cerr<<"Error wrong pixel type"<<std::endl;
		return false;
	}
   }
	
   return ret;
}


template<class TPixel> bool InputImporter::CreateReaderAndRead( )
{
    typedef itk::Image<TPixel, Dimension>        	 InputImageType;
    typedef itk::ImageFileReader<InputImageType>	 ReaderType;
     
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
