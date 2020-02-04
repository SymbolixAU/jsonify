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


/*
 * quick tests which call each of the jsonify::writers::simple::write_values()
 * functions to ensure the package compiles and runs
 */
// [[Rcpp::export]]
void source_tests() {
  
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
  R_xlen_t row;

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
  
  sb.Clear();
  writer.Reset( sb );
  
  unbox = true;
  digits = 2;
  numeric_dates = false;
  row = 1;
  jsonify::writers::simple::write_value( writer, nv, unbox, digits, numeric_dates );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("1.23", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  nv = Rcpp::NumericVector::create(1.234, 5.678);
  unbox = true;
  digits = 2;
  numeric_dates = false;
  row = 1;
  jsonify::writers::simple::write_value( writer, nv, row, digits, numeric_dates );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("5.68", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  iv = Rcpp::IntegerVector::create(1);
  unbox = false;
  numeric_dates = false;
  factors_as_string = true;
  jsonify::writers::simple::write_value( writer, iv, unbox, numeric_dates, factors_as_string);
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("[1]", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  unbox = true;
  digits = 2;
  numeric_dates = false;
  row = 1;
  jsonify::writers::simple::write_value( writer, iv, unbox, digits, numeric_dates, factors_as_string );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("1", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  iv = Rcpp::IntegerVector::create(1,2);
  unbox = true;
  digits = 2;
  numeric_dates = false;
  row = 1;
  jsonify::writers::simple::write_value( writer, iv, row, digits, numeric_dates,factors_as_string );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("2", json, testcounter );
  
  sb.Clear();
  writer.Reset( sb );
  
  lv = Rcpp::LogicalVector::create(true, false);
  unbox = false;
  jsonify::writers::simple::write_value( writer, lv, unbox );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("[true,false]", json, testcounter);
  
  sb.Clear();
  writer.Reset( sb );
  
  lv = Rcpp::LogicalVector::create(true);
  unbox = false;
  jsonify::writers::simple::write_value( writer, lv, unbox );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("[true]", json, testcounter);
  
  sb.Clear();
  writer.Reset( sb );
  
  lv = Rcpp::LogicalVector::create(true);
  unbox = true;
  jsonify::writers::simple::write_value( writer, lv, unbox );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("true", json, testcounter);
  
  sb.Clear();
  writer.Reset( sb );
  
  lv = Rcpp::LogicalVector::create(true, false);
  unbox = false;
  row = 1;
  jsonify::writers::simple::write_value( writer, lv, row );
  res = jsonify::utils::finalise_json( sb );
  json = res[0];
  quick_test("false", json, testcounter);
}


