
// [[Rcpp::depends(rapidjsonr)]]

#include <Rcpp.h>
#include "rapidjson/document.h"

// void get_property_types(const Value& v, std::unordered_map< std::string, std::string>& property_types) {
//   
//   // TODO: move to a header??
//   static const char* ARRAY_TYPES[] =
//     { "Null", "False", "True", "Object", "Array", "String", "Number" };
//   
//   for (Value::ConstMemberIterator iter = v.MemberBegin(); iter != v.MemberEnd(); ++iter){
//     std::string property = iter->name.GetString();
//     
//     std::string type = ARRAY_TYPES[iter->value.GetType()];
//     
//     // check if key exists
//     if (property_types.count(property) == 1) {
//       // it exists
//       std::string existing_type = property_types[property];
//       
//       if (existing_type == "String") {
//         // if it's already a 'String' (JSON type), exit
//         
//       } else if (existing_type != type) {
//         // if it's different, update to be a 'String'
//         property_types[property] = "String";
//         
//       }
//       // if it's not different, exit
//     } else {
//       // doesn't exist
//       property_types[property] = type;
//       
//     }
//     // if it doesn't exist, add the key / type to the map
//   }
// }

void get_property_keys(const rapidjson::Value& v, std::set< std::string >& property_keys) {
  for (rapidjson::Value::ConstMemberIterator iter = v.MemberBegin(); iter != v.MemberEnd(); ++iter){
    property_keys.insert(iter->name.GetString());
  }
}

void parse_objecct(const rapidjson::Value& v) {
  
}

void parse_array(const rapidjson::Value& v) {
  
}

void parse_json(const rapidjson::Value& v) {
  // can dispatch into eithe rparse object or parse array
  // and be recursive, to call itself for nested items
  
  
}


// [[Rcpp::export]]
Rcpp::List test( Rcpp::StringVector json ) {
  
  Rcpp::List properties;
  bool is_vector = true;     // keep track of the lowest possible type
  bool is_data_frame = true; // and negate if it breaks a condition?
  // otherwise it's a list
  
  rapidjson::Document doc;
  doc.SetObject();
  
  const char* j = json[0];
  doc.Parse( j );
  
  //int n = geojson.size();
  
  std::set< std::string > geometry_types;
  std::set< std::string > property_keys;   // storing all the 'key' values from 'properties'
  std::unordered_map< std::string, std::string > property_types;
  
  const rapidjson::Value& js = doc;
  // the type of JSON object will determine if we need to recurse
  
  Rcpp::Rcout << "js size: " << js.Size() << std::endl;
  Rcpp::Rcout << "js is obj: " << js.IsObject() << std::endl;
  Rcpp::Rcout << "js is arr: " << js.IsArray() << std::endl;
  
  // the 'size' is the number of 'rows' at the top level?
  
  if (doc.IsArray() ) {
    auto v = doc.GetArray();
    int n = v.Size();
    
    // iterate into the array and find the keys
    Rcpp::Rcout << "n : " << n << std::endl;
    Rcpp::List properties( property_keys.size() );
    for( int i = 0; i < n; i++ ) {
      const rapidjson::Value& obj = v[i];
      get_property_keys( obj, property_keys );
      properties.names() = property_keys;
      return properties;
    }
  }
  
  // only works for objects, arrays need to be iterated into
  //rapidjson::Value& v = doc;

  return properties;
}


//js <- '[{"x":1},{"x":2},{"x":3},{"y":1},{"y":2}]'

//js <- '[{"x":1,"y":1},{"x":2,"y":2},{"x":3,"y":3},{"x":4,"y":4}]'

// If it's just objects, then the size of the array IS the rows of the data.frame
// then each 'key' can be created wtih an NA vector, and filled iteratively. 