/* vim: set fileformat=unix : */
#include <genericSpin.h>
#include <spinUtil.h>

/*
 * #####################################################################
 * #
 * # Global variables and init code for Spin Model classes
 * #
 * #####################################################################
 */
/*
 * # Create character base option vectors
 * ######################################
 */


std::string sexes[] = {"F","M"};
Rcpp::CharacterVector SPIN_SEX_OPTIONS(sexes, sexes+sizeof(sexes)/sizeof(std::string));

std::string life[] = {"deceased", "living"};
Rcpp::StringVector SPIN_LIFE_STATUS_OPTIONS(life, life+sizeof(life)/sizeof(std::string));


/*
 * # Load & Set Death Rate Data
 * ############################
 */

/* NOTE: The death rate R code that follows the C++ MUST be present!
 * i.e. The R code has not been replaced with C++ code
 */

std::vector<double> death_rate_male;
std::vector<double> death_rate_female;

void setDeathRateTables(std::vector<double> d_r_male, std::vector<double> d_r_female){
    death_rate_male=d_r_male;
    death_rate_female=d_r_female;
}

/*** R
# the following variables should not be global...but i got lazy

#http://www.abs.gov.au/AUSSTATS/abs@.nsf/DetailsPage/3302.0.55.0012008-2010?OpenDocument
#
#lx the number of persons surviving to exact age x;
#qx the proportion of persons dying between exact age x and exact age x+1.
#It is the mortality rate, from which other functions of the life table are derived;
#Lx the number of person years lived within the age interval x to x+1; and
#ex life expectancy at exact age x.

# dr_data<-read.table(file="../data/3302055001DO001_20082010.csv",sep=",",skip=5,header=TRUE,as.is=TRUE)
dr_data<-read.table(file="/home/dom039/ACPs/H12013/COLO/CRC-Spin2/data/3302055001DO001_20082010.csv",sep=",",skip=5,header=TRUE,as.is=TRUE)
dr_data<-dr_data[-c(1,103,104,105),]
death_rate_male<-as.numeric(dr_data$qx)
death_rate_female<-as.numeric(dr_data$qx.1)
setDeathRateTables(death_rate_male, death_rate_female)
*/





/*
 * #####################################################################
 * #
 * # Member function implementation of Generic Spin model classes
 * #
 * #####################################################################
 */


/*
 * # Person functions
 * ##################
 */


Person::Person (SEXP p){
    Rcpp::Reference person(p);
    age=person.field("age");
    sex=*(Rcpp::as<const char*>(person.field("sex")));
    std::string s=person.field("state");
    state=s;
    in_treatment_program=person.field("in_treatment_program");
    study_id=person.field("study_id");
}


Person::Person (int study_id,
        int base_seed,
        int age,
        char sex,
        std::string state,
        bool in_treatment_program,
        ClinicalHistory clinical_history,
        std::vector<int> random_seed_state,
        ...){

    Rcpp::Environment globalEnv = Rcpp::Environment::global_env();
    if (random_seed_state.empty()) {
        Rcpp::Environment baseEnv = Rcpp::Environment::base_env();
        Rcpp::Function set_seed = baseEnv["set.seed"];
        // Rcpp::IntegerVector iv = globalEnv[".Random.seed"];
        // Rcpp::Rcout << ".Random.seed=" << iv[0] << "\n";
        set_seed(base_seed+study_id);
        // iv = globalEnv[".Random.seed"];
        // Rcpp::Rcout << ".Random.seed=" << iv[0] << "\n";
    } else {
        globalEnv[".Random.seed"]=Rcpp::wrap(random_seed_state);
    }

    char s;
    if (sex=='\0' || !(sex=='F' || sex=='M')) {
        Rcpp::RNGScope scope;
        Rcpp::CharacterVector sv = SpinUtil::sample(SPIN_SEX_OPTIONS, 1,
                false, Rcpp::NumericVector::create() );
        s=*((char*)sv[0]);
    } else {
        s=sex;
    }

    this->age=age;
    this->sex=s;
    this->state=state;
    this->in_treatment_program=in_treatment_program;
    this->clinical_history=clinical_history;
    this->study_id=study_id;
    this->random_seed_state=globalEnv[".Random.seed"];

}

