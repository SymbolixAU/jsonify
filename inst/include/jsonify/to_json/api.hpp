#ifndef JSONIFY_TO_JSON_API_H
#define JSONIFY_TO_JSON_API_H

#include <Rcpp.h>
#include "jsonify/to_json/utils.hpp"
#include "jsonify/to_json/writers/complex.hpp"

using namespace rapidjson;

namespace jsonify {
namespace api {

    inline Rcpp::StringVector to_json(
            SEXP lst, 
            bool unbox = false, 
            int digits = -1, 
            bool numeric_dates = true, 
            bool factors_as_string = true, 
            std::string by = "row"
    ) {
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::complex::write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
        return jsonify::utils::finalise_json( sb );
    }

} // namespace api
} // namespace jsonify

#endif
