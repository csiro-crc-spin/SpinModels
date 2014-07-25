library(SpinModels)
cc<-GenericModel(iterations=99, num_subjects=1000,base_seed=122)
cc<-GenericModel(99, 1,1000,122,20)
# could not find function "GenericModel"

system.time(cc<-CrcSpinModel$new(iterations=99, num_subjects=1000,base_seed=122))
#rror: no valid constructor available for the argument list
# Timing stopped at: 0.004 0 0.021

system.time(dd<-DukesCrcSpinModel$new(99, 50000, 125, 20))
#1.620   0.036   1.657
system.time(dd$run())
#43.250   0.000  43.294

dim(dd$study_results)

res2<-dd$study_results
res.F<-res2[(1:99)*2-1,] 
res.M<-res2[(1:99)*2,]   

number.M<-sum(unlist (lapply(dd$study_group,f<-function(x){x$sex}))=="M")
#Error in envRefInferField(x, what, getClass(class(x)), selfEnv) : 
#  ‘study_group’ is not a valid field or method name for reference class “Rcpp_DukesCrcSpinModel”
number.F<-sum(unlist(lapply(dd$study_group,f<-function(x){x$sex}))=="F")

number.M<-25000
number.F<-25000

qq<-25000

png("DukesCrcSpinModel.png")
plot(res.F[,2]/qq,type="l",col="orange",lwd=2,xlab="age",axes=FALSE,
     ylab="proportion of population",ylim=c(0,0.05),
          main="CRC -- population with no screening")
lines(res2[,3]/qq,col="plum4",lwd=2) #"pre clinical CRC" ie CRC
lines(res2[,4]/qq,col="black",lwd=2) #death CRC

legend(20,0.04,c("population","large adenoma", "CRC","death CRC"), col=c(
"green","orange","plum4","black"),lwd=2)
axis(2, pretty( c(0,0.05),10))
par(new=TRUE)
plot(pp<- qq-cumsum(res.F[,45]),axes=FALSE,type="l",ylab="",xlab="",lty=1,col=3,lwd=2 )

axis(4, pretty(range(c(0,qq),10)))
mtext(side=4, line=3, "population")

axis(1,pretty(range(1:99),10))
box() #- to make it look "as usual"
dev.off()





tamar: svn export https://svnserv.csiro.au/svn/dom039/CRC-Spin2/branches/CRC-Spin2-Rcpp-package
A    CRC-Spin2-Rcpp-package
A    CRC-Spin2-Rcpp-package/script1.s
A    CRC-Spin2-Rcpp-package/SpinModels
A    CRC-Spin2-Rcpp-package/SpinModels/R
A    CRC-Spin2-Rcpp-package/SpinModels/R/misc.R
A    CRC-Spin2-Rcpp-package/SpinModels/R/zzz.R
A    CRC-Spin2-Rcpp-package/SpinModels/DESCRIPTION
A    CRC-Spin2-Rcpp-package/SpinModels/src
A    CRC-Spin2-Rcpp-package/SpinModels/src/crcSpin.cpp
A    CRC-Spin2-Rcpp-package/SpinModels/src/dukesCrcSpin.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/Makevars.win
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers/crcSpin_postRcpp.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers/crcSpin_preRcpp.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers/genericSpin_postRcpp.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers/genericSpin_preRcpp.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers/dukesCrcSpin_postRcpp.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/split_headers/dukesCrcSpin_preRcpp.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/crcSpin.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/genericSpin.cpp
A    CRC-Spin2-Rcpp-package/SpinModels/src/spinUtil.cpp
A    CRC-Spin2-Rcpp-package/SpinModels/src/Makevars
A    CRC-Spin2-Rcpp-package/SpinModels/src/genericSpin.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/spinUtil.h
A    CRC-Spin2-Rcpp-package/SpinModels/src/dukesCrcSpin.cpp
A    CRC-Spin2-Rcpp-package/SpinModels/vignettes
A    CRC-Spin2-Rcpp-package/SpinModels/vignettes/RCSpin.Rnw
A    CRC-Spin2-Rcpp-package/SpinModels/vignettes/figs
A    CRC-Spin2-Rcpp-package/SpinModels/data
A    CRC-Spin2-Rcpp-package/SpinModels/data/death_rates.RData
A    CRC-Spin2-Rcpp-package/SpinModels/man
A    CRC-Spin2-Rcpp-package/SpinModels/man/SpinModelResultTables.html
A    CRC-Spin2-Rcpp-package/SpinModels/man/SpinModels-package.Rd
A    CRC-Spin2-Rcpp-package/SpinModels/man/SpinModelResultTables.rst
A    CRC-Spin2-Rcpp-package/SpinModels/NAMESPACE
Exported revision 244.



## #######################################################################################################
## #Friday, June 20, 2014
## require(roxygen2)
## roxygenize("RCSpin")
## #Error in .Object$initialize(...) : object 'CrcRiskParams' not found