void Person::saveRNGState() {
    Rcpp::Environment globalEnv = Rcpp::Environment::global_env();
    Rcpp::IntegerVector rs = globalEnv[".Random.seed"];
    // Rcpp::IntegerVector rs(globalEnv[".Random.seed"]);
    random_seed_state=Rcpp::as< std::vector<int> >(rs);
}

void Person::restoreRNGState() {
    Rcpp::Environment globalEnv = Rcpp::Environment::global_env();
    globalEnv[".Random.seed"]=Rcpp::wrap(random_seed_state);
}

void Person::modelDeathFromOtherCauses() {

    Rcpp::RNGScope scope;
    if(sex=='F'){
        Rcpp::NumericVector prob(2);
        prob[0]=death_rate_female[age-1];
        prob[1]=1-death_rate_female[age-1];
        Rcpp::StringVector sv = SpinUtil::sample(SPIN_LIFE_STATUS_OPTIONS, 1,
                false, prob);
        state=sv[0];
    }
    if(sex=='M'){
        Rcpp::NumericVector prob(2);
        prob[0]=death_rate_male[age-1];
        prob[1]=1-death_rate_male[age-1];
        Rcpp::StringVector sv = SpinUtil::sample(SPIN_LIFE_STATUS_OPTIONS, 1,
                false, prob);
        state=sv[0];
    }
}

void Person::show(){
    Rcpp::Rcout << "Spin Person object of class "
        << typeid(this).name() << "\n";
    Rcpp::Rcout << "Age:" << age << "\n";
    Rcpp::Rcout << "Sex:" << sex << "\n";
    Rcpp::Rcout << "State:" << state << "\n";
    Rcpp::Rcout << "In treatment program:" << in_treatment_program << "\n";
    Rcpp::Rcout << "Study id:" << study_id << "\n";
    Rcpp::Rcout << "Clinical history:\n";
    clinical_history.show();
}




/*
 * # ClinicalTest functions
 * #########################
 */

void Test::show(){
    Rcpp::Rcout << "Spin ClinicalEvent object of class "
        << typeid(this).name() << "\n";
    Rcpp::Rcout << "Age:" << age << "\n";
    Rcpp::Rcout << "Type of test:" << type << "\n";
    Rcpp::Rcout << "Compliance:" << compliance << "\n";
    Rcpp::Rcout << "Result:" << result << "\n";
    Rcpp::Rcout << "State:" << state << "\n";
}



/*
 * # Clinical History functions
 * ############################
 */

void ClinicalHistory::show(){
    Rcpp::Rcout << "Spin ClinicalHistory object of class" <<
        typeid(this).name() << "\n";
    Rcpp::Rcout << "Status:" << status << "\n";
    Rcpp::Rcout << "Events:\n";
    for (int i = 0; i < events.size(); i++) {
        Rcpp::Rcout << "Event[" << i <<"]:\n";
        events[i]->show();
    }
}




/*
 * #####################################################################
 * #
 * # Rcpp Modules definition
 * #
 * #####################################################################
 */

using namespace Rcpp;

RCPP_MODULE(generic_spin) {

    function( "setDeathRateTables", &setDeathRateTables);

    class_< GenericModel<Person> >("GenericModel")

    .field( "study_results", &GenericModel<Person>::study_results)

    .constructor<int,int,int,int,int,Rcpp::IntegerMatrix>()
    .constructor<std::vector<Person>,int,int,int,int,int,Rcpp::IntegerMatrix>()

    .method( "run", &GenericModel<Person>::run)
    .method( "show", &GenericModel<Person>::show)
    ;

}

