#ifndef R_JSONIFY_FROM_JSON_H
#define R_JSONIFY_FROM_JSON_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]
#include "rapidjson/document.h"

using namespace rapidjson;

namespace jsonify {
namespace from_json {

  // Dump objects from a rapidjson array to an R list.
  Rcpp::List array_to_list(rapidjson::Value::ConstArray& array, int& array_len) {
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
        
      // array
      case 4: {
        int curr_array_len = array[i].Size();
        rapidjson::Value::ConstArray curr_array = array[i].GetArray();
        out[i] = array_to_list(curr_array, curr_array_len);
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
  
  
  // Parse an array object, return an SEXP that contains the objects from the
  // array.
  SEXP parse_array(rapidjson::Value::ConstArray& array) {
    bool list_out = false;
    int array_len = array.Size();
    int data_type = array[0].GetType();
    
    if(data_type == 2) {
      data_type = 1;
    }
    
    if(data_type == 6) {
      if(array[0].IsDouble()) {
        data_type = 8;
      } else {
        data_type = 9;
      }
    }

    // Check to see if the array has different data types, which means return
    // will be a list. Exception to this rule is for bool types...false has
    // data type of 1, and true has data type of 2. So, effectively, data types
    // 1 and 2 are identical (since they're both bool).
    int curr_dtype;
    for(int i = 1; i < array_len; ++i) {
      curr_dtype = array[i].GetType();
      if(curr_dtype == 2) {
        curr_dtype = 1;
      }
      if(curr_dtype == 6) {
        if(array[i].IsDouble()) {
          curr_dtype = 8;
        } else {
          curr_dtype = 9;
        }
      }
      if(curr_dtype != data_type) {
        list_out = true;
        break;
      }
    }
    
    // If the values in the input array are not all of the same data type,
    // return the array values as an R list.
    if(list_out) {
      return array_to_list(array, array_len);
    }
    
    // Get current value
    switch(data_type) {
    
    // bool
    case 1: {
      Rcpp::LogicalVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        out[i] = array[i].GetBool();
      }
      return out;
    }
      
    // string
    case 5: {
      Rcpp::CharacterVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        out[i] = array[i].GetString();
      }
      return out;
    }
      
    // double
    case 8: {
      Rcpp::NumericVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        out[i] = array[i].GetDouble();
      }
      return out;
    }
      // int
    case 9: {
      Rcpp::IntegerVector out(array_len);
      for(int i = 0; i < array_len; ++i) {
        out[i] = array[i].GetInt();
      }
      return out;
    }
      
    // array
    case 4: {
      Rcpp::List out(array_len);
      for(int i = 0; i < array_len; ++i) {
        rapidjson::Value::ConstArray curr_array = array[i].GetArray();
        out[i] = parse_array(curr_array);
      }
      return out;
    }
    }
    
    return R_NilValue;
  }
  
  
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
        
      // array
      case 4: {
        rapidjson::Value::ConstArray curr_array = itr->value.GetArray();
        out[i] = parse_array(curr_array);
        break;
      }
        
      // json
      case 3: {
        out[i] = parse_value(itr->value);
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
        
      // array
      case 4: {
        rapidjson::Value::ConstArray curr_array = itr->value.GetArray();
        out[i] = parse_array(curr_array);
        break;
      }
        
      // json
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
  
  // Parse JSON string.
  // Takes a JSON string as input, returns an R list of key-value pairs.
  Rcpp::List from_json(const char * json) {
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
    
    // Else if input is an array, pass each element of doc through parse_value(),
    // saving the results of each iteration in an Rcpp::List.
    int json_len = doc.Size();
    Rcpp::List out(json_len);
    
    for(int i = 0; i < json_len; ++i) {
      const rapidjson::Value& curr_val = doc[i];
      out[i] = parse_value(curr_val);
    }
    
    return out;
  }



} // namespace from_json
} // namespace jsonify

#endif
