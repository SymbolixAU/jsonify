#ifndef R_JSONIFY_FROM_JSON_H
#define R_JSONIFY_FROM_JSON_H

#include <Rcpp.h>

#include "from_json_utils.hpp"
#include "simplify/simplify.hpp"

namespace jsonify {
namespace from_json {

  inline SEXP json_to_sexp(
    const rapidjson::Value& json,
    bool& simplify,
    bool& fill_na,
    int sequential_array_counter
  ) {

    Rcpp::List res(1);

    int json_type = json.GetType();
    int json_length = json.Size();

    if(json_length == 0) {
      if( json_type == 4 ) {
        // array goes to list
        return Rcpp::List::create();
      } else {
        return R_NilValue;
      }
    }


    R_xlen_t i;

    std::unordered_set< int > dtypes;
    dtypes = get_dtypes( json );

    if( json_type == rapidjson::kObjectType ) {
      // object {}

      Rcpp::List out( json_length );
      Rcpp::CharacterVector names( json_length );

      int i = 0;
      for(rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr) {

        // Get current key
        names[i] = itr->name.GetString();

        // Get current value
        switch(itr->value.GetType()) {

        // bool - false/ true
        case rapidjson::kFalseType: {}
        case rapidjson::kTrueType: {
          out[i] = itr->value.GetBool();
          break;
        }

          // string
        case rapidjson::kStringType: {
          out[i] = itr->value.GetString();
          break;
        }

          // numeric
        case rapidjson::kNumberType: {
          if(itr->value.IsDouble()) {
            // double
            out[i] = itr->value.GetDouble();
          } else {
            // int
            out[i] = itr->value.GetInt();
          }
          break;
        }

        // array
        case rapidjson::kArrayType: {
          const rapidjson::Value& temp_array = itr->value;
          out[i] = json_to_sexp( temp_array, simplify, fill_na, sequential_array_counter );
          break;
        }
        case rapidjson::kObjectType: {
          out[i] = json_to_sexp( itr->value, simplify, fill_na, sequential_array_counter );
          break;
        }

          // null
        case rapidjson::kNullType: {
          out[i] = R_NA_VAL;
          break;
        }
        // some other data type not covered
        default: {
          Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool, array, and json");
        }
        }

        // Bump i
        i++;
      } // for

      out.attr("names") = names;
      res[0] = out;

    } else if( json_type == rapidjson::kArrayType && !contains_object_or_array( dtypes ) ) {
      // array of scalars (no internal arrays or objects)
      rapidjson::Value::ConstArray curr_array = json.GetArray();
      res[0] = array_to_vector( curr_array, simplify );

    } else if ( json_type == rapidjson::kArrayType ) {
      // array with internal array
      // possibly simplified to matrix

      Rcpp::List array_of_array( json_length );

      for( i = 0; i < json_length; i++ ) {

        switch( json[i].GetType() ) {

        case rapidjson::kNullType: {
          sequential_array_counter = 0;
          array_of_array[i] = R_NA_VAL;
          break;
        }
        case rapidjson::kFalseType: {}
        case rapidjson::kTrueType: {
          sequential_array_counter = 0;
          array_of_array[i] = json[i].GetBool();
          break;
        }
        case rapidjson::kStringType: {
          sequential_array_counter = 0;
          array_of_array[i] = json[i].GetString();
          break;
        }
        // numeric
        case rapidjson::kNumberType: {
          sequential_array_counter = 0;
          if(json[i].IsDouble()) {
          // double
          array_of_array[i] = json[i].GetDouble();
        } else {
          // int
          array_of_array[i] = json[i].GetInt();
        }
        break;
        }
        // array
        case rapidjson::kArrayType: {
          array_of_array[i] = json_to_sexp( json[i], simplify, fill_na, sequential_array_counter );
          sequential_array_counter++;
          break;
        }
          // object
        case rapidjson::kObjectType: {
          sequential_array_counter = 0;
          const rapidjson::Value& temp_val = json[i];
          array_of_array[i] = json_to_sexp( temp_val, simplify, fill_na, sequential_array_counter );
          break;
        }
        default: {
          Rcpp::stop("jsonify - case not handled");
        }
        } // switch
      }   // for

      if( sequential_array_counter > 0  && simplify ) {

        res[0] = jsonify::from_json::list_to_matrix( array_of_array );

      } else if ( contains_object( dtypes ) && dtypes.size() == 1 && !contains_array( dtypes ) && simplify ) {
        
        if( fill_na ) {
          res[0] = jsonify::from_json::simplify_dataframe_fill_na( array_of_array, json_length );
        } else {
          res[0] = jsonify::from_json::simplify_dataframe( array_of_array, json_length );
        }
      } else {
        res[0] = array_of_array;
      }

    } else {
      Rcpp::stop("jsonify - case not handled");
    }

    return res[0];
  }
  
  // Test array types
  // If it's an object, it's 'simplify' value will be a data.frame
  // if it's an array, this test finds the types inside the array
  inline Rcpp::IntegerVector test_dtypes( const char * json ) {
    
    rapidjson::Document doc;
    doc.Parse(json);
    
    std::unordered_set< int > dtypes;
    
    if(!doc.IsArray()) {
      return Rcpp::IntegerVector(0); // if not an array it doesn't call get_dtypes
    }
    
    dtypes = get_dtypes<rapidjson::Document>(doc);
    Rcpp::IntegerVector iv(dtypes.begin(), dtypes.end());
    return iv;
  }
  
} // namespace from_json
} // namespace jsonify

#endif
