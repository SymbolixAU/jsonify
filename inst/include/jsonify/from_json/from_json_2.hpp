#ifndef R_JSONIFY_FROM_JSON_2_H
#define R_JSONIFY_FROM_JSON_2_H

#include <Rcpp.h>

#include "from_json_utils.hpp"
#include "simplify/simplify.hpp"


namespace jsonify {
namespace from_json {
  
  template< typename T > SEXP parse_json( const T& json );
  template< typename T > SEXP parse_object( const T& json );
  template< typename T > SEXP parse_array( const T& json );
  
  template< typename T >
  inline SEXP parse_array( const T& json ) {
    
    R_xlen_t json_length = json.Size();
    Rcpp::List out( json_length );
    
    R_xlen_t i = 0;
    for ( auto& child : json.GetArray() ) {
      out[ i++ ] = parse_json( child );
    }
    return out;
  }
  
  template< typename T >
  inline SEXP parse_object( const T& json ) {
    
    R_xlen_t json_length = json.Size();
    
    Rcpp::List out( json_length );
    Rcpp::CharacterVector names( json_length );
    R_xlen_t i = 0;
    
    // https://github.com/Tencent/rapidjson/issues/162#issuecomment-341824061
  #if __cplusplus >= 201703L
    for ( auto& [key, value] : json.GetObject() ) {
      out[ i ] = parse_json( value );
      names[ i++ ] = std::string( key );
    }
  #else
    for ( auto& key_value : json.GetObject() ) {
      out[ i ] = parse_json( key_value.value );
      names[ i++ ] = std::string( key_value.name.GetString() );
    }
  #endif
    out.attr("names") = names;
    return out;
  }
  
  inline SEXP simple(
    Rcpp::List& out,
    std::unordered_set< int > dtypes,
    R_xlen_t json_length
  ) {
    bool fill_na = true;
    Rcpp::List res(1);
    
    if( dtypes.size() == 1 && contains_array( dtypes ) ) { 
      res[0] = jsonify::from_json::list_to_matrix( out );
      
    } else if ( contains_object( dtypes ) && dtypes.size() == 1 && !contains_array( dtypes ) ) {
      if( fill_na ) {
        res[0] = jsonify::from_json::simplify_dataframe_fill_na( out, json_length );
      } else {
        res[0] = jsonify::from_json::simplify_dataframe( out, json_length );
      }
    } else {
      res[0] = out;
    }
    return res;
  }
  
  
  template< typename T >
  inline SEXP parse_json( const T& json ) {
    
    Rcpp::List res( 1 );
    std::unordered_set< int > dtypes;
    
    bool fill_na = true;
    bool simplify = true;
    R_xlen_t json_length = json.Size();
    
    
    switch( json.GetType() ) {
    
      case rapidjson::kNullType: {
        res[0] = R_NA_VAL;
        break;
      }
      case rapidjson::kFalseType: {}
      case rapidjson::kTrueType: {
        res[0] = Rcpp::wrap< bool >( json.GetBool() );
        break;
      }
      case rapidjson::kStringType: {
        res[0] = Rcpp::wrap( std::string( json.GetString() ) );
        break;
      }
        // numeric
      case rapidjson::kNumberType: {
        if(json.IsDouble()) {
        // double
        res[0] = Rcpp::wrap< double >( json.GetDouble() );
      } else {
        // int
        res[0] = Rcpp::wrap< int >( json.GetInt() );
      }
      break;
      }
      case rapidjson::kObjectType: {
        res[0] = parse_object( json );
        break;
      }
      case rapidjson::kArrayType: {
        dtypes.clear();
        dtypes = get_dtypes( json );
        Rcpp::List arr = parse_array( json );
        res[0] = simple( arr, dtypes, json_length );
        break;
      }
      default: {
        Rcpp::stop("jsonify - case not handled");
      }
    }
    
    return res[0];
    //return R_NilValue;
  }
  
  template< typename T >
  inline SEXP from_json2(
    const T& json,
    bool simplify,
    bool fill_na
  ) {
    
    int json_type = json.GetType();
    R_xlen_t json_length = json.Size();
    
    if(json_length == 0) {
      if( json_type == 4 ) {
        // array goes to list
        return Rcpp::List::create();
      } else {
        return R_NilValue;
      }
    }
    
    return parse_json( json );
    
  }


} // namespace from_json
} // namespace jsonify

#endif
