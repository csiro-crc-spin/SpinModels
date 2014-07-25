#include <split_headers/crcSpin_preRcpp.h>


/*
 * ########################################################################
 * #
 * # CRC spin model class definitions
 * #
 * ########################################################################
 */

// SOME of the following class definitions MUST appear BEFORE include of Rcpp.h
// so that the Rcpp template dispatch mechanism can see the
// DukesPersonWithColon::PersonWithColon(SEXP) constructor

/*
 * # Symptomatic Presentation event class
 * ######################################
 */

class SymptomaticPresentation : public ClinicalEvent {
    public:
        std::string cancer_stage;

        SymptomaticPresentation (int subject_age, std::string stage);

        virtual void show();
};


/*
 * # Dukes Adenoma parameters class
 * ################################
 */

class DukesAdenoma;

class DukesAdenomaParams : public AdenomaParams {

    protected:
        // Modelling variables related to DukesAdenoma class
        // but determined by the model being run
        int sojournA_min;
        int sojournA_max;
        int sojournB_val;
        int sojournC_min;
        int sojournC_max;
        int sojournD_val;

    public:

        DukesAdenomaParams ();
        void setParams(
                std::vector<std::string> param_keys=std::vector<std::string>(),
                std::vector<double> param_vals=std::vector<double>());
        friend class DukesAdenoma;
};


/*
 * # Dukes Adenoma class
 * #####################
 */

class DukesAdenoma : public Adenoma {

    protected:
        static DukesAdenomaParams default_params;
        DukesAdenomaParams *adenoma_model_params;

    public:
        std::string stage;
        int transition_to_crc_year;
        int crc_survival_time;
        int sojourn_time_A;
        int sojourn_time_B;
        int sojourn_time_C;
        int sojourn_time_D;
        int transition_to_stage_A;
        int transition_to_stage_B;
        int transition_to_stage_C;
        int transition_to_stage_D;

        DukesAdenoma (DukesAdenomaParams &params=default_params,
                 int initiated_in_year=0,
                 double size=1,
                 std::string state="adenoma",
                 std::string stage="",
                 ...);

        DukesAdenoma (AdenomaParams &params,
                 int initiated_in_year=0,
                 double size=1,
                 std::string state="adenoma",
                 std::string stage="",
                 ...);

        void transition(int subject_age=0);

        bool checkCRCSymptomPresentation(int subject_age=0);

};



/*
 * # Dukes Colon Class
 * ###################
 */
template <class DukesAdenomaType = DukesAdenoma, class DukesAdenomaParamsType = DukesAdenomaParams>
class DukesColon : public Colon<DukesAdenomaType, DukesAdenomaParamsType> {

    protected:
        struct DukesAdenomaStageComparer : public std::binary_function<DukesAdenomaType,std::string,bool> {
            bool operator() (DukesAdenomaType a, std::string s) const { return (a.stage==s);}
        };

    public:
        std::string stage;

        DukesColon (std::string host_risk_level="standard",
               char host_sex='\0',
               CrcRiskParams *crcrisk_model_params=NULL);

        void treatCRC();

        bool modelTransitionToClinicalCRC(int subject_age);

        std::string getMostAdvancedAdenomaState();

        std::string getMostAdvancedAdenomaDukesStage();

        void updateState();

};


/*
 * # Person with colon class
 * #########################
 */
template< class DukesColonType = DukesColon<> >
class DukesPersonWithColon : public PersonWithColon<DukesColonType> {

    public:
        DukesPersonWithColon (SEXP p);

        DukesPersonWithColon (DukesColonType colon,
                         int study_id=0,
                         int base_seed=125,
                         int age=1,
                         CrcRiskParams *crcrisk_model_params=NULL,
                         char sex='\0',
                         std::string state="living",
                         bool in_treatment_program=false,
                         ClinicalHistory clinical_history=ClinicalHistory(),
                         std::vector<int> random_seed_state=std::vector<int>(),
                         std::string risk_level="standard",
                         ...);

        DukesPersonWithColon (int study_id=0,
                         int base_seed=125,
                         int age=1,
                         CrcRiskParams *crcrisk_model_params=NULL,
                         char sex='\0',
                         std::string state="living",
                         bool in_treatment_program=false,
                         ClinicalHistory clinical_history=ClinicalHistory(),
                         std::vector<int> random_seed_state=std::vector<int>(),
                         std::string risk_level="standard",
                         ...);

        void modelDeathFromCRC();

        bool isDead();

        void updateState();

        void modelPreClinicalToClinicalTransition();

        int treatmentRecordSize();

        std::vector<int> initiateCRCTreatment();
        std::vector<int> initiateCRCTreatment(double prob_colonoscopy_performed);

        int medicalSnapshotSize();

        std::vector<int> getMedicalSnapshot();

        void show();

};


