#include "Rcpp.h"
#include "jsonify.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_to_json( Rcpp::DataFrame df ) {
  return jsonify::to_json( df );
}

// // [[Rcpp::export]]
// Rcpp::StringVector class_test( Rcpp::DataFrame df ) {
//   int n_cols = df.ncol();
//   Rcpp::StringVector classes( n_cols );
//   
//   for (int i = 0; i < n_cols; i++) {
//     SEXP v = df[ i ];
//     classes[i] = jsonify::get_r_class( v );
//   }
//   return classes;
// }
