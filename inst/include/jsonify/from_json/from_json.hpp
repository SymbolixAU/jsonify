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
  inline void get_dtypes(T& doc, bool scalar_only = false) {
    // Clear all values from dtypes
    dtypes.clear();
    
    int doc_len = doc.Size();
    //Rcpp::Rcout << "doc_len: " << doc_len << std::endl;
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
      // such that double is 8 and int is 9.
      if(curr_dtype == 6) {
        if(doc[i].IsDouble()) {
          curr_dtype = 8;
        } else {
          curr_dtype = 9;
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
    //Rcpp::Rcout << "dtypes: " << dtypes << std::endl;
    //Rcpp::Rcout << "dtypes: " << std::endl;
    // for( auto itr = dtypes.begin(); itr != dtypes.end(); itr++ ) {
    //   Rcpp::Rcout << (*itr) << std::endl;
    // }
  }
  
  
  // Dump objects from a rapidjson array to an R list.
  // Can handle JSON objects that have keys and those that do not have keys.
  template<typename T>
  inline Rcpp::List array_to_list(T& array, int& array_len) {
    Rcpp::Rcout << "array_to_list" << std::endl;
    Rcpp::List out(array_len);
    for(int i = 0; i < array_len; ++i) {
      
      switch(array[i].GetType()) {
      
      // bool - false
      case 1: {
        out[i] = array[i].GetBool();
        break;
      }
        
      // bool - true
      case 2: {
        out[i] = array[i].GetBool();
        break;
      }
        
      // string
      case 5: {
        out[i] = array[i].GetString();
        break;
      }
        
      // numeric
      case 6: {
        if(array[i].IsDouble()) {
          // double
          out[i] = array[i].GetDouble();
        } else {
          // int
          out[i] = array[i].GetInt();
        }
        break;
      }
      
      // null
      case 0: {
        out[i] = R_NA_VAL;
        break;
      }
        
      // array
      case 4: {
        int curr_array_len = array[i].Size();
        T curr_array = array[i].GetArray();
        out[i] = array_to_list<T>(curr_array, curr_array_len);
        break;
      }
        
      // some other data type not covered
      default: {
        Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool, and array");
      }
        
      }
    }
    
    return out;
  }
  
  
  // Parse an array object, return an SEXP that contains the objects from the
  // array.
  // Can handle JSON objects that have keys and those that do not have keys.
  template<typename T>
  inline SEXP parse_array(T& array) {
    
    int array_len = array.Size();
    
    // Get set of unique data types of each value in the array.
    get_dtypes<T>(array);
    
    // simplify logic
    
    // If there's only one unique data type in the input array, or if the array
    // is made up a simple data type and nulls (simple being int, double,
    // string, bool), then return an R vector. Otherwise, return an R list.
    
    if(dtypes.size() > 2) {
      return array_to_list<T>(array, array_len);
    }
    
    int data_type;
    if(dtypes.size() == 1) {
      data_type = *dtypes.begin();
    } else {
      // Dump dtypes values to a vector.
      std::vector<int> dtype_vect(dtypes.begin(), dtypes.end());
      
      // Check to see if 0 is in dtypes.
      if(dtypes.find(0) != dtypes.end()) {
        // If 0 is in dtypes and dtypes size is two, get the int in dtypes
        // that is not 0.
        data_type = dtype_vect[0];
        if(data_type == 0) {
          data_type = dtype_vect[1];
        }
        if(data_type == 3 || data_type == 4) {
          // If dtype_vect is [0, 3] or [0, 4], return R list.
          return array_to_list<T>(array, array_len);
        }
      } else {
        // If dtype_vect size is 2 and 0 is not one of the values, return R list.
        return array_to_list<T>(array, array_len);
      }
    }
    
    // Get current value
    switch(data_type) {
    
    // bool
    case 1: {
      Rcpp::LogicalVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        if(array[i].GetType() == 0) {
          out[i] = NA_LOGICAL;
        } else {
          out[i] = array[i].GetBool();
        }
      }
      return out;
    }
      
    // string
    case 5: {
      Rcpp::CharacterVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        if(array[i].GetType() == 0) {
          out[i] = NA_STRING;
        } else {
          out[i] = array[i].GetString();
        }
      }
      return out;
    }
      
    // double
    case 8: {
      Rcpp::NumericVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        if(array[i].GetType() == 0) {
          out[i] = NA_REAL;
        } else {
          out[i] = array[i].GetDouble();
        }
      }
      return out;
    }
      
    // int
    case 9: {
      Rcpp::IntegerVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        if(array[i].GetType() == 0) {
          out[i] = NA_INTEGER;
        } else {
          out[i] = array[i].GetInt();
        }
      }
      return out;
    }
      
    // null
    case 0: {
      Rcpp::LogicalVector out(array_len, NA_LOGICAL);
      return out;
    }
    
    // JSON object
    case 3: {
      Rcpp::List out(array_len);
      for(int i = 0; i < array_len; ++i) {
        const rapidjson::Value& curr_val = array[i];
        out[i] = parse_value(curr_val);
      }
      return(out);
    }
      
    // array
    case 4: {
      Rcpp::List out(array_len);
      for(int i = 0; i < array_len; ++i) {
        T curr_array = array[i].GetArray();
        out[i] = parse_array<T>(curr_array);
      }
      return out;
    }
    }
    
    return R_NilValue;
  }
  
  
  // Parse rapidjson::Value object.
  inline Rcpp::List parse_value(const rapidjson::Value& val) {
    
    int json_len = val.Size();
    Rcpp::List out( json_len );
    Rcpp::CharacterVector names( json_len );
    
    int i = 0;
    for (rapidjson::Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr) {
      
      // Get current key
      names[i] = itr->name.GetString();
      
      Rcpp::Rcout << "parse_value names: " << names << std::endl;
      
      // Get current value
      switch(itr->value.GetType()) {
      
      // bool - false
      case 1: {
        out[i] = itr->value.GetBool();
        break;
      }
        
      // bool - true
      case 2: {
        out[i] = itr->value.GetBool();
        break;
      }
        
      // string
      case 5: {
        out[i] = itr->value.GetString();
        break;
      }
        
      // numeric
      case 6: {
        if(itr->value.IsDouble()) {
          // double
          out[i] = itr->value.GetDouble();
        } else {
          // int
          out[i] = itr->value.GetInt();
        }
        break;
      }
      
      // null
      case 0: {
        out[i] = R_NA_VAL;  
        // TODO
        // Need to know the 'type' of null; 
        // i.e., it should match the 'previous' null object type in the vector
        break;
      }
        
      // array
      case 4: {
        rapidjson::Value::ConstArray curr_array = itr->value.GetArray();
        out[i] = parse_array<rapidjson::Value::ConstArray>(curr_array);
        break;
      }
        
      // JSON object
      case 3: {
        const rapidjson::Value& curr_val = itr->value;
        Rcpp::Rcout << "going back through parse_value() " << std::endl;
        out[i] = parse_value(curr_val);
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
    return out;
  }
  
  
  /*
   * parse_document
   * 
   * Parse rapidjson::Document object
   * 
   */
  inline SEXP parse_document(rapidjson::Document& doc) {
    int json_len = doc.Size();
    
    Rcpp::Rcout << "parse_document" << std::endl;
    
    // If doc has length zero, return NULL.
    if(json_len == 0) {
      return R_NilValue;
    }
    
    Rcpp::List out(json_len);
    Rcpp::CharacterVector names(json_len);
    
    int i = 0;
    for(rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {

      // Get current key
      names[i] = itr->name.GetString();

      // Get current value
      switch(itr->value.GetType()) {

      // bool - false
      case 1: {
        out[i] = itr->value.GetBool();
        break;
      }

      // bool - true
      case 2: {
        out[i] = itr->value.GetBool();
        break;
      }

      // string
      case 5: {
        out[i] = itr->value.GetString();
        break;
      }

      // numeric
      case 6: {
        if(itr->value.IsDouble()) {
          // double
          out[i] = itr->value.GetDouble();
        } else {
          // int
          out[i] = itr->value.GetInt();
        }
        break;
      }

      // null
      case 0: {
        out[i] = R_NA_VAL;
        break;
      }

      // array
      case 4: {
        rapidjson::Value::ConstArray curr_array = itr->value.GetArray();
        out[i] = parse_array<rapidjson::Value::ConstArray>(curr_array);
        break;
      }

      // JSON object
      case 3: {
        const rapidjson::Value& temp_val = itr->value;
        out[i] = parse_value(temp_val);
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
    
    Rcpp::Rcout << "names: " << names << std::endl;
    
    out.attr("names") = names;
    return out;
  }
  
  
  // Parse rapidjson::Document object that contains "keyless" JSON data of the
  // same data type. Returns an R vector.
  inline SEXP doc_to_vector(rapidjson::Document& doc, int& dtype) {
    // no simplification required
    
    Rcpp::Rcout << "doc_to_vector" << std::endl;
    
    int doc_len = doc.Size();
    
    // Get current value
    switch(dtype) {
    
    // bool
    case 1: {
      Rcpp::LogicalVector out(doc_len);
      for(int i = 0; i < doc_len; ++i) {
        if(doc[i].GetType() == 0) {
          out[i] = NA_LOGICAL;
        } else {
          out[i] = doc[i].GetBool();
        }
      }
      return out;
    }
      
    // string
    case 5: {
      Rcpp::CharacterVector out(doc_len);
      for(int i = 0; i < doc_len; ++i) {
        if(doc[i].GetType() == 0) {
          out[i] = NA_STRING;
        } else {
          out[i] = doc[i].GetString();
        }
      }
      return out;
    }
      
    // double
    case 8: {
      Rcpp::NumericVector out(doc_len);
      for(int i = 0; i < doc_len; ++i) {
        if(doc[i].GetType() == 0) {
          out[i] = NA_REAL;
        } else {
          out[i] = doc[i].GetDouble();
        }
      }
      return out;
    }
      
    // int
    case 9: {
      Rcpp::IntegerVector out(doc_len);
      for(int i = 0; i < doc_len; ++i) {
        if(doc[i].GetType() == 0) {
          out[i] = NA_INTEGER;
        } else {
          out[i] = doc[i].GetInt();
        }
      }
      return out;
    }
      
    // null
    case 0: {
      Rcpp::LogicalVector out(doc_len, NA_LOGICAL);
      return out;
    }
    }
    
    return R_NilValue;
  }
  
  
  // Parse rapidjson::Document object that contains "keyless" JSON data that
  // contains a variety of data types. Returns an R list.
  inline SEXP doc_to_list(rapidjson::Document& doc, bool& simplify, std::string by = "row" ) {
    
    int doc_len = doc.Size();
    Rcpp::Rcout << "doc_len: " << doc_len << std::endl;
    Rcpp::IntegerVector iv_dtypes( dtypes.begin(), dtypes.end() );
    Rcpp::Rcout << "dtypes: " << iv_dtypes << std::endl;
    
    Rcpp::List out(doc_len);
    
    bool return_df = true;
    names_map.clear();
    
//    Rcpp::IntegerVector iv_lengths( doc_len );  // possibly make an unordered_set??
    std::unordered_set<int> list_lengths;
    std::unordered_set<int> list_types;
    
    for(int i = 0; i < doc_len; ++i) {
      
      // Get current value
      switch(doc[i].GetType()) {
      
      // bool - false
      case 1: {
        out[i] = doc[i].GetBool();
        list_lengths.insert(1);
        list_types.insert( LGLSXP );
//        iv_lengths[i] = 1;
        return_df = false;
        break;
      }
        
      // bool - true
      case 2: {
        out[i] = doc[i].GetBool();
        list_lengths.insert(1);
        list_types.insert( LGLSXP );
//        iv_lengths[i] = 1;
        return_df = false;
        break;
      }
        
      // string
      case 5: {
        out[i] = doc[i].GetString();
        list_lengths.insert(1);
        list_types.insert( STRSXP );
//        iv_lengths[i] = 1;
        return_df = false;
        break;
      }
        
      // numeric
      case 6: {
        if(doc[i].IsDouble()) {
          // double
          out[i] = doc[i].GetDouble();
          list_types.insert( REALSXP );
        } else {
          // int
          out[i] = doc[i].GetInt();
          list_types.insert( INTSXP );
        }
        list_lengths.insert(1);
//        iv_lengths[i] = 1;
        return_df = false;
        break;
      }
      
      // null
      case 0: {
        out[i] = R_NA_VAL;
        list_lengths.insert(1);
        list_types.insert( LGLSXP );
//        iv_lengths[i] = 1;
        return_df = false;
        break;
      }
        
      // array
      case 4: {
        rapidjson::Value::Array curr_array = doc[i].GetArray();
        //iv_lengths[i] = curr_array.Size();
        list_lengths.insert( curr_array.Size() );
        Rcpp::Rcout << "doc_to_list :: parse_array()" << std::endl;
        out[i] = parse_array<rapidjson::Value::Array>(curr_array);
        list_types.insert( TYPEOF( out[i] ) );
        return_df = false;
        break;
      }
        
      // JSON object
      case 3: {
        const rapidjson::Value& temp_val = doc[i];
        //iv_lengths[i] = temp_val.Size();
        list_lengths.insert( temp_val.Size() );
        Rcpp::Rcout << "doc_to_list :: parse_value()" << std::endl;
        pv_list = parse_value(temp_val);
        out[i] = pv_list;
        list_types.insert( TYPEOF( out[i] ) );
        
        // If simplify is true and i is 0, record the data types of
        // each named element of doc[i] in unordered_map names_map.
        if(simplify && i == 0) {
          pv_len = pv_list.size();
          names = pv_list.attr("names");
          for(unsigned int n = 0; n < names.size(); ++n) {
            temp_name = Rcpp::as<std::string>(names[n]);
            names_map[temp_name] = TYPEOF(pv_list[n]);
          }
          break;
        }
        
        // // If simplify and return_df are both true, compare the data 
        // // types of each named element of doc[i] with the elements in 
        // // names_map. If the names do not align, or the data types of the 
        // // names do not align, set return_df to false.
        // if(simplify && return_df) {
        //   if(pv_list.size() != pv_len) {
        //     return_df = false;
        //     break;
        //   }
        //   names = pv_list.attr("names");
        //   for(unsigned int n = 0; n < names.size(); ++n) {
        //     temp_name = Rcpp::as<std::string>(names[n]);
        //     if(names_map.count(temp_name) == 0) {
        //       return_df = false;
        //       break;
        //     }
        //     if(names_map[temp_name] != TYPEOF(pv_list[n])) {
        //       return_df = false;
        //       break;
        //     }
        //   }
        // }
        
        break;
      }
        
      // some other data type not covered
      default: {
        Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool, array, and json");
      }
      }
    }
    
    if( simplify && list_lengths.size() == 1 ) { 
      // one length means a tablular structure / all list items are the same length
      
      // if dtype_len == 1 (only 1 data type)
      // 
      Rcpp::Rcout << "simplify table" << std::endl;
      
      if( dtypes.size() == 1 ) {   // one object
        
        Rcpp::Rcout << "single type of object to simplify" << std::endl;
        
        // if array, make a matrix (and all lengths are the same)
        int this_type = iv_dtypes[0];
        if( this_type == 4 ) {  
          Rcpp::Rcout << "matrix needed here" << std::endl;
          return jsonify::from_json::simplify_matrix( out, doc_len, list_lengths, list_types, by );
        } else if ( this_type == 3 ) 
          Rcpp::Rcout << "object needs simplifying" << std::endl;
        return jsonify::from_json::simplify_dataframe( out, doc_len );
      } else {
        Rcpp::Rcout << "multiple types of objects to simplify " << std::endl;
      }
    }
    
    // // If simplify and return_df are both true, convert List "out" to 
    // // a dataframe, with the names of "out" making up the df col headers.
    // if(simplify && return_df) {
    //   Rcpp::List df_out = Rcpp::List(pv_len);
    //   for(int i = 0; i < pv_len; ++i) {
    //     temp_name = names[i];
    //     switch(names_map[temp_name]) {
    //     case 10: {
    //       extract_lgl_vector(out);
    //       df_out[i] = df_out_lgl;
    //       break;
    //     }
    //     case 13: {
    //       extract_int_vector(out);
    //       df_out[i] = df_out_int;
    //       break;
    //     }
    //     case 14: {
    //       extract_dbl_vector(out);
    //       df_out[i] = df_out_dbl;
    //       break;
    //     }
    //     default: { // string, case 16
    //       extract_str_vector(out);
    //       df_out[i] = df_out_str;
    //       break;
    //     }
    //     }
    //   }
    //   
    //   df_out.attr("names") = names;
    //   df_out.attr("class") = "data.frame";
    //   df_out.attr("row.names") = Rcpp::seq(1, doc_len);
    //   
    //   return df_out;
    // }

    return out;
  }
  
  
  //' Parse JSON String
  //'
  //' Takes a JSON string as input, returns an R list of key-value pairs
  //'
  //' @param json const char, JSON string to be parsed. Coming from R, this
  //'  input should be a character vector of length 1.
  //' @export
  inline SEXP from_json(const char * json, bool& simplify) {
    rapidjson::Document doc;
    doc.Parse(json);
    
    // Make sure there were no parse errors
    if(doc.HasParseError()) {
      Rcpp::Rcerr << "parse error for json string: " << json << std::endl;
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
    
    // If input is not an array, pass doc through parse_document(), and return
    // the result.
    if(!doc.IsArray()) {
      return parse_document(doc);
    }
    
    // If input is an empty array, return NULL.
    if(doc.Size() == 0) {
      return R_NilValue;
    }
    
    // Get set of unique data types in doc.
    get_dtypes<rapidjson::Document>(doc, true);
    int dtype_len = dtypes.size();
    Rcpp::Rcout << "dtype_len: " << dtype_len << std::endl;

    // If dtype_len is greater than 2, return an R list of values.
    if(dtype_len > 2) {
      Rcpp::Rcout << "doc_to_list 1" << std::endl;
      return doc_to_list( doc, simplify );
    }

    // If dtype_len is 2 and 0 does not appear in dtypes, return an
    // R list of values.
    if(dtype_len == 2 && dtypes.find(0) == dtypes.end()) {
      Rcpp::Rcout << "doc_to_list 2" << std::endl;
      return doc_to_list( doc, simplify );
    }

    // If 3 or 4 is in dtypes, return an R list of values.
    if(dtypes.find(3) != dtypes.end() ||
       dtypes.find(4) != dtypes.end()) {
      Rcpp::Rcout << "doc_to_list 3" << std::endl;
      return doc_to_list( doc, simplify );
    }

    // Dump ints from dtypes to an std vector.
    std::vector<int> dtype_vect(dtypes.begin(), dtypes.end());
    int dt = dtype_vect[0];

    // If dtype_len is 1, return an R vector.
    if(dtype_len == 1) {
      return doc_to_vector(doc, dt);
    }

    // Else if dtype_len is 2 and 0 is in dtypes, return an R vector.
    if(dtype_len == 2) {
      if(dt == 0) {
        dt = dtype_vect[1];
      }
    }

    return doc_to_vector(doc, dt);
  }
  
  // Test array types
  // If it's an object, it's 'simplify' value will be a data.frame
  // if it's an array, this test finds the types inside the array
  inline Rcpp::IntegerVector test_dtypes( const char * json ) {
    
    rapidjson::Document doc;
    doc.Parse(json);
    
    if(!doc.IsArray()) {
      return Rcpp::IntegerVector(0); // if not an array it doesn't call get_dtypes
    }
    
    get_dtypes<rapidjson::Document>(doc);
    Rcpp::IntegerVector iv(dtypes.begin(), dtypes.end());
    return iv;
  }

} // namespace from_json
} // namespace jsonify

#endif
