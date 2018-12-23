#include "Rcpp.h"
#include "jsonify/to_json.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_df_to_json( Rcpp::DataFrame df, bool unbox = false, int digits = -1 ) {
  return jsonify::dataframe::to_json( df, unbox, digits );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_to_json( Rcpp::NumericVector nv, bool unbox = false, int digits = -1 ) {
  if( digits >= 0 ) {
    Rcpp::NumericVector nv2 = Rcpp::clone( nv );
    return jsonify::vectors::to_json( nv2, unbox, digits );
  }
  return jsonify::vectors::to_json( nv, unbox, digits );
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
Rcpp::StringVector rcpp_list_to_json( SEXP lst, bool unbox = false, int digits = -1 ) {
  if ( digits >= 0 ) {
    SEXP lst2 = Rcpp::clone( lst );
    return jsonify::vectors::to_json( lst2, unbox, digits );
  }
  return jsonify::vectors::to_json( lst, unbox, digits );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_integer_matrix_to_json( Rcpp::IntegerMatrix m, bool unbox = false ) {
  return jsonify::matrix::to_json( m, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_numeric_matrix_to_json( Rcpp::NumericMatrix m, bool unbox = false, int digits = -1 ) {
  if ( digits >= 0 ) {
    Rcpp::NumericMatrix m2 = Rcpp::clone( m );
    return jsonify::matrix::to_json( m2, unbox, digits );
  }
  return jsonify::matrix::to_json( m, unbox, digits );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_logical_matrix_to_json( Rcpp::LogicalMatrix m, bool unbox = false, int digits = -1 ) {
  return jsonify::matrix::to_json( m, unbox );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_character_matrix_to_json( Rcpp::CharacterMatrix m, bool unbox = false ) {
  return jsonify::matrix::to_json( m, unbox );
}
