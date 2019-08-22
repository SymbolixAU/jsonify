#ifndef R_JSONIFY_FROM_JSON_UTILS_H
#define R_JSONIFY_FROM_JSON_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]
#include "rapidjson/document.h"

#define R_NA_VAL Rcpp::LogicalVector::create(NA_LOGICAL);

namespace jsonify {
namespace from_json {

  Rcpp::List parse_value( const rapidjson::Value& val, bool& simplify, std::string by );

  std::unordered_set<int> dtypes;
  std::unordered_map<std::string, int> names_map;
  int pv_len;
  std::string temp_name;
  std::vector<bool> df_out_lgl;
  std::vector<int> df_out_int;
  std::vector<double> df_out_dbl;
  std::vector<std::string> df_out_str;
  Rcpp::CharacterVector names;
  Rcpp::List pv_list;
  
  // Extract all logical values from each named element of a nested list.
  inline void extract_lgl_vector(Rcpp::List& x) {
    df_out_lgl.clear();
    for(unsigned int i = 0; i < x.size(); ++ i) {
      pv_list = x[i];
      df_out_lgl.push_back(pv_list[temp_name]);
    }
  }
  
  // Extract all int values from each named element of a nested list.
  inline void extract_int_vector(Rcpp::List& x) {
    df_out_int.clear();
    Rcpp::Rcout << "x.size(): " << x.size() << std::endl;
    for(unsigned int i = 0; i < x.size(); ++ i) {
      pv_list = x[i];
      
      Rcpp::Rcout << "TYPEOF( pv_list ) " << TYPEOF( pv_list ) << std::endl;
      Rcpp::Rcout << "temp_name: " << temp_name << std::endl;
      
      df_out_int.push_back(pv_list[temp_name]);
    }
  }
  
  // Extract all double values from each named element of a nested list.
  inline void extract_dbl_vector(Rcpp::List& x) {
    df_out_dbl.clear();
    for(unsigned int i = 0; i < x.size(); ++ i) {
      pv_list = x[i];
      df_out_dbl.push_back(pv_list[temp_name]);
    }
  }
  
  // Extract all string values from each named element of a nested list.
  inline void extract_str_vector(Rcpp::List& x) {
    df_out_str.clear();
    for(unsigned int i = 0; i < x.size(); ++ i) {
      pv_list = x[i];
      df_out_str.push_back(pv_list[temp_name]);
    }
  }
  
