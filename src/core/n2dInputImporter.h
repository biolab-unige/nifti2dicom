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


#ifndef N2DINPUTIMPORTER_H
#define N2DINPUTIMPORTER_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"

namespace n2d {

//BEGIN class n2d::InputImporter
/*!
 * \brief Imports a 3D image
 *
 * \warning Image format must be supported by ITK
 * \todo Copy MetaDataDictionary ?
 */
class InputImporter
{
	
public:
    InputImporter(const InputArgs& inputArgs) :
            m_InputArgs(inputArgs)
    {
    }

    ~InputImporter() {}

/*!
 * \brief Get internal DICOM tags dictionary
 *
 * \return Internal DICOM tags dictionary.
 */
    inline n2d::DictionaryType& getMetaDataDictionary( void ) const { return *m_dictionary; }

/*!
 * \brief Import a 3D image image.
 *
 * \return true on success.
 * \throw n2d::exception on failure.
 */
    bool Import( void );


    template<class TPixel> bool CreateReaderAndRead( );

/*!
 * \brief Get imported image.
 *
 * \return Imported image
 */
    inline n2d::ImageType::Pointer getImportedImage( void ) const { return m_ImportedImage; }
    
    inline n2d::PixelType getPixelType(void) const{return m_pixelType; }


private:
    const InputArgs&		 		m_InputArgs; //!< Input Arguments. 
    n2d::ImageType::Pointer 		 	m_ImportedImage; //!< Imported image.
    n2d::PixelType 				m_pixelType ;
    n2d::DictionaryType*			m_dictionary;
   // typedef typename ReaderType::Pointer 	reader; //!< Internal reader.
};
//END class n2d::InputImporter

} // namespace n2d


#endif // N2DINPUTIMPORTER_H
