#include <split_headers/genericSpin_preRcpp.h>


/*
 * ########################################################################
 * #
 * # CRC spin model class definitions
 * #
 * ########################################################################
 */

// The following class definitions MUST appear BEFORE include of Rcpp.h
// so that the Rcpp template dispatch mechanism can see the
// PersonWithColon::PersonWithColon(SEXP) constructor

// Required for following definition of PersonWithColon
/*
 * # CRC risk parameters class
 * ###########################
 */

class CrcRisk;

class CrcRiskParams {

    protected:
        // Static variables related to CrcRisk class
        // but determined by the model being run
        double sex_linked_mean;
        double sex_linked_sd;
        double baseline_mean;
        double baseline_sd;
        double age_mean;
        double age_sd;

    public:

        CrcRiskParams ();
        void setParams(
                std::vector<std::string> param_keys=std::vector<std::string>(),
                std::vector<double> param_vals=std::vector<double>());
        friend class CrcRisk;
};



/*
 * # CRC risk class
 * ################
 */

class CrcRisk : public Risk {

    public:
        std::string risk_level;
        CrcRiskParams *crcrisk_model_params;
        double age_risk[4];

        CrcRisk (char subject_sex='\0',
                 std::string subject_risk_level="standard",
                 CrcRiskParams *params=NULL);

        void init (char subject_sex);

};



/*
 * # Adenoma parameters class
 * ##########################
 */

class Adenoma;

class AdenomaParams {

    protected:
        // Modelling variables related to Adenoma class
        // but determined by the model being run
        int beta1_min;
        int beta1_max;
        int beta2_min;
        int beta2_max;
        double gamma1_min;
        double gamma1_max;
        double gamma2_min;
        double gamma2_max;
        double gamma3_val;

    public:

        AdenomaParams ();
        void setParams(
                std::vector<std::string> param_keys=std::vector<std::string>(),
                std::vector<double> param_vals=std::vector<double>());
        friend class Adenoma;
};


/*
 * # Adenoma class
 * ###############
 */

double risk_of_an_adenoma(CrcRisk risk_params, int subject_age = 1);

class Adenoma {

    public:
        int initiated_in_year;
        double size;
        std::string location;
        std::string state;
        std::string stage;
        double beta1;
        double beta2;
        double gamma1;
        double gamma2;
        double gamma3;
        double d10;
        double lambda;
        double sojourn_time;
        double transition_to_preclinical_crc_year;
        double nu_colon;
        double xi_colon;

        Adenoma (AdenomaParams &params,
                 int initiated_in_year=0,
                 double size=1,
                 std::string state="adenoma",
                 ...);

        void grow(int subject_age=0);

        void transition(int subject_age=0);

        bool checkCRCSymptomPresentation(int subject_age=0);

        void show();

};



/*
 * # Colon Class
 * #############
 */
template <class AdenomaType = Adenoma, class AdenomaParamsType = AdenomaParams> class Colon {

    protected:
        struct AdenomaStateComparer : public std::binary_function<AdenomaType,std::string,bool> {
            bool operator() (AdenomaType a, std::string s) const { return (a.state==s);}
        };

    public:
        std::string state;
        std::string cancer_site;
        CrcRisk risk;
        std::vector<AdenomaType> sites;

        Colon (std::string host_risk_level="standard",
               char host_sex='\0',
               CrcRiskParams *crcrisk_model_params=NULL);

        void initiateAdenomas(AdenomaParamsType &adenoma_modeling_params,
                int subject_age);

        void growAdenomas(int subject_age);

        void modelAdenomaGrowth(AdenomaParamsType &adenoma_model_params,
                int subject_age);

        void modelAdenomaTransitions(int subject_age);

        bool modelTransitionToClinicalCRC(int subject_age);

        std::string getMostAdvancedAdenomaState();

        std::string getCancerSite();

        bool isCancerous();

        void updateState();

        void show();

};


/*
 * # Person with colon class
 * #########################
 */

template< class ColonType = Colon<> >
class PersonWithColon : public Person {

    protected:
        static std::string getAdenomaState(Adenoma a);

        static std::string getAdenomaStage(Adenoma a);

    public:
        std::string colon_clinical;
        ColonType colon;
        std::string risk_level;

        PersonWithColon (SEXP p);

        PersonWithColon (ColonType colon,
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

        PersonWithColon (int study_id=0,
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

        void init(ColonType colon, std::string risk_level);

        bool isDead();

        bool showingCRCSymptoms();

        void updateState();

        void modelPreClinicalToClinicalTransition();

        int treatmentRecordSize();

        std::vector<int> initiateCRCTreatment();

        int medicalSnapshotSize();

        std::vector<int> getMedicalSnapshot();

        void show();

};


