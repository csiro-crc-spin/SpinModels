=========================
CrcSpin result matrix map
=========================

Below is pseudo code describing the expressions used to obtain study_result
column values in CrcSpinModel and DukesCrcSpinModel.

The expressions are applied over all ``persons``, then ``colons`` (one per person),
then ``adenomas`` (many per colon) in the study. The object nesting
``person.colon.adenoma`` is implied and is not written explicitly for brevity.

::

    ///////////////////////////////
    // CRC Spin Columns
    
    /////////// set in getMedicalSnapshot
    // Count of adenoma states overall
    aa[1]=count(adenoma.state=="adenoma");
    aa[2]=count(adenoma.state=="large adenoma");
    aa[3]=count(adenoma.state=="CRC");
    
    // How many people have transitioned to Clinical CRC
    aa[4]=count(person.colon_clinical=="symptomatic CRC");
    
    // Count of colon states oveall
    aa[5]=count(colon.state=="clear");
    aa[6]=count(colon.state=="adenoma");
    aa[7]=count(colon.state=="large adenoma");
    aa[8]=count(colon.state=="CRC");
    aa[9]=count(colon.state=="symptomatic CRC");
    
    
    // Count of colon sites overall
    aa[10]=count(colon.cancer_site=="cecum");
    aa[11]=count(colon.cancer_site=="ascending");
    aa[12]=count(colon.cancer_site=="transverse");
    aa[13]=count(colon.cancer_site=="descending");
    aa[14]=count(colon.cancer_site=="sigmoid");
    aa[15]=count(colon.cancer_site=="rectum");
    
    
    aa[16]=count(state=="deceased");
    aa[17]=count(person.colon_clinical=="symptomatic CRC");

    /////////// set in initiateCRCTreatment
    aa[18]=count(person.in_treatment_program)
    
    
    //////////////////////////////
    // Dukes CRC Spin Columns
    
    /////////// set in getMedicalSnapshot
    // Count of adenoma states overall
    aa[1] =count(adenoma.state=="adenoma");
    aa[2] =count(adenoma.state=="large adenoma");
    aa[3] =count(adenoma.state=="CRC");
    aa[4] =count(adenoma.state=="deceased");
    
    // How many people have transitioned to Clinical CRC
    aa[5] =count(person.colon_clinical=="symptomatic CRC");
    
    // Count of colon states overall
    aa[6] =count(colon.state=="clear");
    aa[7] =count(colon.state=="adenoma");
    aa[8] =count(colon.state=="large adenoma");
    aa[9] =count(colon.state=="CRC");
    aa[10]=count(colon.state=="symptomatic CRC");
    aa[11]=count(colon.state=="deceased");
    
    // Count of adenoma stages in people with colon state CRC 
    aa[12]=count(colon.state=="CRC" && adenoma.stage=="A");
    aa[13]=count(colon.state=="CRC" && adenoma.stage=="B");
    aa[14]=count(colon.state=="CRC" && adenoma.stage=="C");
    aa[15]=count(colon.state=="CRC" && adenoma.stage=="D");
    aa[16]=count(colon.state=="CRC" && adenoma.stage=="deceased");
    
    // Count of cancer sites in people with colon state CRC 
    aa[17]=count(colon.state=="CRC" && colon.cancer_site=="cecum");
    aa[18]=count(colon.state=="CRC" && colon.cancer_site=="ascending");
    aa[19]=count(colon.state=="CRC" && colon.cancer_site=="transverse");
    aa[20]=count(colon.state=="CRC" && colon.cancer_site=="descending");
    aa[21]=count(colon.state=="CRC" && colon.cancer_site=="sigmoid");
    aa[22]=count(colon.state=="CRC" && colon.cancer_site=="rectum");
    
    // Count of adenoma stages in people with colon state CRC 
    aa[23]=count(colon.state=="symptomatic CRC" && adenoma.stage=="A");
    aa[24]=count(colon.state=="symptomatic CRC" && adenoma.stage=="B");
    aa[25]=count(colon.state=="symptomatic CRC" && adenoma.stage=="C");
    aa[26]=count(colon.state=="symptomatic CRC" && adenoma.stage=="D");
    aa[27]=count(colon.state=="symptomatic CRC" && adenoma.stage=="deceased");
    
    // Count of cancer sites in people with colon state CRC 
    aa[28]=count(colon.state=="symptomatic CRC" && colon.cancer_site=="cecum");
    aa[29]=count(colon.state=="symptomatic CRC" && colon.cancer_site=="ascending");
    aa[30]=count(colon.state=="symptomatic CRC" && colon.cancer_site=="transverse");
    aa[31]=count(colon.state=="symptomatic CRC" && colon.cancer_site=="descending");
    aa[32]=count(colon.state=="symptomatic CRC" && colon.cancer_site=="sigmoid");
    aa[33]=count(colon.state=="symptomatic CRC" && colon.cancer_site=="rectum");
    
    // Count of cancer sites overall
    aa[34]=count(colon.cancer_site=="cecum");
    aa[35]=count(colon.cancer_site=="ascending");
    aa[36]=count(colon.cancer_site=="transverse");
    aa[37]=count(colon.cancer_site=="descending");
    aa[38]=count(colon.cancer_site=="sigmoid");
    aa[39]=count(colon.cancer_site=="rectum");
    
    // Count of colon stages overall
    aa[40]=count(colon.stage=="A");
    aa[41]=count(colon.stage=="B");
    aa[42]=count(colon.stage=="C");
    aa[43]=count(colon.stage=="D");
    aa[44]=count(colon.stage=="deceased");
    
    aa[45]=count(person.state=="deceased" || person.state=="deceased CRC");
    aa[46]=count(person.colon_clinical=="symptomatic CRC");

    //////////////// set in initiateCRCTreatment
    // Reserved for screening results
    aa[47]=count(if(CRC_treatment_initiated){FALSE} else {FALSE})
    aa[48]=count(if(CRC_treatment_initiated){FALSE} else {FALSE})
    aa[49]=count(if(CRC_treatment_initiated){FALSE} else {FALSE})
    aa[50]=count(if(CRC_treatment_initiated){FALSE} else {FALSE})
    aa[51]=count(if(CRC_treatment_initiated){FALSE} else {FALSE})
    
    // Counts of colonoscopy and colon info
    aa[52]=count(if(CRC_treatment_initiated){colonoscopy_performed} else {FALSE})
    aa[53]=count(if(CRC_treatment_initiated){(colon.state=="adenoma" || colon.state=="large adenoma")} else {FALSE})
    aa[54]=count(if(CRC_treatment_initiated){(colon.state=="symptomatic CRC" && colon.stage="A")} else {FALSE})
    aa[55]=count(if(CRC_treatment_initiated){(colon.state=="symptomatic CRC" && colon.stage="B")} else {FALSE})
    aa[56]=count(if(CRC_treatment_initiated){(colon.state=="symptomatic CRC" && colon.stage="C")} else {FALSE})
    aa[57]=count(if(CRC_treatment_initiated){(colon.state=="symptomatic CRC" && colon.stage="D")} else {FALSE})
    aa[58]=count(if(CRC_treatment_initiated){(person.in_treatment_program=TRUE))} else {FALSE})
    aa[59]=count(if(CRC_treatment_initiated && colonoscopy_performed){colonoscopy_caused_bleeding} else {FALSE})
    aa[60]=count(if(CRC_treatment_initiated && colonoscopy_performed){colonoscopy_caused_perforation} else {FALSE})

