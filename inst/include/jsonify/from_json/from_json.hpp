#ifndef R_JSONIFY_FROM_JSON_H
#define R_JSONIFY_FROM_JSON_H

#include <Rcpp.h>

#include "from_json_utils.hpp"

namespace jsonify {
namespace from_json {

  // Iterate over a rapidjson object and get the unique data types of each value.
  // Save unique data types as ints to unordered_set dtypes.
  // Compatible with rapidjson::Array and rapidjson::Value.
  template<typename T>
  inline std::unordered_set< int > get_dtypes(T& doc, bool scalar_only = false) {

    std::unordered_set< int > dtypes;
    
    int doc_len = doc.Size();
    
    int curr_dtype;
    for(int i = 0; i < doc_len; ++i) {
      curr_dtype = doc[i].GetType();
      // rapidjson uses separate ints for types true (2) and false (1)...combine
      // them into one value such that bool is 1.
      if(curr_dtype == 2) {
        curr_dtype = 1;
      }
      
      // rapidjson uses the same int for types double and int...split them up,
      // such that double is 9 and int is 8.
      if(curr_dtype == 6) {
        if(doc[i].IsDouble()) {
          curr_dtype = 9;
        } else {
          curr_dtype = 8;
        }
      }
      
      dtypes.insert(curr_dtype);
      
      // If scalar_only is true, break if the current value is
      // type 3 (JSON object) or 4 (Array object).
      if(scalar_only) {
        if(curr_dtype == 3 || curr_dtype == 4) {
          break;
        }
      }
    }
    
    return dtypes;
  }
  
  //template< typename T >
  inline SEXP array_to_vector(
      const rapidjson::Value& array, 
      bool& simplify
    ) {
    // takes an array of scalars (any types) and returns
    // them in an R vector
    
    int r_type = 0;
    int first_r_type; // for keeping track if the vector has been coerced when simplified
    // every member of the array will be coerced to the max d-type sexp type
    // but first, get each element and put into a list
    R_xlen_t arr_len = array.Size();
    R_xlen_t i;
    Rcpp::List out( arr_len );
    
    for( i = 0; i < arr_len; i++ ) {
      
      switch(array[i].GetType()) {
      // bool - false
      case 1: {
        out[i] = array[i].GetBool();
        r_type = LGLSXP > r_type ? LGLSXP : r_type;
        break;
      }
        
      // bool - true
      case 2: {
        out[i] = array[i].GetBool();
        r_type = LGLSXP > r_type ? LGLSXP : r_type;
        break;
      }
        
      // string
      case 5: {
        out[i] = array[i].GetString();
        r_type = STRSXP > r_type ? STRSXP : r_type;
        break;
      }
        
      // numeric
      case 6: {
        if(array[i].IsDouble()) {
        // double
        out[i] = array[i].GetDouble();
        r_type = REALSXP > r_type ? REALSXP : r_type;
      } else {
        // int
        out[i] = array[i].GetInt();
        r_type = INTSXP > r_type ? INTSXP : r_type;
      }
      break;
      }
        
      // null
      case 0: {
        out[i] = R_NA_VAL;
        r_type = LGLSXP > r_type ? LGLSXP : r_type;
        break;
      }
      // some other data type not covered, includign arrays and objects
      default: {
        Rcpp::stop("jsonify - array_to_vector only able to parse int, double, string and bool");
      }
      }
      if( i == 0 ) {
        first_r_type = r_type;
      }
    }
    
    if( simplify ) {
      return jsonify::from_json::simplify_vector( out, r_type, 1 );
    }
    return out;
  }
  
  inline bool contains_array( std::unordered_set< int >& dtypes ) {
    return dtypes.find(4) != dtypes.end();
  }
  
  inline bool contains_object( std::unordered_set< int >& dtypes ) {
    return dtypes.find(3) != dtypes.end();
  }
  
  inline bool contains_object_or_array( std::unordered_set< int >& dtypes ) {
    return contains_array( dtypes ) || contains_object( dtypes );
  }
  
  // for testing iterative parsing
  inline void parse_recursive(rapidjson::Value::ConstMemberIterator it) {
    
  }
  
