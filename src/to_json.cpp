#include "Rcpp.h"
#include "jsonify/to_json/api.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_to_json( Rcpp::NumericVector nv, bool unbox = false, int digits = -1 ) {
  if( digits >= 0 ) {
    Rcpp::NumericVector nv2 = Rcpp::clone( nv );
    return jsonify::api::to_json( nv2, unbox, digits );
  }
  return jsonify::api::to_json( nv, unbox, digits );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_to_json( Rcpp::StringVector sv, bool unbox = false ) {
  return jsonify::api::to_json( sv, unbox );
}


// [[Rcpp::export]]
Rcpp::StringVector rcpp_list_to_json( SEXP lst, bool unbox = false, int digits = -1 ) {
  if ( digits >= 0 ) {
    SEXP lst2 = Rcpp::clone( lst );
    //return jsonify::writers::write_value( lst2, unbox, digits );
    return jsonify::api::to_json( lst2, unbox, digits );
  }
  return jsonify::api::to_json( lst, unbox, digits );
}
