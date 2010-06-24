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
  	std::cout<<__PRETTY_FUNCTION__<<std::endl;

    setWindowTitle(tr("QNifti2Dicom"));
    this->setGeometry(10,10,1000,600);

    setOption(QWizard::HaveHelpButton,true);
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    connect(button(QWizard::HelpButton),SIGNAL(clicked())
			,this,SLOT(showHelp()));

}

Wizard::~Wizard()
{
   std::cout<<"Called ~Wizard"<<std::endl;
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