  // //template<typename T>
  // inline SEXP json_to_sexp(
  //   //T& json,
  //   bool& simplify, 
  //   int sequential_array_counter
  // ) {
  // 
  //   Rcpp::List res(1);
  //   
  //   int json_type = json.GetType();
  //   int json_length = json.Size();
  //   
  //   if(json_length == 0) {
  //     if( json_type == 4 ) {
  //       // array goes to list
  //       return Rcpp::List::create();
  //     } else {
  //       return R_NilValue;
  //     }
  //   }
  //   
  //   
  //   R_xlen_t i;
  //   
  //   std::unordered_set< int > dtypes;
  //   dtypes = get_dtypes< T >( json );
  //   
  //   if( json_type == rapidjson::kObjectType ) {
  //     // object {}
  // 
  //     Rcpp::List out( json_length );
  //     Rcpp::CharacterVector names( json_length );
  //     
  //     int i = 0;
  //     for(rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr) {
  //       
  //       // Get current key
  //       names[i] = itr->name.GetString();
  //       
  //       // Get current value
  //       switch(itr->value.GetType()) {
  //       
  //       // bool - false/ true
  //       case rapidjson::kFalseType: {}
  //       case rapidjson::kTrueType: {
  //         out[i] = itr->value.GetBool();
  //         break;
  //       }
  //         
  //         // string
  //       case rapidjson::kStringType: {
  //         out[i] = itr->value.GetString();
  //         break;
  //       }
  //         
  //         // numeric
  //       case rapidjson::kNumberType: {
  //         if(itr->value.IsDouble()) {
  //           // double
  //           out[i] = itr->value.GetDouble();
  //         } else {
  //           // int
  //           out[i] = itr->value.GetInt();
  //         }
  //         break;
  //       }
  //       
  //       // array
  //       case rapidjson::kArrayType: {
  //         //https://github.com/Tencent/rapidjson/issues/1187#issuecomment-370224050
  //         //rapidjson::Document temp_doc;
  //         //temp_doc.CopyFrom( itr->value, temp_doc.GetAllocator() );
  //         //out[i] = json_to_sexp( temp_doc, simplify, sequential_array_counter );
  //         
  //         //rapidjson::Value temp_array( rapidjson::kArrayType );
  //         //const rapidjson::Value::ConstArray temp_array = itr->value;
  //         //out[i] = json_to_sexp< rapidjson::Value::ConstArray >( temp_array, simplify, sequential_array_counter );
  //         
  //         //parse_recursive( itr );
  //         
  //         //out[i] = json_to_sexp( itr->value, simplify, sequential_array_counter );
  //         break;
  //       }
  //       case rapidjson::kObjectType: {
  //         //https://github.com/Tencent/rapidjson/issues/1187#issuecomment-370224050
  //         rapidjson::Document temp_doc;
  //         temp_doc.CopyFrom( itr->value, temp_doc.GetAllocator() );
  //         out[i] = json_to_sexp( temp_doc, simplify, sequential_array_counter );
  //         break;
  //       }
  //         
  //         // null
  //       case rapidjson::kNullType: {
  //         out[i] = R_NA_VAL;
  //         break;
  //       }
  //       // some other data type not covered
  //       default: {
  //         Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool, array, and json");
  //       }
  //       }
  //       
  //       // Bump i
  //       i++;
  //     } // for
  //     
  //     out.attr("names") = names;
  //     res[0] = out;
  //     
  //   } else if( json_type == rapidjson::kArrayType && !contains_object_or_array( dtypes ) ) {
  //     // array of scalars (no internal arrays or objects)
  //     rapidjson::Value::Array curr_array = json.GetArray();
  //     res[0] = array_to_vector( curr_array, simplify );
  //     
  //   } else if ( json_type == rapidjson::kArrayType ) {
  //     // array with internal array
  //     // possibly simplified to matrix
  // 
  //     Rcpp::List array_of_array( json_length );
  //     
  //     for( i = 0; i < json_length; i++ ) {
  // 
  //       switch( json[i].GetType() ) {
  //       
  //       case rapidjson::kNullType: {
  //         sequential_array_counter = 0;
  //         array_of_array[i] = R_NA_VAL;
  //         break;
  //       }
  //       case rapidjson::kFalseType: {}
  //       case rapidjson::kTrueType: {
  //         sequential_array_counter = 0;
  //         array_of_array[i] = json[i].GetBool();
  //         break;
  //       }
  //       case rapidjson::kStringType: {
  //         sequential_array_counter = 0;
  //         array_of_array[i] = json[i].GetString();
  //         break;
  //       }
  //       // numeric
  //       case rapidjson::kNumberType: {
  //         sequential_array_counter = 0;
  //         if(json[i].IsDouble()) {
  //         // double
  //         array_of_array[i] = json[i].GetDouble();
  //       } else {
  //         // int
  //         array_of_array[i] = json[i].GetInt();
  //       }
  //       break;
  //       }
  //       // array
  //       case rapidjson::kArrayType: {
  //         // consecutive inner-arrays *can* be simplified to a matrix
  //         rapidjson::Value::Array inner_array = json[i].GetArray();
  //         array_of_array[i] = json_to_sexp( json[i], simplify, sequential_array_counter );
  //         sequential_array_counter++;
  //         break;
  //       }
  //         // object
  //       case rapidjson::kObjectType: {
  //         sequential_array_counter = 0;
  //         rapidjson::Value& temp_val = json[i];
  //         array_of_array[i] = json_to_sexp( temp_val, simplify, sequential_array_counter );
  //         break;
  //       }
  //       default: {
  //         Rcpp::stop("jsonify - case not handled");
  //       }
  //       } // switch
  //     }   // for
  //     
  //     if( sequential_array_counter > 0  && simplify ) {
  //       
  //       res[0] = jsonify::from_json::list_to_matrix( array_of_array );
  //       
  //     } else if ( contains_object( dtypes ) && dtypes.size() == 1 && !contains_array( dtypes ) && simplify ) {
  //       
  //       res[0] = jsonify::from_json::simplify_dataframe( array_of_array, json_length );
  //       
  //     } else {
  //       res[0] = array_of_array;
  //     }
  // 
  //   } else {
  //     Rcpp::stop("jsonify - case not handled");
  //   }
  //   
  //   return res[0];
  // }
  
