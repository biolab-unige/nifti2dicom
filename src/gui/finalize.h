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


#ifndef FINALIZE_H
#define FINALIZE_H


#include <QtCore/qglobal.h>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWizardPage>
#else
#include <QtWidgets/QWizardPage>
#endif

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
