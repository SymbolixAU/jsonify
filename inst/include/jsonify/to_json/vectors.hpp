#ifndef R_JSONIFY_VECTORS_H
#define R_JSONIFY_VECTORS_H

#include <Rcpp.h>
#include "jsonify/jsonify.hpp"

using namespace rapidjson;

namespace jsonify {
namespace vectors {

  inline Rcpp::StringVector to_json( Rcpp::NumericVector& nv, bool unbox = false ) { 
   
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, nv, unbox );
    return jsonify::utils::finalise_json( sb );
      
  }

  inline Rcpp::StringVector to_json( Rcpp::StringVector& sv, bool unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, sv, unbox );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::IntegerVector& iv, bool unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, iv, unbox );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( Rcpp::LogicalVector& lv, bool unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lv, unbox );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_json( SEXP& lst, bool unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lst, unbox );
    return jsonify::utils::finalise_json( sb );
  }

} // namespace vectors
} // namespace jsonify

#endif