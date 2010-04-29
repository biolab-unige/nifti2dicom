#ifndef WIZARD_H
#define WIZARD_H

#include <QtGui/QWizard>
#include "../core/n2dDefsCommandLineArgsStructs.h"

namespace n2d{
namespace gui{

class Wizard : public QWizard
{
        Q_OBJECT
public:
    Wizard(QWizard* parent=0);

    inline const AccessionNumberArgs& getaccessionNumberArgs()const{return m_accessionNumberArgs;}
    inline const DicomHeaderArgs&     getdicomHeaderArgs()const{return m_dicomHeaderArgs;}
    inline const DicomClassArgs&      getdicomClassArgs()const{return m_dicomClassArgs;}
    inline const OtherDicomTagsArgs&  getotherDicomTagsArgs()const{return m_otherDicomTagsArgs;}
    inline const PatientArgs&         getpatientArgs()const{return m_patientArgs;}
    inline const StudyArgs&           getstudyArgs()const{return m_studyArgs;}
    inline const SeriesArgs&          getseriesArgs()const{return m_seriesArgs;}
    inline const AcquisitionArgs&     getacquisitionArgs()const{return m_acquisitionArgs;}
    inline const InputArgs&           getinputArgs()const{return m_inputArgs;}
    inline const FiltersArgs&         getfiltersArgs()const{return m_filtersArgs;}
    inline const InstanceArgs&        getinstanceArgs()const{return m_instanceArgs;}
    inline const OutputArgs&          getoutputArgs()const{return m_outputArgs;}
    

public slots:
   inline void storeAccessionNumberArgs(AccessionNumberArgs& a){m_accessionNumberArgs = a;}
   inline void storeDicomHeaderArgs(DicomHeaderArgs& a){m_dicomHeaderArgs = a;}
   inline void storeDicomClassArgs(DicomClassArgs& a){m_dicomClassArgs = a;}
   inline void storeOtherDicomTagsArgs(OtherDicomTagsArgs& a){m_otherDicomTagsArgs = a;}
   inline void storePatientArgs(PatientArgs& a){m_patientArgs = a;}
   inline void storeStudyArgs(StudyArgs& a){m_studyArgs = a;}
   inline void storeSeriesArgs(SeriesArgs& a){m_seriesArgs = a;}
   inline void storeAcquisitionArgs(AcquisitionArgs& a){m_acquisitionArgs = a;}
   inline void storeInputArgs(InputArgs& a){m_inputArgs = a;}
   inline void storeFiltersArgs(FiltersArgs& a){m_filtersArgs = a;}
   inline void storeInstanceArgs(InstanceArgs& a){m_instanceArgs = a;}
   inline void storeOutputArgs(OutputArgs& a){m_outputArgs = a;}

private:

    AccessionNumberArgs m_accessionNumberArgs;
    DicomHeaderArgs     m_dicomHeaderArgs;
    DicomClassArgs      m_dicomClassArgs;
    OtherDicomTagsArgs  m_otherDicomTagsArgs;
    PatientArgs         m_patientArgs;
    StudyArgs           m_studyArgs;
    SeriesArgs          m_seriesArgs;
    AcquisitionArgs     m_acquisitionArgs;
    InputArgs           m_inputArgs;
    FiltersArgs         m_filtersArgs;
    InstanceArgs        m_instanceArgs;
    OutputArgs          m_outputArgs;
};

}//namespace gui
}//namespace n2d
#endif // WIZARD_H
