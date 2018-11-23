#include "Rcpp.h"
#include "jsonify/to_json.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_df_to_json( Rcpp::DataFrame df ) {
  return jsonify::dataframe::to_json( df );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_to_json( Rcpp::NumericVector nv, bool auto_unbox = false ) {
  return jsonify::vectors::to_json( nv, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_to_json( Rcpp::StringVector sv, bool auto_unbox = false ) {
  return jsonify::vectors::to_json( sv, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_integer_to_json( Rcpp::IntegerVector iv, bool auto_unbox = false ) {
  return jsonify::vectors::to_json( iv, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_logical_to_json( Rcpp::LogicalVector iv, bool auto_unbox = false ) {
  return jsonify::vectors::to_json( iv, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_list_to_json( SEXP lst, bool auto_unbox = false ) {
  return jsonify::vectors::to_json( lst, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_integer_matrix_to_json( Rcpp::IntegerMatrix m, bool auto_unbox = false ) {
  return jsonify::matrix::to_json( m, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_matrix_to_json( Rcpp::NumericMatrix m, bool auto_unbox = false ) {
  return jsonify::matrix::to_json( m, auto_unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_matrix_to_json( Rcpp::CharacterMatrix m, bool auto_unbox = false ) {
  return jsonify::matrix::to_json( m, auto_unbox );
}
