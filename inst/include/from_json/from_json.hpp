#ifndef R_JSONIFY_FROM_JSON_H
#define R_JSONIFY_FROM_JSON_H

#include <Rcpp.h>

#include "from_json/utils.hpp"

using namespace rapidjson;

namespace jsonify {
namespace from_json {

  // Will iterate over an object and return IntegerVector indicating the data
  // type of each element. Compatible with these rapidjson objects: Document,
  // Array, Value.
  template<typename T>
  Rcpp::IntegerVector get_dtypes(T& doc) {
    int doc_len = doc.Size();
    Rcpp::IntegerVector out(doc_len);
    int curr_dtype;
    for(int i = 0; i < doc_len; ++i) {
      curr_dtype = doc[i].GetType();
      if(curr_dtype == 2) {
        curr_dtype = 1;
      }
      if(curr_dtype == 6) {
        if(doc[i].IsDouble()) {
          curr_dtype = 8;
        } else {
          curr_dtype = 9;
        }
      }
      
      out[i] = curr_dtype;
    }
    
    return out;
  }
  
  
  // Dump objects from a rapidjson array to an R list.
  // Can handle key/value pair JSON, and JSON that is missing keys.
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
        out[i] = Rcpp::CharacterVector(NA_STRING);
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
  // Can handle key/value pair JSON, and JSON that is missing keys.
  template<typename T>
  SEXP parse_array(T& array) {
    int array_len = array.Size();
    
    // Check the data type of each value in the array.
    Rcpp::IntegerVector dtypes = get_dtypes<T>(array);
    Rcpp::IntegerVector u_dtypes = unique(dtypes).sort();
    int data_type = u_dtypes[0];
    
    // If the group of unique data types is longer than two, or if it's length is
    // two and 0 is NOT one of the two values (0 corresponds to null/NA), then
    // pass the input array along to array_to_list().
    if(u_dtypes.size() > 2) {
      return array_to_list<T>(array, array_len);
    }
    if(u_dtypes.size() == 2) {
      if(u_dtypes[0] == 0) {
        int dt1 = u_dtypes[1];
        if(dt1 == 4) {
          return array_to_list<T>(array, array_len);
        } else {
          data_type = dt1;
        }
      } else {
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
        out[i] = Rcpp::CharacterVector(NA_STRING);
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
        out[i] = Rcpp::CharacterVector(NA_STRING);
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
        out[i] = Rcpp::CharacterVector(NA_STRING);
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
    
    // If input in an array, first check the data type of each value in the
    // array.
    Rcpp::IntegerVector dtypes = get_dtypes<rapidjson::Document>(doc);
    Rcpp::IntegerVector u_dtypes = unique(dtypes).sort();
    
    // If all of the data types are simple and of the same type
    // (i.e. all strings, ints, double, or bool), then return a vector of values.
    if(u_dtypes.size() == 1) {
      int dt = u_dtypes[0];
      if(dt == 0 || dt == 1 || dt == 5 || dt == 8 || dt == 9) {
        return doc_to_vector(doc, dt);
      }
    } else if(u_dtypes.size() == 2) {
      if(u_dtypes[0] == 0) {
        int dt1 = u_dtypes[1];
        if(dt1 == 1 || dt1 == 5 || dt1 == 8 || dt1 == 9) {
          return doc_to_vector(doc, dt1);
        }
      }
    }
    
    // Otherwise, if there's a variety of data types, return a list of values.
    return doc_to_list(doc);
  }

} // namespace from_json
} // namespace jsonify

#endif
