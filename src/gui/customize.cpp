#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

#include "../core/n2dDefsMetaData.h"

#include "customize.h"
#include "wizard.h"

namespace n2d{
namespace gui{

customize::customize(QWidget* parent):QWizardPage(parent)
{
	m_parent = dynamic_cast<n2d::gui::Wizard* >(parent);
	
	QStringList labels;
	m_dicomTable = new QTableWidget(0,2,this);
	m_dicomTable->setColumnWidth(0,100);
	m_dicomTable->setColumnWidth(1,450);
	
	m_dicomTable->setGeometry(0, 0, 600, 500);

    labels << tr("Tag") << tr("Value");
    m_dicomTable->setHorizontalHeaderLabels(labels);
	n2d::DictionaryType m_dictionary = m_parent->getDictionary();

}

void customize::initializePage()
{

    n2d::DictionaryType::ConstIterator itr = m_dictionary.Begin();
    n2d::DictionaryType::ConstIterator end = m_dictionary.End();

    while(itr != end)
    {
        int row = m_dicomTable->rowCount();
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
					new QTableWidgetItem(item1)  ; 
				QTableWidgetItem* tagvalueitem = 
					new QTableWidgetItem(item2);

                m_dicomTable->insertRow(row);
                m_dicomTable->setItem(row,0,tagkeyitem);
                m_dicomTable->setItem(row,1,tagvalueitem);

        }
        ++itr;

    }
	
	connect( m_dicomTable, SIGNAL(itemChanged(QTableWidgetItem* )), 
				this, SLOT( OnItemChange(QTableWidgetItem* ) ));
}

bool customize::OnItemChange(QTableWidgetItem* item)
{
	std::string value = item->data(0).toString().toStdString();
	QTableWidgetItem* itemTag = m_dicomTable->itemAt(item->row(),0);
	std::string tag = itemTag->data(0).toString().toStdString();
	std::cout<<"Save into m_dictionaryionary tag: ("<<tag<<") = ["<<value<<"]"<<std::endl;

	itk::EncapsulateMetaData<std::string>(m_dictionary, tag, value);
	return true;
}

bool customize::validatePage()
{

	m_parent->setDictionary(m_dictionary);
	return true;
}


}//namespace gui
}//namespace n2d
