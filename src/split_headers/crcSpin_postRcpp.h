// These class definitions below MUST appear AFTER include of
// Rcpp.h as it is using Rcpp types defined in that header.


#include <split_headers/genericSpin_postRcpp.h>
#include <spinUtil.h>

const std::string adenoma_locations[] = {"cecum","ascending","transverse","descending","sigmoid","rectum"};
const double location_probs[] = {0.08,0.23,0.24,0.12,0.24,0.09};
const Rcpp::StringVector CRCSPIN_ADENOMA_LOCATION_OPTIONS(adenoma_locations, adenoma_locations+sizeof(adenoma_locations)/sizeof(std::string));
const Rcpp::NumericVector CRCSPIN_ADENOMA_LOCATION_PROBS(location_probs, location_probs+sizeof(location_probs)/sizeof(double));



/*
 * # CRC model class
 * #################
 */

template <class CrcRiskParamsType   = CrcRiskParams,
          class AdenomaParamsType   = AdenomaParams,
          class PersonWithColonType = PersonWithColon<> >
class CrcSpinModel : public GenericModel<PersonWithColonType> {

    public:
        CrcRiskParamsType crcrisk_model_params;
        AdenomaParamsType adenoma_model_params;

        CrcSpinModel (  std::vector<PersonWithColonType> study_group,
                        int iterations=0,
                        int num_subjects=0,
                        int base_seed=125,
                        int commencement_age=20,
                        ...);

        CrcSpinModel (  int iterations=0,
                        int num_subjects=0,
                        int base_seed=125,
                        int commencement_age=20,
                        ...);

        bool subjectHasNotLeftStudy(PersonWithColonType person);
        void modelSubjectDiseaseDevelopment(PersonWithColonType &person);
        Rcpp::IntegerVector testForAndTreatCRC(PersonWithColonType &person);
        Rcpp::IntegerVector modelSubjectTesting(PersonWithColonType &person);
        int getModelResultSize();
        Rcpp::IntegerVector updateSubject(PersonWithColonType &person);
        void setAdenomaModelingParameters(std::vector<std::string> paramkeys, std::vector<double> paramvals);
        void propegateModelParameters();
        void setCrcRiskModelingParameters(std::vector<std::string> paramkeys, std::vector<double> paramvals);
        Rcpp::IntegerMatrix getStudyResults();

        // The following wrappers that call inherited members are
        // only required to work around limitations in RCPP_MODULE
        void runWrap();
        void showWrap();
};




/*********************************************************
 * Implementation of class templates
 */

/*
 * # CRC model class implementation
 * ################################
 */

// Define a macro otherwise the function definitions look messy
#define CRC_TEMPLATE_PROTO class CrcRiskParamsType, class AdenomaParamsType, class PersonWithColonType
#define CRC_TEMPLATE_ARGS CrcRiskParamsType, AdenomaParamsType, PersonWithColonType


template <CRC_TEMPLATE_PROTO>
CrcSpinModel<CRC_TEMPLATE_ARGS>::CrcSpinModel (
                int iterations,
                int num_subjects,
                int base_seed,
                int commencement_age,
                ...) {

    crcrisk_model_params=CrcRiskParamsType();
    adenoma_model_params=AdenomaParamsType();

    std::vector<PersonWithColonType> sg;
    sg.reserve(num_subjects);
    for (int i=1; i<=num_subjects; i++){
        sg.push_back(PersonWithColonType(i, base_seed,
                                     commencement_age,
                                     &(this->crcrisk_model_params))
                    );
    }

    GenericModel<PersonWithColonType>::init(iterations, 1, num_subjects,
            Rcpp::IntegerMatrix(iterations*2,
                                sg[0].treatmentRecordSize()+
                                sg[0].medicalSnapshotSize()),
            base_seed, commencement_age, sg);

}

template <CRC_TEMPLATE_PROTO>
CrcSpinModel<CRC_TEMPLATE_ARGS>::CrcSpinModel (
                std::vector<PersonWithColonType> study_group,
                int iterations,
                int num_subjects,
                int base_seed,
                int commencement_age,
                ...)
: GenericModel<PersonWithColonType>(study_group, iterations, 1, num_subjects,
        base_seed, commencement_age,
        Rcpp::IntegerMatrix(iterations*2,
                            study_group[0].treatmentRecordSize()+
                            study_group[0].medicalSnapshotSize())
        ) {

    crcrisk_model_params=CrcRiskParamsType();
    adenoma_model_params=AdenomaParamsType();
}

template <CRC_TEMPLATE_PROTO>
bool CrcSpinModel<CRC_TEMPLATE_ARGS>::subjectHasNotLeftStudy(PersonWithColonType person) {
    if ( !person.isDead() && !person.in_treatment_program){
        return true;
    } else {
        return false;
    }
}

