#include <Rcpp.h>
#include "jsonify/jsonify.hpp"

#include "rapidjson/writer.h"

using namespace Rcpp;

void quick_test( std::string expected, std::string json, int& testcounter ) {
  testcounter++;
  if ( json != expected ) {
    Rcpp::Rcout << expected << std::endl;
    Rcpp::Rcout << json << std::endl;
    Rcpp::Rcout << "test number : " << testcounter << std::endl;
    Rcpp::stop("failed tests");
  }
  
}


// [[Rcpp::export]]
void test() {
  
  int testcounter = 0;
  Rcpp::StringVector res;
  std::string json;
  rapidjson::StringBuffer sb;
  rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
  
  Rcpp::StringVector sv;
  Rcpp::IntegerVector iv;
  Rcpp::NumericVector nv;
  Rcpp::LogicalVector lv;
  bool unbox, numeric_dates, factors_as_string;
  int digits;
  int row;
  // 
  // Rcpp::IntegerVector iv = Rcpp::seq(1,5);
  // 
  // Rcpp::DataFrame df = Rcpp::DataFrame::create(
  //   _["id"] = iv
  // );
  // 
  // res = jsonify::api::to_json( df );
  // Rcpp::Rcout << json << std::endl;
  
  sv = Rcpp::StringVector::create("a");
  unbox = false;
  jsonify::writers::simple::write_value( writer, sv, unbox );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test( "[\"a\"]", json, testcounter );

  sb.Clear();
  writer.Reset( sb );
  
  unbox = true;
  jsonify::writers::simple::write_value( writer, sv, unbox );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test( "\"a\"", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  sv = Rcpp::StringVector::create("a","b");
  unbox = false;
  row = 0;
  jsonify::writers::simple::write_value( writer, sv, row );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test( "\"a\"", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  nv = Rcpp::NumericVector::create(1.234);
  unbox = false;
  digits = 2;
  numeric_dates = false;
  jsonify::writers::simple::write_value( writer, nv, unbox, digits, numeric_dates );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("[1.23]", json, testcounter );
  
}


