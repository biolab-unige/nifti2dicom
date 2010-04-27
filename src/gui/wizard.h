#ifndef WIZARD_H
#define WIZARD_H

#include <QtGui/QWizard>
namespace n2d{
namespace gui{

class Wizard : public QWizard
{
        Q_OBJECT
public:
   Wizard(QWizard* parent=0);
};

}//namespace gui
}//namespace n2d
#endif // WIZARD_H