## library(devtools)
## document("RCSpin")
## Updating RCSpin documentation
##  Loading RCSpin
##  Writing NAMESPACE
##  Writing GenericModel-class.Rd
##  Writing Test-class.Rd
##  Writing ClinicalHistory-class.Rd
##  Writing Person-class.Rd
##  Writing Risk-class.Rd
##  Writing lappend.Rd
##  Writing CrcSpinModel-class.Rd
##  Writing DukesAdenomaParams-class.Rd


## ############################################################
## # Generated by roxygen2 (4.0.1): do not edit by hand

## export(lappend)
## #################################################################

## system("cp NAMESPACE RCSpin/NAMESPACE")
## or svn revert RCSpin/NAMESPACE


## system("R CMD build RCSpin")
## system("sudo R CMD INSTALL RCSpin_0.1.tar.gz")
## #restart

## library(RCSpin)
## cc<-GenericModel(iterations=99, num_subjects=10,base_seed=122)
## cc<-CrcSpinModel$new(iterations=99, num_subjects=10,base_seed=122)
## cc$run()
## dd<-DukesCrcSpinModel$new(iterations=99, num_subjects=10, base_seed=125, commencement_age=20)
## dd$run()
## help(RCSpin)
## help(death_rates)
## help(GenericModel)  # takes the #' section at the tot of the file and add all the
##                     # "" from each method
## GenericModel$methods()
## GenericModel$fields()
## GenericModel$help()
## GenericModel$help(updateSubject)

## help(CrcSpinModel)  # takes the #' section just before setRefClass(CrcSpinModel ..)
##                     #and adds all the  "" from each method
## help(CrcRiskParams) #nothing


## help(Test-class)#works if I type it in but not from emacs -- how can that happen?
## Test$help()
## help(Test)

## help(ClinicalHistory)  # these are quite different. help(ClinicalHistory) gives the roxoygen help
## ClinicalHistory$help() # this gives what appears to be some template of the function
## help(Person)
## Person$help()
## Person$help(initialize)
## help(PersonWithColon)        #nothing
## help(Risk)
## Risk$help()
## help(lappend)
## help(Adenoma) 
## help(AdenomaParams)
## help(Colon)  #nothing
## help(CrcRisk)
## help(DukesCrcSpinModel)
## help(DukesAdenomaParams)     #nothing
## help(DukesAdenoma)          #nothing
## help(DukesColon)           #nothing
## help(DukesPersonWithColon)  #nothing
## help(SymptomaticPresentation)  #nothing
## help(organize_results)#nothing
## vignette("RCSpin")

## system.time(cc<-GenericModel(iterations=99, num_subjects=1000,base_seed=122))
## #   user  system elapsed
## #   2.008   0.012   2.023
## system.time(cc$run())
## # 30.762   0.036  30.827
## cc$study_group[1]


## system.time(cc<-CrcSpinModel$new(iterations=99, num_subjects=1000,base_seed=122))
## # 20.233   0.000  20.251
## system.time(cc$run())
## #107.987   0.040 108.160
## cc$study_group[1]

## system.time(dd<-DukesCrcSpinModel$new(iterations=99, num_subjects=1000, base_seed=125, commencement_age=20))
## #20.778   0.012  20.808
## system.time(dd$run())
## #159.030   0.528 159.928

## system.time(dd<-DukesCrcSpinModel$new(iterations=99, num_subjects=50000, base_seed=125, commencement_age=20))
## system.time(dd$run())

## dim(dd$study_results)




## #######################################################################################
## ccgm<-GenericModel(iterations=99, num_subjects=50,base_seed=122)
## dim(ccgm$study_results)

## res2<-ccgm$study_results
## res.F<-res2[(1:99)*2-1,]
## res.M<-res2[(1:99)*2,]

## number.M<-sum(unlist (lapply(ccgm$study_group,f<-function(x){x$sex}))=="M")
## number.F<-sum(unlist(lapply(ccgm$study_group,f<-function(x){x$sex}))=="F")

## plot(1:99,res.M,col="blue",ylim=c(0,550),type="n",
##      xlab="age",ylab="population")
## temp<-rep(0,99)
## temp[1]<-number.M
## for ( ii in 2:99){
##     temp[ii]<- temp[ii-1]-death_rate_male[ii-1]*temp[ii-1]
## }
## lines(temp,col="blue")
## temp<-rep(0,99)
## temp[1]<-number.F
## for ( ii in 2:99){
##     temp[ii]<- temp[ii-1]-death_rate_female[ii-1]*temp[ii-1]
## }
## lines(temp,col="red")
## legend(20,300,c("male","female"),col=c("blue","red"),lty=1)




