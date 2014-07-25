/* vim: set fileformat=unix : */
#include <dukesCrcSpin.h>
#include <spinUtil.h>
#include <cmath>
#include <algorithm>
#include <functional>


/*
 * ##############################
 * #
 * # Dukes CRC spin model classes
 * #
 * ##############################
 */



/*
 * # Symptomatic Presentation functions
 * ####################################
 */

SymptomaticPresentation::SymptomaticPresentation(
        int age,
        std::string stage){

    this->age=age;
    this->cancer_stage=stage;
}

void SymptomaticPresentation::show(){
    Rcpp::Rcout << "Spin ClinicalEvent object of class "
        << typeid(this).name() << "\n";
    Rcpp::Rcout << "Age:" << age << "\n";
    Rcpp::Rcout << "Cancer Stage:" << cancer_stage << "\n";

}

/*
 * # Adenoma modeling parameter class
 * ##################################
 */

DukesAdenomaParams::DukesAdenomaParams()
: AdenomaParams() {

    // Set Duke's specific default values here
    sojournA_min = 1;
    sojournA_max = 3;
    sojournB_val = 1;
    sojournC_min = 1;
    sojournC_max = 2;
    sojournD_val = 1;

    // call setParams() with no arguments to ensure
    // your defaults meet valid ranges
    // NOTE: that we force the use of this (base)
    // Classes setParams to avoid running a
    // derived class' setParams function before
    // they might have had a chance to set their
    // defaults
    DukesAdenomaParams::setParams();

}


void DukesAdenomaParams::setParams(
            std::vector<std::string> param_keys,
            std::vector<double> param_vals){

    std::vector<std::string> pass_through_keys;
    std::vector<double> pass_through_vals;

    if(param_keys.size()==param_vals.size()){
        if(!param_keys.empty()){
            for (unsigned int i = 0; i < param_keys.size(); i++) {
                if ( param_keys[i] == "sojournA_min" ){
                    sojournA_min=static_cast<int>(param_vals[i]);

                } else if ( param_keys[i] == "sojournA_max" ){
                    sojournA_max=static_cast<int>(param_vals[i]);

                } else if ( param_keys[i] == "sojournB_val" ){
                    sojournB_val=static_cast<int>(param_vals[i]);

                } else if ( param_keys[i] == "sojournC_min" ){
                    sojournC_min=static_cast<int>(param_vals[i]);

                } else if ( param_keys[i] == "sojournC_max" ){
                    sojournC_max=static_cast<int>(param_vals[i]);

                } else if ( param_keys[i] == "sojournD_val" ){
                    sojournD_val=static_cast<int>(param_vals[i]);

                } else {
                    // We don't know about this setting, but our
                    // superclass might
                    // Save key value pair to a pass_through list
                    pass_through_keys.push_back(param_keys[i]);
                    pass_through_vals.push_back(param_vals[i]);
                }
            } // end for

            // Pass settings we don't recognise through to superclass
            AdenomaParams::setParams(pass_through_keys, pass_through_vals);

        }

        // Enforce ordering on min max relationships
        sojournA_max
            =std::max(sojournA_min, sojournA_max);
        sojournC_max
            =std::max(sojournC_min, sojournC_max);


    } else {
        Rcpp::Rcout << "DukesAdenomaParams::setParams() "
                << "param_keys (size=" << param_keys.size() << ") and"
                << "param_vals (size=" << param_vals.size() << ") "
                << "do not have the same number of elements : No parameters set\n";
    }

}



/*
 * # Duke's Adenoma class
 * ######################
 */

DukesAdenoma::DukesAdenoma (DukesAdenomaParams &params,
         int initiated_in_year,
         double size,
         std::string state,
         std::string stage,
         ...)
: Adenoma(params, initiated_in_year, size, state) {

    this->stage=stage;
    this->adenoma_model_params = &params;

}


