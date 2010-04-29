#ifndef INIT_H
#define INIT_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>
#include <QString>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "itkCommand.h"
#include "n2dDefsIO.h"
#include "n2dDefsImageIO.h"

#include "QVTKWidget.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkCurvatureFlowImageFilter.h"
#include "vtkImageViewer.h"
#include "vtkImageData.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"


namespace Ui {
    class init;
}
namespace n2d{
namespace gui{

class n2d::InputImporter;

class init : public QWizardPage, public n2d::InputImporter {
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

    typedef itk::VTKImageExport< ImageType > 		ExportFilterType;
    typedef itk::MetaDataObject< std::string> 		MetaDataStringType;

    vtkImageImport* 			vtkImporter;
    ExportFilterType::Pointer 	itkExporter;


    template <typename ITK_Exporter, typename VTK_Importer>
    void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer);

private slots:
    bool loadInImage();
    bool loadIndcmHDR();
    bool OnSliderChange();
};
}
}
#endif // INIT_H
