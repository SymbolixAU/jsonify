
#include <Rcpp.h>
using namespace Rcpp;

#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"


#include "R_ext/Riconv.h"


// reference: https://stackoverflow.com/questions/40833243/rapidjson-pretty-print-using-json-string-as-input-to-the-writer

// [[Rcpp::export]]
Rcpp::StringVector rcpp_pretty_json( const char* json ) {

  rapidjson::Document d;
  d.Parse(json);
  
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter< rapidjson::StringBuffer > writer(sb);
  d.Accept(writer);
  
  Rcpp::StringVector js(1);
  js[0] = Rcpp::String(sb.GetString());
  
  js.attr("class") = "json";
  return js;
}

// [[Rcpp::export]]
Rcpp::StringVector rcpp_minify_json( const char* json ) {
  
  rapidjson::Document d;
  d.Parse(json);
  
  rapidjson::StringBuffer sb;
  rapidjson::Writer< rapidjson::StringBuffer > writer(sb);
  d.Accept(writer);
  
  Rcpp::StringVector js(1);
  js[0] = Rcpp::String(sb.GetString());
  
  js.attr("class") = "json";
  return js;
}

// [[Rcpp::export]]
void rcpp_pretty_print( const char* json ) {
  rapidjson::Document d;
  d.Parse(json);
  
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter< rapidjson::StringBuffer > writer(sb);
  d.Accept(writer);
  Rcpp::Rcout << sb.GetString() << std::endl;
}