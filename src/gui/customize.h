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