Below is a tabular summary of **ALL** columns across both models with a mapping
to which elements of the result array they occupy in each model.


============= ======== ============================================================================== ========================================================================================================================================================
DukesCrcSpin  CrcSpin  Expression                                                                     Description
============= ======== ============================================================================== ========================================================================================================================================================
1             1        count(adenoma.state=="adenoma");                                               A count of adenomas in all patients of the study group that have state "adenoma"
2             2        count(adenoma.state=="large adenoma");                                         A count of adenomas in all patients of the study group that have state "large adenoma"
3             3        count(adenoma.state=="CRC");                                                   A count of adenomas in all patients of the study group that have state "CRC"
4             .        count(adenoma.state=="deceased");                                              A count of adenomas in all patients of the study group that have state "deceased"
5             4        count(person.colon_clinical=="symptomatic CRC");                               A count of patients that have colon_clinical state "symptomatic CRC"
6             5        count(colon.state=="clear");                                                   A count of how many peoples colons were in state "clear"
7             6        count(colon.state=="adenoma");                                                 A count of how many peoples colons were in state "adenoma"
8             7        count(colon.state=="large adenoma");                                           A count of how many peoples colons were in state "large adenoma"
9             8        count(colon.state=="CRC");                                                     A count of how many peoples colons were in state "CRC"
10            9        count(colon.state=="symptomatic CRC");                                         A count of how many peoples colons were in state "symptomatic CRC"
11            .        count(colon.state=="deceased");                                                A count of how many peoples colons were in state "deceased"
12            .        count(colon.state=="CRC" && adenoma.stage=="A");                               A count of the adenomas that are at stage "A" in colons with state "CRC" across all people in the study group
13            .        count(colon.state=="CRC" && adenoma.stage=="B");                               A count of the adenomas that are at stage "B" in colons with state "CRC" across all people in the study group
14            .        count(colon.state=="CRC" && adenoma.stage=="C");                               A count of the adenomas that are at stage "C" in colons with state "CRC" across all people in the study group
15            .        count(colon.state=="CRC" && adenoma.stage=="D");                               A count of the adenomas that are at stage "D" in colons with state "CRC" across all people in the study group
16            .        count(colon.state=="CRC" && adenoma.stage=="deceased");                        A count of the adenomas that are at stage "deceased" in colons with state "CRC" in all people in the study group
17            .        count(colon.state=="CRC" && colon.cancer_site=="cecum");                       A count of colons in "state" that have the majority of their adenomas in the "cecum" location across all people in the study group
18            .        count(colon.state=="CRC" && colon.cancer_site=="ascending");                   A count of colons in "state" that have the majority of their adenomas in the "ascending" location across all people in the study group
19            .        count(colon.state=="CRC" && colon.cancer_site=="transverse");                  A count of colons in "state" that have the majority of their adenomas in the "transverse" location across all people in the study group
20            .        count(colon.state=="CRC" && colon.cancer_site=="descending");                  A count of colons in "state" that have the majority of their adenomas in the "descending" location across all people in the study group
21            .        count(colon.state=="CRC" && colon.cancer_site=="sigmoid");                     A count of colons in "state" that have the majority of their adenomas in the "sigmoid" location across all people in the study group
22            .        count(colon.state=="CRC" && colon.cancer_site=="rectum");                      A count of colons in "state" that have the majority of their adenomas in the "rectum" location across all people in the study group
23            .        count(colon.state=="symptomatic CRC" && adenoma.stage=="A");                   A count of the adenomas that are at stage "A" in people of the study group with colon_clinical set to "symptomatic CRC"
24            .        count(colon.state=="symptomatic CRC" && adenoma.stage=="B");                   A count of the adenomas that are at stage "B" in people of the study group with colon_clinical set to "symptomatic CRC"
25            .        count(colon.state=="symptomatic CRC" && adenoma.stage=="C");                   A count of the adenomas that are at stage "C" in people of the study group with colon_clinical set to "symptomatic CRC"
26            .        count(colon.state=="symptomatic CRC" && adenoma.stage=="D");                   A count of the adenomas that are at stage "D" in people of the study group with colon_clinical set to "symptomatic CRC"
27            .        count(colon.state=="symptomatic CRC" && adenoma.stage=="deceased");            A count of the adenomas that are at stage "deceased" in people of the study group with colon_clinical set to "symptomatic CRC"
28            .        count(colon.state=="symptomatic CRC" && colon.cancer_site=="cecum");           A count of colons in "state" that have the majority of their adenomas in the "cecum" location across all people in the study group
29            .        count(colon.state=="symptomatic CRC" && colon.cancer_site=="ascending");       A count of colons in "state" that have the majority of their adenomas in the "ascending" location across all people in the study group
30            .        count(colon.state=="symptomatic CRC" && colon.cancer_site=="transverse");      A count of colons in "state" that have the majority of their adenomas in the "transverse" location across all people in the study group
31            .        count(colon.state=="symptomatic CRC" && colon.cancer_site=="descending");      A count of colons in "state" that have the majority of their adenomas in the "descending" location across all people in the study group
32            .        count(colon.state=="symptomatic CRC" && colon.cancer_site=="sigmoid");         A count of colons in "state" that have the majority of their adenomas in the "sigmoid" location across all people in the study group
33            .        count(colon.state=="symptomatic CRC" && colon.cancer_site=="rectum");          A count of colons in "state" that have the majority of their adenomas in the "rectum" location across all people in the study group
34            10       count(colon.cancer_site=="cecum");                                             A count of how many people's colons overall that have the majority of their adenomas in the "cecum" location
35            11       count(colon.cancer_site=="ascending");                                         A count of how many people's colons overall that have the majority of their adenomas in the "ascending" location
36            12       count(colon.cancer_site=="transverse");                                        A count of how many people's colons overall that have the majority of their adenomas in the "transverse" location
37            13       count(colon.cancer_site=="descending");                                        A count of how many people's colons overall that have the majority of their adenomas in the "descending" location
38            14       count(colon.cancer_site=="sigmoid");                                           A count of how many people's colons overall that have the majority of their adenomas in the "sigmoid" location
39            15       count(colon.cancer_site=="rectum");                                            A count of how many people's colons overall that have the majority of their adenomas in the "rectum" location
40            .        count(colon.stage=="A");                                                       A count of how many people's colons overall were in stage "A"
41            .        count(colon.stage=="B");                                                       A count of how many people's colons overall were in stage "B"
42            .        count(colon.stage=="C");                                                       A count of how many people's colons overall were in stage "C"
43            .        count(colon.stage=="D");                                                       A count of how many people's colons overall were in stage "D"
44            .        count(colon.stage=="deceased");                                                A count of how many people's colons overall were in stage "deceased"
.             16       count(person.state=="deceased");                                               A count of people in the study that have died (i.e. their state is "deceased")
45            .        count(person.state=="deceased" || person.state=="deceased CRC");               A count of people in the study that have died (i.e. their state is "deceased" or "deceased CRC")
46            17       count(person.colon_clinical=="symptomatic CRC");                               A count of patients that have colon_clinical state "symptomatic CRC"
47            .        FALSE|0 (not implemented)                                        
48            .        FALSE|0 (not implemented)                                       
49            .        FALSE|0 (not implemented)                                       
50            .        FALSE|0 (not implemented)                                       
51            .        FALSE|0 (not implemented)
52            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, in which a colonoscopy was performed
                            &&(colonoscopy_performed))