template <CRC_TEMPLATE_PROTO>
void CrcSpinModel<CRC_TEMPLATE_ARGS>::modelSubjectDiseaseDevelopment(PersonWithColonType &person){

    // model the growth of adenomas
    person.colon.modelAdenomaGrowth(
                            adenoma_model_params,
                            person.age);

    // transition adenomas through different classification
    // stages and states owing to their growth
    person.colon.modelAdenomaTransitions(person.age);

    // update the patient's and their colon's state based
    // on adenoma growth and classification
    person.updateState();

    // If the person has not already died
    if (!person.isDead()) {
        // model transition of a person and their colon to full CRC
        person.modelPreClinicalToClinicalTransition();
    }

}

template <CRC_TEMPLATE_PROTO>
Rcpp::IntegerVector CrcSpinModel<CRC_TEMPLATE_ARGS>::testForAndTreatCRC(PersonWithColonType &person){

    Rcpp::IntegerVector treatment_record(person.treatmentRecordSize(), 0);

    // if person has symptoms of CRC but is not in treatment
    // program
    if (person.showingCRCSymptoms()
        && !person.in_treatment_program){

        // perform initial treatments if necessary
        treatment_record=person.initiateCRCTreatment();

    }

    return treatment_record;
}

template <CRC_TEMPLATE_PROTO>
Rcpp::IntegerVector CrcSpinModel<CRC_TEMPLATE_ARGS>::modelSubjectTesting(PersonWithColonType &person){

    // Test for and treat CRC
    Rcpp::IntegerVector treatment_record = testForAndTreatCRC(person);

    return treatment_record;
}

template <CRC_TEMPLATE_PROTO>
int CrcSpinModel<CRC_TEMPLATE_ARGS>::getModelResultSize(){
    PersonWithColonType temp();
    return temp.treatmentRecordSize()+temp.medicalSnapshotSize();
}

template <CRC_TEMPLATE_PROTO>
Rcpp::IntegerVector CrcSpinModel<CRC_TEMPLATE_ARGS>::updateSubject(PersonWithColonType &person) {

    Rcpp::IntegerVector treatment_record(person.treatmentRecordSize(),0);
    Rcpp::IntegerVector medical_snapshot(person.medicalSnapshotSize(),0);
    Rcpp::IntegerVector result(CrcSpinModel<CRC_TEMPLATE_ARGS>::study_results.ncol(), 0);

    if ( subjectHasNotLeftStudy(person) ){

        person.restoreRNGState();

        // age the person
        person.age += CrcSpinModel<CRC_TEMPLATE_ARGS>::iteration_resolution;

        // model disease in a person through various stages
        modelSubjectDiseaseDevelopment(person);

        // if the person has not dies prior to CRC being detected
        if (!person.isDead()) {

            // if person is not already in a treatment program
            if (!person.in_treatment_program){
                treatment_record=modelSubjectTesting(person);
            }

            // model natural/other death
            person.modelDeathFromOtherCauses();
        }

        // get a snapshot of the person's health at
        // this point in time
        medical_snapshot=person.getMedicalSnapshot();


        person.saveRNGState();

    }

    // combine the treatment record and medical snapshot
    result[Rcpp::Range(0,person.medicalSnapshotSize()-1)]
            =medical_snapshot;
    result[Rcpp::Range(person.medicalSnapshotSize(),
                       person.medicalSnapshotSize()+
                       person.treatmentRecordSize()-1)]
            =treatment_record;

    return result;
}


template <CRC_TEMPLATE_PROTO>
Rcpp::IntegerMatrix CrcSpinModel<CRC_TEMPLATE_ARGS>::getStudyResults(){
    return GenericModel<PersonWithColonType>::study_results;
}


template <CRC_TEMPLATE_PROTO>
void CrcSpinModel<CRC_TEMPLATE_ARGS>::setAdenomaModelingParameters(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){
    adenoma_model_params.setParams(param_keys, param_vals);
}

template <CRC_TEMPLATE_PROTO>
void CrcSpinModel<CRC_TEMPLATE_ARGS>::propegateModelParameters(){
    for (unsigned int i = 0; i < CrcSpinModel<CRC_TEMPLATE_ARGS>::study_group.size(); i++) {
            CrcSpinModel<CRC_TEMPLATE_ARGS>::
                study_group[i].colon.risk.init(CrcSpinModel<CRC_TEMPLATE_ARGS>
                                                    ::study_group[i].sex);
    }
}

template <CRC_TEMPLATE_PROTO>
void CrcSpinModel<CRC_TEMPLATE_ARGS>::setCrcRiskModelingParameters(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){
    crcrisk_model_params.setParams(param_keys, param_vals);
    propegateModelParameters();

}

