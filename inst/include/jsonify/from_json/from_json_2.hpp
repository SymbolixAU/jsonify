#ifndef R_JSONIFY_FROM_JSON_2_H
#define R_JSONIFY_FROM_JSON_2_H

#include <Rcpp.h>

#include "from_json_utils.hpp"
#include "simplify/simplify.hpp"


// namespace jsonify {
// namespace from_json {
//   
//   template< typename T > SEXP parse_json( const T& json, bool simplify, bool fill_na );
//   template< typename T > SEXP parse_object( const T& json, bool simplify, bool fill_na );
//   template< typename T > SEXP parse_array( const T& json, bool simplify, bool fill_na );
//   
//   template< typename T >
//   inline SEXP parse_array(
//       const T& json,
//       bool simplify,
//       bool fill_na
//     ) {
//     
//     R_xlen_t json_length = json.Size();
//     Rcpp::List out( json_length );
//     
//     R_xlen_t i = 0;
//     for ( auto& child : json.GetArray() ) {
//       out[ i++ ] = parse_json( child, simplify, fill_na );
//     }
//     return out;
//   }
//   
//   template< typename T >
//   inline SEXP parse_object(
//       const T& json,
//       bool simplify,
//       bool fill_na
//     ) {
//     
//     R_xlen_t json_length = json.Size();
//     
//     Rcpp::List out( json_length );
//     Rcpp::CharacterVector names( json_length );
//     R_xlen_t i = 0;
//     
//     // https://github.com/Tencent/rapidjson/issues/162#issuecomment-341824061
//   #if __cplusplus >= 201703L
//     for ( auto& [key, value] : json.GetObject() ) {
//       out[ i ] = parse_json( value, simplify, fill_na );
//       names[ i++ ] = std::string( key );
//     }
//   #else
//     for ( auto& key_value : json.GetObject() ) {
//       out[ i ] = parse_json( key_value.value, simplify, fill_na );
//       names[ i++ ] = std::string( key_value.name.GetString() );
//     }
//   #endif
//     out.attr("names") = names;
//     return out;
//   }
//   
//   template< typename T >
//   inline SEXP parse_json(
//       const T& json,
//       bool simplify,
//       bool fill_na
//     ) {
//     
//     Rcpp::List res( 1 );
//     std::unordered_set< int > dtypes;
//     
//     R_xlen_t json_length = json.Size();
//     
//     switch( json.GetType() ) {
//     
//       case rapidjson::kNullType: {
//         res[0] = R_NA_VAL;
//         break;
//       }
//       case rapidjson::kFalseType: {}
//       case rapidjson::kTrueType: {
//         res[0] = Rcpp::wrap< bool >( json.GetBool() );
//         break;
//       }
//       case rapidjson::kStringType: {
//         res[0] = Rcpp::wrap( std::string( json.GetString() ) );
//         break;
//       }
//         // numeric
//       case rapidjson::kNumberType: {
//         if(json.IsDouble()) {
//         // double
//         res[0] = Rcpp::wrap< double >( json.GetDouble() );
//       } else {
//         // int
//         res[0] = Rcpp::wrap< int >( json.GetInt() );
//       }
//       break;
//       }
//       case rapidjson::kObjectType: {
//         res[0] = parse_object( json, simplify, fill_na );
//         break;
//       }
//       case rapidjson::kArrayType: {
//         dtypes.clear();
//         dtypes = get_dtypes( json );
//         
//         if( !contains_object_or_array( dtypes ) ) {
//           res[0] = array_to_vector( json.GetArray(), simplify );
//         } else {
//           Rcpp::List arr = parse_array( json, simplify, fill_na );
//           res[0] = jsonify::from_json::simplify( arr, dtypes, json_length );
//         }
//         break;
//       }
//       default: {
//         Rcpp::stop("jsonify - case not handled");
//       }
//     }
//     
//     return res[0];
//     //return R_NilValue;
//   }
//   
//   template< typename T >
//   inline SEXP from_json2(
//     const T& json,
//     bool simplify,
//     bool fill_na
//   ) {
//     
//     int json_type = json.GetType();
//     R_xlen_t json_length = json.Size();
//     
//     if(json_length == 0) {
//       if( json_type == 4 ) {
//         // array goes to list
//         return Rcpp::List::create();
//       } else {
//         return R_NilValue;
//       }
//     }
//     
//     return parse_json( json, simplify, fill_na );
//   }
// 
// 
// } // namespace from_json
// } // namespace jsonify

#endif
