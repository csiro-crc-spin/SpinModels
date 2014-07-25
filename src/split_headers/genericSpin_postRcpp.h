// The GenericModel class definition below MUST appear AFTER include of
// Rcpp.h as it is using Rcpp types defined in that header.


/*
 * # Model class
 * #############
 */

template <class PersonType> class GenericModel {

    public:
        int iterations;
        int iteration_resolution;
        int commencement_age;
        std::vector<PersonType> study_group;
        Rcpp::IntegerMatrix study_results;

    protected:
        void init(  int iterations=0,
                    int iteration_resolution=1,
                    int num_subjects=0,
                    Rcpp::IntegerMatrix study_results=Rcpp::IntegerMatrix(0,0),
                    int base_seed=NA_INTEGER,
                    int commencement_age=1,
                    std::vector<PersonType> study_group=std::vector<PersonType>());

    public:

        GenericModel (  std::vector<PersonType> study_group,
                        int iterations=0,
                        int iteration_resolution=1,
                        int num_subjects=0,
                        int base_seed=NA_INTEGER,
                        int commencement_age=1,
                        Rcpp::IntegerMatrix study_results=Rcpp::IntegerMatrix(0,0));

        GenericModel (  int iterations=0,
                        int iteration_resolution=1,
                        int num_subjects=0,
                        int base_seed=NA_INTEGER,
                        int commencement_age=1,
                        Rcpp::IntegerMatrix study_results=Rcpp::IntegerMatrix(0,0));

        void run();

        int getModelResultSize();

        virtual Rcpp::IntegerVector updateSubject(PersonType &person);

        virtual Rcpp::IntegerMatrix doIteration();

        void show();
};



/*
 * # GenericModel functions
 * ########################
 */


template <class PersonType> void GenericModel<PersonType>::init(
            int iterations,
            int iteration_resolution,
            int num_subjects,
            Rcpp::IntegerMatrix study_results,
            int base_seed,
            int commencement_age,
            std::vector<PersonType> study_group){


    if (study_group.empty() && num_subjects>0) {
        this->study_group.reserve(num_subjects);
        for (int i=1; i<=num_subjects; i++){
            this->study_group.push_back(
                PersonType(i, base_seed, commencement_age));
        }
    } else {
        // checks & processing should be done here to
        // truncate study_group length to num_subjects
        // TODO...
        this->study_group=study_group;
    }

    if ((study_results.ncol()==0 || study_results.nrow()==0)
        && iterations > 0) {
        this->study_results = Rcpp::IntegerMatrix(iterations*2,1);
    } else if (study_results.nrow() < iterations*2
               && study_results.ncol() < 1){
        // Handle the situation where provided
        // study_results matrix does not have enough rows or cols
        // to hold an aggregated population result for each
        // iteration/step of the study

        // If you do not want this granularity of study result
        // (e.g. you want a single aggregated across all
        // subjects AND iterations/steps), this code and
        // run()/doIteration() will need to be overwritten in
        // a derived class
    } else {
        this->study_results=study_results;
    }

    this->iterations=iterations;
    this->iteration_resolution=iteration_resolution;
    this->commencement_age=commencement_age;
}


template <class PersonType> GenericModel<PersonType>::GenericModel (
                std::vector<PersonType> study_group,
                int iterations,
                int iteration_resolution,
                int num_subjects,
                int base_seed,
                int commencement_age,
                Rcpp::IntegerMatrix study_results){

    init(iterations, iteration_resolution, num_subjects,
            study_results, base_seed, commencement_age,
            study_group);


}

template <class PersonType> GenericModel<PersonType>::GenericModel (
                int iterations,
                int iteration_resolution,
                int num_subjects,
                int base_seed,
                int commencement_age,
                Rcpp::IntegerMatrix study_results){

    init(iterations, iteration_resolution, num_subjects,
            study_results, base_seed, commencement_age,
            std::vector<PersonType>());

}

template <class PersonType> void GenericModel<PersonType>::run(){

    for (int i=commencement_age-1; i<iterations; i++){
        // We have to a long winded assignment in place of a simpler
        // study_results(Rcpp::Range(i*2,i*2+1),Rcpp::_)=doIteration()
        // due to lack of Rcpp operator= for Rcpp::SubMatrix
        Rcpp::IntegerMatrix res=doIteration();
        study_results(i*2,Rcpp::_)=res(0,Rcpp::_);
        study_results(i*2+1,Rcpp::_)=res(1,Rcpp::_);
    }

}

template <class PersonType> int GenericModel<PersonType>::getModelResultSize(){
    return 1;
}

template <class PersonType> Rcpp::IntegerVector GenericModel<PersonType>::updateSubject(PersonType &subject) {
    if (subject.state=="living") {
        subject.restoreRNGState();
        subject.age += iteration_resolution;
        subject.modelDeathFromOtherCauses();
        subject.saveRNGState();
    }
    return Rcpp::IntegerVector(1,(subject.state=="living"));
}

template <class PersonType> Rcpp::IntegerMatrix GenericModel<PersonType>::doIteration(){

    Rcpp::IntegerMatrix study_record(2, study_results.ncol());
    for (unsigned int i=0; i<study_group.size(); i++){
        int sex_row=0;
        if (study_group[i].sex == 'F') {
            sex_row=0;
        } else {
            sex_row=1;
        }

        Rcpp::IntegerMatrix::Row cur_record=study_record(sex_row, Rcpp::_);
        cur_record=cur_record+updateSubject(study_group[i]);
    }
    return study_record;
}

template <class PersonType> void GenericModel<PersonType>::show(){
    Rcpp::Rcout << "Spin Model object of class " <<
        typeid(this).name() << "\n";
    Rcpp::Rcout << "Iterations: " << iterations << "\n";
    Rcpp::Rcout << "Iteration resolution: " << iteration_resolution << "\n";
    Rcpp::Rcout << "Commencement age: " << commencement_age << "\n";
    // Rcpp::Rcout << "Study group:\n";
    // methods::show(study_group)
    Rcpp::Rcout << "Study group size: " << study_group.size() << "\n";
}

