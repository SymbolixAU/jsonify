#include "Rcpp.h"
#include "jsonify/to_json.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_df_to_json( Rcpp::DataFrame df, bool unbox = false ) {
  return jsonify::dataframe::to_json( df, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_to_json( Rcpp::NumericVector nv, bool unbox = false ) {
  return jsonify::vectors::to_json( nv, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_to_json( Rcpp::StringVector sv, bool unbox = false ) {
  return jsonify::vectors::to_json( sv, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_integer_to_json( Rcpp::IntegerVector iv, bool unbox = false ) {
  return jsonify::vectors::to_json( iv, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_logical_to_json( Rcpp::LogicalVector iv, bool unbox = false ) {
  return jsonify::vectors::to_json( iv, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_list_to_json( SEXP lst, bool unbox = false ) {
  return jsonify::vectors::to_json( lst, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_integer_matrix_to_json( Rcpp::IntegerMatrix m, bool unbox = false ) {
  return jsonify::matrix::to_json( m, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_matrix_to_json( Rcpp::NumericMatrix m, bool unbox = false ) {
  return jsonify::matrix::to_json( m, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_matrix_to_json( Rcpp::CharacterMatrix m, bool unbox = false ) {
  return jsonify::matrix::to_json( m, unbox );
}
