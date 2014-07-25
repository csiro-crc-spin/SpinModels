// These class definitions below MUST appear AFTER include of
// Rcpp.h as it is using Rcpp types defined in that header.


#include <split_headers/crcSpin_postRcpp.h>


/*
 * # Dukes model class
 * ###################
 */

template <class CrcRiskParamsType   = CrcRiskParams,
          class DukesAdenomaParamsType   = DukesAdenomaParams,
          class DukesPersonWithColonType = DukesPersonWithColon<> >
class DukesCrcSpinModel : public CrcSpinModel<CrcRiskParamsType,
                                              DukesAdenomaParamsType,
                                              DukesPersonWithColonType> {

    public:
        DukesCrcSpinModel (  std::vector<DukesPersonWithColonType> study_group,
                        int iterations=0,
                        int num_subjects=0,
                        int base_seed=125,
                        int commencement_age=20,
                        ...);

        DukesCrcSpinModel (  int iterations=0,
                        int num_subjects=0,
                        int base_seed=125,
                        int commencement_age=20,
                        ...);

        void iFOBTscreening(DukesPersonWithColonType &person);
        Rcpp::IntegerVector NBCSP(DukesPersonWithColonType &person);
        Rcpp::IntegerVector modelSubjectTesting(DukesPersonWithColonType &person);

        // The following wrappers that call inherited members are
        // only required to work around limitations in RCPP_MODULE
        Rcpp::IntegerMatrix getStudyResultsWrap();
        void runWrap();
        void showWrap();
        void setCrcRiskModelingParametersWrap(std::vector<std::string> paramkeys, std::vector<double> paramvals);
        void setAdenomaModelingParametersWrap(std::vector<std::string> paramkeys, std::vector<double> paramvals);
};




/*********************************************************
 * Implementation of class templates
 */

/*
 * # Dukes CRC model class implementation
 * ######################################
 */

// Define a macro otherwise the function definitions look messy
#define TEMPLATE_PROTO class CrcRiskParamsType, class DukesAdenomaParamsType, class DukesPersonWithColonType
#define TEMPLATE_ARGS CrcRiskParamsType, DukesAdenomaParamsType, DukesPersonWithColonType


template <TEMPLATE_PROTO>
DukesCrcSpinModel<TEMPLATE_ARGS>::DukesCrcSpinModel (
                int iterations,
                int num_subjects,
                int base_seed,
                int commencement_age,
                ...)
: CrcSpinModel<TEMPLATE_ARGS>(iterations, num_subjects,
        base_seed, commencement_age) {}

template <TEMPLATE_PROTO>
DukesCrcSpinModel<TEMPLATE_ARGS>::DukesCrcSpinModel (
                std::vector<DukesPersonWithColonType> study_group,
                int iterations,
                int num_subjects,
                int base_seed,
                int commencement_age,
                ...)
: CrcSpinModel<TEMPLATE_ARGS>(study_group, iterations, num_subjects,
        base_seed, commencement_age) {}


// The following wrappers that call inherited members are
// only required to work around limitations in RCPP_MODULE
template <TEMPLATE_PROTO>
Rcpp::IntegerMatrix DukesCrcSpinModel<TEMPLATE_ARGS>::getStudyResultsWrap(){
    return DukesCrcSpinModel<TEMPLATE_ARGS>::getStudyResults();
}

template <TEMPLATE_PROTO>
void DukesCrcSpinModel<TEMPLATE_ARGS>::runWrap(){ DukesCrcSpinModel<TEMPLATE_ARGS>::run(); }

template <TEMPLATE_PROTO>
void DukesCrcSpinModel<TEMPLATE_ARGS>::showWrap(){ DukesCrcSpinModel<TEMPLATE_ARGS>::show(); }

template <TEMPLATE_PROTO>
void DukesCrcSpinModel<TEMPLATE_ARGS>::setCrcRiskModelingParametersWrap(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){
    DukesCrcSpinModel<TEMPLATE_ARGS>::setCrcRiskModelingParameters(param_keys, param_vals);
}

