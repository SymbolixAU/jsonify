#ifndef R_JSONIFY_FROM_JSON_UTILS_H
#define R_JSONIFY_FROM_JSON_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]
#include "rapidjson/document.h"

#define R_NA_VAL Rcpp::LogicalVector::create(NA_LOGICAL);

namespace jsonify {
namespace from_json {

  inline int where_is(
      Rcpp::String to_find,
      Rcpp::StringVector& sv ) {
    int n = sv.size();
    int i;
    for( i = 0; i < n; i++ ) {
      if ( to_find == sv[i] ) {
        return i;
      }
    }
    return -1;
  }

  template <int RTYPE>
  inline R_xlen_t sexp_length(Rcpp::Vector<RTYPE> v) {
    return v.length();
  }
  
  inline R_xlen_t get_sexp_length( SEXP s ) {
    
    //Rcpp::Rcout << "get_sexp_length TYPEOF( s ) " << TYPEOF( s ) << std::endl;
    
    // if( Rf_isMatrix( s ) ) {
    //   Rcpp::stop("length of matrix needed");
    //   
    // }
    
    switch( TYPEOF(s) ) {
    case NILSXP: 
      return 0;
    case LGLSXP:
      return sexp_length< LGLSXP >( s );
    case REALSXP:
      return sexp_length< REALSXP >( s );
    case VECSXP:
      //return 2; // number bigger than 1 (vector)
      return sexp_length< VECSXP >( s );
    case INTSXP:
      return sexp_length< INTSXP >( s );
    case STRSXP:
      return sexp_length< STRSXP >( s );
    default: Rcpp::stop("jsonify - unknown vector type");
    }
    return 0;
  }
  
  // inline int get_sexp_column_type( SEXP s ) {
  // 
  //   if( Rf_isMatrix( s ) ) {
  //     return 2;
  //   }
  // 
  //   switch( TYPEOF(s) ) {
  //   case LGLSXP: {}
  //   case REALSXP: {}
  //   case INTSXP: {}
  //   case STRSXP: {
  //     return 1;
  //   }
  //   case VECSXP:
  //     // TODO: if a single element, make it a vector??
  //     return 3;
  //   default: Rcpp::stop("jsonify - unknown vector type");
  //   }
  //   return -1;
  // }
  
  // Rcpp::List parse_value( const rapidjson::Value& val, bool& simplify, std::string by );

  // std::unordered_set<int> dtypes;
  // std::unordered_map<std::string, int> names_map;
  // int pv_len;
  // std::string temp_name;
  // std::vector<bool> df_out_lgl;
  // std::vector<int> df_out_int;
  // std::vector<double> df_out_dbl;
  // std::vector<std::string> df_out_str;
  // Rcpp::CharacterVector names;
  // Rcpp::List pv_list;
  