53            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, who had colons in state "adenoma" or "large adenoma"
                            &&(colon.state=="adenoma" || colon.state=="large adenoma"))
54            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, who had colons in state "symptomatic CRC" and in stage "A"
                            &&(colon.state=="symptomatic CRC" && colon.stage=="A"))
55            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, who had colons in state "symptomatic CRC" and in stage "B"
                            &&(colon.state=="symptomatic CRC" && colon.stage=="B"))
56            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, who had colons in state "symptomatic CRC" and in stage "C"
                            &&(colon.state=="symptomatic CRC" && colon.stage=="C"))
57            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, who had colons in state "symptomatic CRC" and in stage "D"
                            &&(colon.state=="symptomatic CRC" && colon.stage=="D"))
58            18       count(where(person.initiateCRCTreatment() called))                             A count of people put into a treatment program THIS ITERATION!!!
                            // => person.in_treatment_program
59            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, whose colonoscopy caused bleeding
                            && colonoscopy_performed && colonoscopy_caused_bleeding)
60            .        count(where(person.initiateCRCTreatment() called)                              A count of people that where treated for CRC IN THIS ITERATION, whose colonoscopy caused perforation
                            && colonoscopy_performed && colonoscopy_caused_perforation)
============= ======== ============================================================================== ========================================================================================================================================================

