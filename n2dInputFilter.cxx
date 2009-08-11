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


#include "n2dInputFilter.h"

#include <itkRescaleIntensityImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkOrientImageFilter.h>


namespace n2d {

bool InputFilter::Filter( void )
{
    // -----------------------------------------------------------------------------
    // Orientamento dell'immagine
    // -----------------------------------------------------------------------------


    typedef itk::OrientImageFilter<ImageType,ImageType> OrienterType;
    OrienterType::Pointer orienter = OrienterType::New();
    orienter->UseImageDirectionOn();
    orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI); //Orient to RAI
    orienter->SetInput(m_InputImage);

    try
    {
        std::cout << "Orienting... " << std::flush;
        orienter->Update();
        std::cout << "DONE" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << message << std::endl;
        return false;
    }


    // -----------------------------------------------------------------------------
    // Riscalamento o cast dell'immagine
    // -----------------------------------------------------------------------------

    typedef itk::RescaleIntensityImageFilter< ImageType, DICOM3DImageType > RescaleType;
    RescaleType::Pointer rescaleFilter;

    typedef itk::CastImageFilter < ImageType, DICOM3DImageType > CastType;
    CastType::Pointer cast;

    if (m_FiltersArgs.rescale)
    {
        // Rescale
        rescaleFilter = RescaleType::New();

        rescaleFilter->SetInput(orienter->GetOutput());
        rescaleFilter->SetOutputMinimum( 0 );
        rescaleFilter->SetOutputMaximum( (2^11)-1 );

        try
        {
            std::cout << "Rescaling... " << std::flush;
            rescaleFilter->Update();
            std::cout << "DONE" << std::endl;
        }
        catch ( itk::ExceptionObject & ex )
        {
            std::string message;
            message = ex.GetLocation();
            message += "\n";
            message += ex.GetDescription();
            std::cerr << message << std::endl;
            return true;
        }

        m_FilteredImage =rescaleFilter->GetOutput();
    }
    else
    {
        // Caster
        cast = CastType::New();
        cast->SetInput(orienter->GetOutput());
        try
        {
            std::cout << "Casting... " << std::flush;
            cast->Update();
            std::cout << "DONE" << std::endl;
        }
        catch ( itk::ExceptionObject & ex )
        {
            std::string message;
            message = ex.GetLocation();
            message += "\n";
            message += ex.GetDescription();
            std::cerr << message << std::endl;
            return false;
        }
        m_FilteredImage = cast->GetOutput();
    }

        return true;
}


} // namespace n2d
