#include <QtGui/QFileDialog>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>
#include <QtGui/QSizePolicy>
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkRenderer.h"
#include "vtkActor2d.h"
#include "vtkRenderWindow.h"
#include "vtkImageMapper.h"
#include "QVTKWidget.h"

#include "../core/n2dInputImporter.h"
#include "../core/n2dHeaderImporter.h"
#include "../core/n2dDicomClass.h"
#include "../core/n2dOtherDicomTags.h"
#include "../core/n2dPatient.h"
#include "../core/n2dStudy.h"
#include "../core/n2dSeries.h"
#include "../core/n2dAcquisition.h"
#include "wizard.h"
#include "init.h"

namespace n2d{
namespace gui{

init::init(QWidget *parent) :
    QWizardPage(parent),
	m_renderPreview(NULL),
	m_renderer(NULL)
{
	m_parent = dynamic_cast<n2d::gui::Wizard* >(parent);
    this->setTitle("First Step");
    this->setSubTitle("required input filename and optional dicom reference header");

	QGridLayout *layout 		= new QGridLayout();
	QPushButton *openImage 		= new QPushButton("Open Image");
	QPushButton *openHeader		= new QPushButton("Open Header");
	m_headerEntries 			= new QTableWidget(0,2);
	m_horizontalSlider			= new QSlider(Qt::Horizontal);


    QSizePolicy policy( QSizePolicy::MinimumExpanding, 
						QSizePolicy::MinimumExpanding, 
						QSizePolicy::DefaultType );
    policy.setHeightForWidth(true);
	

    m_renderPreview = new QVTKWidget();
	m_renderPreview->setGeometry(0,0,200,200);

	m_renderPreview->setSizePolicy(policy);
	m_renderPreview->updateGeometry();

	layout->addWidget(openImage, 0,0);
	layout->addWidget(openHeader, 0,1);
	layout->addWidget(m_renderPreview,1,0);
	layout->addWidget(m_headerEntries,1,1);
	layout->addWidget(m_horizontalSlider,2,0);

	
    m_imageviewer      	= vtkImageViewer::New();
	m_renderer			= m_imageviewer->GetRenderer();
	m_renderWin			= m_imageviewer->GetRenderWindow();

	m_renderWin->SetSize(300,300);
	m_imageviewer->SetSize(300,300);
    m_renderPreview->SetRenderWindow(m_renderWin);
    m_renderer->SetBackground(1,0,0);

    m_renderPreview->show();

    m_vtkImporter			= vtkImageImport::New();
    m_inputArgs				= new n2d::InputArgs();
	m_dicomHeaderArgs		= new n2d::DicomHeaderArgs();

    QStringList labels;
    labels << tr("Tag") << tr("Value");
    m_headerEntries->setHorizontalHeaderLabels(labels);
    m_headerEntries->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    m_headerEntries->setEnabled(false);


	m_dictionary = m_parent->getDictionary();
	m_importedDictionary = m_parent->getImportedDictionary();

	setLayout(layout);
	
	std::cout<<__PRETTY_FUNCTION__<<m_dictionary<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<m_importedDictionary<<std::endl;

	connect(openImage, SIGNAL(clicked()),this,SLOT(loadInImage()));
	connect(openHeader, SIGNAL(clicked()),this,SLOT(loadIndcmHDR()));
	connect(m_horizontalSlider, SIGNAL(valueChanged(int )),this,SLOT(OnSliderChange(int )));
}

init::~init()
{
	m_vtkImporter->Delete();
    m_imageviewer->Delete();

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

    m_imageviewer->SetInput(connector->GetOutput());
	//This is required otherwise RefCount goes to zero and connector'd be destroyed//
	m_connector = connector.GetPointer();
	
	m_imageviewer->GetActor2D()->SetWidth(200);
	m_imageviewer->GetActor2D()->SetHeight(200);
    m_renderer->ResetCamera();
	m_renderPreview->update();

	std::cout<<m_renderer->GetSize()[0]<<std::endl;
	std::cout<<m_renderer->GetSize()[1]<<std::endl;
	std::cout<<m_renderer->GetCenter()[0]<<std::endl;
	std::cout<<m_renderer->GetCenter()[1]<<std::endl;
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

bool init::OnSliderChange(int z)
{
    m_imageviewer->SetZSlice(z);
	m_renderPreview->GetRenderWindow()->Render();
    return true;
}


bool init::loadIndcmHDR()
{

    m_dcmRefHDRFname = QFileDialog::getOpenFileName(this,"","");
    if(m_dcmRefHDRFname.isEmpty()) return false;
	m_dicomHeaderArgs->dicomheaderfile = m_dcmRefHDRFname.toStdString();
	m_headerImporter	= new n2d::HeaderImporter(*m_dicomHeaderArgs , *m_importedDictionary);

	m_parent->setDicomHeaderImporter(m_headerImporter);
	m_parent->storeDicomHeaderArgs(*m_dicomHeaderArgs);

    try{
        m_headerImporter->Import();
    }catch(...){
        std::cerr<<"Error While Reading Header Information"<<std::endl;
        exit(100);
    }

    n2d::DictionaryType::ConstIterator itr = m_importedDictionary->Begin();
    n2d::DictionaryType::ConstIterator end = m_importedDictionary->End();

    QTableWidgetItem* tagkeyitem  ; 
	QTableWidgetItem* tagvalueitem; 

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
                    QString item1(tagkey.c_str());
                    QString item2(tagvalue.c_str());

                    tagkeyitem   = new QTableWidgetItem(item1);
                    tagvalueitem = new QTableWidgetItem(item2);

                    m_headerEntries->insertRow(row);
                    m_headerEntries->setItem(row,0,tagkeyitem);
                    m_headerEntries->setItem(row,1,tagvalueitem);

                }
        }
        ++itr;

    }

    delete tagkeyitem;
	delete tagvalueitem;
    return true;
}

bool init::validatePage()
{	


	std::cout<<__PRETTY_FUNCTION__<<m_dictionary<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<m_importedDictionary<<std::endl;

	n2d::DicomClassArgs 	dicomClassArgs;
	n2d::AcquisitionArgs 	acquisitionArgs;
	n2d::OtherDicomTagsArgs otherDicomTagsArgs;
	n2d::SeriesArgs			seriesArgs;
	n2d::StudyArgs			studyArgs;
	n2d::PatientArgs		patientArgs;

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

void init::resizeEvent ( QResizeEvent * event )
{
	QWizardPage::resizeEvent(event);
	if(m_renderer!=NULL && m_renderPreview!=NULL)
	{
		m_imageviewer->SetSize(100,100);
		m_renderWin->SetSize(100,100);
		m_renderer->ResetCamera();
		m_renderPreview->update();
	}

}

}//namespace gui
}//namespace n2d
