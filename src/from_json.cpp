#include "Rcpp.h"
#include "from_json/from_json.hpp"


// [[Rcpp::export]]
SEXP rcpp_from_json( const char * json, bool& simplifyDataFrame ) {
  return jsonify::from_json::from_json( json, simplifyDataFrame );
}

// [[Rcpp::export]]
SEXP rcpp_get_col_headers( Rcpp::List& x) {
  return jsonify::from_json::get_col_headers( x );
}

// [[Rcpp::export]]
Rcpp::List rcpp_transpose_list( Rcpp::List& x, Rcpp::CharacterVector& names ) {
  return jsonify::from_json::transpose_list( x, names );
}

// [[Rcpp::export]]
void rcpp_null_to_na( Rcpp::List& x ) {
  return jsonify::from_json::null_to_na( x );
}


/*
 
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

void parse_object(const rapidjson::Value& v) {

}

void parse_array(const rapidjson::Value& v) {

static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
int n = v.Size();
Rcpp::Rcout << "array size: " << n << std::endl;

// for ( rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
//   printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
// }

// for ( auto& x : v.GetObject() ) {
//   std::string key = x.name.GetString();
//   Rcpp::Rcout << "key: " << key << std::endl;
// }

}

Rcpp::List parse_json(rapidjson::Document& d) {
// can dispatch into either parse object or parse array
// and be recursive, to call itself for nested items
Rcpp::List res;

if (d.IsObject()) {

Rcpp::Rcout << "parsing object" << std::endl;
parse_object( d );

} else if (d.IsArray() ) {

Rcpp::Rcout << "parsing array" << std::endl;
parse_array( d );

} else {

Rcpp::stop("unknown javascript type");
}


return res;
}

// if in Object and next is Array, switch. and vice versa


// [[Rcpp::export]]
Rcpp::List test( Rcpp::StringVector json ) {

Rcpp::List res;
// otherwise it's a list

rapidjson::Document doc;
doc.SetObject();

const char* j = json[0];
doc.Parse( j );

//int n = geojson.size();

std::set< std::string > keys;   // storing all the 'key' values from 'properties'
std::unordered_map< std::string, std::string > types;

const rapidjson::Value& js = doc;
// the type of JSON object will determine if we need to recurse

Rcpp::Rcout << "js size: " << js.Size() << std::endl;
Rcpp::Rcout << "js is obj: " << js.IsObject() << std::endl;
Rcpp::Rcout << "js is arr: " << js.IsArray() << std::endl;

// the 'size' is the number of 'rows' at the top level?
res = parse_json( doc );


// if (doc.IsArray() ) {
//   auto v = doc.GetArray();
//   int n = v.Size();
//   
//   // iterate into the array and find the keys
//   Rcpp::Rcout << "n : " << n << std::endl;
//   Rcpp::List properties( property_keys.size() );
//   for( int i = 0; i < n; i++ ) {
//     
//     const rapidjson::Value& obj = v[i];
//     get_property_keys( obj, property_keys );
//     properties.names() = property_keys;
//     return properties;
//     
//   }
// }

// only works for objects, arrays need to be iterated into
//rapidjson::Value& v = doc;

return res;
}


//js <- '[{"x":1},{"x":2},{"x":3},{"y":1},{"y":2}]'

//js <- '[{"x":1,"y":1},{"x":2,"y":2},{"x":3,"y":3},{"x":4,"y":4}]'

// If it's just objects, then the size of the array IS the rows of the data.frame
// then each 'key' can be created wtih an NA vector, and filled iteratively. 

*/
