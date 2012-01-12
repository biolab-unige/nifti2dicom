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


#ifndef N2DOUTPUTEXPORTER_H
#define N2DOUTPUTEXPORTER_H

#include "n2dDefsCommandLineArgsStructs.h"
#include "n2dDefsImage.h"
#include "n2dDefsMetadata.h"
#include "n2dDefsIO.h"

//#define DONT_USE_ARRAY

namespace n2d {

//BEGIN class n2d::OutputExporter
/*!
 * \brief [...]
 *
 */
class OutputExporter
{
public:

#ifndef DONT_USE_ARRAY
    OutputExporter(const OutputArgs& outputArgs, DICOM3DImageType::ConstPointer image, DictionaryArrayType& dictionaryArray, DICOMImageIOType::Pointer dicomIO) :
            m_OutputArgs(outputArgs),
            m_Image(image),
            m_DictionaryArray(dictionaryArray),
            m_DicomIO(dicomIO)
    {
    }
#else // DONT_USE_ARRAY
    OutputExporter(const OutputArgs& outputArgs, DICOM3DImageType::ConstPointer image, DictionaryType& dictionary, DICOMImageIOType::Pointer dicomIO) :
            m_OutputArgs(outputArgs),
            m_Image(image),
            m_Dict(dictionary),
            m_DicomIO(dicomIO)
    {
    }
#endif // DONT_USE_ARRAY

    ~OutputExporter() {}

    bool Export( void );

private:
    const OutputArgs& m_OutputArgs;
    DICOM3DImageType::ConstPointer m_Image;

#ifndef DONT_USE_ARRAY
    DictionaryArrayType& m_DictionaryArray;
#else // DONT_USE_ARRAY
    DictionaryType& m_Dict;
#endif // DONT_USE_ARRAY

    DICOMImageIOType::Pointer m_DicomIO;
};
//END class n2d::OutputExporter


//BEGIN FIXME
//FIXME This command is needed because "seriesWriter->Update()" causes a segmentation fault after
//      writing the second image (I cannot find a reason for this)
//      Adding a command fix this, but this behaviour should be investigated.

#include "itkCommand.h"

class CommandUpdate : public itk::Command
{
public:
    typedef CommandUpdate Self;
    typedef itk::Command Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    itkNewMacro( Self );

protected:
    CommandUpdate() {}

    inline void Execute(itk::Object* object, const itk::EventObject& event)
    {
        Execute( (const itk::Object*)object, event);
    }

    inline void Execute(const itk::Object *object, const itk::EventObject & event )
    {
        if ( ! itk::ProgressEvent().CheckEvent( &event ) );

        const SeriesWriterType* ptr = dynamic_cast<const SeriesWriterType*>(object);
        if (ptr)
        {
            std::cout << "\t" << (ptr->GetProgress())*100 << "%" << std::endl;
        }
    }


};

//END FIXME


}

#endif // N2DOUTPUTEXPORTER_H
