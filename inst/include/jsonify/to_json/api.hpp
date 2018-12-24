#ifndef JSONIFY_API_H
#define JSONIFY_API_H

#include <Rcpp.h>
#include "jsonify/jsonify.hpp"

using namespace rapidjson;

namespace jsonify {
namespace api {
 
 // TODO:
 // - if 'handle_dates == TRUE' && lv_dates[i] == TRUE
 // the numeric is actually a date
 // so convert to a Datetime string using CCTZ?
 
 inline Rcpp::StringVector to_json( 
       SEXP& lst, Rcpp::LogicalVector lv_dates, bool unbox = false, int digits = -1 
 ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    //jsonify::writers::write_value( writer, lv_dates, lst, unbox, digits );
    jsonify::writers::write_value( writer, lst, unbox, digits );
    return jsonify::utils::finalise_json( sb );
 }
 
 inline Rcpp::StringVector to_json( SEXP& lst, bool unbox = false, int digits = -1 ) {
   
   Rcpp::LogicalVector lv_dates(1);
   lv_dates[0] = false;
   return to_json( lst, lv_dates, unbox, digits );
   // rapidjson::StringBuffer sb;
   // rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
   // jsonify::writers::write_value( writer, lst, unbox, digits );
   // return jsonify::utils::finalise_json( sb );
 }

} // namespace api
} // namespace jsonify

#endif