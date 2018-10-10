#ifndef R_JSONIFY_MATRIX_H
#define R_JSONIFY_MATRIX_H

#include <Rcpp.h>
#include "jsonify/jsonify.hpp"

namespace jsonify {
namespace matrix {

  inline Rcpp::StringVector to_json( Rcpp::NumericMatrix& nm ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    writer.StartArray();
    
    int n = nm.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::NumericVector this_row = nm(i, Rcpp::_);
      writers::write_value( writer, this_row );
    }
    writer.EndArray();
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::IntegerMatrix& im ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    writer.StartArray();
    
    int n = im.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::IntegerVector this_row = im(i, Rcpp::_);
      writers::write_value( writer, this_row );
    }
    writer.EndArray();
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::CharacterMatrix& cm ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    writer.StartArray();
    
    int n = cm.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::StringVector this_row = cm(i, Rcpp::_);
      writers::write_value( writer, this_row );
    }
    writer.EndArray();
    return jsonify::utils::finalise_json( sb );
  }


} // namespace matrix
} // namespace jsonify

#endif