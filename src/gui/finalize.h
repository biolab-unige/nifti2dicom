#ifndef FINALIZE_H
#define FINALIZE_H

#include <QtGui/QWizardPage>
#include "../core/n2dDefsMetaData.h"
#include "../core/n2dDefsImage.h"
#include "../core/n2dDefsIO.h"

class QWidget;
class QTableWidget;

namespace n2d{

class OutputExporter;

namespace gui{

class Wizard;

class finalize: public QWizardPage
{
	Q_OBJECT
	public:
		finalize(QWidget* parent=0);

	private:
		Wizard*						m_parent;
		n2d::DictionaryType*		m_dictionary;
		n2d::ImageType::Pointer 	m_image;
		QTableWidget*				m_headerTable;

		void initializePage();
		bool validatePage();


};

}
}
#endif
