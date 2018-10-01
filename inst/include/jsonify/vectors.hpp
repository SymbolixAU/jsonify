#ifndef R_JSONIFY_VECTORS_H
#define R_JSONIFY_VECTORS_H

#include <Rcpp.h>
#include "writers.hpp"

// [[Rcpp::depends(rapidjsonr)]]

using namespace rapidjson;

namespace jsonify {
namespace vectors {

  inline Rcpp::StringVector to_json( Rcpp::IntegerVector& iv ) {
    int n = iv.size();
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::utils::should_start_array_single( writer, n );
    jsonify::writers::write_value( writer, iv );
    jsonify::utils::should_end_array_single( writer, n );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::NumericVector& nv) { 
    int n = nv.size();
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    
    jsonify::utils::should_start_array_single( writer, n );
    jsonify::writers::write_value( writer, nv );
    jsonify::utils::should_end_array_single( writer, n );
    
    return jsonify::utils::finalise_json( sb );
      
  }

  inline Rcpp::StringVector to_json( Rcpp::StringVector& sv ) {
    int n = sv.size();
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::utils::should_start_array_single( writer, n );
    jsonify::writers::write_value( writer, sv );
    jsonify::utils::should_end_array_single( writer, n );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::LogicalVector& lv ) {
    int n = lv.size();
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::utils::should_start_array_single( writer, n );
    jsonify::writers::write_value( writer, lv );
    jsonify::utils::should_end_array_single( writer, n );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( SEXP& lst ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lst );
    return jsonify::utils::finalise_json( sb );
  }

} // namespace vectors
} // namespace jsonify

#endif