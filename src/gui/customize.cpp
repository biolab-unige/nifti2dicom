#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>
#include <QtGui/QHeaderView>

#include "../core/n2dDefsMetaData.h"
#include "../core/n2dToolsMetaDataDictionary.h"

#include "customize.h"
#include "wizard.h"

namespace n2d{
namespace gui{

customize::customize(QWidget* parent):QWizardPage(parent)
{
	m_parent	 = dynamic_cast<n2d::gui::Wizard* >(parent);
	
	QStringList labels;
	m_dicomTable = new QTableWidget(0,2,this);
	m_dicomTable->setColumnWidth(0,100);
	m_dicomTable->setColumnWidth(1,450);
    m_dicomTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	
	m_dicomTable->setGeometry(0, 0, 600, 500);

    labels << tr("Tag") << tr("Value");
    m_dicomTable->setHorizontalHeaderLabels(labels);
	m_dictionary = m_parent->getDictionary();

	std::cout<<__PRETTY_FUNCTION__<<m_dictionary<<std::endl;
	
}

void customize::initializePage()
{

    n2d::DictionaryType::ConstIterator itr = m_dictionary->Begin();
    n2d::DictionaryType::ConstIterator end = m_dictionary->End();
	
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
					new QTableWidgetItem(item1,Qt::ItemIsEditable); 
				QTableWidgetItem* tagvalueitem = 
					new QTableWidgetItem(item2,Qt::ItemIsEditable);

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

	itk::EncapsulateMetaData<std::string>(*m_dictionary, tag, value);
	return true;
}

bool customize::validatePage()
{
	return true;
}


}//namespace gui
}//namespace n2d
