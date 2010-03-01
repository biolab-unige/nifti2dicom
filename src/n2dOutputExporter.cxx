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


#include "n2dOutputExporter.h"
#include "n2dToolsMetaDataDictionary.h"

#include <string>
#include <sstream>


namespace n2d {

bool OutputExporter::Export( void )
{
#ifdef DEBUG
    std::cout << "OutputExporter - BEGIN" << std::endl;
#endif // DEBUG

    unsigned int nbSlices = (m_Image->GetLargestPossibleRegion().GetSize())[2];

//BEGIN Output filename
    std::ostringstream fmt;

    fmt << m_OutputArgs.outputdirectory;
    fmt << "/";
    fmt << m_OutputArgs.prefix;
    fmt << "%0";
    fmt << m_OutputArgs.digits;
    fmt << "d";
    fmt << m_OutputArgs.suffix;

    std::string Format = fmt.str();
#ifdef DEBUG
    std::cout << "Format: " << Format << std::endl;
#endif // DEBUG

    NameGeneratorType::Pointer namesGenerator = NameGeneratorType::New();
    namesGenerator->SetStartIndex( 1 );
    namesGenerator->SetEndIndex( nbSlices );
    namesGenerator->SetIncrementIndex( 1 );

    namesGenerator->SetSeriesFormat( Format.c_str() );

//END Output filename


    itksys::SystemTools::MakeDirectory( m_OutputArgs.outputdirectory.c_str() ); // Create directory if it does not exist yet


//BEGIN Writer
    SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();
    seriesWriter->SetInput( m_Image );
    seriesWriter->SetImageIO( m_DicomIO );
    seriesWriter->SetFileNames( namesGenerator->GetFileNames() );

#ifndef DONT_USE_ARRAY
    seriesWriter->SetMetaDataDictionaryArray( &m_DictionaryArray );
#else // DONT_USE_ARRAY
    seriesWriter->SetMetaDataDictionary( m_Dict );
    //seriesWriter->SetMetaDataDictionary(*m_DictionaryArray[0]);
#endif // DONT_USE_ARRAY


//BEGIN FIXME (see n2dOutputExporter.h)
    CommandUpdate::Pointer observer = CommandUpdate::New();
    seriesWriter->AddObserver(itk::AnyEvent(), observer);
//END FIXME (see n2dOutputExporter.h)

    try
    {
        std::cout << " * \033[1;34mWriting\033[0m... " << std::endl;
        seriesWriter->Update();
        std::cout << " * \033[1;34mWriting\033[0m... \033[1;32mDONE\033[0m" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << " * \033[1;34mWriting\033[0m... \033[1;31mFAIL\033[0m" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << message << std::endl;
        return false;
    }
//END Writer

#ifdef DEBUG
    std::cout << "OutputExporter - END" << std::endl;
#endif // DEBUG

    return true;
}


} // namespace n2d
