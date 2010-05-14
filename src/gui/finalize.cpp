#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>

#include "../core/n2dInputFilter.h"
#include "../core/n2dInstance.h"
#include "../core/n2dOutputExporter.h"
#include "../core/n2dInputImporter.h"
#include "../core/n2dDefsImage.h"
#include "../core/n2dAccessionNumberValidator.h"
#include "../core/n2dDefsCommandLineArgsStructs.h"

#include "finalize.h"
#include "wizard.h"

namespace n2d{
namespace gui	{

finalize::finalize(QWidget* parent):QWizardPage(parent)
{
	m_parent	 = dynamic_cast<n2d::gui::Wizard* >(parent);
	m_dictionary = m_parent->getDictionary();
    
    this->setTitle("Last Step");
    this->setSubTitle("Review the final header, fill the output directory and the accession number");
	//Styling grid layout
	QGridLayout *baselayout			= new QGridLayout();
	QGridLayout *rightlayout		= new QGridLayout();
	QGridLayout *leftlayout			= new QGridLayout();
	m_outDirLine 					= new QLineEdit();
	m_accessionNumberLine			= new QLineEdit();
	QLabel *label1					= new QLabel("Output directory");
	QLabel *label2					= new QLabel("Accession Number");
	m_rescaleBox					= new QCheckBox("Rescale");
	m_headerTable					= new QTableWidget(0,2);
	m_digits 						= 4;

	m_headerTable->setColumnWidth(0,100);
	m_headerTable->setColumnWidth(1,200);

	
	QStringList labels;
    labels << tr("Tag") << tr("Value");
    m_headerTable->setHorizontalHeaderLabels(labels);
    m_headerTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

	rightlayout->addWidget(m_outDirLine,0,1);
	rightlayout->addWidget(label1,0,0);
	rightlayout->addWidget(m_accessionNumberLine,1,1);
	rightlayout->addWidget(label2,1,0);
	rightlayout->addWidget(m_rescaleBox,2,0);

	leftlayout->addWidget(m_headerTable,1,0);
	
	baselayout->addLayout(leftlayout,0,1);
	baselayout->addLayout(rightlayout,0,0);
	setLayout(baselayout);

	connect(m_accessionNumberLine,SIGNAL(editingFinished()),this, SLOT(OnAccessionNumberChange()));
	connect(m_outDirLine,SIGNAL(editingFinished()),this, SLOT(OnOutputDirectoryChange()));

	registerField("gino* ", m_accessionNumberLine);
	
	std::cout<<__PRETTY_FUNCTION__<<m_dictionary<<std::endl;
}

void finalize::initializePage()
{

	m_image = m_parent->getInputImporter()->getImportedImage();
	n2d::DictionaryType::ConstIterator itr = m_dictionary->Begin();
    n2d::DictionaryType::ConstIterator end = m_dictionary->End();
	
    while(itr != end)
    {
        int row = m_headerTable->rowCount();
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryvalue = 
			dynamic_cast<MetaDataStringType* >(entry.GetPointer());

		if(entryvalue)
        {
                std::string tagkey  = itr->first;
                std::string tagvalue= 
						entryvalue->GetMetaDataObjectValue();
                QString item1(tagkey.c_str());
                QString item2(tagvalue.c_str());

    			QTableWidgetItem* tagkeyitem = 
					new QTableWidgetItem(item1); 
				QTableWidgetItem* tagvalueitem = 
					new QTableWidgetItem(item2);

				tagkeyitem->setFont(QFont("Verdana",10));
				tagvalueitem->setFont(QFont("Verdana",10));

                m_headerTable->insertRow(row);
                m_headerTable->setItem(row,0,tagkeyitem);
                m_headerTable->setItem(row,1,tagvalueitem);

        }
        ++itr;

    }
}

void finalize::OnAccessionNumberChange()
{
	m_accessionNumber = m_accessionNumberLine->text().toStdString();
	std::cout<<__PRETTY_FUNCTION__<<m_accessionNumber<<std::endl;
	completeChanged();
}

void finalize::OnOutputDirectoryChange()
{
	m_outputDirectory = m_outDirLine->text().toStdString(); 
	std::cout<<__PRETTY_FUNCTION__<<m_accessionNumber<<std::endl;
	completeChanged();
}


bool finalize::validatePage()
{

	n2d::PixelType inputPixelType 			= m_parent->getInputImporter()->getPixelType();
    n2d::DICOMImageIOType::Pointer dicomIO	= n2d::DICOMImageIOType::New();
    n2d::DICOM3DImageType::ConstPointer 	filteredImage;
	n2d::FiltersArgs 						filtersArgs;
	n2d::InstanceArgs 						instanceArgs;
	n2d::OutputArgs							outputArgs;
	n2d::AccessionNumberArgs				accessionNumberArgs;


	filtersArgs.rescale 					= m_rescaleBox->checkState();
	outputArgs.outputdirectory				= m_outputDirectory;
	outputArgs.suffix						= ".dcm";//m_suffix;
	outputArgs.prefix						= "N2D"; //m_prefix;
	outputArgs.digits						= m_digits;
	accessionNumberArgs.accessionnumber		= m_accessionNumber;

	std::cout<<m_accessionNumber<<std::endl;


	std::cout<<__PRETTY_FUNCTION__<<m_dictionary<<std::endl;


//BEGIN DICOM accession number validation
    try
    {
        n2d::AccessionNumberValidator accessionNumberValidator(accessionNumberArgs, *m_dictionary);
        if (!accessionNumberValidator.Validate())
        {
            std::cerr << "ERROR in \"DICOM accession number validation\"." << std::endl;
			return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"DICOM accession number validation\"." << std::endl;
		return false;
    }
//END DICOM accession number validation

//BEGIN Input filtering
    try
    {
        n2d::InputFilter inputFilter(filtersArgs, m_image, inputPixelType, *m_dictionary);
        if (!inputFilter.Filter())
        {
            std::cerr << "ERROR in \"Input filtering\"." << std::endl;
			return false;
        }
        filteredImage = inputFilter.getFilteredImage();
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Input filtering\"." << std::endl;
        return false;
    }
//END Input filtering



//BEGIN Instance
    try
    {
        n2d::Instance instance(instanceArgs, filteredImage, *m_dictionary, m_dictionaryArray);
        if (!instance.Update())
        {
            std::cerr << "ERROR in \"Instance\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Instance\"." << std::endl;
        return false;
    }
//END Instance


	std::cout<<std::setw(20)<<m_accessionNumber<<outputArgs.outputdirectory<<std::endl;

//BEGIN Output
    try
    {

        n2d::OutputExporter outputExporter(outputArgs, filteredImage, m_dictionaryArray, dicomIO);
        if (!outputExporter.Export())
        {
            std::cerr << "ERROR in \"Output\"." << std::endl;
            return false;
        }
    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Output\"." << std::endl;
		return false;
	}
//END Output

	return true;
}

bool finalize::isComplete() const 
{
	if(m_accessionNumber.empty() || 
		m_outputDirectory.empty())
		return false;
	else 
		return true;
}

}//namespace gui
}//namespace n2d
