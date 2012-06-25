//  This file is part of Nifti2Dicom, is an open source converter from
//  3D NIfTI images to 2D DICOM series.
//
//  Copyright (C) 2010 Gabriele Arnulfo <gabriele.arnulfo@dist.unige.it>
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


#include <QtGui/QAbstractButton>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>

#include "wizard.h"
#include <n2dInputImporter.h>
#include <n2dHeaderImporter.h>

namespace n2d{
namespace gui{

Wizard::Wizard(QWizard* parent):QWizard(parent)
{

    setWindowTitle(tr("QNifti2Dicom"));
    this->setGeometry(10,10,1000,600);

    setOption(QWizard::HaveHelpButton,true);
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    connect(button(QWizard::HelpButton),SIGNAL(clicked())
			,this,SLOT(showHelp()));

}

Wizard::~Wizard()
{
}

void Wizard::showHelp()
{

     QString message;

     switch (currentId()) 
	 {
     case 1:
     	message = tr("In this first page you should provide the nifti image to convert and optionally a reference DICOM header from which copy anagraphical data of the patient ");
        break;
     case 2:
	 	message = tr("In this page you should edit DICOM field filling proper information required for the output header. To do this simply right-click on the proper row. At the end press enter to confirm your change for the selected tag.");
		break;

	 case 3:
	 	message = tr("In this page you would be asked for an output directory where all the dicom slices will be written to and the accession Number. All those fields are mandatory. At the right side of the page you could review the final header. Nothing has been allready written to the final files so you could go back to previews page in order to edit correctly the header tags");
		break;

     default:
         message = tr("This Dialog will contain information for the current step");
     }

     QMessageBox::information(this, tr("QNifti2Dicom Help dialog"), message);
	
}

}//namespace gui
}//namespace n2d



