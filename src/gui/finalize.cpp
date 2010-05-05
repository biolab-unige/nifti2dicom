#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>

#include "finalize.h"
#include "wizard.h"

namespace n2d{
namespace gui	{

finalize::finalize(QWidget* parent):QWizardPage(parent)
{
	m_parent	 = dynamic_cast<n2d::gui::Wizard* >(parent);
	m_dictionary = m_parent->getDictionary();

	//Styling grid layout
	QGridLayout *baselayout			= new QGridLayout();
	QGridLayout *rightlayout		= new QGridLayout();
	QGridLayout *leftlayout			= new QGridLayout();
	QLineEdit *outDirLine 			= new QLineEdit();
	QLineEdit *accessionNumberLine	= new QLineEdit();
	QCheckBox *rescaleBox			= new QCheckBox("Rescale");
	QLabel *label1					= new QLabel("Output directory");
	QLabel *label2					= new QLabel("Accession Number");
	QPushButton	*goButton			= new QPushButton("Go");
	m_headerTable					= new QTableWidget(0,2);

	m_headerTable->setColumnWidth(0,100);
	m_headerTable->setColumnWidth(1,200);
	

	QStringList labels;
    labels << tr("Tag") << tr("Value");
    m_headerTable->setHorizontalHeaderLabels(labels);
    m_headerTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    m_headerTable->setEnabled(false);

	rightlayout->addWidget(outDirLine,0,1);
	rightlayout->addWidget(label1,0,0);
	rightlayout->addWidget(accessionNumberLine,1,1);
	rightlayout->addWidget(label2,1,0);
	rightlayout->addWidget(rescaleBox,2,0);

	leftlayout->addWidget(goButton,0,0);
	leftlayout->addWidget(m_headerTable,1,0);
	
	baselayout->addLayout(leftlayout,0,1);
	baselayout->addLayout(rightlayout,0,0);
	setLayout(baselayout);
	
	std::cout<<__PRETTY_FUNCTION__<<m_dictionary<<std::endl;
}

void finalize::initializePage()
{

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

                m_headerTable->insertRow(row);
                m_headerTable->setItem(row,0,tagkeyitem);
                m_headerTable->setItem(row,1,tagvalueitem);

        }
        ++itr;

    }
}

bool finalize::validatePage()
{
	return true;
}

}//namespace gui
}//namespace n2d
