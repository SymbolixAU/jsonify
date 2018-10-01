
// [[Rcpp::depends(rapidjsonr)]]

#include <Rcpp.h>
#include "rapidjson/document.h"



// [[Rcpp::export]]
void test( Rcpp::StringVector geojson ) {
  
  rapidjson::Document doc;
  
  int n = geojson.size();
  
  // the type of JSON object will determine if we need to recurse
  
  doc.SetObject();
  
  
}
