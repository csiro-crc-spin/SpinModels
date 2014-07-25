 organize_results<-function(object){
    if(class(object)=="Rcpp_CrcSpinModel"){
#        number.M<-sum(unlist (lapply(object$study_group,f<-function(x){x$sex}))=="M")
#        number.F<-sum(unlist(lapply(object$study_group,f<-function(x){x$sex}))=="F")
        number.M<-25000
        number.F<-25000
        results.F<-data.frame(object$study_results[(1:99)*2-1,] )
        results.M<-data.frame(object$study_results[(1:99)*2,]   )
        names(results.F)<-c(
            "adenoma.state.adenoma",
            "adenoma.state.large.adenoma",
            "adenoma.state.pre.symptomatic.CRC",
            "person.colon_clinical.CRC",
            "colon.state.clear",
            "colon.state.adenoma",
            "colon.state.large.adenoma",
            "colon.state.pre.symptomatic.CRC",
            "colon.state.CRC",
            "colon.cancer_site.cecum",
            "colon.cancer_site.ascending",
            "colon.cancer_site.transverse",
            "colon.cancer_site.descending",
            "colon.cancer_site.sigmoid",
            "colon.cancer_site.rectum",
            "person.state.deceased",
            "person.colon_clinical.CRC",
            "person.initiateCRCTreatment.called"
            )
        names(results.M)<-names(results.F)
        descriptions<-c(
            "A count of adenomas in all patients of the study group that have state 'adenoma'",
            "A count of adenomas in all patients of the study group that have state 'large adenoma'",
            "A count of adenomas in all patients of the study group that have state 'pre symptomatic CRC'",
            "A count of patients that have colon_clinical state 'CRC'",
            "A count of how many peoples colons were in state 'clear'",
            "A count of how many peoples colons were in state 'adenoma'",
            "A count of how many peoples colons were in state 'large adenoma'",
            "A count of how many peoples colons were in state 'pre symptomatic CRC'",
            "A count of how many peoples colons were in state 'CRC'",
            "A count of how many people's colons overall that have the majority of their adenomas in the 'cecum' location",
            "A count of how many people's colons overall that have the majority of their adenomas in the 'ascending' location",
            "A count of how many people's colons overall that have the majority of their adenomas in the 'transverse' location",
            "A count of how many people's colons overall that have the majority of their adenomas in the 'descending' location",
            "A count of how many people's colons overall that have the majority of their adenomas in the 'sigmoid' location",
            "A count of how many people's colons overall that have the majority of their adenomas in the 'rectum' location",
            "A count of people in the study that have died (i.e. their state is 'deceased')",
            "A count of patients that have colon_clinical state 'CRC'",
            "A count of people put into a treatment program THIS ITERATIN"
            )
        descriptions<-data.frame(names(results.F),descriptions)
        res<-list(number.M,number.F,results.M,results.F,descriptions)
        names(res)<-c("number.M","number.F","results.M","results.F","descriptions")
    }
    if(class(object)=="Rcpp_DukesCrcSpinModel"){
#        number.M<-sum(unlist (lapply(object$study_group,f<-function(x){x$sex}))=="M")
#        number.F<-sum(unlist(lapply(object$study_group,f<-function(x){x$sex}))=="F")
        number.M<-25000
        number.F<-25000
        results.F<-data.frame(object$study_results[(1:99)*2-1,] )
        results.M<-data.frame(object$study_results[(1:99)*2,]   )
        names(results.F)<-c("adenoma.state.adenoma",
                            "adenoma.state.large.adenoma",
                            "adenoma.state.CRC",
                            "adenoma.state.deceased",
                            "person.colon_clinical.symptomatic.CRC",
                            "colon.state.clear",
                            "colon.state.adenoma",
                            "colon.state.large.adenoma",
                            "colon.state.CRC",
                            "colon.state.symptomatic.CRC",
                            "colon.state.deceased",
                            "colon.state.CRC.adenoma.stage.A",
                            "colon.state.CRC.adenoma.stage.B",
                            "colon.state.CRC.adenoma.stage.C",
                            "colon.state.CRC.adenoma.stage.D",
                            "colon.state.CRC.adenoma.stage.deceased",
                            "colon.state.CRC.colon.cancer_site.cecum",
                            "colon.state.CRC.colon.cancer_site.ascending",
                            "colon.state.CRC.colon.cancer_site.transverse",
                            "colon.state.CRC.colon.cancer_site.descending",
                            "colon.state.CRC.colon.cancer_site.sigmoid",
                            "colon.state.CRC.colon.cancer_site.rectum",
                            "colon.state.symptomatic.CRC.adenoma.stage.A",
                            "colon.state.symptomatic.CRC.adenoma.stage.B",
                            "colon.state.symptomatic.CRC.adenoma.stage.C",
                            "colon.state.symptomatic.CRC.adenoma.stage.D",
                            "colon.state.symptomatic.CRC.adenoma.stage.deceased",
                            "colon.state.symptomatic.CRC.colon.cancer_site.cecum",
                            "colon.state.symptomatic.CRC.colon.cancer_site.ascending",
                            "colon.state.symptomatic.CRC.colon.cancer_site.transverse",
                            "colon.state.symptomatic.CRC.colon.cancer_site.descending",
                            "colon.state.symptomatic.CRC.colon.cancer_site.sigmoid",
                            "colon.state.symptomatic.CRC.colon.cancer_site.rectum",
                            "colon.cancer_site.cecum",
                            "colon.cancer_site.ascending",
                            "colon.cancer_site.transverse",
                            "colon.cancer_site.descending",
                            "colon.cancer_site.sigmoid",
                            "colon.cancer_site.rectum",
                            "colon.stage.A",
                            "colon.stage.B",
                            "colon.stage.C",
                            "colon.stage.D",
                            "colon.stage.deceased",
                            "person.state.deceased.person.state.deceased.CRC",
                            "person.colon_clinical.symptomatic.CRC",
                            "not.implemented",
                            "not.implemented",
                            "not.implemented",
                            "not.implemented",
                            "not.implemented",
                            "person.initiateCRCTreatment.colonoscopy_performed",
                            "person.initiateCRCTreatment.colon.state.large.adenoma",
                            "person.initiateCRCTreatment.colon.stage.A",
                            "person.initiateCRCTreatment.colon.stage.B",
                            "person.initiateCRCTreatment.colon.stage.C",
                            "person.initiateCRCTreatment.colon.stage.D",
                            "person.initiateCRCTreatment.person.in_treatment_program",
                            "person.initiateCRCTreatment.colonoscopy_caused_bleeding",
                            "person.initiateCRCTreatment.colonoscopy_caued_perforation"
                            )
        names(results.M)<-names(results.F)
        descriptions<-c("A count of adenomas in all patients of the study group that have state \"adenoma\"",
                        "A count of adenomas in all patients of the study group that have state \"large adenoma\"",
                        "A count of adenomas in all patients of the study group that have state \"CRC\"",
                        "A count of adenomas in all patients of the study group that have state \"deceased\"",
                        "A count of patients that have colon_clinical state \"symptomatic CRC\"",
                        "A count of how many peoples colons were in state \"clear\"",
                        "A count of how many peoples colons were in state \"adenoma\"",
                        "A count of how many peoples colons were in state \"large adenoma\"",
                        "A count of how many peoples colons were in state \"CRC\"",
                        "A count of how many peoples colons were in state \"symptomatic CRC\"",
                        "A count of how many peoples colons were in state \"deceased\"",
                        "A count of the adenomas that are at stage \"A\" in colons with state \"CRC\" across all people in the study group",
                        "A count of the adenomas that are at stage \"B\" in colons with state \"CRC\" across all people in the study group",
                        "A count of the adenomas that are at stage \"C\" in colons with state \"CRC\" across all people in the study group",
                        "A count of the adenomas that are at stage \"D\" in colons with state \"CRC\" across all people in the study group",
                        "A count of the adenomas that are at stage \"deceased\" in colons with state \"CRC\" in all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"cecum\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"ascending\" location across all people in the study grou",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"transverse\" location across all people in the study gro",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"descending\" location across all people in the study gro",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"sigmoid\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"rectum\" location across all people in the study group",
                        "A count of the adenomas that are at stage \"A\" in people of the study group with colon_clinical set to \"symptomatic CRC\"",
                        "A count of the adenomas that are at stage \"B\" in people of the study group with colon_clinical set to \"symptomatic CRC\"",
                        "A count of the adenomas that are at stage \"C\" in people of the study group with colon_clinical set to \"symptomatic CRC\"",
                        "A count of the adenomas that are at stage \"D\" in people of the study group with colon_clinical set to \"symptomatic CRC\"",
                        "A count of the adenomas that are at stage \"deceased\" in people of the study group with colon_clinical set to \"symptomatic CRC\"",
#                        "A count of the adenomas that are at stage \"deceased\" in people of the study group with colon_clinical set to \"symptomatic CRC\"",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"cecum\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"ascending\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"transverse\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"descending\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"sigmoid\" location across all people in the study group",
                        "A count of colons in \"state\" that have the majority of their adenomas in the \"rectum\" location across all people in the study group",
                        "A count of how many people's colons overall that have the majority of their adenomas in the \"cecum\" location",
                        "A count of how many people's colons overall that have the majority of their adenomas in the \"ascending\" location",
                        "A count of how many people's colons overall that have the majority of their adenomas in the \"transverse\" location",
                        "A count of how many people's colons overall that have the majority of their adenomas in the \"descending\" location",
                        "A count of how many people's colons overall that have the majority of their adenomas in the \"sigmoid\" location",
                        "A count of how many people's colons overall that have the majority of their adenomas in the \"rectum\" location",
                        "A count of how many people's colons overall were in stage \"A\"",
                        "A count of how many people's colons overall were in stage \"B\"",
                        "A count of how many people's colons overall were in stage \"C\"",
                        "A count of how many people's colons overall were in stage \"D\"",
                        "A count of how many people's colons overall were in stage \"deceased\"",
 #                       "A count of people in the study that have died (i.e. their state is \"deceased\")",
                        "A count of people in the study that have died (i.e. their state is \"deceased\" or \"deceased CRC\")",
                        "A count of patients that have colon_clinical state \"symptomatic CRC\"",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "A count of people that where treated for CRC IN THIS ITERATION, in which a colonoscopy was performed",
                        "A count of people that where treated for CRC IN THIS ITERATION, who had colons in state \"adenoma\" or \"large adenoma\"",
                        "A count of people that where treated for CRC IN THIS ITERATION, who had colons in state \"symptomatic CRC\" and in stage \"A\"",
                        "A count of people that where treated for CRC IN THIS ITERATION, who had colons in state \"symptomatic CRC\" and in stage \"B\"",
                        "A count of people that where treated for CRC IN THIS ITERATION, who had colons in state \"symptomatic CRC\" and in stage \"C\"",
                        "A count of people that where treated for CRC IN THIS ITERATION, who had colons in state \"symptomatic CRC\" and in stage \"D\"",
                        "A count of people put into a treatment program THIS ITERATION!!!",
                        "A count of people that where treated for CRC IN THIS ITERATION, whose colonoscopy caused bleeding",
                        "A count of people that where treated for CRC IN THIS ITERATION, whose colonoscopy caused perforation")
        
        descriptions<-data.frame(names(results.F),descriptions)
        names(descriptions)<-c("variable.name", "description")
        res<-list(number.M,number.F,results.M,results.F,descriptions)
        names(res)<-c("number.M","number.F","results.M","results.F","descriptions")
    }
    res
}
