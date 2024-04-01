#include "Rcpp.h"
#include "jsonify/to_json/api.hpp"

// [[Rcpp::export]]
Rcpp::StringVector rcpp_to_json(
    SEXP lst, 
    bool unbox = false, 
    int digits = -1, 
    bool numeric_dates = true, 
    bool factors_as_string = true,
    std::string by = "row",
    bool json_verbatim = false
) {
  
  SEXP lst2 = Rcpp::clone( lst );
  return jsonify::api::to_json( lst2, unbox, digits, numeric_dates, factors_as_string, by, json_verbatim );
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_to_ndjson(
  SEXP lst,
  bool unbox = false,
  int digits = -1,
  bool numeric_dates = true,
  bool factors_as_string = true,
  std::string by = "row",
  bool json_verbatim = false
) {
  return jsonify::api::to_ndjson(lst, unbox, digits, numeric_dates, factors_as_string, by, json_verbatim );
}
