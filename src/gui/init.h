#ifndef INIT_H
#define INIT_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>
#include <QString>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "QVTKWidget.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkImageViewer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"

#include "../core/n2dDefsImage.h"
#include "../core/n2dDefsIO.h"


namespace Ui {
    class init;
}
class vtkImageImport;

namespace n2d{

class InputImporter;
struct InputArgs;
namespace gui{

class Wizard;

class init : public QWizardPage {
    Q_OBJECT
public:
    init(QWidget *parent = 0);
    ~init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::init 					*ui;
    QString	 					m_inFname;
    QString 	 				m_dcmRefHDRFname;
    QVTKWidget*  				renderPreview;
    vtkImageViewer* 			imageviewer;
    vtkRenderer* 				renderer;
    vtkRenderWindow* 			renderWin;
    vtkRenderWindowInteractor*	renderInteractor;
	vtkImageImport*				m_vtkImporter;
	n2d::InputImporter*			m_inputImporter;
    Wizard*						m_parent;

    n2d::InputArgs* 			m_inputArgs;

    template <typename ITK_Exporter, typename VTK_Importer>
    void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer);

    template<class TPixel>bool showImage(n2d::ImageType::Pointer );

private slots:
    bool loadInImage();
    bool loadIndcmHDR();
    bool OnSliderChange();
};
}//namespace gui
}//namespace n2d
#endif // INIT_H
