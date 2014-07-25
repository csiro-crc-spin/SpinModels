/* vim: set fileformat=unix : */
/*
 * CrcSpin Classes header file
 * NOTE: The contents of this header file must be split in two so that
 * certain class and function declarations are visible to the Rcpp template
 * dispatch mechanism in Rcpp.h (included from RcppArmadilloExtensions/sample.h)
 *
 * split_headers/crcSpin_preRcpp.h:
 * Contains any class definitions that DO NOT rely on Rcpp.h definitions,
 * particularly dependencies of the aforementioned "certain" classes and
 * functions
 *
 * RcppArmadilloExtensions/sample.h:
 * Includes Rcpp.h
 *
 * split_headers/crcSpin_postRcpp.h:
 * Contains class definitions that rely on Rcpp.h definitions
 */


#include <RcppCommon.h>

#include <split_headers/dukesCrcSpin_preRcpp.h>

#include <Rcpp.h>

#include <split_headers/dukesCrcSpin_postRcpp.h>
