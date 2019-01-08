#ifndef R_JSONIFY_VECTORS_H
#define R_JSONIFY_VECTORS_H

#include <Rcpp.h>
#include "jsonify/to_json/api.hpp"

using namespace rapidjson;

// TODO( remove when spatialwidget v0.2 is on CRAN )

namespace jsonify {
namespace vectors {

  inline Rcpp::StringVector to_json( Rcpp::NumericVector& nv, bool unbox = false, int digits = -1 ) {
    // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
    return jsonify::api::to_json( nv, unbox, digits );
  }

  inline Rcpp::StringVector to_json( Rcpp::StringVector& sv, bool unbox = false ) {
    // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
    return jsonify::api::to_json( sv, unbox );
  }

  inline Rcpp::StringVector to_json( Rcpp::IntegerVector& iv, bool unbox = false ) {
    // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
    return jsonify::api::to_json( iv, unbox );
  }

  inline Rcpp::StringVector to_json( Rcpp::LogicalVector& lv, bool unbox = false ) {
    // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
    return jsonify::api::to_json( lv, unbox );
  }

  inline Rcpp::StringVector to_json( SEXP& lst, bool unbox = false, int digits = -1 ) {
    // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
    return jsonify::api::to_json( lst, unbox, digits );
  }

} // namespace vectors
} // namespace jsonify

#endif
