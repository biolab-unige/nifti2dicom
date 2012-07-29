//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2010, 2012 Gabriele Arnulfo <gabriele.arnulfo@dist.unige.it>
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


#include <QtGui/QFileDialog>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>
#include <QtGui/QSizePolicy>
#include <QtCore/QSize>
#include <QtGui/QFont>
#include <QtGui/QErrorMessage>

#include "vtkImageViewer2.h"
#include "itkImage.h"
#include "itkExceptionObject.h"
#include "vtkRenderer.h"
#include "vtkActor2D.h"
#include "vtkRenderWindow.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkKWImageIO.h"
#include "vtkKWImage.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageData.h"
#include "vtkIndent.h"
#include "QVTKWidget.h"

#include <gdcmDict.h>
#include <gdcmGlobal.h>
#include <gdcmDicts.h>

#include <n2dInputImporter.h>
#include <n2dHeaderImporter.h>
#include <n2dDicomClass.h>
#include <n2dOtherDicomTags.h>
#include <n2dPatient.h>
#include <n2dStudy.h>
#include <n2dSeries.h>
#include <n2dAcquisition.h>


#include <sstream>

#include <QtTest/QSignalSpy>

#include "wizard.h"
#include "init.h"

namespace n2d{
namespace gui{

init::init(QWidget *parent) :
    QWizardPage(parent),
    m_renderPreview(NULL),
    m_renderer(NULL),
    m_interactor(NULL),
    m_dictionary(NULL)
{
    m_parent = dynamic_cast<n2d::gui::Wizard* >(parent);
    this->setTitle("First Step");
    this->setSubTitle("Required input: Nifti filename and optional dicom reference header");

    QGridLayout *layout              = new QGridLayout();
    QGridLayout *infoOpenImageLayout = new QGridLayout();
    QPushButton *openImage           = new QPushButton("Open Volume");
    QPushButton *openHeader          = new QPushButton("Open Dicom Header");
    QLineEdit    *openedFileName     = new QLineEdit();
    QLineEdit    *openedFileSizes    = new QLineEdit();
    m_headerEntries                  = new QTableWidget(0,3);
    m_horizontalSlider               = new QSlider(Qt::Horizontal);
    m_renderPreview                  = new QVTKWidget();

    m_horizontalSlider->setVisible(0);

    openedFileName->setReadOnly(1);
    openedFileSizes->setReadOnly(1);

    infoOpenImageLayout->setRowMinimumHeight(0,10);

    infoOpenImageLayout->addWidget(openedFileName,0,0);
    infoOpenImageLayout->addWidget(openedFileSizes,0,1);

    layout->addWidget(openImage, 0,0);
    layout->addWidget(openHeader, 0,1);
    layout->addWidget(m_renderPreview,1,0);
    layout->addWidget(m_headerEntries,1,1);
    layout->addLayout(infoOpenImageLayout,2,0);
    layout->addWidget(m_horizontalSlider,3,0);


    m_imageviewer        = vtkImageViewer2::New();
    m_renderer           = m_imageviewer->GetRenderer();
    m_renderWin          = m_imageviewer->GetRenderWindow();


    //BEGIN Test vtkKWImage//
    m_reader             = vtkKWImageIO::New();
    m_localVTKImage      = vtkKWImage::New();
    m_importedDictionary = m_parent->getImportedDictionary();
    m_dictionary         = m_parent->getDictionary();
    //END


    m_imageviewer->SetSliceOrientationToXY();

    m_renderPreview->SetRenderWindow(m_renderWin);
    m_interactor = m_renderWin->GetInteractor();
    m_interactor->Disable();

    m_inputArgs          = new n2d::InputArgs();
    m_dicomHeaderArgs    = new n2d::DicomHeaderArgs();

    QStringList labels;
    labels << tr("Tag") << tr("Value") << tr("Desc");
    m_headerEntries->setHorizontalHeaderLabels(labels);
    m_headerEntries->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    m_headerEntries->setEnabled(false);
    m_headerEntries->setColumnWidth(0,100);
    m_headerEntries->setColumnWidth(1,350);
    m_headerEntries->setColumnWidth(2,100);

    setLayout(layout);

    connect(openImage, SIGNAL(clicked()),this,SLOT(loadInImage()));
    connect(openHeader, SIGNAL(clicked()),this,SLOT(loadIndcmHDR()));
    connect(m_horizontalSlider, SIGNAL(valueChanged(int )),this,SLOT(OnSliderChange(int )));
}


init::~init()
{
    //std::cout<<"Called ~init"<<std::endl;
    m_imageviewer->Delete();
    m_reader->Delete();
    m_localVTKImage->Delete();
    //m_interactor->Delete();
}


bool init::loadInImage()
{
    m_inFname = QFileDialog::getOpenFileName(this,
                                             tr("Open Volume"),
                                             ".",
                                             tr("Nifti Volume (*.nii.gz *.nii);;"
                                                "Analyze Volume (*.hdr);;"
                                                "MetaImage Volume (*.mhd);;"
                                                "Nrrd Volume (*.nhdr *.nrrd);;"
                                                "VTK Volume (*.vtk);;"
                                                "All Files (*)"));
    if(m_inFname.isEmpty())
        return false;

    m_reader->SetFileName(m_inFname.toStdString() );
    try {
        m_reader->ReadImage();
    } catch(itk::ExceptionObject excp) {
        std::cerr << "Error while opening image" << excp.GetDescription() << std::endl;

        QErrorMessage error_message;
        error_message.showMessage(excp.GetDescription());
        error_message.exec();

        return false;
    }

    double range[2];
    m_localVTKImage = m_reader->HarvestReadImage();
    m_localVTKImage->GetVTKImage()->GetScalarRange(range);
    vtkLookupTable* lookupTable = vtkLookupTable::New();
    lookupTable->SetValueRange(0.0,1.0);
    lookupTable->SetSaturationRange(0.0,0.0);
    lookupTable->SetRampToLinear();
    lookupTable->SetRange(range);
    lookupTable->Build();
    m_imageviewer->GetWindowLevel()->SetLookupTable(lookupTable);
    m_imageviewer->GetWindowLevel()->SetInput(m_localVTKImage->GetVTKImage());

    m_imageviewer->GetRenderer()->ResetCamera();
    m_renderPreview->GetRenderWindow()->Render();
    m_parent->setImportedImage(m_localVTKImage);

    m_horizontalSlider->setVisible(1);
    m_horizontalSlider->setRange(0,m_localVTKImage->GetVTKImage()->GetDimensions()[2]);
    completeChanged();

    lookupTable->Delete();

    // update QLineEdit with proper values
    QGridLayout *tmp_layout         = dynamic_cast<QGridLayout *>(this->layout());
    QGridLayout *tmp_single_cell    = dynamic_cast<QGridLayout *>(tmp_layout->itemAtPosition(2,0));
    QLineEdit *tmp_fname_cell       = dynamic_cast<QLineEdit *>(tmp_single_cell->itemAtPosition(0,0)->widget());
    QLineEdit *tmp_fname_cell2      = dynamic_cast<QLineEdit *>(tmp_single_cell->itemAtPosition(0,1)->widget());

    tmp_fname_cell->insert(m_inFname);
    int *dimensions = m_localVTKImage->GetVTKImage()->GetDimensions();

    std::ostringstream str_dimensions;

    str_dimensions<<"["<<dimensions[0]<<","<<dimensions[1]<<","<<dimensions[2]<<"]";

    tmp_fname_cell2->insert(str_dimensions.str().c_str());

    return true;
}


bool init::OnSliderChange(int z)
{
    m_imageviewer->SetSlice(z);
    m_renderPreview->update();
    return true;
}


bool init::loadIndcmHDR()
{
    m_dcmRefHDRFname = QFileDialog::getOpenFileName(this,tr("Open Dicom Header file"),"",tr("DICOM (*.dcm);;All (*)"));
    if(m_dcmRefHDRFname.isEmpty()) return false;
    m_dicomHeaderArgs->dicomheaderfile = m_dcmRefHDRFname.toStdString();
    m_headerImporter = new n2d::HeaderImporter(*m_dicomHeaderArgs , *m_importedDictionary);

    m_parent->setDicomHeaderImporter(m_headerImporter);
    m_parent->storeDicomHeaderArgs(*m_dicomHeaderArgs);

    if(!m_headerImporter->Import())
    {
        QErrorMessage error_message;
        error_message.showMessage("Not a valid DICOM Header");
        error_message.exec();

        return false;
    }

    n2d::DictionaryType::ConstIterator itr = m_importedDictionary->Begin();
    n2d::DictionaryType::ConstIterator end = m_importedDictionary->End();

    QTableWidgetItem* tagkeyitem  ;
    QTableWidgetItem* tagvalueitem;
    QTableWidgetItem* desc;

    const gdcm::Global& g = gdcm::Global::GetInstance();
    const gdcm::Dicts &dicts = g.GetDicts();
    const gdcm::Dict &pub = dicts.GetPublicDict(); // Part 6

    while(itr != end)
    {
        int row = m_headerEntries->rowCount();
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryvalue = 
        dynamic_cast<MetaDataStringType* >(entry.GetPointer());

        if(entryvalue)
        {
            std::string tagkey  = itr->first;
            if(!tagkey.compare(0,4,"0010"))
            {
                std::string tagvalue= entryvalue->GetMetaDataObjectValue();
                int a = 0;
                int b = 0;

                sscanf(tagkey.substr(0,4).c_str(), "%x", &a);
                sscanf(tagkey.substr(5,4).c_str(), "%x", &b);

                gdcm::Tag t(a,b);
                const gdcm::DictEntry &entry1 = pub.GetDictEntry(t);

                QString item1(tagkey.c_str());
                QString item2(tagvalue.c_str());
                QString item3(entry1.GetName());

                tagkeyitem   = new QTableWidgetItem(item1);
                tagvalueitem = new QTableWidgetItem(item2);
                desc         = new QTableWidgetItem(item3);

                tagkeyitem->setFont(QFont("Verdana",10));
                tagvalueitem->setFont(QFont("Verdana",10));
                desc->setFont(QFont("Verdana",10));

                m_headerEntries->insertRow(row);
                m_headerEntries->setItem(row,0,tagkeyitem);
                m_headerEntries->setItem(row,1,tagvalueitem);
                m_headerEntries->setItem(row,2,desc);
            }
        }
        ++itr;
    }

    delete tagkeyitem;
    delete tagvalueitem;
    delete desc;
    return true;
}


bool init::isComplete() const
{
    return m_localVTKImage->GetVTKImage()->GetDimensions()[0] != 0;
}


bool init::validatePage()
{
    n2d::DicomClassArgs         dicomClassArgs;
    n2d::AcquisitionArgs        acquisitionArgs;
    n2d::OtherDicomTagsArgs     otherDicomTagsArgs;
    n2d::SeriesArgs             seriesArgs;
    n2d::StudyArgs              studyArgs;
    n2d::PatientArgs            patientArgs;


    seriesArgs.useoriginalseries    = false;
    studyArgs.donotuseoriginalstudy = false;
    studyArgs.studydescription      = "qnifti2dicom";

    //BEGIN DICOM Class
    try
    {
        n2d::DicomClass dicomClass(dicomClassArgs, *m_importedDictionary, *m_dictionary);
        if (!dicomClass.Update())
        {
            std::cerr << "ERROR in \"DICOM Class\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"DICOM Class\"." << std::endl;
        return false;
    }
    //END DICOM Class



    //BEGIN Other DICOM Tags
    try
    {
        n2d::OtherDicomTags otherDicomTags(otherDicomTagsArgs, *m_dictionary);
        if (!otherDicomTags.Update())
        {
            std::cerr << "ERROR in \"Other DICOM Tags\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Other DICOM Tags\"." << std::endl;
    return false;
    }
    //END Other DICOM Tags



    //BEGIN Patient
    try
    {
        n2d::Patient patient(patientArgs, *m_importedDictionary, *m_dictionary);
        if (!patient.Update())
        {
            std::cerr << "ERROR in \"Patient\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Patient\"." << std::endl;
        return false;
    }
    //END Patient


    //BEGIN Study
    try
    {
        n2d::Study study(studyArgs, *m_importedDictionary, *m_dictionary);
        if (!study.Update())
        {
            std::cerr << "ERROR in \"Study\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Study\"." << std::endl;
        return false;
    }
    //END Study



    //BEGIN Series
    try
    {
        n2d::Series series(seriesArgs, *m_importedDictionary, *m_dictionary);
        if (!series.Update())
        {
            std::cerr << "ERROR in \"Series\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Series\"." << std::endl;
        return false;
    }
    //END Series



    //BEGIN Acquisition
    try
    {
        n2d::Acquisition acquisition(acquisitionArgs, *m_dictionary);
        if (!acquisition.Update())
        {
            std::cerr << "ERROR in \"Acquisition\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Acquisition\"." << std::endl;
        return false;
    }
    //END Acquisition

    m_parent->storeDicomClassArgs(dicomClassArgs);
    m_parent->storeAcquisitionArgs(acquisitionArgs);
    m_parent->storeOtherDicomTagsArgs(otherDicomTagsArgs);
    m_parent->storeSeriesArgs(seriesArgs);
    m_parent->storeStudyArgs(studyArgs);
    m_parent->storePatientArgs(patientArgs);

    return true;
}


//bool init::isComplete() const
//{
//    if(m_localVTKImage == NULL)
//        return false;
//    else
//    {
//        return true;
//    }
//}

} //namespace gui
} //namespace n2d
