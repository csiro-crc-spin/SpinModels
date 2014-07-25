/*
 * #####################################################################
 * #
 * # Generic spin model class definitions
 * #
 * #####################################################################
 */

// The following class definitions up to Person class
// definition MUST appear BEFORE include of Rcpp.h (incl. from
// RcpparmadilloExtensions.sample.h) so that the
// Rcpp template dispatch mechanism can see the Person::Person(SEXP)
// constructor

// Required for following definition of Person

/*
 * # Clinical Event class
 * ######################
 */

class ClinicalEvent {
    public:
        int age;

        virtual void show() = 0;
};

/*
 * # Clinical Test class
 * #####################
 */

class Test : public ClinicalEvent {
    public:
        std::string type;
        std::string compliance;
        std::string result;
        std::string state;

        virtual void show();
};


/*
 * # Clinical History class
 * ########################
 */

class ClinicalHistory {

    public:
        std::string status;
        std::vector<ClinicalEvent*> events;

        void show();

};




/*
 * # Person class
 * ##############
 */

class Person {

    public:
        int age;
        char sex;
        std::string state; //#"deceased","living", etc (as needed by subclasses)
        bool in_treatment_program;
        ClinicalHistory clinical_history;
        double study_id;
        std::vector<int> random_seed_state;

        Person (SEXP p);

        Person (int study_id=0,
                int base_seed=125,
                int age=1,
                char sex='\0',
                std::string state="living",
                bool in_treatment_program=false,
                ClinicalHistory clinical_history=ClinicalHistory(),
                std::vector<int> random_seed_state=std::vector<int>(),
                ...);

        void saveRNGState();

        void restoreRNGState();

        void modelDeathFromOtherCauses();

        void show();

};



/*
 * # Risk parameters class
 * #######################
 */

class Risk {
    public:
        double baseline_risk;
        double sex_linked_risk;
        double age_risk;
};

