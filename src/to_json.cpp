#include "Rcpp.h"
#include "jsonify/to_json/api.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_to_json( SEXP lst, Rcpp::List r_types, bool unbox = false, int digits = -1 ) {
  if ( digits >= 0 ) {
    SEXP lst2 = Rcpp::clone( lst );
    return jsonify::api::to_json( lst2, r_types, unbox, digits );
  }
  return jsonify::api::to_json( lst, r_types, unbox, digits );
}
