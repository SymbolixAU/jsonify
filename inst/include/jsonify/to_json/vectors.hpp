#ifndef R_JSONIFY_VECTORS_H
#define R_JSONIFY_VECTORS_H

#include <Rcpp.h>
#include "jsonify/jsonify.hpp"

using namespace rapidjson;

namespace jsonify {
namespace vectors {

  inline Rcpp::StringVector to_json( Rcpp::NumericVector& nv, bool auto_unbox = false ) { 
   
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, nv, auto_unbox );
    return jsonify::utils::finalise_json( sb );
      
  }

  inline Rcpp::StringVector to_json( Rcpp::StringVector& sv, bool auto_unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, sv, auto_unbox );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::IntegerVector& iv, bool auto_unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, iv, auto_unbox );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::LogicalVector& lv, bool auto_unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lv, auto_unbox );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( SEXP& lst, bool auto_unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lst, auto_unbox );
    return jsonify::utils::finalise_json( sb );
  }

} // namespace vectors
} // namespace jsonify

#endif