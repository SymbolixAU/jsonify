#ifndef R_JSONIFY_PARSE_JSON_H
#define R_JSONIFY_PARSE_JSON_H

#include <Rcpp.h>

#include "from_json_utils.hpp"
#include "simplify/simplify.hpp"



namespace jsonify {
namespace parse_json {

  SEXP parse_json( const rapidjson::Value& json );
  SEXP parse_object( const rapidjson::Value& json );
  SEXP parse_array( const rapidjson::Value& json );
  
  inline SEXP parse_array(const rapidjson::Value& json) {
  
    R_xlen_t json_length = json.Size();
    Rcpp::List out( json_length );
    R_xlen_t i;
  
    for( i = 0; i < json_length; ++i ) {
      out[i] = parse_json( json[i] );
    }   // for
  
    return out;
    //}
  }
  
  inline SEXP parse_object(const rapidjson::Value& json ) {
  
    R_xlen_t json_length = json.Size();
    
    Rcpp::List out( json_length );
    Rcpp::CharacterVector names( json_length );
    R_xlen_t i = 0;
    
    for(rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr) {
  
      // Get current key
      names[i] = Rcpp::String( itr->name.GetString() );
  
      // Get current value
      out[i] = parse_json( itr->value );
      ++i;
    } // for
  
    out.attr("names") = names;
    return out;
  }
  
  inline SEXP parse_json(
      const rapidjson::Value& json
  ) {

    switch( json.GetType() ) {
      
      case rapidjson::kNullType: {
        return R_NA_VAL;
      }
      case rapidjson::kFalseType: {}
      case rapidjson::kTrueType: {
        return Rcpp::wrap< bool >( json.GetBool() );
      }
      case rapidjson::kStringType: {
        return Rcpp::wrap( std::string( json.GetString() ) );
      }
        // numeric
      case rapidjson::kNumberType: {
        if(json.IsDouble()) {
          // double
          return Rcpp::wrap< double >( json.GetDouble() );
        } else {
          // int
          return Rcpp::wrap< int >( json.GetInt() );
        }
      }
      case rapidjson::kObjectType: {
        return parse_object( json );
      }
      case rapidjson::kArrayType: {
        return parse_array( json );
      }
      default: {
        Rcpp::stop("jsonify - case not handled");
      }
    }
    return R_NilValue;
  }


} // namespace from_json
} // namespace jsonify

#endif
