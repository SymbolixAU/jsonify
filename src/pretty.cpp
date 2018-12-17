
#include <Rcpp.h>
using namespace Rcpp;

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

// reference: https://stackoverflow.com/questions/40833243/rapidjson-pretty-print-using-json-string-as-input-to-the-writer

// [[Rcpp::export]]
Rcpp::StringVector rcpp_pretty_json( const char* json ) {

  rapidjson::Document d;
  d.Parse(json);
  
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter< rapidjson::StringBuffer > writer(sb);
  d.Accept(writer);
  
  // Output {"project":"rapidjson","stars":11}
  //Rcpp::Rcout << sb.GetString() << std::endl;
  return sb.GetString();
}
