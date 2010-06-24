#ifndef INIT_H
#define INIT_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>
#include <QString>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "QVTKWidget.h"

#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"

#include <n2dDefsImage.h>
#include <n2dDefsIO.h>
#include <n2dDefsMetadata.h>

class QTableWidget;
class QSlider;
class QFont;
class vtkKWImageIO;
class vtkKWImage;

namespace n2d{

class InputImporter;
class HeaderImporter;

struct InputArgs;
struct DicomHeaderArgs;	

namespace gui{

class Wizard;

class init : public QWizardPage {
    Q_OBJECT
public:
    init(QWidget *parent = 0);
    ~init();


private:
    QString	 			m_inFname;
    QString 	 			m_dcmRefHDRFname;
    QVTKWidget*  			m_renderPreview;
    vtkImageViewer2* 			m_imageviewer;
    vtkRenderer* 			m_renderer;
    vtkRenderWindow* 			m_renderWin;
    vtkKWImageIO*			m_reader;
    vtkKWImage*				m_localVTKImage;
    n2d::HeaderImporter*		m_headerImporter;
    Wizard*				m_parent;
    n2d::DictionaryType* 		m_importedDictionary;
    n2d::DictionaryType*		m_dictionary;
    QTableWidget*			m_headerEntries;
    QSlider*				m_horizontalSlider;
    QFont*				m_font;

    n2d::InputArgs* 			m_inputArgs;
    n2d::DicomHeaderArgs*		m_dicomHeaderArgs;


private slots:
    bool loadInImage();
    bool loadIndcmHDR();
    bool OnSliderChange(int );
    bool validatePage();
    bool isComplete() const;
};
}//namespace gui
}//namespace n2d
#endif // INIT_H
