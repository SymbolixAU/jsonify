#ifndef R_JSONIFY_FROM_JSON_UTILS_H
#define R_JSONIFY_FROM_JSON_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]
#include "rapidjson/document.h"

#define R_NA_VAL Rcpp::LogicalVector::create(NA_LOGICAL);

namespace jsonify {
namespace from_json {

  Rcpp::List parse_value(const rapidjson::Value& val, int lvl);

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
  
  // Get all unique names from a list object.
  // Equivalent to the R command:
  // unique(unlist(lapply(list_obj, names), recursive = FALSE, use.names = FALSE))
  SEXP get_col_headers(Rcpp::List& list_obj) {
    Rcpp::List curr_list;
    Rcpp::CharacterVector curr_names;
    std::string curr_str;
    std::unordered_set<std::string> names_set;
    std::vector<std::string> names;
    for(unsigned int i = 0; i < list_obj.size(); ++i) {
      if(TYPEOF(list_obj[i]) != VECSXP) {
        return R_NilValue;
      }
      curr_list = list_obj[i];
      curr_names = curr_list.attr("names");
      for(unsigned int n = 0; n < curr_names.size(); ++n) {
        curr_str = Rcpp::as<std::string>(curr_names[n]);
        if(names_set.find(curr_str) == names_set.end()) {
          names.push_back(curr_str);
          names_set.insert(curr_str);
        }
      }
    }
    
    return Rcpp::wrap(names);
  }
  
  // Tranpose a list.
  // Equivalent to the R command:
  // cols <- unique(unlist(lapply(list_obj, names)))
  // lapply(cols, function(x) lapply(list_obj, "[[", x))
  Rcpp::List transpose_list(Rcpp::List& x, Rcpp::CharacterVector& names) {
    int x_len = x.size();
    int names_len = names.size();
    Rcpp::List out(names_len);
    Rcpp::CharacterVector curr_names;
    Rcpp::String curr_name;
    Rcpp::List curr_list;
    
    for(int i = 0; i < names_len; ++i) {
      curr_name = names[i];
      Rcpp::List col(x_len);
      for(int j = 0; j < x_len; ++j) {
        curr_list = x[j];
        curr_names = curr_list.attr("names");
        for(unsigned int k = 0; k < curr_names.size(); ++k) {
          if(curr_names[k] == curr_name) {
            col[j] = curr_list[k];
            break;
          }
        }
      }
      
      out[i] = col;
    }
    
    return out;
  }
  
  // Convert all NULL elements in a list to NA.
  void null_to_na(Rcpp::List& x) {
    for(unsigned int i = 0; i < x.size(); ++i) {
      if(Rf_isNull(x[i])) {
        x[i] = R_NA_VAL;
      }
    }
  }

} // namespace from_json
} // namespace jsonify

#endif
