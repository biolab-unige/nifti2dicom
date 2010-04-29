#include <iostream>
#include <QtGui/QApplication>
#include <QtGui/QWizard>
#include "init.h"
#include "customize.h"
#include "finalize.h"
#include "wizard.h"

int main(int argc, char* argv[])
{
        QApplication app(argc,argv);
        n2d::gui::Wizard* wiz   = new n2d::gui::Wizard(0);
        n2d::gui::init* page1     = new n2d::gui::init(wiz);
        //custom* page2   = new custom(wiz);
        //finalize* page3 = new finalize(wiz);


        wiz->addPage(page1);
       // wiz->addPage(page2);
       // wiz->addPage(page3);

        wiz->show();


        return app.exec();
}
