#include "jsonify/from_json/api.hpp"

#include <Rcpp.h>

// [[Rcpp::export]]
SEXP rcpp_from_json(const char * json, bool& simplify, bool& fill_na ) {
  return jsonify::api::from_json( json, simplify, fill_na );
}

// [[Rcpp::export]]
SEXP rcpp_from_ndjson(const char * ndjson, bool& simplify, bool& fill_na ) {
  return jsonify::api::from_ndjson( ndjson, simplify, fill_na );
}

// [[Rcpp::export]]
Rcpp::IntegerVector rcpp_get_dtypes( const char * json ) {
  return jsonify::from_json::test_dtypes( json );
}

// [[Rcpp::export]]
SEXP rcpp_simplify_vector( Rcpp::List& lst, int& r_type, R_xlen_t n ) {
  return jsonify::from_json::simplify_vector( lst, r_type, n );
}


