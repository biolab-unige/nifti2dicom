//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2010,2012 Gabriele Arnulfo <gabriele.arnulfo@dist.unige.it>
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


#include <QtCore/QCoreApplication>
#include <QtCore/qglobal.h>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QGridLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>
#include <QtGui/QProgressBar>
#else
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#endif

#include <n2dInputFilter.h>
#include <n2dInstance.h>
#include <n2dOutputExporter.h>
#include <n2dInputImporter.h>
#include <n2dDefsImage.h>
#include <n2dAccessionNumberValidator.h>
#include <n2dDefsCommandLineArgsStructs.h>

#include "finalize.h"
#include "wizard.h"

namespace n2d{
namespace gui	{

finalize::finalize(QWidget* parent):QWizardPage(parent)
{

	m_parent		= dynamic_cast<n2d::gui::Wizard* >(parent);
	m_dictionary 	= m_parent->getDictionary();
    
	this->setTitle("Last Step");
	this->setSubTitle("Review the final header, fill the output directory and the accession number");
	//Styling grid layout
	QGridLayout *baselayout			= new QGridLayout();
	QGridLayout *rightlayout		= new QGridLayout();
	QGridLayout *leftlayout			= new QGridLayout();
	QGridLayout *browselayout		= new QGridLayout();
	QProgressBar *progressBar		= new QProgressBar();
	QLineEdit	 *progressInfo		= new QLineEdit();
	m_outDirLine 					= new QLineEdit();
	m_accessionNumberLine			= new QLineEdit();
	QLabel *label1					= new QLabel("Output directory");
	QLabel *label2					= new QLabel("Accession Number");
	m_rescaleBox					= new QCheckBox("Rescale");
	m_headerTable					= new QTableWidget(0,2);
	QPushButton *browseFile			= new QPushButton("Browse");
	
	m_digits 						= 4;
	m_headerTable->setColumnWidth(0,100);
	m_headerTable->setColumnWidth(1,200);

	QStringList labels;
	labels << tr("Tag") << tr("Value");
	m_headerTable->setHorizontalHeaderLabels(labels);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
	m_headerTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
#else
    m_headerTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
#endif
    browselayout->addWidget(m_outDirLine,0,1);
	browselayout->addWidget(browseFile,0,0);
	
	rightlayout->addLayout(browselayout,0,1);
	rightlayout->addWidget(label1,0,0);
	rightlayout->addWidget(m_accessionNumberLine,1,1);
	rightlayout->addWidget(label2,1,0);
	rightlayout->addWidget(m_rescaleBox,2,0);

	leftlayout->addWidget(m_headerTable,1,0);
	
	baselayout->addLayout(leftlayout,0,1);
	baselayout->addLayout(rightlayout,0,0);
	baselayout->addWidget(progressBar,1,0);
	baselayout->addWidget(progressInfo,1,1);


//	progressbar->setMaximum(4);
//	progressbar->setMinimum(0);
	progressBar->setRange(0,3);
	progressInfo->setReadOnly(1);

	setLayout(baselayout);
	
	connect(m_accessionNumberLine,SIGNAL(textChanged(const QString & )),this, 
				SLOT(OnAccessionNumberChange(const QString &)));
	connect(m_outDirLine,SIGNAL(textChanged(const QString & )),this, 
				SLOT(OnOutputDirectoryChange(const QString & )));
	connect(browseFile,SIGNAL(clicked()),this,SLOT(OnBrowseClick()));	
}

void finalize::initializePage()
{

    m_image = m_parent->getImportedImage();
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

void finalize::OnAccessionNumberChange(const QString& in)
{
	m_accessionNumber =in.toStdString();
	completeChanged();
}

void finalize::OnOutputDirectoryChange(const QString& in)
{
	m_outputDirectory = in.toStdString(); 
	completeChanged();
}
finalize::~finalize()
{
//    std::cout<<"Called ~finalize"<<std::endl;
}

bool finalize::validatePage()
{

    n2d::PixelType inputPixelType 			= m_parent->getImportedPixelType();
    n2d::DICOMImageIOType::Pointer dicomIO	= n2d::DICOMImageIOType::New();

    dicomIO->KeepOriginalUIDOn(); // Preserve the original DICOM UID of the input files
    dicomIO->UseCompressionOff();
    n2d::DICOM3DImageType::ConstPointer filteredImage;
    n2d::FiltersArgs 					filtersArgs;
    n2d::InstanceArgs 					instanceArgs;
    n2d::OutputArgs						outputArgs;
    n2d::AccessionNumberArgs			accessionNumberArgs;

    filtersArgs.rescale 				= m_rescaleBox->checkState();
    outputArgs.outputdirectory			= m_outputDirectory;
    outputArgs.suffix					= ".dcm";//m_suffix;
    outputArgs.prefix					= "N2D"; //m_prefix;
    outputArgs.digits					= m_digits;
    accessionNumberArgs.accessionnumber	= m_accessionNumber;

	QGridLayout *tmp_layout			= dynamic_cast<QGridLayout *>(this->layout());
	QLineEdit   *tmp_progressInfo 	= dynamic_cast<QLineEdit *>(tmp_layout->itemAtPosition(1,1)->widget());
	QProgressBar *tmp_progressBar 	= dynamic_cast<QProgressBar *>(tmp_layout->itemAtPosition(1,0)->widget());


	//BEGIN DICOM accession number validation
    try
    {
        n2d::AccessionNumberValidator accessionNumberValidator(accessionNumberArgs, *m_dictionary);
        if (!accessionNumberValidator.Validate())
        {
            std::cerr << "ERROR in \"DICOM accession number validation\"." << std::endl;
			return false;
        }
		tmp_progressInfo->insert("Accession Number Validated");
		tmp_progressBar->setValue(1);
		QCoreApplication::processEvents();
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
		tmp_progressInfo->clear();
		tmp_progressInfo->insert("Volume ranges properly filtered");
		tmp_progressBar->setValue(2);
		QCoreApplication::processEvents();
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
		tmp_progressInfo->clear();
		tmp_progressInfo->insert("DICOM Header built");
		tmp_progressBar->setValue(3);
		QCoreApplication::processEvents();

    }
    catch (...)
    {
        std::cerr << "Unknown ERROR in \"Instance\"." << std::endl;
        return false;
    }
	//END Instance


	//BEGIN Output
    try
    {

        n2d::OutputExporter outputExporter(outputArgs, filteredImage, m_dictionaryArray, dicomIO);
        if (!outputExporter.Export())
        {
            std::cerr << "ERROR in \"Output\"." << std::endl;
            return false;
        }
		tmp_progressInfo->clear();
		tmp_progressInfo->insert("Output Saved to disk");
		tmp_progressBar->setValue(4);
		QCoreApplication::processEvents();
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
	if(m_accessionNumber.empty() || m_outputDirectory.empty())
	{
		return false;
	}
	else 
		return true;
}

void finalize::OnBrowseClick()
{
	m_outDirLine->clear();
	QString outDir =QFileDialog::getExistingDirectory(this,"","") ;
	m_outDirLine->insert(outDir);

}

}//namespace gui
}//namespace n2d