template <TEMPLATE_PROTO>
void DukesCrcSpinModel<TEMPLATE_ARGS>::setAdenomaModelingParametersWrap(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){
    DukesCrcSpinModel<TEMPLATE_ARGS>::setAdenomaModelingParameters(param_keys, param_vals);
}


/*
 * # Duke's Colon Class
 * ####################
 */

template <class DukesAdenomaType, class DukesAdenomaParamsType>
DukesColon<DukesAdenomaType, DukesAdenomaParamsType>::DukesColon(std::string host_risk_level,
        char host_sex,
        CrcRiskParams *crcrisk_model_params) :
Colon<DukesAdenomaType, DukesAdenomaParamsType>(host_risk_level, host_sex, crcrisk_model_params),
stage("clear"){}

template <class DukesAdenomaType, class DukesAdenomaParamsType>
void DukesColon<DukesAdenomaType, DukesAdenomaParamsType>::treatCRC() {
    //This is a very simple treatment model. If Dukes A or B then all adenomas and tumors are removed.
    //If Dukes C or D then treatment is ineffective and the cancer in incurable.

    if ( (this->state=="adenoma") | (this->state=="large adenoma")){
        //I dont think this ever happens --
        //if state=="adenoma" "large adenoma then colon_clinical will not be CRC
        this->sites.clear();
        this->state="clear";
        stage="clear";
    } else if (this->state=="CRC"){
        if (stage=="A"){
            this->sites.clear();
            this->state="clear";
            stage="clear";
        }
        if (stage=="B"){
            this->sites.clear();
            this->state="clear";
            stage="clear";
        }
        if (stage=="C"){
            // we do nothing
        }
        if(stage=="D"){
            //we do nothing.
        }
    }
}


template <class DukesAdenomaType, class DukesAdenomaParamsType>
bool DukesColon<DukesAdenomaType, DukesAdenomaParamsType>::modelTransitionToClinicalCRC(int subject_age){

    // Why are we using "Colon<DukesAdenomaType>" or "this->"
    // for some variables when these variables are inherited members from
    // parent classes??
    //
    // It is a "nuance" of the C++ language compiler standards
    // In class templates, variable/function/class names that do
    // not depend on the template parameters are call nondependant
    // names, those that are, are called dependant names.
    // The compiler will not try to resolve nondependant names
    // by looking in dependant classes
    //
    // "Colon<DukesAdenomaType, DukesAdenomaParamsType>" or "this->" forces the reference to the
    // variable/function/class to become a dependant reference, and so, the
    // parent class dependant on the same template parameter is consulted

    if (this->state=="CRC"){
        Rcpp::NumericVector p(2);
        p.fill(-1);
        if (stage=="A"){
            p[0]=0.04;
            p[1]=1-0.04;
        } else if (stage=="B"){
            p[0]=0.18;
            p[1]=1-0.18;
        } else if (stage=="C"){
            p[0]=0.37;
            p[1]=1-0.37;
        } else if (stage=="D"){
            p[0]=0.74;
            p[1]=1-0.74;
        }

        if(!(p[0]==-1 && p[1]==-1)){
            Rcpp::StringVector opts(2);
            opts[0]="symptoms";
            opts[1]="no symptoms";
            Rcpp::StringVector sv = SpinUtil::sample(opts, 1, false, p);
            std::string presenting(sv[0]);

            if (presenting=="symptoms"){
                this->state="symptomatic CRC";
                return true;
            }
        }
    }
    return false;
}

template <class DukesAdenomaType, class DukesAdenomaParamsType>
std::string DukesColon<DukesAdenomaType, DukesAdenomaParamsType>::getMostAdvancedAdenomaState(){

    std::string adv_state;

    // Call Colon superclass implementation to do
    // Classic CRC spin checks
    adv_state=Colon<DukesAdenomaType, DukesAdenomaParamsType>::getMostAdvancedAdenomaState();

    // Now do additional Duke's updates
    std::vector<DukesAdenomaType> &sites=Colon<DukesAdenomaType, DukesAdenomaParamsType>::sites;
    if (!sites.empty()){
        if ( std::count_if(sites.begin(),
                           sites.end(),
                           std::bind2nd(Colon<DukesAdenomaType, DukesAdenomaParamsType>::AdenomaStateComparer(),
                           std::string("deceased"))) >0 ) {
            adv_state="deceased";
        }
    }

    return adv_state;
}


