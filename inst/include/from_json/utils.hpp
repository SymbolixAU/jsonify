#ifndef R_JSONIFY_FROM_JSON_UTILS_H
#define R_JSONIFY_FROM_JSON_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]
#include "rapidjson/document.h"

using namespace rapidjson;

#define R_NA_STR Rcpp::CharacterVector(NA_STRING);

namespace jsonify {
namespace from_json {

  Rcpp::List parse_value(const rapidjson::Value& val);

  std::unordered_set<int> dtypes;
  std::unordered_map<std::string, int> names_map;
  int pv_len;
  std::string temp_name;
  
  std::vector<bool> df_out_lgl;
  std::vector<int> df_out_int;
  std::vector<double> df_out_dbl;
  std::vector<std::string> df_out_str;
  
  Rcpp::CharacterVector names;
  Rcpp::List pv_list;

} // namespace from_json
} // namespace jsonify

#endif
