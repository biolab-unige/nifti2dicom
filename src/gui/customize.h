#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include <QtGui/QWizardPage>

namespace n2d{
namespace gui{

class QWidget;

class customize : public QWizardPage
{
   Q_OBJECT
   
   public:
     customize(QWidget parent=0);

};
}
}
#endif
