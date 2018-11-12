#ifndef R_JSONIFY_FROM_JSON_UTILS_H
#define R_JSONIFY_FROM_JSON_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]
#include "rapidjson/document.h"

using namespace rapidjson;

namespace jsonify {
namespace from_json {

  Rcpp::List parse_value(const rapidjson::Value& val);

} // namespace from_json
} // namespace jsonify

#endif
