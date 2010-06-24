#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include <QtGui/QWizardPage>
#include <n2dDefsMetadata.h>

class QWidget;
class QTableWidget;
class QTableWidgetItem;

namespace n2d{
namespace gui{

class Wizard;

class customize : public QWizardPage
{
   Q_OBJECT

	public:
	      customize(QWidget* parent=0);
	      ~customize();

	private slots:
	      bool OnItemChange(QTableWidgetItem* );
	private:

	      Wizard* 			m_parent;
	      QTableWidget* 		m_dicomTable;
	      n2d::DictionaryType*	m_dictionary;

	      virtual void initializePage();
	      virtual bool validatePage();


};
}//namespace gui
}//namespace n2d
#endif
