#ifndef R_JSONIFY_FROM_JSON_H
#define R_JSONIFY_FROM_JSON_H

#include <Rcpp.h>

#include "from_json/utils.hpp"

using namespace rapidjson;

namespace jsonify {
namespace from_json {

  // Iterate over a rapidjson object and get the unique data types of each value.
  // Save unique data types as ints to unordered_set js_vals::dtypes.
  // Compatible with these rapidjson::Array and rapidjson::Value.
  template<typename T>
  void get_dtypes(T& doc, bool scalar_only = false) {
    // Clear all values from dtypes
    dtypes.clear();
    
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
  }
  
  
  // Dump objects from a rapidjson array to an R list.
  // Can handle JSON objects that have keys and those that do not have keys.
  template<typename T>
  Rcpp::List array_to_list(T& array, int& array_len) {
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
        out[i] = R_NA_STR;
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
  SEXP parse_array(T& array) {
    int array_len = array.Size();
    
    // Get set of unique data types of each value in the array.
    get_dtypes<T>(array);
    
    // If there's only one unique data type in the input array, or if the array
    // is made up a simple data type and nulls (simple being int, double,
    // string, bool), then return an R vector. Otherwise, return an R list.
    
    if(dtypes.size() > 2) {
      return array_to_list<T>(array, array_len);
    }
    
    int data_type;
    std::vector<int> dtype_vect(dtypes.begin(), dtypes.end());
    if(dtype_vect.size() == 2) {
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
    } else {
      // If dtype_vect size is only 1.
      data_type = dtype_vect[0];
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
      Rcpp::CharacterVector out(array_len, NA_STRING);
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
  Rcpp::List parse_value(const rapidjson::Value& val) {
    int json_len = val.Size();
    Rcpp::List out(json_len);
    Rcpp::CharacterVector names(json_len);
    
    int i = 0;
    for (rapidjson::Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr) {
      
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
        out[i] = R_NA_STR;
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
        out[i] = parse_value(curr_val);
        break;
      }
        
      // some other data type not covered
      default: {
        Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool");
      }
      }
      
      // Bump i
      i++;
    }
    
    out.attr("names") = names;
    return out;
  }
  
  
  // Parse rapidjson::Document object.
  Rcpp::List parse_document(rapidjson::Document& doc) {
    int json_len = doc.Size();
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
        out[i] = R_NA_STR;
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
        Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool");
      }
      }
      
      // Bump i
      i++;
    }
    
    out.attr("names") = names;
    return out;
  }
  
  
  // Parse rapidjson::Document object that contains "keyless" JSON data of the
  // same data type. Returns an R vector.
  SEXP doc_to_vector(rapidjson::Document& doc, int& dtype) {
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
      Rcpp::CharacterVector out(doc_len, NA_STRING);
      return out;
    }
    }
    
    return R_NilValue;
  }
  
  
  // Parse rapidjson::Document object that contains "keyless" JSON data that
  // contains a variety of data types. Returns an R list.
  Rcpp::List doc_to_list(rapidjson::Document& doc) {
    int doc_len = doc.Size();
    Rcpp::List out(doc_len);
    
    for(int i = 0; i < doc_len; ++i) {
      
      // Get current value
      switch(doc[i].GetType()) {
      
      // bool - false
      case 1: {
        out[i] = doc[i].GetBool();
        break;
      }
        
      // bool - true
      case 2: {
        out[i] = doc[i].GetBool();
        break;
      }
        
      // string
      case 5: {
        out[i] = doc[i].GetString();
        break;
      }
        
      // numeric
      case 6: {
        if(doc[i].IsDouble()) {
          // double
          out[i] = doc[i].GetDouble();
        } else {
          // int
          out[i] = doc[i].GetInt();
        }
        break;
      }
      
      // null
      case 0: {
        out[i] = R_NA_STR;
        break;
      }
        
      // array
      case 4: {
        rapidjson::Value::Array curr_array = doc[i].GetArray();
        out[i] = parse_array<rapidjson::Value::Array>(curr_array);
        break;
      }
        
      // JSON object
      case 3: {
        const rapidjson::Value& temp_val = doc[i];
        out[i] = parse_value(temp_val);
        break;
      }
        
      // some other data type not covered
      default: {
        Rcpp::stop("Uknown data type. Only able to parse int, double, string, bool");
      }
      }
    }
    
    return out;
  }
  
  
  //' Parse JSON String
  //'
  //' Takes a JSON string as input, returns an R list of key-value pairs
  //'
  //' @param json const char, JSON string to be parsed. Coming from R, this
  //'  input should be a character vector of length 1.
  //' @export
  // [[Rcpp::export]]
  SEXP from_json(const char * json) {
    rapidjson::Document doc;
    doc.Parse(json);
    
    // Make sure there were no parse errors
    if(doc.HasParseError()) {
      Rcpp::Rcerr << "parse error for json string: "<< json << std::endl;
      Rcpp::stop("json parse error");
    }
    
    // If input is not an array, pass doc through parse_document(), and return
    // the result.
    if(!doc.IsArray()) {
      return parse_document(doc);
    }
    
    // Get set of unique data types in doc.
    get_dtypes<rapidjson::Document>(doc, true);
    int dtype_len = dtypes.size();
    
    // If dtype_len is greater than 2, return an R list of values.
    if(dtype_len > 2) {
      return doc_to_list(doc);
    }
    
    // If dtype_len is 2 and 0 does not appear in dtypes, return an
    // R list of values.
    if(dtype_len == 2 && dtypes.find(0) == dtypes.end()) {
      return doc_to_list(doc);
    }
    
    // If 3 or 4 is in dtypes, return an R list of values.
    if(dtypes.find(3) != dtypes.end() ||
       dtypes.find(4) != dtypes.end()) {
      return doc_to_list(doc);
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

} // namespace from_json
} // namespace jsonify

#endif