  template<typename T>
  inline SEXP json_to_sexp(
      //T& json,
      rapidjson::Value::ConstMemberIterator json,
      bool& simplify, 
      int sequential_array_counter
  ) {
    
    Rcpp::List res(1);
    
    int json_type = json->value.GetType();
    int json_length = json->value.Size();
    
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
    dtypes = get_dtypes( json->value );
    
    if( json_type == rapidjson::kObjectType ) {
      // object {}
      
      Rcpp::List out( json_length );
      Rcpp::CharacterVector names( json_length );
      
      int i = 0;
      for(rapidjson::Value::ConstMemberIterator itr = json->value.MemberBegin(); itr != json->value.MemberEnd(); ++itr) {
        
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
          //https://github.com/Tencent/rapidjson/issues/1187#issuecomment-370224050
          //rapidjson::Document temp_doc;
          //temp_doc.CopyFrom( itr->value, temp_doc.GetAllocator() );
          //out[i] = json_to_sexp( temp_doc, simplify, sequential_array_counter );
          
          //rapidjson::Value temp_array( rapidjson::kArrayType );
          //const rapidjson::Value::ConstArray temp_array = itr->value;
          //out[i] = json_to_sexp< rapidjson::Value::ConstArray >( temp_array, simplify, sequential_array_counter );
          
          //parse_recursive( itr );
          
          //out[i] = json_to_sexp( itr->value, simplify, sequential_array_counter );
          break;
        }
        case rapidjson::kObjectType: {
          //https://github.com/Tencent/rapidjson/issues/1187#issuecomment-370224050
          //rapidjson::Document temp_doc;
          //temp_doc.CopyFrom( itr->value, temp_doc.GetAllocator() );
          //out[i] = json_to_sexp( temp_doc, simplify, sequential_array_counter );
          
          for( auto curr_obj_it = itr->value.MemberBegin(); curr_obj_it != itr->value.MemberEnd(); curr_obj_it++) {
            out[i] = json_to_sexp( curr_obj_it, simplify, sequential_array_counter );
          }
          
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
      const rapidjson::Value& curr_array = json->value; //.GetArray();
      res[0] = array_to_vector( curr_array, simplify );
      
    } else if ( json_type == rapidjson::kArrayType ) {
      // array with internal array
      // possibly simplified to matrix
      
      Rcpp::List array_of_array( json_length );
      const rapidjson::Value& curr_array = json->value;
      
      for( i = 0; i < json_length; i++ ) {
        
        switch( curr_array[i].GetType() ) {
        
        case rapidjson::kNullType: {
          sequential_array_counter = 0;
          array_of_array[i] = R_NA_VAL;
          break;
        }
        case rapidjson::kFalseType: {}
        case rapidjson::kTrueType: {
          sequential_array_counter = 0;
          array_of_array[i] = curr_array[i].GetBool();
          break;
        }
        case rapidjson::kStringType: {
          sequential_array_counter = 0;
          array_of_array[i] = curr_array[i].GetString();
          break;
        }
          // numeric
        case rapidjson::kNumberType: {
          sequential_array_counter = 0;
          if(curr_array[i].IsDouble()) {
            // double
            array_of_array[i] = curr_array[i].GetDouble();
          } else {
            // int
            array_of_array[i] = curr_array[i].GetInt();
          }
          break;
        }
          // array
        case rapidjson::kArrayType: {
          // consecutive inner-arrays *can* be simplified to a matrix
          const rapidjson::Value::ConstArray& inner_array = curr_array[i].GetArray();
          for( auto curr_arr_it = inner_array.Begin(); curr_arr_it != inner_array.End(); curr_arr_it++ ) {
            array_of_array[i] = json_to_sexp( curr_arr_it, simplify, sequential_array_counter );
          }
          // array_of_array[i] = json_to_sexp( json[i], simplify, sequential_array_counter );
          sequential_array_counter++;
          break;
        }
          // object
        case rapidjson::kObjectType: {
          sequential_array_counter = 0;
          const rapidjson::Value& temp_val = curr_array[i];
          // array_of_array[i] = json_to_sexp( temp_val, simplify, sequential_array_counter );
          for( auto curr_obj_it = temp_val.MemberBegin(); curr_obj_it != temp_val.MemberEnd(); curr_obj_it++) {
            array_of_array[i] = json_to_sexp( curr_obj_it, simplify, sequential_array_counter );
          }
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
        
        res[0] = jsonify::from_json::simplify_dataframe( array_of_array, json_length );
        
      } else {
        res[0] = array_of_array;
      }
      
    } else {
      Rcpp::stop("jsonify - case not handled");
    }
    
    return res[0];
  }
  
  //' Parse JSON String
  //'
  //' Takes a JSON string as input, returns an R list of key-value pairs
  //'
  //' @param json const char, JSON string to be parsed. Coming from R, this
  //'  input should be a character vector of length 1.
  //' @export
  inline SEXP from_json(const char * json, bool& simplify ) {
    
    rapidjson::Document doc;
    doc.Parse(json);
    
    std::unordered_set< int > dtypes;
    
    int sequential_array_count = 0;
    
    // Make sure there were no parse errors
    if(doc.HasParseError()) {
      Rcpp::stop("json parse error");
    }
    
    // If the input is a scalar value of type int, double, string, or bool, 
    // return Rcpp vector with length 1.
    if( doc.IsInt() ) {
      Rcpp::IntegerVector x(1);
      x[0] = doc.GetInt();
      return x;
    }
    
    if( doc.IsDouble() ) {
      Rcpp::NumericVector x(1);
      x[0] = doc.GetDouble();
      return x;
    }
    
    if( doc.IsString() ) {
      Rcpp::CharacterVector x(1);
      x[0] = doc.GetString();
      return x;
    }
    
    if( doc.IsBool() ) {
      Rcpp::LogicalVector x(1);
      x[0] = doc.GetBool();
      return x;
    }
    
    Rcpp::List res(1);
    for( auto json_itr = doc.MemberBegin(); json_itr != doc.MemberEnd(); json_itr++ ) {
      res[0] = json_to_sexp( json_itr, simplify, sequential_array_count );
    }
    
    return res;
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
