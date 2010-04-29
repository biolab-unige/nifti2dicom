#include "init.h"
#include <QtGui/QFileDialog>
#include <QtGui/QTableWidgetItem>
#include "itkCommand.h"
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkActor2D.h"
#include "QVTKWidget.h"
#include "ui_init.h"

namespace n2d{
namespace gui{

init::init(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::init)
{
    ui->setupUi(this);

    this->setTitle("First Step");
    this->setSubTitle("required input filename and optional dicom reference header");
    imageviewer      = vtkImageViewer::New();
    renderInteractor = vtkRenderWindowInteractor::New();

    imageviewer->SetupInteractor(renderInteractor);
    renderPreview = new QVTKWidget(ui->proxyRender);
    renderPreview->SetRenderWindow(imageviewer->GetRenderWindow());
    imageviewer->GetRenderWindow()->SetSize(500,600);
    renderPreview->resize(500,600);
    imageviewer->GetRenderer()->SetBackground(0,0,0);

    renderPreview->GetRenderWindow()->Render();

    renderPreview->show();

    vtkImporter = vtkImageImport::New();
    reader      = ReaderType::New();
    itkExporter = ExportFilterType::New();
    dicomReader = DICOMReaderType::New();
    dicomImageIO= DICOMImageIOType::New();


    QStringList labels;
    labels << tr("Tag") << tr("Value");
    ui->headerEntries->setHorizontalHeaderLabels(labels);
    ui->headerEntries->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->headerEntries->setEnabled(false);

}

init::~init()
{
    delete ui;

    vtkImporter->Delete();
    imageviewer->Delete();
    renderInteractor->Delete();

}

bool init::loadInImage()
{

    m_inFname = QFileDialog::getOpenFileName(this,"","/Users/biolab/test3/resources");
    if(m_inFname.isEmpty()) return false;

    //Here we could use inputImporter maybe//

    reader->SetFileName(m_inFname.toStdString() );
    reader->Update();

    itkExporter->SetInput( reader->GetOutput() );
    ConnectPipelines(itkExporter, vtkImporter);

    imageviewer->SetInput(vtkImporter->GetOutput());
    imageviewer->GetRenderer()->ResetCamera();
    imageviewer->GetActor2D()->SetPosition( -70, 20);
    renderPreview->GetRenderWindow()->Render();

    return true;
}

bool init::OnSliderChange()
{
    imageviewer->SetZSlice(ui->horizontalSlider->value());
    renderPreview->GetRenderWindow()->Render();
    return true;
}


bool init::loadIndcmHDR()
{
    m_dcmRefHDRFname = QFileDialog::getOpenFileName(this,"","/opt/n2d/nifti2dicom/resources");
    if(m_dcmRefHDRFname.isEmpty()) return false;

    dicomReader->SetImageIO( dicomImageIO );
    dicomReader->SetFileName( m_dcmRefHDRFname.toStdString());
    try{
        dicomReader->Update();
    }catch(...){
        std::cerr<<"casino"<<std::endl;
        exit(100);
    }

    dictionary = dicomReader->GetMetaDataDictionary();
    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();

    while(itr != end)
    {
        int row = ui->headerEntries->rowCount();
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryvalue = 
		dynamic_cast<MetaDataStringType* >(entry.GetPointer());
        if(entryvalue)
        {
                std::string tagkey  = itr->first;
                if(!tagkey.compare(0,4,"0010"))
                {
                    std::string tagvalue= entryvalue->GetMetaDataObjectValue();
                    QString item1(tagkey.c_str());
                    QString item2(tagvalue.c_str());

                    QTableWidgetItem* tagkeyitem   = new QTableWidgetItem(item1);
                    QTableWidgetItem* tagvalueitem = new QTableWidgetItem(item2);

                    ui->headerEntries->insertRow(row);
                    ui->headerEntries->setItem(row,0,tagkeyitem);
                    ui->headerEntries->setItem(row,1,tagvalueitem);
                }
        }
        ++itr;

    }

//Here we should set up all the dicom-related structs//
    return true;
}

void init::changeEvent(QEvent *e)
{

    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


template <typename ITK_Exporter, typename VTK_Importer>
void init::ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}

}//namespace gui
}//namespace n2d