## #ccsm<-CrcSpinModel$new(iterations=99, num_subjects=50000,base_seed=122)
## #ccsm$run()
## #dim(ccsm$study_results)# 198  18
## #aa<-organize_results(ccsm)
## #save(aa,file="aa_ccsm.Rdata")
## #load(file="aa_ccsm.Rdata")
## data("CrcSpinModel_output")
## attach(CrcSpinModel_output)

## #png("CrcSpinModel.png")
## plot(results.M$colon.state.large.adenoma/number.M,type="l",col="orange",lwd=2,xlab="age",axes=FALSE,
##      ylab="proportion of population",ylim=c(0,0.1),
##           main="CRC -- male population with no screening")
## lines(results.M$colon.state.pre.symptomatic.CRC/number.M,col="plum4",lwd=2)
## lines(results.M$colon.state.CRC/number.M,col="purple",lwd=2)
## legend(20,0.07,c("population","large adenoma", "CRC","symptomatic CRC"), col=c(
## "green","orange","plum4","purple"),lwd=2)
## axis(2, pretty( c(0,0.1),10))
## par(new=TRUE)
## plot(pp<- number.M-cumsum(results.M$person.state.deceased),axes=FALSE,type="l",ylab="",xlab="",lty=1,col=3,lwd=2 )
## axis(4, pretty(range(c(0,number.M),10)))
## mtext(side=4, line=3, "population")

## axis(1,pretty(range(1:99),10))
## box() #- to make it look "as usual"
## #dev.off()



## #dddm<-DukesCrcSpinModel$new(iterations=99, num_subjects=50000, base_seed=125, commencement_age=20)
## #dddm$run()
## #dim(dddm$study_results)  #198  60
## #aa<-organize_results(dddm)
## #dim(dddm$study_results)  #198  60
## #[1] 198  60
## # aa<-organize_results(dddm)
## #save(aa,file="aa_dddm.Rdata")

## data(DukesCrcSpinModel_output)
## attach(DukesCrcSpinModel_output)

## #png("DukesCrcSpinModel.png")
## plot(results.M$colon.state.large.adenoma/number.M,type="l",col="orange",lwd=2,xlab="age",axes=FALSE,
##      ylab="proportion of population",ylim=c(0,0.1),
##           main="CRC -- male population with no screening")
## lines(results.M$colon.state.CRC/number.M,col="plum4",lwd=2)
## lines(results.M$colon.state.symptomatic.CRC/number.M,col="purple",lwd=2)
## legend(20,0.07,c("population","large adenoma", "CRC","symptomatic CRC"), col=c(
## "green","orange","plum4","purple"),lwd=2)
## axis(2, pretty( c(0,0.1),10))
## par(new=TRUE)
## plot(pp<- number.M-cumsum(results.M$person.state.deceased),axes=FALSE,type="l",ylab="",xlab="",lty=1,col=3,lwd=2 )
## axis(4, pretty(range(c(0,number.M),10)))
## mtext(side=4, line=3, "population")
## axis(1,pretty(range(1:99),10))
## box() #- to make it look "as usual"


## #load(file="aa_ccsm.Rdata")
## #CrcSpinModel_output<-aa
## #save(CrcSpinModel_output,file="RCSpin/data/CrcSpinModel_output.RData")

## #load(file="aa_dddm.Rdata")
## #DukesCrcSpinModel_output<-aa
## #save(DukesCrcSpinModel_output,file="RCSpin/data/DukesCrcSpinModel_output.RData")


## #http://win-builder.r-project.org/upload.aspx
## #to create a windows library


svn export https://svnserv.csiro.au/svn/dom039/CRC-Spin2/branches/CRC-Spin2-Rcpp-package
git config --global user.email "rob.dunne@csiro.au"
git config --global user.name "dun280"
cd SpinModels/

git init
git add .
git remote add origin https://github.com/csiro-crc-spin/RCSpin.git
git push origin master
git pull https://github.com/parsifal9/test1.git/  #there is a README.md file that has to be pulled down first

tamar: git push origin master
#Counting objects: 32, done.
#Delta compression using up to 2 threads.
#Compressing objects: 100% (31/31), done.
#Writing objects: 100% (31/31), 141.14 KiB | 0 bytes/s, done.
#Total 31 (delta 3), reused 0 (delta 0)
#To https://github.com/parsifal9/test1.git
#   31fc568..18c666c  master -> master


library(devtools)
install_github("csiro-crc-spin/RCSpin",args=" -l ~/Downloads/temp")
library("RCSpin",lib.loc="~/Downloads/temp")
cc<-GenericModel(iterations=99, num_subjects=10,base_seed=122)
cc<-CrcSpinModel$new(iterations=99, num_subjects=10,base_seed=122)
cc$run()
dd<-DukesCrcSpinModel$new(iterations=99, num_subjects=10, base_seed=125, commencement_age=20)
dd$run()
help(RCSpin)


##########################################################################################
