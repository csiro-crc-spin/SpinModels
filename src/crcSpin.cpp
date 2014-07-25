/* vim: set fileformat=unix : */
#include <crcSpin.h>
#include <spinUtil.h>
#include <cmath>
#include <algorithm>
#include <functional>


/*
 * ########################
 * #
 * # CRC spin model classes
 * #
 * ########################
 */




/*
 * # CRC risk parameters class
 * ###########################
 */

CrcRiskParams::CrcRiskParams(){

    sex_linked_mean = -0.3;
    sex_linked_sd   = 0.04;
    baseline_mean   = -6.7;
    baseline_sd     = 0.27;
    age_mean        = 0.03;
    age_sd          = 0.003;

}

void CrcRiskParams::setParams(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){

    if(param_keys.size()==param_vals.size()){
        for (unsigned int i = 0; i < param_keys.size(); i++) {
            if ( param_keys[i] == "sex_linked_mean" ){
                sex_linked_mean=param_vals[i];
            } else if ( param_keys[i] == "sex_linked_sd" ){
                sex_linked_sd=param_vals[i];
            } else if ( param_keys[i] == "baseline_mean" ){
                baseline_mean=param_vals[i];
            } else if ( param_keys[i] == "baseline_sd" ){
                baseline_sd=param_vals[i];
            } else if ( param_keys[i] == "age_mean" ){
                age_mean=param_vals[i];
            } else if ( param_keys[i] == "age_sd" ){
                age_sd=param_vals[i];
            } else{
                Rcpp::Rcout << "CrcRiskParams::setParams() Unknown CRC risk modeling parameter " << param_keys[i] << ": Ignoring\n";
            }
        } // end for
    } else {
        Rcpp::Rcout << "CrcRiskParams::setParams() "
                << "param_keys (size=" << param_keys.size() << ") and"
                << "param_vals (size=" << param_vals.size() << ") "
                << "do not have the same number of elements : No parameters set\n";
    }

}



/*
 * # CRC risk class
 * ################
 */

CrcRisk::CrcRisk(char subject_sex,
        std::string subject_risk_level,
        CrcRiskParams *crcrisk_params)
: Risk()
{
    if(crcrisk_params==NULL){
        // no CrcRiskParams were passed in
        // create a temporary one with default values
        crcrisk_params=new CrcRiskParams();
    }

    this->risk_level           = subject_risk_level;
    this->crcrisk_model_params = crcrisk_params;
    init(subject_sex);
}

void CrcRisk::init(char subject_sex){

    Rcpp::RNGScope scope;

    // Set sex linked risk based on subject sex
    sex_linked_risk = R::rnorm(crcrisk_model_params->sex_linked_mean, crcrisk_model_params->sex_linked_sd);
    if(subject_sex=='M'){
        sex_linked_risk = -sex_linked_risk;
    }

    // Set the baseline risk according to subject_risk_level
    if (risk_level=="high"){
        baseline_risk = R::rnorm(-4, 0.27);
    } else {
        double mu     = R::rnorm(crcrisk_model_params->baseline_mean, crcrisk_model_params->baseline_sd);
        double sigma  = R::runif(0.1, 2.5);
        baseline_risk = R::rnorm(mu, sigma);
    }

    // Set age link risk
    Rcpp::NumericVector ar = Rcpp::rnorm(4, crcrisk_model_params->age_mean, crcrisk_model_params->age_sd);
    age_risk[0] = std::max(ar[0], 0.0);
    age_risk[1] = std::max(ar[1], 0.0);
    age_risk[2] = std::max(ar[2], 0.0);
    age_risk[3] = std::max(ar[3], 0.0);


}


/*
 * # Adenoma modeling parameter class
 * ##################################
 */

AdenomaParams::AdenomaParams(){

    // Set default values here
    beta1_min  = 1;
    beta1_max  = 100;
    beta2_min  = 1;
    beta2_max  = 4;
    gamma1_min = 0.02;
    gamma1_max = 0.05;
    gamma2_min = 0.0;
    gamma2_max = 0.02;
    gamma3_val = 0.5;

    // call setParams() with no arguments to ensure
    // your defaults meet valid ranges
    // NOTE: that we force the use of this (base)
    // Classes setParams to avoid running a
    // derived class' setParams function before
    // they might have had a chance to set their
    // defaults
    AdenomaParams::setParams();

}


