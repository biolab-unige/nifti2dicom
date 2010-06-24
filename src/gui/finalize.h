#ifndef FINALIZE_H
#define FINALIZE_H

#include <QtGui/QWizardPage>
#include <n2dDefsMetadata.h>
#include <n2dDefsImage.h>
#include <n2dDefsIO.h>

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
		~finalize();

	private:
		Wizard*					m_parent;
		n2d::DictionaryType*			m_dictionary;
		n2d::ImageType::ConstPointer 		m_image;
		QTableWidget*				m_headerTable;
		QCheckBox*				m_rescaleBox;
		QLineEdit*				m_outDirLine;			
		QLineEdit*				m_accessionNumberLine;


		n2d::OutputExporter*			m_Exporter;

		std::string				m_outputDirectory;
		std::string				m_suffix;
		std::string				m_prefix;
		std::string				m_accessionNumber;
		int					m_digits;
		n2d::DictionaryArrayType		m_dictionaryArray;

		void initializePage();
		bool validatePage();
		bool isComplete() const;

	private slots:
		void OnAccessionNumberChange(const QString & );
		void OnOutputDirectoryChange(const QString &);
		void OnBrowseClick();
		
};

}
}
#endif
