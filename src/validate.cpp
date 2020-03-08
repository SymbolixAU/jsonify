
#include "jsonify/validate/validate.hpp"
#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::LogicalVector rcpp_validate_json( Rcpp::StringVector json ) {
  R_xlen_t n = json.size();
  R_xlen_t i;
  Rcpp::LogicalVector res( n );
  for ( i = 0; i < n; ++i ) {
    res[i] = jsonify::validate::validate_json( json[i] );
  }
  return res;
}