// The following wrappers that call inherited members are
// only required to work around limitations in RCPP_MODULE
template <CRC_TEMPLATE_PROTO>
void CrcSpinModel<CRC_TEMPLATE_ARGS>::runWrap(){ CrcSpinModel<CRC_TEMPLATE_ARGS>::run(); }

template <CRC_TEMPLATE_PROTO>
void CrcSpinModel<CRC_TEMPLATE_ARGS>::showWrap(){ CrcSpinModel<CRC_TEMPLATE_ARGS>::show(); }


/*
 * # Colon Class
 * #############
 */

template <class AdenomaType, class AdenomaParamsType>
Colon<AdenomaType, AdenomaParamsType>::Colon(std::string host_risk_level,
        char host_sex,
        CrcRiskParams *crcrisk_model_params) {

    this->state="clear";
    this->cancer_site="";
    this->risk=CrcRisk(host_sex, host_risk_level, crcrisk_model_params);
    this->sites=std::vector<AdenomaType>();

}

template <class AdenomaType, class AdenomaParamsType>
void Colon<AdenomaType, AdenomaParamsType>::initiateAdenomas(AdenomaParamsType &adenoma_params,
        int subject_age){

    Rcpp::NumericVector prob(2);
    prob[1]=risk_of_an_adenoma(risk, subject_age);
    prob[0]=1.0-prob[1];
    Rcpp::IntegerVector coin(2);
    coin[0]=0;
    coin[1]=1;
    if(prob[0]<0.0 || prob[1]<0.0){
        Rcpp::Rcout << "WARNING: function crcSpin.cpp:risk_of_an_adenoma(...) returned a value that induced a negative probability\n";
        Rcpp::Rcout << "         please check the combination of parameters set by calling crcSpin.cpp:setCrcRiskModelingParameters(...)\n";
    }
    Rcpp::IntegerVector iv=SpinUtil::sample(coin,1,false,prob);
    int new_adenomas=iv[0];

    if (new_adenomas>0){

        for (int i = 0; i < new_adenomas; i++) {
            sites.push_back(AdenomaType(adenoma_params, subject_age, 1.0, "adenoma"));
        }
    }
}

template <class AdenomaType, class AdenomaParamsType>
void Colon<AdenomaType, AdenomaParamsType>::growAdenomas(int subject_age) {
    for (unsigned int i = 0; i < sites.size(); i++) {
        sites[i].grow(subject_age);
    }
}

template <class AdenomaType, class AdenomaParamsType>
void Colon<AdenomaType, AdenomaParamsType>::modelAdenomaGrowth(AdenomaParamsType &adenoma_model_params,
        int subject_age) {

    initiateAdenomas(adenoma_model_params, subject_age);

    growAdenomas(subject_age);

}

template <class AdenomaType, class AdenomaParamsType>
void Colon<AdenomaType, AdenomaParamsType>::modelAdenomaTransitions(int subject_age) {
    for (unsigned int i = 0; i < sites.size(); i++) {
        sites[i].transition(subject_age);
    }
}

template <class AdenomaType, class AdenomaParamsType>
bool Colon<AdenomaType, AdenomaParamsType>::modelTransitionToClinicalCRC(int subject_age){
    if (state=="CRC"){
        if(!sites.empty()){
            for (unsigned int i = 0; i < sites.size(); i++) {
                if(sites[i].checkCRCSymptomPresentation(subject_age)){
                    state="symptomatic CRC";
                    return true;
                }
            }
        }
    }
    return false;
}

template <class AdenomaType, class AdenomaParamsType>
std::string Colon<AdenomaType, AdenomaParamsType>::getMostAdvancedAdenomaState(){

    std::string adv_state;
    if (!sites.empty()){

        // Following count_if(...)>0 adv_statement might more succinctly be
        // expressed using Rcpp::Sugar any(lapply(...)=="string")
        if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(AdenomaStateComparer(), std::string("CRC"))) >0 ) {
            adv_state="CRC";
        } else if ( std::count_if(sites.begin(), sites.end(), std::bind2nd(AdenomaStateComparer(), std::string("large adenoma"))) >0 ) {
            adv_state="large adenoma";
        } else {
            adv_state="adenoma";
        }

    } else {
        adv_state="clear";
    }

    return adv_state;
}

template <class AdenomaType, class AdenomaParamsType>
std::string Colon<AdenomaType, AdenomaParamsType>::getCancerSite(){

    std::string cancer_location;
    if (!sites.empty()){
        cancer_location=sites[0].location;
    } else {
        cancer_location="";
    }

    return cancer_location;
}

template <class AdenomaType, class AdenomaParamsType>
bool Colon<AdenomaType, AdenomaParamsType>::isCancerous(){
    return (state=="CRC" || state=="symptomatic CRC");
}