void AdenomaParams::setParams(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){

    if(param_keys.size()==param_vals.size()){
        for (unsigned int i = 0; i < param_keys.size(); i++) {
            if ( param_keys[i] == "beta1_min" ){
                beta1_min=static_cast<int>(param_vals[i]);

            } else if ( param_keys[i] == "beta1_max" ){
                beta1_max=static_cast<int>(param_vals[i]);

            } else if ( param_keys[i] == "beta2_min" ){
                beta2_min=static_cast<int>(param_vals[i]);

            } else if ( param_keys[i] == "beta2_max" ){
                beta2_max=static_cast<int>(param_vals[i]);

            } else if ( param_keys[i] == "gamma1_min" ){
                gamma1_min=param_vals[i];

            } else if ( param_keys[i] == "gamma1_max" ){
                gamma1_max=param_vals[i];

            } else if ( param_keys[i] == "gamma2_min" ){
                gamma2_min=param_vals[i];

            } else if ( param_keys[i] == "gamma2_max" ){
                gamma2_max=param_vals[i];

            } else if ( param_keys[i] == "gamma3_val" ){
                gamma3_val=param_vals[i];

            } else {
                Rcpp::Rcout << "AdenomaParams::setParams() Unknown Adenoma modeling parameter " << param_keys[i] << ": Ignoring\n";
            }
        } // end for

        beta1_max
            =std::max(beta1_min, beta1_max);
        beta2_max
            =std::max(beta2_min, beta2_max);
        gamma1_max
            =std::max(gamma1_min,gamma1_max);
        gamma2_max
            =std::max(gamma2_min, gamma2_max);

    } else {
        Rcpp::Rcout << "AdenomaParams::setParams() "
                << "param_keys (size=" << param_keys.size() << ") and"
                << "param_vals (size=" << param_vals.size() << ") "
                << "do not have the same number of elements : No parameters set\n";
    }

}



/*
 * # Adenoma class
 * ###############
 */


double risk_of_an_adenoma(CrcRisk risk_params,
                        int subject_age){

    double r1   = risk_params.baseline_risk;
    double * aa = risk_params.age_risk;
           r1   = r1+risk_params.sex_linked_risk;
    double r2   = 0;
    double r3   = 0;

    if (subject_age >= 20){
        int k=0;
        if (subject_age>=70){
            k=3;
        } else if (subject_age>=60){
            k=2;
        } else if (subject_age>=50){
            k=1;
        } else {
            k=0;
        }
        int A[] ={20,50,60,70,120};
        r2=subject_age*aa[k];

        r3=0;
        if ( k >= 1 ){
            for (int j=1; j<=k; j++){
                r3=r3+A[j]*(aa[j-1]-aa[j]);
            }
        }
    }

    return std::max(0.0, std::min(1.0, exp(r1+r2+r3)));
}




Adenoma::Adenoma (AdenomaParams &params,
         int initiated_in_year,
         double size,
         std::string state,
         ...) {

    // variables prefixed with "l_" are function local versions of
    // class member variables
    Rcpp::RNGScope scope;
    Rcpp::StringVector sv =
        SpinUtil::sample(
                CRCSPIN_ADENOMA_LOCATION_OPTIONS,
                1,
                false,
                CRCSPIN_ADENOMA_LOCATION_PROBS);
    std::string a1(sv[0]);
    double l_beta1   = R::runif((double)params.beta1_min, (double)params.beta1_max);
    double l_beta2   = R::runif((double)params.beta2_min, (double)params.beta2_max);
    double dinfinity = 50.0;
    double d0        = 1.0;
    double l_d10     = l_beta1 * pow( -log(R::runif(0,1)) , -1/l_beta2 );

    double l_gamma1, l_gamma2, l_gamma3;
    if (a1=="rectum"){
        l_gamma1 = R::runif(params.gamma1_min, params.gamma1_max);
        l_gamma2 = R::runif(params.gamma2_min, params.gamma2_max);
        l_gamma3 = params.gamma3_val;
    } else {
        l_gamma1 = R::runif(params.gamma1_min, params.gamma1_max);
        l_gamma2 = R::runif(params.gamma2_min, params.gamma2_max);
        l_gamma3 = params.gamma3_val;
    }

    double mean_colon = R::runif(0.5,5);
    double tau_colon  = R::runif(0.1,1.5);
    double l_nu_colon = sqrt(log(pow(tau_colon,2)+1));
    double l_xi_colon = log(mean_colon)-0.5*pow(l_nu_colon,2);

    double l_lambda= (-1/l_d10)*(log( (dinfinity-10)/(dinfinity-d0)));

    this->initiated_in_year    = initiated_in_year;
    this->size                 = size;
    this->location             = a1;
    this->state                = state;
    this->beta1                = l_beta1;
    this->beta2                = l_beta2;
    this->gamma1               = l_gamma1;
    this->gamma2               = l_gamma2;
    this->gamma3               = l_gamma3;
    this->d10                  = l_d10;
    this->lambda               = l_lambda;
    this->nu_colon             = l_nu_colon;
    this->xi_colon             = l_xi_colon;

}

