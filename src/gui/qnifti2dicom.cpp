#include <iostream>
#include <QtGui/QApplication>
#include <QtGui/QWizard>
#include <QtGui/QMainWindow>
#include "init.h"
#include "customize.h"
#include "finalize.h"
#include "wizard.h"

int main(int argc, char* argv[])
{
        QApplication app(argc,argv);

        n2d::gui::Wizard*	wiz	= new n2d::gui::Wizard(0);
        n2d::gui::init*		page1	= new n2d::gui::init(wiz);
        n2d::gui::customize*	page2	= new n2d::gui::customize(wiz);
        n2d::gui::finalize* 	page3 	= new n2d::gui::finalize(wiz);

        wiz->addPage(page1);
        wiz->addPage(page2);
        wiz->addPage(page3);

        wiz->show();
	
	int ret = app.exec();
	
	delete wiz;

        return ret;
}