template <class AdenomaType, class AdenomaParamsType>
void Colon<AdenomaType, AdenomaParamsType>::updateState() {

    state=getMostAdvancedAdenomaState();

    if (isCancerous()){
        cancer_site=getCancerSite();
    }

}

template <class AdenomaType, class AdenomaParamsType>
void Colon<AdenomaType, AdenomaParamsType>::show(){
    Rcpp::Rcout << "Spin Colon object of class "
        << typeid(this).name() << "\n";
}


/*
 * # Person with colon class
 * #########################
 */
template <class ColonType>
std::string PersonWithColon<ColonType>::getAdenomaState(Adenoma a){
    return a.state;
}

template <class ColonType>
std::string PersonWithColon<ColonType>::getAdenomaStage(Adenoma a){
    return a.stage;
}

template <class ColonType>
PersonWithColon<ColonType>::PersonWithColon (SEXP p) : Person::Person(p){
    Rcpp::Reference person(p);
    std::string r_level=person.field("risk_level");
    init( ColonType(r_level,
                  this->sex),
                  r_level);
}

template <class ColonType>
PersonWithColon<ColonType>::PersonWithColon(
                ColonType colon,
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
Person::Person( study_id, base_seed, age, sex, state, in_treatment_program,
        clinical_history, random_seed_state){

    init(colon, risk_level);
}

template <class ColonType>
PersonWithColon<ColonType>::PersonWithColon(
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
Person::Person( study_id, base_seed, age, sex, state, in_treatment_program,
        clinical_history, random_seed_state){

    init( ColonType(risk_level,
                  this->sex,
                  crcrisk_model_params),
          risk_level);
}

template <class ColonType>
void PersonWithColon<ColonType>::init(
                ColonType colon,
                std::string risk_level){

    this->risk_level = risk_level;
    this->colon = colon;
    this->colon_clinical = "clear";

}

template <class ColonType>
bool PersonWithColon<ColonType>::isDead(){
    return (state=="deceased");
}

template <class ColonType>
bool PersonWithColon<ColonType>::showingCRCSymptoms() {
    return (colon_clinical=="symptomatic CRC");
}

template <class ColonType>
void PersonWithColon<ColonType>::updateState() {
    // update our colon's state
    colon.updateState();
}

template <class ColonType>
void PersonWithColon<ColonType>::modelPreClinicalToClinicalTransition() {

    if(colon_clinical!="symptomatic CRC"){

        if(colon.modelTransitionToClinicalCRC(age)){
            colon_clinical="symptomatic CRC";
        }
    }
}

template <class ColonType>
int PersonWithColon<ColonType>::treatmentRecordSize(){
    return 1;
}

template <class ColonType>
std::vector<int> PersonWithColon<ColonType>::initiateCRCTreatment(){
        in_treatment_program=true;
        return std::vector<int>(1,true);
}

template <class ColonType>
int PersonWithColon<ColonType>::medicalSnapshotSize(){
    return 17;
}

template <class ColonType>
std::vector<int> PersonWithColon<ColonType>::getMedicalSnapshot() {

    std::vector<int> aa(medicalSnapshotSize(), 0);
    if (!colon.sites.empty()){
        std::vector<std::string> states(colon.sites.size());
        // Following transform can probably be replaced with Rcpp::Sugar
        // lapply expression
        std::transform(
                colon.sites.begin(),
                colon.sites.end(),
                states.begin(), getAdenomaState);
        aa[0]=std::count(states.begin(), states.end(), "adenoma");
        aa[1]=std::count(states.begin(), states.end(), "large adenoma");
        aa[2]=std::count(states.begin(), states.end(), "CRC");
    }

    aa[3]=(colon_clinical=="symptomatic CRC");

    aa[4]=(colon.state=="clear");
    aa[5]=(colon.state=="adenoma");
    aa[6]=(colon.state=="large adenoma");
    aa[7]=(colon.state=="CRC");
    aa[8]=(colon.state=="symptomatic CRC");


    aa[9]=(colon.cancer_site=="cecum");
    aa[10]=(colon.cancer_site=="ascending");
    aa[11]=(colon.cancer_site=="transverse");
    aa[12]=(colon.cancer_site=="descending");
    aa[13]=(colon.cancer_site=="sigmoid");
    aa[14]=(colon.cancer_site=="rectum");

    aa[15]=(state=="deceased");
    aa[16]=(colon_clinical=="symptomatic CRC");

    return aa;
}

template <class ColonType>
void PersonWithColon<ColonType>::show(){
    Person::show();
    Rcpp::Rcout << "Risk level:\n";
    Rcpp::Rcout << "    " << risk_level << "\n";
    Rcpp::Rcout << "Colon clinical characteristic:\n";
    Rcpp::Rcout << "    " << colon_clinical << "\n";
    Rcpp::Rcout << "Colon:\n";
    colon.show();
}
