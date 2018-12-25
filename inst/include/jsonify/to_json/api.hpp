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
 
 inline Rcpp::StringVector to_json( SEXP& lst, Rcpp::List r_types, bool unbox = false, int digits = -1 ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    int counter = 0;
    jsonify::writers::write_value( writer, lst, r_types, counter, unbox, digits );
    return jsonify::utils::finalise_json( sb );
 }

} // namespace api
} // namespace jsonify

#endif