#ifndef WIZARD_H
#define WIZARD_H

#include <QtGui/QWizard>
#include <n2dDefsCommandLineArgsStructs.h>
#include <n2dDefsMetadata.h>
#include <QtTest/QSignalSpy>

#include "vtkKWImage.h"

//////////////////////////////////////
// FIXME CONTROL FOR NULL POINTERS  //
//////////////////////////////////////

namespace n2d{

class HeaderImporter;


namespace gui{

class Wizard : public QWizard
{
        Q_OBJECT
public:
    Wizard(QWizard* parent=0);
    virtual ~Wizard();

    inline const n2d::AccessionNumberArgs& 	getaccessionNumberArgs()const{return m_accessionNumberArgs;}
    inline const n2d::DicomHeaderArgs&     	getdicomHeaderArgs()const{return m_dicomHeaderArgs;}
    inline const n2d::DicomClassArgs&      	getdicomClassArgs()const{return m_dicomClassArgs;}
    inline const n2d::OtherDicomTagsArgs&  	getotherDicomTagsArgs()const{return m_otherDicomTagsArgs;}
    inline const n2d::PatientArgs&         	getpatientArgs()const{return m_patientArgs;}
    inline const n2d::StudyArgs&           	getstudyArgs()const{return m_studyArgs;}
    inline const n2d::SeriesArgs&          	getseriesArgs()const{return m_seriesArgs;}
    inline const n2d::AcquisitionArgs&     	getacquisitionArgs()const{return m_acquisitionArgs;}
    inline const n2d::InputArgs&           	getinputArgs()const{return m_inputArgs;}
    inline const n2d::FiltersArgs&         	getfiltersArgs()const{return m_filtersArgs;}
    inline const n2d::InstanceArgs&        	getinstanceArgs()const{return m_instanceArgs;}
    inline const n2d::OutputArgs&          	getoutputArgs()const{return m_outputArgs;}

    inline n2d::DictionaryType*			getImportedDictionary(){return &m_importedDictionary;}
    inline n2d::DictionaryType*			getDictionary(){return &m_dictionary;}
    inline void					setImportedImage( vtkKWImage* in) {m_importedImage = in;}
    inline n2d::PixelType			getImportedPixelType()const{return m_importedImage->GetITKScalarPixelType();}
    inline n2d::ImageType::ConstPointer		getImportedImage()const{return m_importedImage->GetITKImageBase();}
    inline void 				setDicomHeaderImporter(n2d::HeaderImporter* a){m_HeaderImporter = a;}
    inline void 				setImportedDictionary(n2d::DictionaryType& b){m_importedDictionary = b;}
    
    inline QSignalSpy*				getSpy()const{return m_spy;}
    inline void					setSpy(QSignalSpy* in){m_spy=in;}

public slots:
   inline void storeAccessionNumberArgs(n2d::AccessionNumberArgs& a){m_accessionNumberArgs = a;}
   inline void storeDicomHeaderArgs(n2d::DicomHeaderArgs& a){m_dicomHeaderArgs = a;}
   inline void storeDicomClassArgs(n2d::DicomClassArgs& a){m_dicomClassArgs = a;}
   inline void storeOtherDicomTagsArgs(n2d::OtherDicomTagsArgs& a){m_otherDicomTagsArgs = a;}
   inline void storePatientArgs(n2d::PatientArgs& a){m_patientArgs = a;}
   inline void storeStudyArgs(n2d::StudyArgs& a){m_studyArgs = a;}
   inline void storeSeriesArgs(n2d::SeriesArgs& a){m_seriesArgs = a;}
   inline void storeAcquisitionArgs(n2d::AcquisitionArgs& a){m_acquisitionArgs = a;}
   inline void storeInputArgs(n2d::InputArgs& a){m_inputArgs = a;}
   inline void storeFiltersArgs(n2d::FiltersArgs& a){m_filtersArgs = a;}
   inline void storeInstanceArgs(n2d::InstanceArgs& a){m_instanceArgs = a;}
   inline void storeOutputArgs(n2d::OutputArgs& a){m_outputArgs = a;}

private slots:
	void showHelp();

private:
//BEGIN Common Structures
    n2d::AccessionNumberArgs	m_accessionNumberArgs;
    n2d::DicomHeaderArgs	m_dicomHeaderArgs;
    n2d::DicomClassArgs		m_dicomClassArgs;
    n2d::OtherDicomTagsArgs 	m_otherDicomTagsArgs;
    n2d::PatientArgs        	m_patientArgs;
    n2d::StudyArgs           	m_studyArgs;
    n2d::SeriesArgs          	m_seriesArgs;
    n2d::AcquisitionArgs     	m_acquisitionArgs;
    n2d::InputArgs           	m_inputArgs;
    n2d::FiltersArgs         	m_filtersArgs;
    n2d::InstanceArgs        	m_instanceArgs;
    n2d::OutputArgs          	m_outputArgs;
//END Structures 
	
//BEGIN Common object
    vtkKWImage*			m_importedImage;
//END Common Object

//BEGIN n2dClassess
    n2d::HeaderImporter* 	m_HeaderImporter;
//END n2dClasses

//BEGIN n2dDictionaryObject
    n2d::DictionaryType		m_importedDictionary;
    n2d::DictionaryType		m_dictionary;
//END n2dDictionaryObject


    QSignalSpy*			m_spy;
};

}//namespace gui
}//namespace n2d
#endif // WIZARD_H
