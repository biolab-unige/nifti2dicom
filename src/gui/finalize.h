#ifndef FINALIZE_H
#define FINALIZE_H

#include <QtGui/QWizardPage>
#include "../core/n2dDefsMetaData.h"
#include "../core/n2dDefsImage.h"
#include "../core/n2dDefsIO.h"

class QWidget;
class QTableWidget;
class QCheckBox;
class QLineEdit;

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
		Wizard*							m_parent;
		n2d::DictionaryType*			m_dictionary;
		n2d::ImageType::ConstPointer 	m_image;
		QTableWidget*					m_headerTable;
		QCheckBox*						m_rescaleBox;
		QLineEdit*						m_outDirLine;			
		QLineEdit*						m_accessionNumberLine;


		n2d::OutputExporter*		m_Exporter;

		std::string					m_outputDirectory;
		std::string					m_suffix;
		std::string					m_prefix;
		std::string					m_accessionNumber;
		int							m_digits;
		n2d::DictionaryArrayType	m_dictionaryArray;

		void initializePage();
		bool validatePage();

	private slots:
		void OnAccessionNumberChange( );
		void OnOutputDirectoryChange( );
		
};

}
}
#endif
