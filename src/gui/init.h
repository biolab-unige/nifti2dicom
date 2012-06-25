//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2010 Gabriele Arnulfo <gabriele.arnulfo@dist.unige.it>
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


#ifndef INIT_H
#define INIT_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>
#include <QtCore/QString>
#include "QVTKWidget.h"


#include <n2dDefsImage.h>
#include <n2dDefsIO.h>
#include <n2dDefsMetadata.h>

class QTableWidget;
class QSlider;
class QFont;
class vtkKWImageIO;
class vtkKWImage;
class vktRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkImageViewer2;

namespace n2d{

class InputImporter;
class HeaderImporter;

struct InputArgs;
struct DicomHeaderArgs;	

namespace gui{

class Wizard;

class init : public QWizardPage {
    Q_OBJECT
public:
    init(QWidget *parent = 0);
    ~init();


private:
    QString	 						m_inFname;
    QString 	 					m_dcmRefHDRFname;
    QVTKWidget*  					m_renderPreview;
    vtkImageViewer2* 				m_imageviewer;
    vtkRenderer* 					m_renderer;
    vtkRenderWindow* 				m_renderWin;
    vtkRenderWindowInteractor*		m_interactor;
    vtkKWImageIO*					m_reader;
    vtkKWImage*						m_localVTKImage;
    n2d::HeaderImporter*			m_headerImporter;
    Wizard*							m_parent;
    n2d::DictionaryType* 			m_importedDictionary;
    n2d::DictionaryType*			m_dictionary;
    QTableWidget*					m_headerEntries;
    QSlider*						m_horizontalSlider;
    QFont*							m_font;

    n2d::InputArgs* 				m_inputArgs;
    n2d::DicomHeaderArgs*			m_dicomHeaderArgs;


private slots:
    bool loadInImage();
    bool loadIndcmHDR();
    bool OnSliderChange(int );
    bool validatePage();
    bool isComplete() const;
};
}//namespace gui
}//namespace n2d
#endif // INIT_H