  // 
  inline SEXP simplify_vector( Rcpp::List& x ) {
    
  }
  
  
  SEXP simplify_matrix(
      Rcpp::List& out,
      int& doc_len, 
      std::unordered_set<int>& list_lengths,
      int& r_type,
      std::string by = "row"
      ) {
    int n_col = doc_len;
    //int n_row = iv_lengths[0]; // not happy wiht this
    std::unordered_set<int>::iterator it_lengths = list_lengths.begin();
    int n_row = *(it_lengths);
    
    switch( r_type ) {
    case INTSXP: {
      
      if( by == "col") {
      Rcpp::IntegerMatrix mat( n_row, n_col );
      
      for(int i = 0; i < n_col; i++ ) {
        Rcpp::IntegerVector this_vec = out[i];
        mat( Rcpp::_, i ) = this_vec;
      }
      return mat;
    } else {
      
      Rcpp::IntegerMatrix mat( n_col, n_row );
      for( int i = 0; i < n_col; i++ ) {
        Rcpp::IntegerVector this_vec = out[i];
        for( int j = 0; j < n_row; j++ ) {
          int this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    }
    }
    case REALSXP: {
      if( by == "col" ) {
      Rcpp::NumericMatrix mat( n_row, n_col );
      //if by == col
      for(int i = 0; i < n_col; i++ ) {
        Rcpp::NumericVector this_vec = out[i];
        mat( Rcpp::_, i ) = this_vec;
      }
      return mat;
    } else {
      
      Rcpp::NumericMatrix mat( n_col, n_row );
      for( int i = 0; i < n_col; i++ ) {
        Rcpp::NumericVector this_vec = out[i];
        for( int j = 0; j < n_row; j++ ) {
          double this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    }
    }
    case LGLSXP: {
      if( by == "col" ) {
      Rcpp::LogicalMatrix mat( n_row, n_col );
      //if by == col
      for(int i = 0; i < n_col; i++ ) {
        Rcpp::LogicalVector this_vec = out[i];
        mat( Rcpp::_, i ) = this_vec;
      }
      return mat;
    } else {
      
      Rcpp::LogicalMatrix mat( n_col, n_row );
      for( int i = 0; i < n_col; i++ ) {
        Rcpp::LogicalVector this_vec = out[i];
        for( int j = 0; j < n_row; j++ ) {
          bool this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    }
    }
    case VECSXP: {
      // list / data.frame
      Rcpp::Rcout << "VECSPX needs simplifying " << std::endl;
      return out;
    }
    default: {
      // string
      if( by == "col" ) {
      Rcpp::StringMatrix mat( n_row, n_col );
      //if by == col
      for(int i = 0; i < n_col; i++ ) {
        Rcpp::StringVector this_vec = out[i];
        mat( Rcpp::_, i ) = this_vec;
      }
      return mat;
    } else {
      
      Rcpp::StringMatrix mat( n_col, n_row );
      for( int i = 0; i < n_col; i++ ) {
        Rcpp::StringVector this_vec = out[i];
        for( int j = 0; j < n_row; j++ ) {
          Rcpp::String this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    }
    }
    }
    return Rcpp::LogicalMatrix(0); // never reaches?
  }
  
  SEXP simplify_dataframe(
    Rcpp::List& out,
    int& doc_len
  ) {
    
    Rcpp::Rcout << "simplify_dataframe()" << std::endl;
    
    // If simplify and return_df are both true, compare the data
    // types of each named element of doc[i] with the elements in
    // names_map. If the names do not align, or the data types of the
    // names do not align, set return_df to false.
    if(pv_list.size() != pv_len) {
      //return_df = false;
      //break;
      Rcpp::Rcout << "pv_list.size != pv_len " << std::endl;
      return out; // cant' simplify
    }
    names = pv_list.attr("names");
    for(unsigned int n = 0; n < names.size(); ++n) {
      temp_name = Rcpp::as<std::string>( names[n] );
      Rcpp::Rcout << "temp_name: " << temp_name << std::endl;
      Rcpp::Rcout << "tyep pv_list " << TYPEOF( pv_list[n] ) << std::endl;
      if(names_map.count(temp_name) == 0) {
        // return_df = false;
        // break;
        Rcpp::Rcout << "names_map.count(temp_name) == 0 " << std::endl;
        return out; // can't simplify
      }
      // TODO
      // is this check (TYPEOF( pv_list[n] ) != LGLSXP ) correct? 
      // if it's logical && is NULL value it can be coerced to numeric NA?
      if(names_map[temp_name] != TYPEOF( pv_list[n] ) && TYPEOF( pv_list[n] ) != LGLSXP  ) {
        // return_df = false;
        // break;
        Rcpp::Rcout << "names_map[temp_name] != TYPEOF(pv_list[n])" << std::endl;
        return out; // can't simplify
      }
    }

    Rcpp::Rcout << "constructing df_out" << std::endl;
    Rcpp::Rcout << "pv_len " << pv_len << std::endl;
    Rcpp::List df_out = Rcpp::List( pv_len );
    for(int i = 0; i < pv_len; ++i) {
      temp_name = names[i];
      Rcpp::Rcout << "temp_name: " << temp_name << std::endl;
      int temp_name_map = names_map[ temp_name ];
      Rcpp::Rcout << "temp_name_map: " << temp_name_map << std::endl;
      switch(names_map[temp_name]) {
      case LGLSXP: {
        extract_lgl_vector(out);
        df_out[i] = df_out_lgl;
        break;
      }
      case INTSXP: {
        extract_int_vector(out);
        df_out[i] = df_out_int;
        break;
      }
      case REALSXP: {
        extract_dbl_vector(out);
        df_out[i] = df_out_dbl;
        break;
      }
      case VECSXP: {
        Rcpp::stop("VECSXP needs simplifying??");
      }
      default: { // string, case 16
        extract_str_vector(out);
        df_out[i] = df_out_str;
        break;
      }
      }
    }
    
    df_out.attr("names") = names;
    df_out.attr("class") = "data.frame";
    df_out.attr("row.names") = Rcpp::seq(1, doc_len);

    return df_out;
  }
  
  SEXP simplify_list(
    Rcpp::List& out,
    int& doc_len,
    std::unordered_set< int >& list_types,
    std::unordered_set< int >& list_lengths,
    Rcpp::IntegerVector& iv_dtypes,
    std::string by = "row"
  ) {
    
    Rcpp::Rcout << "out size: " << out.size() << std::endl;
    Rcpp::Rcout << "list_lenghts.size(): " << list_lengths.size() << std::endl;
    
    if ( list_lengths.size() == 1 ) {
    
      // one length means a tablular structure / all list items are the same length
      std::unordered_set<int>::iterator it_types = list_types.begin();
      int r_type = *(it_types);
      Rcpp::Rcout << "r_type: " << r_type << std::endl;
      
      // if dtype_len == 1 (only 1 data type)
      Rcpp::Rcout << "simplify table" << std::endl;
      
      if( dtypes.size() == 1 ) {   // one object
        
        Rcpp::Rcout << "single type of object to simplify" << std::endl;
        
        // if array, make a matrix (and all lengths are the same)
        int this_type = iv_dtypes[0];
        
        Rcpp::Rcout << "this_type: " << this_type << std::endl;
        
        if( this_type == 4 && r_type != VECSXP ) {
          Rcpp::Rcout << "matrix (or list) needed here" << std::endl;
          return jsonify::from_json::simplify_matrix( out, doc_len, list_lengths, r_type, by );
          
        } else if ( this_type == 3 ) {
          Rcpp::Rcout << "object needs simplifying" << std::endl;
          return jsonify::from_json::simplify_dataframe( out, doc_len );
          
        } else {
          Rcpp::Rcout << "not simplifying " << std::endl;
          return out;
          
        }
      } else {
        Rcpp::Rcout << "multiple types of objects to simplify " << std::endl;
      }
    } else {
      Rcpp::Rcout << "list_lengths.size() != 1 " << std::endl;
    }
    return out;
  }
  
  
  // Get all unique names from a list object.
  // Equivalent to the R command:
  // unique(unlist(lapply(list_obj, names), recursive = FALSE, use.names = FALSE))
  SEXP get_col_headers(Rcpp::List& list_obj) {
    Rcpp::List curr_list;
    Rcpp::CharacterVector curr_names;
    std::string curr_str;
    std::unordered_set<std::string> names_set;
    std::vector<std::string> names;
    for(unsigned int i = 0; i < list_obj.size(); ++i) {
      if(TYPEOF(list_obj[i]) != VECSXP) {
        return R_NilValue;
      }
      curr_list = list_obj[i];
      curr_names = curr_list.attr("names");
      for(unsigned int n = 0; n < curr_names.size(); ++n) {
        curr_str = Rcpp::as<std::string>(curr_names[n]);
        if(names_set.find(curr_str) == names_set.end()) {
          names.push_back(curr_str);
          names_set.insert(curr_str);
        }
      }
    }
    
    return Rcpp::wrap(names);
  }
  
  // Tranpose a list.
  // Equivalent to the R command:
  // cols <- unique(unlist(lapply(list_obj, names)))
  // lapply(cols, function(x) lapply(list_obj, "[[", x))
  Rcpp::List transpose_list(Rcpp::List& x, Rcpp::CharacterVector& names) {
    int x_len = x.size();
    int names_len = names.size();
    Rcpp::List out(names_len);
    Rcpp::CharacterVector curr_names;
    Rcpp::String curr_name;
    Rcpp::List curr_list;
    
    for(int i = 0; i < names_len; ++i) {
      curr_name = names[i];
      Rcpp::List col(x_len);
      for(int j = 0; j < x_len; ++j) {
        curr_list = x[j];
        curr_names = curr_list.attr("names");
        for(unsigned int k = 0; k < curr_names.size(); ++k) {
          if(curr_names[k] == curr_name) {
            col[j] = curr_list[k];
            break;
          }
        }
      }
      
      out[i] = col;
    }
    
    return out;
  }
  
  // Convert all NULL elements in a list to NA.
  void null_to_na(Rcpp::List& x) {
    for(unsigned int i = 0; i < x.size(); ++i) {
      if(Rf_isNull(x[i])) {
        x[i] = R_NA_VAL;
      }
    }
  }

} // namespace from_json
} // namespace jsonify

#endif
