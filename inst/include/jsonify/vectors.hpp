#ifndef R_JSONIFY_VECTORS_H
#define R_JSONIFY_VECTORS_H

#include <Rcpp.h>
#include "writers.hpp"

namespace jsonify {
namespace vectors {

  inline Rcpp::StringVector to_json( Rcpp::NumericVector& nv) { 
   
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, nv );
    Rcpp::StringVector js = sb.GetString();
    return js;
      
  }

  inline Rcpp::StringVector to_json( Rcpp::StringVector& sv ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, sv );
    Rcpp::StringVector js = sb.GetString();
    return js;
  }

  inline Rcpp::StringVector to_json( Rcpp::IntegerVector& iv ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, iv );
    Rcpp::StringVector js = sb.GetString();
    return js;
  }

  inline Rcpp::StringVector to_json( Rcpp::LogicalVector& lv ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lv );
    Rcpp::StringVector js = sb.GetString();
    return js;
  }

  inline Rcpp::StringVector to_json( SEXP& lst ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, lst );
    Rcpp::StringVector js = sb.GetString();
    return js;
  }


} // namespace vectors
} // namespace // jsonify

#endif