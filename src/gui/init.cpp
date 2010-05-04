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
#include "../core/n2dDicomClass.h"
#include "../core/n2dOtherDicomTags.h"
#include "../core/n2dPatient.h"
#include "../core/n2dStudy.h"
#include "../core/n2dSeries.h"
#include "../core/n2dAcquisition.h"
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

	return ret;

}

bool init::OnSliderChange()
{
    imageviewer->SetZSlice(ui->horizontalSlider->value());
	renderPreview->GetRenderWindow()->Render();
    return true;
}


bool init::loadIndcmHDR()
{

    m_dcmRefHDRFname = QFileDialog::getOpenFileName(this,"","");
    if(m_dcmRefHDRFname.isEmpty()) return false;
	m_dicomHeaderArgs->dicomheaderfile = m_dcmRefHDRFname.toStdString();
	m_headerImporter	= new n2d::HeaderImporter(*m_dicomHeaderArgs , m_importedDictionary);

	m_parent->setDicomHeaderImporter(m_headerImporter);
	m_parent->storeDicomHeaderArgs(*m_dicomHeaderArgs);

    try{
        m_headerImporter->Import();
    }catch(...){
        std::cerr<<"Error While Reading Header Information"<<std::endl;
        exit(100);
    }

	m_parent->setImportedDictionary(m_importedDictionary);
    n2d::DictionaryType::ConstIterator itr = m_importedDictionary.Begin();
    n2d::DictionaryType::ConstIterator end = m_importedDictionary.End();

    QTableWidgetItem* tagkeyitem  ; 
	QTableWidgetItem* tagvalueitem; 

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

                    tagkeyitem   = new QTableWidgetItem(item1);
                    tagvalueitem = new QTableWidgetItem(item2);

                    ui->headerEntries->insertRow(row);
                    ui->headerEntries->setItem(row,0,tagkeyitem);
                    ui->headerEntries->setItem(row,1,tagvalueitem);

                }
        }
        ++itr;

    }

    delete tagkeyitem;
	delete tagvalueitem;
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

bool init::validatePage()
{	
	n2d::DictionaryType dictionary = m_parent->getDictionary();

	n2d::DicomClassArgs 	dicomClassArgs;
	n2d::AcquisitionArgs 	acquisitionArgs;
	n2d::OtherDicomTagsArgs otherDicomTagsArgs;
	n2d::SeriesArgs			seriesArgs;
	n2d::StudyArgs			studyArgs;
	n2d::PatientArgs		patientArgs;

//BEGIN DICOM Class
    try
    {
        n2d::DicomClass dicomClass(dicomClassArgs, m_importedDictionary, dictionary);
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
        n2d::OtherDicomTags otherDicomTags(otherDicomTagsArgs, dictionary);
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
        n2d::Patient patient(patientArgs, m_importedDictionary, dictionary);
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
        n2d::Study study(studyArgs, m_importedDictionary, dictionary);
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
        n2d::Series series(seriesArgs, m_importedDictionary, dictionary);
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
        n2d::Acquisition acquisition(acquisitionArgs, dictionary);
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


	m_parent->setDictionary(dictionary);
	m_parent->storeDicomClassArgs(dicomClassArgs);
	m_parent->storeAcquisitionArgs(acquisitionArgs);
	m_parent->storeOtherDicomTagsArgs(otherDicomTagsArgs);
	m_parent->storeSeriesArgs(seriesArgs);
	m_parent->storeStudyArgs(studyArgs);
	m_parent->storePatientArgs(patientArgs);

	return true;

}


}//namespace gui
}//namespace n2d
