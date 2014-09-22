An implementation of the CRC-SPIN model in R using the Rcpp framework.

It can be installed from github using the devtools package i.e.

library(devtools)
install_github("csiro-crc-spin/SpinModels",args=" -l ~/Downloads/temp")
library("SpinModels",lib.loc="~/Downloads/temp")

where "~/Downloads/temp" is a directory that you have write permissions for.

testing