void Adenoma::grow(int subject_age){
    // time is since the initiation of the adenoma i.e
    // person@age-object@initiated.in.year or
    int adenoma_age  = subject_age-initiated_in_year;
    double dinfinity = 50.0;
    double d0        = 1.0;
    size             = dinfinity-(dinfinity-d0)*exp(-lambda*adenoma_age);
}

void Adenoma::transition(int subject_age){
    // The function looks after transition of ADENOMAS:
    // adenoma to large adenoma
    // large adenoma "CRC"
    //
    // The transtition of COLON to "symptomatic CRC" is done in the function
    //  Colon::updateState()

    Rcpp::RNGScope Scope;

    if (size >=10 && state=="adenoma"){
        state="large adenoma";
    }

    // if it is already a "CRC" then there is nothing to do
    // if it is an adenoma (or large adenoma)  we check to see if it transitions to a "pre clinical CRC"
    if ( state=="adenoma" ||  state=="large adenoma"){
        double p1=Rcpp::stats::pnorm_0(
                (log(gamma1*size)+gamma2*(initiated_in_year-50))/gamma3,
                1, 0);

        std::string transitions[] = {"transition","no transition"};
        double transition_probs[] = {p1,1-p1};
        Rcpp::StringVector CRCSPIN_TRANSITION_OPTIONS(transitions, transitions+sizeof(transitions)/sizeof(std::string));
        Rcpp::NumericVector CRCSPIN_TRANSITION_PROBS(transition_probs, transition_probs+sizeof(transition_probs)/sizeof(double));
        Rcpp::StringVector sv = SpinUtil::sample(
                CRCSPIN_TRANSITION_OPTIONS,
                1,
                false,
                CRCSPIN_TRANSITION_PROBS);
        std::string dice(sv[0]);

        if (dice =="transition"){
          state="CRC";
          transition_to_preclinical_crc_year=subject_age;
          sojourn_time=exp(R::rnorm(xi_colon, nu_colon));
        }
      } //end of transition to "CRC"

}

bool Adenoma::checkCRCSymptomPresentation(int subject_age){
    // This function checks if an adenoma starts causing CRC symptoms to
    // present in a person
    if(state=="CRC"){
        if(transition_to_preclinical_crc_year+sojourn_time <= subject_age){
            return true;
        }
    }
    return false;
}

void Adenoma::show(){
    Rcpp::Rcout << "Spin Adenoma object of class "
        << typeid(this).name() << "\n";
}



// Colon and PersonWithColon classes are class templates.
// Their implementation needs to be in the header file so that the compiler can
// see them WHILE it is compiling any other .cpp files that utilise them (e.g.
// derived classes)

/*
 * # Colon Class
 * #############
 */

// see split_headers/crcSpin_postRcpp.h for implementation of Colon class

/*
 * # Person with colon class
 * #########################
 */

// see split_headers/crcSpin_postRcpp.h for implementation of PersonWithColon class


/*
 * #####################################################################
 * #
 * # Rcpp Modules definition
 * #
 * #####################################################################
 */

using namespace Rcpp;

RCPP_MODULE(crc_spin) {

    class_< CrcSpinModel<> >("CrcSpinModel")

    .property( "study_results", &CrcSpinModel<>::getStudyResults)

    .constructor<int,int,int,int>()
    .constructor<std::vector< PersonWithColon<> >,int,int,int,int>()

    .method( "setCrcRiskModelingParameters", &CrcSpinModel<>::setCrcRiskModelingParameters)
    .method( "setAdenomaModelingParameters", &CrcSpinModel<>::setAdenomaModelingParameters)
    .method( "run", &CrcSpinModel<>::runWrap)
    .method( "show", &CrcSpinModel<>::showWrap)
    ;

}

