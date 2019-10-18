#include "Rcpp.h"
#include "jsonify/from_json/api.hpp"


// [[Rcpp::export]]
SEXP rcpp_from_json(const char * json, bool& simplify ) {
  return jsonify::api::from_json( json, simplify );
}


// [[Rcpp::export]]
void rcpp_null_to_na( Rcpp::List& x ) {
  return jsonify::from_json::null_to_na( x );
}


// [[Rcpp::export]]
Rcpp::IntegerVector rcpp_get_dtypes( const char * json ) {
  return jsonify::from_json::test_dtypes( json );
}

// [[Rcpp::export]]
SEXP rcpp_simplify_vector( Rcpp::List& lst, int& r_type, R_xlen_t n ) {
  return jsonify::from_json::simplify_vector( lst, r_type, n );
}



