#include "Rcpp.h"
#include "jsonify/dataframe.hpp"
#include "jsonify/vectors.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_df_to_json( Rcpp::DataFrame df ) {
  return jsonify::dataframe::to_json( df );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_to_json( Rcpp::NumericVector nv ) {
  return jsonify::vectors::to_json( nv );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_to_json( Rcpp::StringVector sv ) {
  return jsonify::vectors::to_json( sv );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_integer_to_json( Rcpp::IntegerVector iv ) {
  return jsonify::vectors::to_json( iv );
}


// [[Rcpp::export]]
Rcpp::StringVector rcpp_logical_to_json( Rcpp::LogicalVector iv ) {
  return jsonify::vectors::to_json( iv );
}


// // [[Rcpp::export]]
// Rcpp::StringVector rcpp_list_to_json( SEXP lst ) {
//   return jsonify::vectors::to_json( lst );
// }


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
