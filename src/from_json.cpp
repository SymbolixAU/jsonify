#include "Rcpp.h"
#include "jsonify/from_json/from_json.hpp"


// [[Rcpp::export]]
SEXP rcpp_from_json( const char * json, bool& simplify, std::string by ) {
  return jsonify::from_json::from_json( json, simplify, by );
}

// // [[Rcpp::export]]
// SEXP rcpp_get_col_headers( Rcpp::List& x) {
//   return ejsonify::from_json::get_col_headers( x );
// }
// 
// // [[Rcpp::export]]
// Rcpp::List rcpp_transpose_list( Rcpp::List& x, Rcpp::CharacterVector& names ) {
//   return jsonify::from_json::transpose_list( x, names );
// }

// [[Rcpp::export]]
void rcpp_null_to_na( Rcpp::List& x ) {
  return jsonify::from_json::null_to_na( x );
}


// [[Rcpp::export]]
Rcpp::IntegerVector rcpp_get_dtypes( const char * json ) {
  return jsonify::from_json::test_dtypes( json );
}

// [[Rcpp::export]]
SEXP rcpp_simplify_vector( Rcpp::List& lst, int& r_type, R_xlen_t n ) {
  return jsonify::from_json::simplify_vector( lst, r_type, n );
}



