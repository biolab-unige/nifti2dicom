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


#include "n2dOutputExporter.h"

#include <string>
#include <sstream>


namespace n2d
{

bool OutputExporter::Export(void )
{
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
    std::cout << "Format: " << format << std::endl;
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


    //seriesWriter->SetMetaDataDictionaryArray( reader->GetMetaDataDictionaryArray ); //TODO MetaDataDictionary in tutti i files o roba del genere?
    seriesWriter->SetMetaDataDictionaryArray( &m_DictionaryArray );

    try
    {
        std::cout << "Writing... " << std::flush;
        seriesWriter->Update();
        std::cout << "DONE" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "Error Writing:" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << message << std::endl;
        return false;
    }
//END Writer

    return true;
}

} // namespace n2d