template <class DukesAdenomaType, class DukesAdenomaParamsType>
std::string DukesColon<DukesAdenomaType, DukesAdenomaParamsType>::getMostAdvancedAdenomaDukesStage(){

    std::string adv_stage;

    std::vector<DukesAdenomaType> &sites=Colon<DukesAdenomaType, DukesAdenomaParamsType>::sites;
    if (!sites.empty()){
        if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(DukesAdenomaStageComparer(), std::string("deceased"))) >0 ) {
            adv_stage="deceased";
        }else if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(DukesAdenomaStageComparer(), std::string("D"))) >0 ) {
            adv_stage="D";
        } else if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(DukesAdenomaStageComparer(), std::string("C"))) >0 ) {
            adv_stage="C";
        } else if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(DukesAdenomaStageComparer(), std::string("B"))) >0 ) {
            adv_stage="B";
        } else if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(DukesAdenomaStageComparer(), std::string("A"))) >0 ) {
            adv_stage="A";
        } else {
            adv_stage="clear";
        }

    } else {
        adv_stage="clear";
    }

    return adv_stage;
}


template <class DukesAdenomaType, class DukesAdenomaParamsType>
void DukesColon<DukesAdenomaType, DukesAdenomaParamsType>::updateState() {

    // Call the Colon superclass implementation
    // to set state and cancer_site
    Colon<DukesAdenomaType, DukesAdenomaParamsType>::updateState();

    // Set the Duke's stage
    stage=getMostAdvancedAdenomaDukesStage();
}



/*
 * # Person with colon class
 * #########################
 */

template <class DukesColonType>
DukesPersonWithColon<DukesColonType>::DukesPersonWithColon (SEXP p) : PersonWithColon<DukesColonType>::PersonWithColon(p){ }

template <class DukesColonType>
DukesPersonWithColon<DukesColonType>::DukesPersonWithColon(
                DukesColonType colon,
                int study_id,
                int base_seed,
                int age,
                CrcRiskParams *crcrisk_model_params,
                char sex,
                std::string state,
                bool in_treatment_program,
                ClinicalHistory clinical_history,
                std::vector<int> random_seed_state,
                std::string risk_level,
                ...)  :
PersonWithColon<DukesColonType>::PersonWithColon( colon, study_id, base_seed, age, crcrisk_model_params,
        sex, state, in_treatment_program, clinical_history, random_seed_state, risk_level){}

template <class DukesColonType>
DukesPersonWithColon<DukesColonType>::DukesPersonWithColon(
                int study_id,
                int base_seed,
                int age,
                CrcRiskParams *crcrisk_model_params,
                char sex,
                std::string state,
                bool in_treatment_program,
                ClinicalHistory clinical_history,
                std::vector<int> random_seed_state,
                std::string risk_level,
                ...)  :
PersonWithColon<DukesColonType>::PersonWithColon( study_id, base_seed, age, crcrisk_model_params, sex, state,
        in_treatment_program, clinical_history, random_seed_state){}


template <class DukesColonType>
void DukesPersonWithColon<DukesColonType>::modelDeathFromCRC(){
    // Rational for using so many PersonWithColon<DukeColonType>
    // can be found in DukesColon::modelTransitionToClinicalCRC()
    if (this->colon.state=="deceased"){
        this->state="deceased CRC";
    }
}

template <class DukesColonType>
bool DukesPersonWithColon<DukesColonType>::isDead(){
    return (this->state=="deceased");
}

template <class DukesColonType>
void DukesPersonWithColon<DukesColonType>::updateState() {
    // call the PersonWithColon superclass implementation
    // to do Classic CRC updates
    PersonWithColon<DukesColonType>::updateState();

    // Do updates specific to Duke's Model
    modelDeathFromCRC();
}

