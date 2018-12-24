#ifndef R_JSONIFY_MATRIX_H
#define R_JSONIFY_MATRIX_H

#include <Rcpp.h>
#include "jsonify/jsonify.hpp"

namespace jsonify {
namespace matrix {

  inline Rcpp::StringVector to_json( Rcpp::NumericMatrix& nm, bool unbox = false, int digits = -1 ) {
    Rcpp::Rcout << "namespace jsonify::matrix is deprecated. Use jsonify::api instead" << std::endl; 
    return jsonify::api::to_json( nm, unbox, digits );
  }

  inline Rcpp::StringVector to_json( Rcpp::IntegerMatrix& im, bool unbox = false ) {
    Rcpp::Rcout << "namespace jsonify::matrix is deprecated. Use jsonify::api instead" << std::endl;
    return jsonify::api::to_json( im, unbox, digits );
  }

  inline Rcpp::StringVector to_json( Rcpp::CharacterMatrix& cm, bool unbox = false ) {
    Rcpp::Rcout << "namespace jsonify::matrix is deprecated. Use jsonify::api instead" << std::endl;
    return jsonify::api::to_json( cm, unbox, digits );
  }

  inline Rcpp::StringVector to_json( Rcpp::LogicalMatrix& lm, bool unbox = false ) {
    Rcpp::Rcout << "namespace jsonify::matrix is deprecated. Use jsonify::api instead" << std::endl;
    return jsonify::api::to_json( lm, unbox, digits );
  }

} // namespace matrix
} // namespace jsonify

#endif