void DukesAdenoma::transition(int subject_age){
    // The function handles ADENOMA transitions
    //
    // it calls its super class implementation to handle
    // adenoma to large adenoma
    // large adenoma to CRC (no stage set)
    //
    // Then handles transition of CRC stages
    // A to B
    // B to C
    // C  to D
    // D to deceased
    // A,B,C,D, are all "CRC"
    //
    // The transition of COLON to "symptomatic CRC" is done
    // in the function DukesColon::modelTransitionToClinicalCRC()


    // Call Adenoma superclass implementation to
    // handle Classic CRC transitions
    Adenoma::transition(subject_age);

    Rcpp::RNGScope Scope;

    if (state =="CRC"){
        if(transition_to_preclinical_crc_year==subject_age){

            stage="A";
            transition_to_stage_A=subject_age;
;
            sojourn_time_A=R::runif(adenoma_model_params->sojournA_min,adenoma_model_params->sojournA_max);
            sojourn_time_B=adenoma_model_params->sojournB_val;
            sojourn_time_C=R::runif(adenoma_model_params->sojournC_min,adenoma_model_params->sojournC_max);
            sojourn_time_D=adenoma_model_params->sojournD_val;

            // next line overwrites the sojourn time saved by super/parent class
            sojourn_time=sojourn_time_A+sojourn_time_B+sojourn_time_C+sojourn_time_D;
        }

        if(stage =="A"){
            if (transition_to_stage_A+sojourn_time_A  <= subject_age){
                transition_to_stage_B=subject_age;
                stage="B";
            }
        }
        if(stage =="B"){
            if (transition_to_stage_B+sojourn_time_B  <= subject_age){
                transition_to_stage_C=subject_age;
                stage="C";
            }
        }
        if(stage =="C"){
            if (transition_to_stage_C+sojourn_time_C  <= subject_age){
                transition_to_stage_D=subject_age;
                stage="D";
            }
        }
        if(stage =="D"){
            if (transition_to_stage_D+sojourn_time_D  <= subject_age){
                state="deceased";
                stage="deceased";
            }
        }
    }
}

bool DukesAdenoma::checkCRCSymptomPresentation(int subject_age){
    // In a Duke's CRC Spin model, presentation of symptoms
    // is modeled as a function of the colon state/stage
    // and does not involve a direct check of adenomas
    //
    // To avoid accidental errors/bugs, this dumby function
    // explicitly overwrites that inherited from the Classic
    // CRC spin models Adenoma Class, which DOES use a direct
    // check on adenomas
    return false;
}


// DukesColon and DukesPersonWithColon classes are class templates.
// Their implementation needs to be in the header file so that the compiler can
// see them WHILE it is compiling any other .cpp files that utilise them (e.g.
// derived classes)

/*
 * # Dukes Colon Class
 * ###################
 */

// see split_headers/dukesCrcSpin_postRcpp.h for implementation of DukesColon class

/*
 * # Dukes Person with colon class
 * ###############################
 */

// see split_headers/dukesCrcSpin_postRcpp.h for implementation of DukesPersonWithColon class



/*
 * #####################################################################
 * #
 * # Rcpp Modules definition
 * #
 * #####################################################################
 */

using namespace Rcpp;

RCPP_MODULE(dukes_crc_spin) {

    class_< DukesCrcSpinModel<> >("DukesCrcSpinModel")

    .property( "study_results", &DukesCrcSpinModel<>::getStudyResultsWrap)

    .constructor<int,int,int,int>()
    .constructor<std::vector< DukesPersonWithColon<> >,int,int,int,int>()

    .method( "setCrcRiskModelingParameters", &DukesCrcSpinModel<>::setCrcRiskModelingParametersWrap)
    .method( "setAdenomaModelingParameters", &DukesCrcSpinModel<>::setAdenomaModelingParametersWrap)
    .method( "run", &DukesCrcSpinModel<>::runWrap)
    .method( "show", &DukesCrcSpinModel<>::showWrap)
    ;

}

