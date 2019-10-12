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
    // Clear all values from dtypes
    //dtypes.clear();
    
    std::unordered_set< int > dtypes;
    
    int doc_len = doc.Size();
    // Rcpp::Rcout << "get_dtypes - doc_len: " << doc_len << std::endl;
    
    int curr_dtype;
    for(int i = 0; i < doc_len; ++i) {
      curr_dtype = doc[i].GetType();
      // int curr_size = doc[i].GetSize();
      // Rcpp::Rcout << "curr_size: " << curr_size << std::endl;
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
  
  template< typename T >
  inline SEXP array_to_vector( T& array, bool& simplify ) {
    // takes an array of scalars (any types) and returns
    // them in an R vector
    //Rcpp::Rcout << "array_to_vector" << std::endl;
    // Rcpp::IntegerVector iv_dtypes( dtypes.begin(), dtypes.end() );
    // int max_dtype = Rcpp::max( iv_dtypes );
    // Rcpp::Rcout << "max_dtype: " << max_dtype << std::endl;
    
    int r_type = 0;
    int first_r_type; // for keeping track if the vector has been coerced when simplified
    // every member of the array will be coerced to the max d-type sexp type
    // but first, get each element and put into a list
    R_xlen_t arr_len = array.Size();
    // Rcpp::Rcout << "arr_len " << arr_len << std::endl;
    R_xlen_t i;
    Rcpp::List out( arr_len );
    
    for( i = 0; i < arr_len; i++ ) {
      
      // int tp = array[i].GetType();
      // Rcpp::Rcout << "tp: " << tp << std::endl;
      
      // put in a list, then coerce list to vector??
      // each list element will be length 1
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
    
    //Rcpp::Rcout << "final r_type " << r_type << std::endl;
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
  
  template<typename T>
  inline SEXP json_to_sexp(
    T& json,
    bool& simplify, 
    int sequential_array_counter
  ) {

    Rcpp::List res(1);
    
    int json_type = json.GetType();
    int json_length = json.Size();
    
    // Rcpp::Rcout << "json_type: " << json_type << std::endl;
    // Rcpp::Rcout << "json_length: " << json_length << std::endl;
    
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
    dtypes = get_dtypes< T >( json );
    
    // Rcpp::IntegerVector iv_dtypes( dtypes.begin(), dtypes.end() );
    // Rcpp::Rcout << "dtypes: " << iv_dtypes << std::endl;
    
    if( json_type == 3 ) {
      // object {}
      
      // simplifies to a named list, then potentially a data.frame
      // formally 'parse_document()'
      
      // std::unordered_set<int> list_lengths;
      // std::unordered_set<int> list_types;

      Rcpp::List out( json_length );
      Rcpp::CharacterVector names( json_length );
      
      int i = 0;
      for(rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr) {
        
        // Get current key
        names[i] = itr->name.GetString();
        // Rcpp::Rcout << "names: " << names << std::endl;
        
        // Get current value
        switch(itr->value.GetType()) {
        
        // bool - false/ true
        case 1: {}
        case 2: {
          // list_lengths.insert(1);
          // list_types.insert( LGLSXP );
          out[i] = itr->value.GetBool();
          break;
        }
          
          // string
        case 5: {
          // list_lengths.insert(1);
          // list_types.insert( STRSXP );
          out[i] = itr->value.GetString();
          break;
        }
          
          // numeric
        case 6: {
          // list_lengths.insert(1);
          if(itr->value.IsDouble()) {
            // double
            // list_types.insert( REALSXP );
            out[i] = itr->value.GetDouble();
          } else {
            // int
            // list_types.insert( INTSXP );
            out[i] = itr->value.GetInt();
          }
          break;
        }
        
        // array
        case 4: {
          //Rcpp::Rcout << "case 4, i: " << i << std::endl;
          rapidjson::Document temp_doc;
          temp_doc.CopyFrom( itr->value, temp_doc.GetAllocator() );
          out[i] = json_to_sexp( temp_doc, simplify, sequential_array_counter );
          break;
        }
        case 3: {
          rapidjson::Document temp_doc;
          temp_doc.CopyFrom( itr->value, temp_doc.GetAllocator() );
          out[i] = json_to_sexp( temp_doc, simplify, sequential_array_counter );
          break;
        }
          
          // null
        case 0: {
          // list_lengths.insert(1);
          // list_types.insert( LGLSXP );
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
      }
      
      out.attr("names") = names;
      res[0] = out;
      
    } else if( json_type == 4 && !contains_object_or_array( dtypes ) ) {
      // array of scalars (no internal arrays or objects)
      // Rcpp::Rcout << "single array" << std::endl;
      rapidjson::Value::Array curr_array = json.GetArray();
      res[0] = array_to_vector( curr_array, simplify );
      
    } else if ( json_type == 4 ) {
      // array with internal array
      // possibly simplified to matrix

      // iterate over objects.
      // when finds the array, go back into json_to_sexp()
      Rcpp::List array_of_array( json_length );
      //rapidjson::Value::Array curr_array = json.GetArray();
      
      for( i = 0; i < json_length; i++ ) {
  
        switch( json[i].GetType() ) {
        
        case 0: {
          sequential_array_counter = 0;
          array_of_array[i] = R_NA_VAL;
          break;
        }
        case 1: {}
        case 2: {
          sequential_array_counter = 0;
          array_of_array[i] = json[i].GetBool();
          break;
        }
        case 5: {
          sequential_array_counter = 0;
          array_of_array[i] = json[i].GetString();
          break;
        }
        // numeric
        case 6: {
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
        case 4: {
          // consecutive inner-arrays *can* be simplified to a matrix
          rapidjson::Value::Array inner_array = json[i].GetArray();
          array_of_array[i] = json_to_sexp( json[i], simplify, sequential_array_counter );
          // Rcpp::Rcout << "simplify list to matrix? " << sequential_array_counter << std::endl;
          sequential_array_counter++;
          break;
        }
          // object
        case 3: {
          sequential_array_counter = 0;
          // Rcpp::Rcout << "case 3 " << std::endl;
          //rapidjson::Value::Object inner_object = json[i].GetObject();
          // here need to iterate into the object
          rapidjson::Value& temp_val = json[i];
          // Rcpp::Rcout << "going back in" << std::endl;
          array_of_array[i] = json_to_sexp( temp_val, simplify, sequential_array_counter );
          // Rcpp::Rcout << "coming out" << std::endl;
          break;
        }
        default: {
          Rcpp::stop("jsonify - case not handled");
        }
        } // switch
      }   // for
      
      if( sequential_array_counter > 0  && simplify ) {
        // Rcpp::Rcout << "simplify list to matrix (outside for)? " << sequential_array_counter << std::endl;
        
        res[0] = jsonify::from_json::list_to_matrix( array_of_array );
        
      } else if ( contains_object( dtypes ) && dtypes.size() == 1 && !contains_array( dtypes ) && simplify ) {
        
        // Rcpp::Rcout << "simplify to data.frame? " << std::endl;
        // iff all the column lengths are the same, and > 1, the whole column can become a matrix
        // iff all the column lenghts are the same, and == 1, the whole column is a vector
        // iff any column lenghts are different, it's a list
        res[0] = jsonify::from_json::simplify_dataframe( array_of_array, json_length );
        //res[0] = array_of_array;
      } else {
        res[0] = array_of_array;
      }

    } else {
      Rcpp::stop("jsonify - case not handled");
    }
    
    // Rcpp::Rcout << "returning from json_to_sexp() " << std::endl;
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
      //Rcpp::Rcerr << "parse error for json string: " << json << std::endl;
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
    
    return json_to_sexp( doc, simplify, sequential_array_count );

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
