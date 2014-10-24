/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkKWImageIO.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __vtkKWImageIO_h
#define __vtkKWImageIO_h


#include "vtkObject.h"
#include "vtkVersion.h"

#include <string>
#include <vector>

class vtkKWImage;


/** \class vtkKWImageIO
 *  \brief Class that reads image using ITK reader factory.
 *
 *   This class uses the ITK readers/writers factory to load
 *   an image and returns it into a vtkKWImage, which is a 
 *   ITK/VTK image container.
 */
class vtkKWImageIO : public vtkObject
{
public:
  static vtkKWImageIO* New();
#if (VTK_MAJOR_VERSION < 6)
  vtkTypeRevisionMacro(vtkKWImageIO,vtkObject);
#else
  vtkTypeMacro(vtkKWImageIO,vtkObject);
#endif

  // PixelType
  //BTX
  typedef enum
    {
    UNSIGNED_CHAR,
    CHAR,
    UNSIGNED_SHORT,
    SHORT,
    UNSIGNED_INT,
    INT,
    UNSIGNED_LONG,
    LONG,
    FLOAT,
    DOUBLE
    } PixelType;
  //ETX

  PixelType GetImagePixelType();

  // set filename
  void SetFileName( const std::string & fileName );

  // set directory
  void SetDirectory( const std::string & directory );

  // set a vector list containing filenames of an image series

  void SetSeriesFileNames( const std::vector< std::string > & FileNames );

  // ReadImage
  void ReadImage();

  // Read DICOM series
  void ReadImageSeries();

  // Read and cast image
  void ReadAndCastImage();

  // WriteImage
  void WriteImage();

  // Return the vtkKWImage that was previously read.  This image object is
  // returned and disconnected from the reader. Once you receive the image
  // object it is YOUR responsibility to release its memory by calling its
  // Delete() method when you don't need it anymore.
  vtkKWImage * HarvestReadImage();

  // Set the image to be written.
  void SetImageToBeWritten( const vtkKWImage * imageToBeWritten );

protected:
  vtkKWImageIO();
  ~vtkKWImageIO();

private:
  vtkKWImageIO(const vtkKWImageIO&);   // Not implemented.
  void operator=(const vtkKWImageIO&);  // Not implemented.

  // Containter that holds the image read from a file
  vtkKWImage                   *ImageThatHasBeenRead;

  // Containter that holds the image to be written to a file
  const vtkKWImage             *ImageToBeWritten;

  //BTX
  std::string                   FileName;
  std::vector< std::string>     SeriesFileNames;
  std::string                   Directory;
  //ETX

  PixelType                     ImagePixelType;
};

#endif
