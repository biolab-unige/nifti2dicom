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


#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>

#include <gdcmDict.h>
#include <gdcmGlobal.h>
#include <gdcmDicts.h>


#include <n2dDefsMetadata.h>
#include <n2dToolsMetaDataDictionary.h>

#include "customize.h"
#include "wizard.h"

namespace n2d{
namespace gui{

customize::customize(QWidget* parent):QWizardPage(parent)
{

	m_parent	 = dynamic_cast<n2d::gui::Wizard* >(parent);
	
	this->setTitle("Second Step");
	this->setSubTitle("Customize dicom header field clicking on proper "
						"row and the type the desired value");

	QStringList labels;
	QVBoxLayout* layout = new QVBoxLayout();

	m_dicomTable = new QTableWidget(0,3);
	m_dicomTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	m_dicomTable->setColumnWidth(0,100);
	m_dicomTable->setColumnWidth(1,300);
	m_dicomTable->setColumnWidth(2,200);

	labels << tr("Tag") << tr("Value") <<tr("Desc");
	m_dicomTable->setHorizontalHeaderLabels(labels);
	m_dictionary = m_parent->getDictionary();

	layout->addWidget(m_dicomTable);

	setLayout(layout);
		
}
customize::~customize()
{
    //std::cout<<"Called ~customize"<<std::endl;
}
void customize::initializePage()
{

    n2d::DictionaryType::ConstIterator itr = m_dictionary->Begin();
    n2d::DictionaryType::ConstIterator end = m_dictionary->End();

    const gdcm::Global& g = gdcm::Global::GetInstance();
    const gdcm::Dicts &dicts = g.GetDicts();
    const gdcm::Dict &pub = dicts.GetPublicDict(); // Part 6

	
    while(itr != end)
    {
        int row = m_dicomTable->rowCount();
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryvalue = 
		dynamic_cast<MetaDataStringType* >(entry.GetPointer());

		if(entryvalue)
        {
			std::string tagkey  = itr->first;
			std::string tagvalue=  entryvalue->GetMetaDataObjectValue();
			int a = 0;
			int b = 0;

			sscanf(tagkey.substr(0,4).c_str(), "%x", &a);
			sscanf(tagkey.substr(5,4).c_str(), "%x", &b);

			gdcm::Tag t(a,b);
			const gdcm::DictEntry &entry1 = pub.GetDictEntry(t);


			QString item1(tagkey.c_str());
			QString item2(tagvalue.c_str());
			QString item3(entry1.GetName());

    		QTableWidgetItem* tagkeyitem = 
		    	new QTableWidgetItem(item1,Qt::ItemIsEditable); 
			QTableWidgetItem* tagvalueitem = 
			    new QTableWidgetItem(item2,Qt::ItemIsEditable);
    		QTableWidgetItem* desc = 
			    new QTableWidgetItem(item3);

			m_dicomTable->insertRow(row);
			m_dicomTable->setItem(row,0,tagkeyitem);
			m_dicomTable->setItem(row,1,tagvalueitem);
			m_dicomTable->setItem(row,2,desc);

        }
        ++itr;
    }
	connect( m_dicomTable, SIGNAL(itemChanged(QTableWidgetItem* )), 
				this, SLOT( OnItemChange(QTableWidgetItem* ) ));
}

bool customize::OnItemChange(QTableWidgetItem* item)
{
	std::string value = item->data(0).toString().toStdString();
	QTableWidgetItem* itemTag = m_dicomTable->item(item->row(),0);
	std::string tag = itemTag->data(0).toString().toStdString();
	itk::EncapsulateMetaData<std::string>(*m_dictionary, tag, value);
	
	return true;
}

bool customize::validatePage()
{
	return true;
}


}//namespace gui
}//namespace n2d
