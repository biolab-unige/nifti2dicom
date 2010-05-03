#include "init.h"
#include <QtGui/QFileDialog>
#include <QtGui/QTableWidgetItem>
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkActor2D.h"
#include "QVTKWidget.h"
#include "ui_init.h"

#include "../core/n2dInputImporter.h"
#include "../core/n2dHeaderImporter.h"
#include "wizard.h"

namespace n2d{
namespace gui{

init::init(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::init)
{
	m_parent = dynamic_cast<n2d::gui::Wizard* >(parent);
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

    renderPreview->show();

    m_vtkImporter			= vtkImageImport::New();
    m_inputArgs				= new n2d::InputArgs();
	m_dicomHeaderArgs		= new n2d::DicomHeaderArgs();

    QStringList labels;
    labels << tr("Tag") << tr("Value");
    ui->headerEntries->setHorizontalHeaderLabels(labels);
    ui->headerEntries->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->headerEntries->setEnabled(false);

}

init::~init()
{
    delete ui;
	m_vtkImporter->Delete();
    imageviewer->Delete();
    renderInteractor->Delete();

}
template<class TPixel>
bool init::showImage(n2d::ImageType::Pointer in)
{
    typedef  itk::Image<TPixel, n2d::Dimension>			InternalImageType;
	typedef  itk::ImageToVTKImageFilter<InternalImageType> ConnectorType;

    typename InternalImageType::Pointer image 	   = 
		dynamic_cast<InternalImageType* >(m_inputImporter->getImportedImage().GetPointer());

	typename ConnectorType::Pointer connector = ConnectorType::New();
	
	connector->SetInput( image );

    imageviewer->SetInput(connector->GetOutput());
    imageviewer->GetRenderer()->ResetCamera();
    imageviewer->GetActor2D()->SetPosition( -70, 20);
    renderPreview->GetRenderWindow()->Render();
	//This is required otherwise RefCount goes to zero and connector'd be destroyed//
	m_connector = connector.GetPointer();
    return true;
}


bool init::loadInImage()
{
	//This should instantiate all the needed objects and classes 
	//to import image and metadataDictionary
	m_inFname = QFileDialog::getOpenFileName(this,"","");
	if(m_inFname.isEmpty()) return false;

	m_inputArgs->inputfile = m_inFname.toStdString();
	m_parent->storeInputArgs(*m_inputArgs);
	m_inputImporter  = new n2d::InputImporter(*m_inputArgs);

	try
	{
		m_inputImporter->Import();
	}
	catch(...)
	{
		std::cerr<<"Error while importing the image"<<std::endl;
		return false;
	}

	m_parent->setInputImporter(m_inputImporter);
	n2d::PixelType  m_pixelType = m_inputImporter->getPixelType();
    bool ret=false;

    switch(m_pixelType)
    {
        case itk::ImageIOBase::UCHAR:
        {
            ret=showImage<unsigned char>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::CHAR:
        {
            ret=showImage<char>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::USHORT:
        {
            ret=showImage<unsigned short>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::SHORT:
        {
            ret=showImage<short>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::UINT:
        {
            ret=showImage<unsigned int>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::INT:
        {
            ret=showImage<int>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::ULONG:
        {
            ret=showImage<unsigned long>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::LONG:
        {
            ret=showImage<long>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::FLOAT:
        {
            ret=showImage<float>(m_inputImporter->getImportedImage());
            break;
        }
        case itk::ImageIOBase::DOUBLE:
        {
            ret=showImage<double>(m_inputImporter->getImportedImage());
            break;
        }
        default:
        {
            std::cerr<<"ERROR: Unknown pixel type"<<std::endl;
            return false;
        }
   }

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

    m_dcmRefHDRFname = QFileDialog::getOpenFileName(this,"","/Users/biolab/test3/resources");
    if(m_dcmRefHDRFname.isEmpty()) return false;
	m_dicomHeaderArgs->dicomheaderfile = m_dcmRefHDRFname.toStdString();
	m_headerImporter	= new n2d::HeaderImporter(*m_dicomHeaderArgs , m_dictionary);

	m_parent->setDicomHeaderImporter(m_headerImporter);

    try{
        m_headerImporter->Import();
    }catch(...){
        std::cerr<<"Error While Reading Header Information"<<std::endl;
        exit(100);
    }

    n2d::DictionaryType::ConstIterator itr = m_dictionary.Begin();
    n2d::DictionaryType::ConstIterator end = m_dictionary.End();

    while(itr != end)
    {
        int row = ui->headerEntries->rowCount();
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType* >(entry.GetPointer());
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
