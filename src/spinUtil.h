/* vim: set fileformat=unix : */
// spinUtil.h and spinUtil.cpp purely exist to act as a wrapper to the
// RcppArmadillo sample extension, whose implementers have placed function
// definitions in the header file that preclude you from including
// the header in multiple sources of your project

#include <Rcpp.h>

namespace SpinUtil{
    // replicate the definition of RcppArmadilloExtensions::sample from the
    // header file
    template <class T>
    T sample(const T &x, const int size, const bool replace, Rcpp::NumericVector prob_ = Rcpp::NumericVector(0) ) ;
}