template <class DukesColonType>
void DukesPersonWithColon<DukesColonType>::modelPreClinicalToClinicalTransition() {

    if(this->colon_clinical!="symptomatic CRC"){
        // Call our PersonWithColon superclass
        // implementation to do the transition
        PersonWithColon<DukesColonType>::modelPreClinicalToClinicalTransition();

        // If a transition occurred, add a clinical events
        if(this->colon_clinical=="symptomatic CRC"){
            ClinicalEvent *tmp = new SymptomaticPresentation(this->age,this->colon.stage);
            this->clinical_history.events.push_back(tmp);
        }
    }
}

template <class DukesColonType>
int DukesPersonWithColon<DukesColonType>::treatmentRecordSize(){
    return 14;
}

template <class DukesColonType>
std::vector<int> DukesPersonWithColon<DukesColonType>::initiateCRCTreatment(){
    return initiateCRCTreatment(1.0);
}

template <class DukesColonType>
std::vector<int> DukesPersonWithColon<DukesColonType>::initiateCRCTreatment(double prob_colonoscopy_performed){

    // Set up some variables for random sampling
    Rcpp::NumericVector p(2);
    Rcpp::IntegerVector coin(2);
    Rcpp::IntegerVector res;
    coin[0]=0;
    coin[1]=1;


    // model chance of having a colonscopy
    p[0]=1-prob_colonoscopy_performed;
    p[1]=prob_colonoscopy_performed;
    res=SpinUtil::sample(coin, 1, false, p);
    bool colonoscopy_performed=res[0];

    // model chance of complications
    bool colonoscopy_caused_bleeding=false;
    bool colonoscopy_caused_perforation=false;
    if (colonoscopy_performed) {

        p[0]=0.9997;
        p[1]=0.0003;
        res=SpinUtil::sample(coin, 1, false, p);
        colonoscopy_caused_bleeding=res[0];

        p[0]=0.9999;
        p[1]=0.0001;
        res=SpinUtil::sample(coin, 1, false, p);
        colonoscopy_caused_perforation=res[0];
    }

    // Collect more information for the pre-treatment record/chart
    bool colon_has_small_to_large_adenomas=
             (this->colon.state=="adenoma" ||
              this->colon.state=="large adenoma");
    bool colon_state_CRC=
             (this->colon.state=="symptomatic CRC"||
              this->colon.state=="CRC");
    bool colon_in_pre_symtomatic_stage_A=(colon_state_CRC&&this->colon.stage=="A");
    bool colon_in_pre_symtomatic_stage_B=(colon_state_CRC&&this->colon.stage=="B");
    bool colon_in_pre_symtomatic_stage_C=(colon_state_CRC&&this->colon.stage=="C");
    bool colon_in_pre_symtomatic_stage_D=(colon_state_CRC&&this->colon.stage=="D");


    // model treatment of CRC
    this->colon.treatCRC();

    if(this->colon.sites.empty() &&
       this->colon.stage=="clear" &&
       this->colon.state=="clear"){
        this->colon_clinical="clear";
    }

    this->in_treatment_program=true;


    std::vector<int> aa(treatmentRecordSize(), 0);
    aa[0 ]=false; // offered iFOBT
    aa[1 ]=false; // accepted iFOBT
    aa[2 ]=false; // offered blood test
    aa[3 ]=false; // accepted blood test
    aa[4 ]=false; // test result positive
    aa[5 ]=colonoscopy_performed;
    aa[6 ]=colon_has_small_to_large_adenomas;
    aa[7 ]=colon_in_pre_symtomatic_stage_A;
    aa[8 ]=colon_in_pre_symtomatic_stage_B;
    aa[9 ]=colon_in_pre_symtomatic_stage_C;
    aa[10]=colon_in_pre_symtomatic_stage_D;
    aa[11]=this->in_treatment_program;
    aa[12]=colonoscopy_caused_bleeding;
    aa[13]=colonoscopy_caused_perforation;

    return aa;
}

template <class DukesColonType>
int DukesPersonWithColon<DukesColonType>::medicalSnapshotSize(){
    return 46;
}

