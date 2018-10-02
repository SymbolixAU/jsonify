
#include "jsonify/validate/validate.hpp"
#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::LogicalVector rcpp_validate_json( Rcpp::StringVector geojson ) {
  int n = geojson.size();
  Rcpp::LogicalVector res( n );
  for ( int i = 0; i < n; i++ ) {
    res[i] = jsonify::validate::validate_json( geojson[i] );
  }
  return res;
}

