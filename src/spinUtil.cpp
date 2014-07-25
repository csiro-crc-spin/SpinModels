/* vim: set fileformat=unix : */
// spinUtil.h and spinUtil.cpp purely exist to act as a wrapper to the
// RcppArmadillo sample extension, whose implementers have placed function
// definitions in the header file that preclude you from including
// the header in multiple sources of your project


// [[Rcpp::depends(RcppArmadillo)]]
#include <RcppArmadilloExtensions/sample.h>

#include <spinUtil.h>

// create a wrapper to RcppArmadilloExtensions::sample within the SpinUtil
// namespace which simply on passes the arguments passed in
template <class T>
T SpinUtil::sample(const T &x, const int size, const bool replace, Rcpp::NumericVector prob_){
    Rcpp::RNGScope scope;
    return Rcpp::RcppArmadillo::sample(x, size, replace, prob_);
}

// Because the template function definition above is hidden from source
// files that include spinUtil.h we need to trigger the compiler to
// generate the appropriate runtime implementations of the function.

//template Rcpp::Vector<CPLXSXP> SpinUtil::sample(Rcpp::Vector<CPLXSXP> const& , const int, const bool, Rcpp::NumericVector);
template Rcpp::Vector<INTSXP> SpinUtil::sample(Rcpp::Vector<INTSXP> const& , const int, const bool, Rcpp::NumericVector);
template Rcpp::Vector<LGLSXP> SpinUtil::sample(Rcpp::Vector<LGLSXP> const& , const int, const bool, Rcpp::NumericVector);
template Rcpp::Vector<REALSXP> SpinUtil::sample(Rcpp::Vector<REALSXP> const& , const int, const bool, Rcpp::NumericVector);
template Rcpp::Vector<RAWSXP> SpinUtil::sample(Rcpp::Vector<RAWSXP> const& , const int, const bool, Rcpp::NumericVector);
template Rcpp::Vector<STRSXP> SpinUtil::sample(Rcpp::Vector<STRSXP> const& , const int, const bool, Rcpp::NumericVector);
//template Rcpp::Vector<VECSXP> SpinUtil::sample(Rcpp::Vector<VECSXP> const& , const int, const bool, Rcpp::NumericVector);
//template Rcpp::Vector<EXPRSXP> SpinUtil::sample(Rcpp::Vector<EXPRSXP> const& , const int, const bool, Rcpp::NumericVector);