template <class DukesColonType>
std::vector<int> DukesPersonWithColon<DukesColonType>::getMedicalSnapshot() {

    // This function collects a tally of various study parameters
    // related to a DukesPersonWithColon
    //
    // It calls the PersonWithColon superclass's implementation of this
    // function to retrieve some parameters, then augments these results
    std::vector<int> aa(medicalSnapshotSize(), 0);

    ///////////////////////////////////////////////////////////////
    // Copy the stuff we want from our super class results into the
    // location we want them

    // Call the PersonWithColon superclass implementation
    // to do some of the work
    std::vector<int> res=PersonWithColon<DukesColonType>::getMedicalSnapshot();

    // Copy over COUNTS of:
    //   adenomas, large adenomas, & CRC adenomas
    std::copy(res.begin(), res.begin()+3, aa.begin());

    // Copy over FLAGS for:
    //   colon_clinical=="symptomatic CRC"
    //   colon$state=="clear", =="adenoma", =="large adenoma",
    //              =="CRC", =="symptomatic CRC"
    std::copy(res.begin()+3, res.begin()+9, aa.begin()+4);

    // Copy FLAGS indicating location of colon$cancer_site
    std::copy(res.begin()+9, res.begin()+15, aa.begin()+33);

    // Copy FLAG for colon_clinical=="symptomatic CRC"
    aa[45]=res[16];



    //////////////////////////////////////////////////////////
    // Add new information important to Duke's CRCSpin Model

    // Add a count of deceased adenomas
    if (!this->colon.sites.empty()){
        std::vector<std::string> states(this->colon.sites.size());
        // Following transform can probably be replaced with Rcpp::Sugar
        // lapply expression
        std::transform(
                this->colon.sites.begin(),
                this->colon.sites.end(),
                states.begin(),
                this->getAdenomaState
                );
        aa[3]=std::count(states.begin(), states.end(), "deceased");
    }

    // Add flag for colon$state=="deceased"
    aa[10]=(this->colon.state=="deceased");

    // Add in some additional info for when colon$state is
    // symptomatic CRC or CRC
    if (!this->colon.sites.empty()
        && (this->colon.state=="CRC" ||
            this->colon.state=="symptomatic CRC")
        ){

        // rather than duplicating value counting code
        // within the following two if statements (which
        // never execute together) to operate on separate
        // segments of the output vector, we simply use an
        // offset to the start of the effected range to achieve
        // the same results.
        int start_range=11;
        if(this->colon.state=="CRC"){
          start_range=11;
        } else if (this->colon.state=="symptomatic CRC"){
          start_range=22;
        }

        // Add COUNTS of adenomas in stages A,B,C,D,or deceased
        std::vector<std::string> stages(this->colon.sites.size());
        std::transform(
                this->colon.sites.begin(),
                this->colon.sites.end(),
                stages.begin(),
                this->getAdenomaStage);
        aa[start_range+0]=std::count(stages.begin(), stages.end(), "A");
        aa[start_range+1]=std::count(stages.begin(), stages.end(), "B");
        aa[start_range+2]=std::count(stages.begin(), stages.end(), "C");
        aa[start_range+3]=std::count(stages.begin(), stages.end(), "D");
        aa[start_range+4]=std::count(stages.begin(), stages.end(), "deceased");

        // Add FLAGS indication location of colon$cancer_site
        aa[start_range+5 ]=(this->colon.cancer_site=="cecum");
        aa[start_range+6 ]=(this->colon.cancer_site=="ascending");
        aa[start_range+7 ]=(this->colon.cancer_site=="transverse");
        aa[start_range+8 ]=(this->colon.cancer_site=="descending");
        aa[start_range+9 ]=(this->colon.cancer_site=="sigmoid");
        aa[start_range+10]=(this->colon.cancer_site=="rectum");

    }

    // Add FLAGS indicating colon$stage
    aa[39]=(this->colon.stage=="A");
    aa[40]=(this->colon.stage=="B");
    aa[41]=(this->colon.stage=="C");
    aa[42]=(this->colon.stage=="D");
    aa[43]=(this->colon.stage=="deceased");

    // Add FLAG indicating if person is dead
    aa[44]=isDead();

    return aa;

}