  // inline bool name_exists(
  //   Rcpp::String& to_find,
  //   Rcpp::StringVector& sv ) {
  //   int n = sv.size();
  //   int i;
  //   for( i = 0; i < n; i++ ) {
  //     if ( to_find == sv[i] ) {
  //       return true;
  //     }
  //   }
  //   return false;
  // }
  

  
  // takes a list, where each element is the same TYPE and SIZE and simplifies to
  // a vector
  // requires each list element ot be the same size
  // param n is the lenght of each list element
  inline SEXP simplify_vector( Rcpp::List& x, int& r_type, R_xlen_t n ) {
    //Rcpp::Rcout << "simplify_vector" << std::endl;
    //R_xlen_t vec_length = n * r_type;
    R_xlen_t i;
    R_xlen_t x_size = x.size();
    R_xlen_t counter = 0;
    
    R_xlen_t vec_length = x_size* n;
    
    // each list element MUST be length n
    for( i = 0; i < x_size; i++ ) {
      if( Rf_length( x[i] ) != n ) {
        Rcpp::stop("jsonify - list elements different sizes");
      }
    }
    
    switch( r_type ) {
    case LGLSXP: {
      Rcpp::LogicalVector lv( vec_length );
      // iterate each list element and populate vector
      for( i = 0; i < vec_length; counter++, i+= n ) {
        Rcpp::LogicalVector this_vec = x[ counter ];
        std::copy( this_vec.begin(), this_vec.end(), lv.begin() + i );
      }
      return lv;
    }
    case INTSXP: {
      Rcpp::IntegerVector iv( vec_length );

      for( i = 0; i < vec_length; counter++, i+= n ) {
        Rcpp::IntegerVector this_vec = x[ counter ];
        std::copy( this_vec.begin(), this_vec.end(), iv.begin() + i );
      }
      // Rcpp::Rcout << "iv: " << iv << std::endl;
      return iv;
    }
    case REALSXP: {
      Rcpp::NumericVector nv( vec_length );
      for( i = 0; i < vec_length; counter++, i+= n ) {
        Rcpp::NumericVector this_vec = x[ counter ];
        std::copy( this_vec.begin(), this_vec.end(), nv.begin() + i );
      }
      // Rcpp::Rcout << "nv: " << nv << std::endl;
      return nv;
    }
    case VECSXP: {
      // list... can't simplify to a vector.
      // so return as-is?
      //Rcpp::stop("vector list element found");
      Rcpp::Rcout << "not simplifying vector - it has a list element " << std::endl;
      return x;
    }
    default: {
      Rcpp::StringVector sv( vec_length );
      for( i = 0; i < vec_length; counter++, i+= n ) {
        Rcpp::StringVector this_vec = x[ counter ];
        std::copy( this_vec.begin(), this_vec.end(), sv.begin() + i );
      }
      return sv;
    }
    }
    
    return x; // never reaches
  }
  
  
  SEXP simplify_matrix(
      Rcpp::List& out,
      int& n_col,  // n_columns
      int& n_row,
      int& r_type,
      std::string by = "row"
      ) {

    switch( r_type ) {
    case INTSXP: {
      
    if( by == "col") {
      Rcpp::IntegerMatrix mat( n_col, n_row ); // swapped

      for(int i = 0; i < n_col; i++ ) {
        Rcpp::IntegerVector this_vec = out[i];
        mat( Rcpp::_, i ) = this_vec;
      }
      return mat;
    } else {
      
      Rcpp::IntegerMatrix mat( n_row, n_col );
      for( int i = 0; i < n_row; i++ ) {
        Rcpp::IntegerVector this_vec = out[i];
        for( int j = 0; j < n_col; j++ ) {
          int this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
     }
    }
    case REALSXP: {
    // if( by == "col" ) {
    //   Rcpp::NumericMatrix mat( n_row, n_col );
    //   //if by == col
    //   for(int i = 0; i < n_col; i++ ) {
    //     Rcpp::NumericVector this_vec = out[i];
    //     mat( Rcpp::_, i ) = this_vec;
    //   }
    //   return mat;
    // } else {
      
      Rcpp::NumericMatrix mat( n_row, n_col );
      for( int i = 0; i < n_row; i++ ) {
        Rcpp::NumericVector this_vec = out[i];
        for( int j = 0; j < n_col; j++ ) {
          double this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    // }
    }
    case LGLSXP: {
    // if( by == "col" ) {
    //   Rcpp::LogicalMatrix mat( n_row, n_col );
    //   //if by == col
    //   for(int i = 0; i < n_col; i++ ) {
    //     Rcpp::LogicalVector this_vec = out[i];
    //     mat( Rcpp::_, i ) = this_vec;
    //   }
    //   return mat;
    // } else {
      
      Rcpp::LogicalMatrix mat( n_row, n_col );
      for( int i = 0; i < n_row; i++ ) {
        Rcpp::LogicalVector this_vec = out[i];
        for( int j = 0; j < n_col; j++ ) {
          bool this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    // }
    }
    case VECSXP: {
      // list / data.frame
      Rcpp::Rcout << "VECSPX needs simplifying " << std::endl;
      return out;
    }
    default: {
      // string
    // if( by == "col" ) {
    //   Rcpp::StringMatrix mat( n_row, n_col );
    //   //if by == col
    //   for(int i = 0; i < n_col; i++ ) {
    //     Rcpp::StringVector this_vec = out[i];
    //     mat( Rcpp::_, i ) = this_vec;
    //   }
    //   return mat;
    // } else {
      
      Rcpp::StringMatrix mat( n_row, n_col );
      for( int i = 0; i < n_row; i++ ) {
        Rcpp::StringVector this_vec = out[i];
        for( int j = 0; j < n_col; j++ ) {
          Rcpp::String this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
    // }
    }
    }
    return Rcpp::LogicalMatrix(0); // never reaches?
  }
  
  inline SEXP list_to_matrix(
    Rcpp::List& array_of_array,
    std::string by = "row"
  ) {
    R_xlen_t n = array_of_array.size();
    R_xlen_t j;
    std::unordered_set<int> array_lengths;
    std::unordered_set<int> array_types;
    bool can_be_matrix = true;
    for( j = 0; j < n; j++ ) {
      SEXP s = array_of_array[j];
      int this_type = TYPEOF( s );
      if( Rf_isMatrix( s ) || this_type == VECSXP ) {
        // can't be simplified
        can_be_matrix = false;
        break;
      } else {
        // keep track of sizes and types
        array_lengths.insert( get_sexp_length( s ) );
        array_types.insert( this_type );
        if( array_lengths.size() > 1 ) {
          // can't be simplified to matrix
          can_be_matrix = false;
          break;
        }
      }
    }
    if( can_be_matrix ) {
      Rcpp::IntegerVector arr_types( array_types.begin(), array_types.end() );
      int r_type = Rcpp::max( arr_types );
      int n_col = *array_lengths.begin();  // only one sizez
      int n_row = n;
      return simplify_matrix( array_of_array, n_col, n_row, r_type, by );
    } else {
      return array_of_array;
    }
  }
  
  // Convert all NULL elements in a list to NA.
  void null_to_na(Rcpp::List& x) {
    for(unsigned int i = 0; i < x.size(); ++i) {
      if(Rf_isNull(x[i])) {
        x[i] = R_NA_VAL;
      }
    }
  }
  
  // returns -1 if doens't exist
  // else the stored r_type
  inline int column_value(
      std::unordered_map< std::string, int >& column_map,
      const char* to_find
    ) {
    std::string str( to_find );
    std::unordered_map< std::string, int >::iterator it;
    it = column_map.find( str );
    
    if( it != column_map.end() ) {
      int res = it->second;
      return res;
    }
    return -1;
  }

  inline void insert_column_value(
    Rcpp::List& columns,
    const char* this_column,
    SEXP val,
    R_xlen_t& row_index
  ) {
    // Rcpp::Rcout << "updating " << this_column << " at row_index " << row_index << std::endl;
    Rcpp::List lst = columns[ this_column ];
    lst[ row_index ] = val;
    columns[ this_column ] = lst;
  }
  
  // here we don't actually care what the type is yet
  // as for now a 'column' is just a list
  inline void append_new_column(
    Rcpp::List& columns,
    const char* this_column,
    R_xlen_t n_rows
  ) {
    // Rcpp::Rcout << "creating new column " << this_column << " with " << n_rows << " rows" << std::endl;
    Rcpp::List new_column( n_rows );
    columns[ this_column ] = new_column;
    
    // switch( this_type ) {
    // case LGLSXP: {
    //   Rcpp::LogicalVector lv( n_rows, NA_LOGICAL );
    //   columns[ this_column ] = lv;
    //   break;
    // }
    // case INTSXP: {
    //   Rcpp::IntegerVector iv( n_rows, NA_INTEGER );
    //   columns[ this_column ] = iv;
    //   break;
    // }
    // case REALSXP: {
    //   Rcpp::NumericVector nv( n_rows, NA_REAL );
    //   columns[ this_column ] = nv;
    //   break;
    // }
    // case STRSXP: {
    //   Rcpp::StringVector sv( n_rows, Rcpp::StringVector::get_na() );
    //   columns[ this_column ] = sv;
    //   break;
    // }
    // }
  }
  
  // takes a list element and converts it to the correct type
  // only works with single-elements (vectors)
  inline void list_to_vector(
    Rcpp::List& columns,
    std::string& this_name,
    int& r_type,
    int& struct_type // 1 vector, 2 matrix, 3 list
  ) {
    
    Rcpp::List lst = columns[ this_name.c_str() ];
    R_xlen_t n_rows = lst.size();
    R_xlen_t i;
    
    // TODO; is this needed?
    if( n_rows > 0 ) {
    
      // if struct_type == 2; the result is a matrix
      // need the dimensions...
      if( struct_type == 2 ) {
        Rcpp::Rcout << "need to implement matrix column" << std::endl;
        // but only IFF n-row is 1 for each element, right?
        
        // i.e., the entire list element is one matrix.
        // so n_rows remains
        // n_cols is the length of the first list element
        R_xlen_t n_cols = get_sexp_length( lst[0] );
        
        Rcpp::Rcout << "with n_cols : " << n_cols << std::endl;
        
        //Rcpp::Matrix mat( n_row, n_col );
        
        //Rcpp::stop("need to implement matrix column");
        switch( r_type ) {
        case LGLSXP: {
  
          break;
        }
        case INTSXP: {
  
          break;
        }
        case REALSXP: {
  
          break;
        }
        case STRSXP: {
  
          break;
        }
        default: {
          Rcpp::stop("jsonify - invalid matrix-column");
        }
        }
      }
      
      if( struct_type == 3 ) {
        // Rcpp::Rcout << "need to implement list column" << std::endl;
        // TODO:
        // collapse each list element into a single list object
        // Rcpp::List out( n_rows );
        // for( i = 0; i < n_rows; i++ ) {
        //   Rcpp::List inner_lst = lst[i];
        //   out[i] = inner_lst[0];
        // }
        // columns[ this_name ] = out;
      }
      
      if( struct_type == 1 ) {
        
        switch( r_type ) {
        case LGLSXP: {
          Rcpp::LogicalVector lv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            lv[i] = lst[i];
          }
          columns[ this_name ] = lv;
          break;
        }
        case INTSXP: {
          Rcpp::IntegerVector iv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            iv[i] = lst[i];
          }
          columns[ this_name ] = iv;
          break;
        }
        case REALSXP: {
          Rcpp::NumericVector nv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            nv[i] = lst[i];
          }
          columns[ this_name ] = nv;
          break;
        }
        case STRSXP: {
          Rcpp::StringVector sv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            SEXP s = lst[i];
            // Rcpp::Rcout << "TYPEOF(s) " << TYPEOF( s ) << std::endl;
            Rcpp::StringVector x = Rcpp::as< Rcpp::StringVector >( s );
            //Rcpp::Rcout << "sv: " << sv << std::endl;
            sv[i] = x[0];
          }
          // Rcpp::Rcout << "setting as a stringVector " << std::endl;
          columns[ this_name ] = sv;
          break;
        }
        default: {
          Rcpp::stop("jsonify - vector-column not found");
        }
        }
      }
    }
  }
  
  // TODO
  // new simplify_dataframe()
  // which takes a list and works out on-the-fly the data types and column names?
  // where it iterates over the list; 
  // if it finds a column it hasn't seen before
  // it fills the previous values with NA, then inserts the correct value
  inline SEXP simplify_dataframe(
    Rcpp::List& out,
    int& doc_len
  ) {
    // Rcpp::Rcout << "simplify_dataframe()" << std::endl;
    
    // the number of rows is equal to the number of list elements?
    // the number of columns is equal to the unique names
    R_xlen_t n_rows = out.size();
    R_xlen_t n_col;
    R_xlen_t i, j;
    
    // initialise a new list for each column
    // keep track of it's type
    Rcpp::List columns;        // will grow when a new column name is found
    // each element 
    
    // need functions
    // - vector to list (each vector element becomes a list element)
    // - create a new... 
    
    // operate by row or column?
    
    // every column starts as a list
    // then keep track of how simple they can go
    // 
    std::unordered_map< std::string, int > column_types;
    std::unordered_map< std::string, int > column_structs; // int : 1 == vector element, 2 == matrix, 3 == list;
    std::unordered_map< std::string, int > column_lengths;
    
    int struct_type;
    int sexp_length;
    int tp;
    int st;
    int ln;
    
    // Rcpp::Rcout << "n_rows : " << n_rows << std::endl;
    Rcpp::StringVector list_names;
    
    for( i = 0; i < n_rows; i++ ) {
      // iterating list elements
      Rcpp::List this_list = out[i];
      if( i == 0 ) {
        list_names = this_list.names();
      }
      R_xlen_t list_size = this_list.size();
      // Rcpp::Rcout << "n_cols: " << list_size << std::endl;
      //Rcpp::StringVector list_names = this_list.names();
      // Rcpp::Rcout << "names: " << list_names << std::endl;

      if( list_names.size() != list_size ) {
        return out;
      }
      
      // Iterate over names??
      for( j = 0; j < list_size; j++ ) { 
        const char* this_name = list_names[j];
        // Rcpp::Rcout << "this_name: " << this_name << std::endl;
        
        //Rcpp::String temp_s = this_name;
        Rcpp::StringVector these_names = this_list.names();
        int found_name = where_is( this_name, these_names );
        
        if( found_name == -1 ) {
          // can't simplify
          return out;
        }
        
        SEXP this_elem = this_list[ this_name ]; 
        sexp_length = get_sexp_length( this_elem );
        //struct_type = get_sexp_column_type( this_elem );
          
        int this_type = TYPEOF( this_elem );
        bool is_matrix = Rf_isMatrix( this_elem );
        // Rcpp::Rcout << "this_type: " << this_type << std::endl;
        // Rcpp::Rcout << "sexp_length: " << sexp_length << std::endl;
        // Rcpp::Rcout << "is_matrix: " << is_matrix << std::endl;
        
        if( sexp_length > 1 && this_type != VECSXP && !is_matrix ) {
          // the object is more than a scalar, but not a list or matrix
          struct_type = 2; // matrix
        } else if ( ( i == 0 && this_type == VECSXP ) || is_matrix) {
          // the object is a list
          struct_type = 3; // list
        } else { 
          struct_type = 1; // scalar-vector
        }
        
        // Rcpp::Rcout << "struct_type: " << struct_type << std::endl;
        
        tp = column_value( column_types, this_name );
        st = column_value( column_structs, this_name );
        ln = column_value( column_lengths, this_name );
        // Rcpp::Rcout << "column_type: " << tp << std::endl;
        // Rcpp::Rcout << "column_struct: " << st << std::endl;
        
        if( i == 0 && tp >= 0 ) {
          // on the first row, but the column already exists
          return out;
        }
        
        // only add column types if we're on the first 'row'
        if( i == 0 ) {
          column_types[ this_name ] = this_type;
          column_structs[ this_name ] = struct_type;
          column_lengths[ this_name ] = sexp_length;
          //column_types.insert({ this_name, this_type });
          //column_structs.insert({ this_name, struct_type });
          append_new_column( columns, this_name, n_rows );
        }
        
        if( tp == -1 && i > 0 ) {
          Rcpp::Rcout << "can't simplify because new column names" << std::endl;
          return out;
        }
        
        if( i > 0 && st >= 0 ) {
          // onto the second row
          
          // if this struct_type is different to the previous one, make it a list;
          if( struct_type != st || sexp_length != ln ) {
            column_structs[ this_name ] = 3;
          }
          
          if( this_type > tp ) {
            column_types[ this_name ] = this_type;
          }
          
        }
        
        // put the element in the correct column slot
        insert_column_value( columns, this_name, this_elem, i );
      }
    }
    
    for( auto& it: column_types ) {
      // Rcpp::Rcout << "list_to_vector loop" << std::endl;
      std::string this_name = it.first;
      int r_type = it.second;
      struct_type = column_value( column_structs, this_name.c_str() );
      // Rcpp::Rcout << "this_name " << this_name << " r_type " << r_type << " struct_type " << struct_type << std::endl;
      list_to_vector( columns, this_name, r_type, struct_type );
     }
    
     //columns.attr("names") = names;
     columns.attr("class") = "data.frame";
     columns.attr("row.names") = Rcpp::seq(1, n_rows);
     
     return columns;
    
    
    // If simplify and return_df are both true, compare the data
    // types of each named element of doc[i] with the elements in
    // names_map. If the names do not align, or the data types of the
    // names do not align, set return_df to false.
    // if(pv_list.size() != pv_len) {
    //   //return_df = false;
    //   //break;
    //   Rcpp::Rcout << "pv_list.size != pv_len " << std::endl;
    //   return out; // cant' simplify
    // }
    // names = pv_list.attr("names");
    
    // for(unsigned int n = 0; n < names.size(); ++n) {
    //   temp_name = Rcpp::as< std::string >( names[n] );
    //   Rcpp::Rcout << "temp_name: " << temp_name << std::endl;
    //   Rcpp::Rcout << "type pv_list " << TYPEOF( pv_list[n] ) << std::endl;
    //   // if(names_map.count(temp_name) == 0) {
    //   //   // return_df = false;
    //   //   // break;
    //   //   Rcpp::Rcout << "names_map.count(temp_name) == 0 " << std::endl;
    //   //   return out; // can't simplify
    //   // }
    //   // TODO
    //   // is this check (TYPEOF( pv_list[n] ) != LGLSXP ) correct? 
    //   // if it's logical && is NULL value it can be coerced to numeric NA?
    //   // if(names_map[temp_name] != TYPEOF( pv_list[n] ) && TYPEOF( pv_list[n] ) != LGLSXP  ) {
    //   //   // return_df = false;
    //   //   // break;
    //   //   Rcpp::Rcout << "names_map[temp_name] != TYPEOF(pv_list[n])" << std::endl;
    //   //   return out; // can't simplify
    //   // }
    // }

    //Rcpp::Rcout << "constructing df_out" << std::endl;
    
    std::unordered_set< std::string > df_names;
    //R_xlen_t n_rows = out.size();
    //R_xlen_t i;
    // for( i = 0; i < n; i++ ) {
    //   //SEXP this_elem = out[i];
    //   //Rcpp::Rcout << "this_elem type :" << TYPEOF( this_elem ) << std::endl;
    //   // std::string this_name = 
    //   // df_names.insert( )
    // }
    
    // Rcpp::StringVector list_names = out.attr("names");
    // Rcpp::Rcout << "list_names " << list_names << std::endl;
    
    //Rcpp::Rcout << "pv_len " << pv_len << std::endl;
    //Rcpp::List df_out = Rcpp::List( pv_len );
    
    return out;
    
    // for(i = 0; i < pv_len; ++i) {
    //   temp_name = names[i];
    //   Rcpp::Rcout << "temp_name: " << temp_name << std::endl;
    //   int temp_name_map = names_map[ temp_name ];
    //   Rcpp::Rcout << "temp_name_map: " << temp_name_map << std::endl;
    //   
    //   switch( names_map[temp_name] ) {
    //   case LGLSXP: {
    //     Rcpp::Rcout << "getting lgl vector " << std::endl;
    //     df_out[i] = extract_lgl_vector(out);
    //     //df_out[i] = df_out_lgl;
    //     break;
    //   }
    //   case INTSXP: {
    //     Rcpp::Rcout << "getting int vector " << std::endl;
    //     df_out[i] = extract_int_vector(out);
    //     //df_out[i] = df_out_int;
    //     break;
    //   }
    //   case REALSXP: {
    //     Rcpp::Rcout << "getting dbl vector " << std::endl;
    //     df_out[i] = extract_dbl_vector(out);
    //     //df_out[i] = df_out_dbl;
    //     break;
    //   }
    //   case VECSXP: {
    //     Rcpp::stop("VECSXP needs simplifying??");
    //   }
    //   default: { // string, case 16
    //     Rcpp::Rcout << "getting str vector" << std::endl;
    //     df_out[i] = extract_str_vector(out);
    //     //df_out[i] = df_out_str;
    //     break;
    //   }
    //   }
    // }
    // 
    // df_out.attr("names") = names;
    // df_out.attr("class") = "data.frame";
    // df_out.attr("row.names") = Rcpp::seq(1, doc_len);
    // 
    // return df_out;
  }
  
  // SEXP simplify_list(
  //   Rcpp::List& out,
  //   int& doc_len,
  //   std::unordered_set< int >& list_types,
  //   std::unordered_set< int >& list_lengths,
  //   Rcpp::IntegerVector& iv_dtypes,
  //   std::string by = "row"
  // ) {
  //   
  //   Rcpp::Rcout << "out size: " << out.size() << std::endl;
  //   Rcpp::Rcout << "list_lenghts.size(): " << list_lengths.size() << std::endl;
  //   Rcpp::Rcout << "doc_len: " << doc_len << std::endl;
  //   
  //   Rcpp::IntegerVector iv_list_types( list_types.begin(), list_types.end() );
  // 
  //   if ( list_lengths.size() == 1 ) {
  //     
  //     int list_length = *list_lengths.begin();
  //   
  //     // one length means a tablular structure / all list items are the same length
  //     // if there is more than one list_types (r-type), make them the 'highest' value
  //     int r_type = Rcpp::max( iv_list_types );
  //     Rcpp::Rcout << "r_types: " << iv_list_types << std::endl;
  //     Rcpp::Rcout << "r_type: " << r_type << std::endl;
  //     Rcpp::Rcout << "d_types: " << iv_dtypes << std::endl;
  //     
  //     
  //     // if dtype_len == 1 (only 1 data type)
  //     Rcpp::Rcout << "simplify table" << std::endl;
  //     
  //     if( iv_dtypes.size() == 1 ) {   // one data type  (( dtypes or iv_dtypes ?? ))
  //       
  //       Rcpp::Rcout << "single type of object to simplify" << std::endl;
  //       
  //       // if array, make a matrix (and all lengths are the same)
  //       int this_type = iv_dtypes[0];
  //       
  //       Rcpp::Rcout << "this_type: " << this_type << std::endl;
  //       
  //       if( this_type == 4 && r_type != VECSXP ) { // array && not List -> goes to matrix
  //         Rcpp::Rcout << "matrix (or list) needed here" << std::endl;
  //         return jsonify::from_json::simplify_matrix( out, doc_len, list_length, r_type, by );
  //         
  //       } else if ( this_type == 3 ) { // object
  //         Rcpp::Rcout << "object needs simplifying" << std::endl;
  //         return jsonify::from_json::simplify_dataframe( out, doc_len );
  //         
  //       //} else if (this_type == 4 && r_type == VECSXP ) {  // array && List
  //         // lst_lengths == 1 means each list element is the same length
  //         // and it's a list object
  //         
  //         //Rcpp::Rcout << "simplify list" << std::endl;
  //         //return jsonify::from_json::simplify_vector( out );
  //         // the the size of each element, and the max r_type;
  //         // int list_r_type = 0;
  //         // R_xlen_t n = Rf_length( out[0] );  // only need one element; all the same size
  //         // for( unsigned int i = 0; i < out.size(); i++ ) {
  //         //   int this_type = TYPEOF( out[i] );
  //         //   Rcpp::Rcout << "this_type: " << this_type << std::endl;
  //         //   list_r_type = this_type > list_r_type ? this_type : list_r_type;
  //         // }
  //         // return jsonify::from_json::simplify_vector( out, list_r_type, n);
  //         
  //       } else {
  //         Rcpp::Rcout << "not simplifying " << std::endl;
  //         return out;
  //         
  //       }
  //     } else {
  //       Rcpp::Rcout << "multiple types of objects to simplify " << std::endl;
  //       // each element is the same length.??
  //       // 
  //       return jsonify::from_json::simplify_vector( out, r_type, list_length );
  //     }
  //     
  //   } else {
  //     Rcpp::Rcout << "list_lengths.size() != 1 " << std::endl;
  //   }
  //   return out;
  // }

} // namespace from_json
} // namespace jsonify

#endif
