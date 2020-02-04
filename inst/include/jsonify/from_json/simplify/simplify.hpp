#ifndef R_JSONIFY_FROM_JSON_SIMPLIFY_H
#define R_JSONIFY_FROM_JSON_SIMPLIFY_H

namespace jsonify {
namespace from_json {

  // takes a list, where each element is the same TYPE and SIZE and simplifies to
  // a vector
  // requires each list element ot be the same size
  // param n is the lenght of each list element
  inline SEXP simplify_vector( Rcpp::List& x, int& r_type, R_xlen_t n ) {
    
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
      
      return iv;
    }
    case REALSXP: {
      
      Rcpp::NumericVector nv( vec_length );
      for( i = 0; i < vec_length; counter++, i+= n ) {
        Rcpp::NumericVector this_vec = x[ counter ];
        std::copy( this_vec.begin(), this_vec.end(), nv.begin() + i );
      }
      
      return nv;
    }
    case VECSXP: {
      // list... can't simplify to a vector.
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

  inline SEXP array_to_vector(
      const rapidjson::Value::ConstArray& array, 
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
      case rapidjson::kFalseType: {
      out[i] = array[i].GetBool();
      r_type = LGLSXP > r_type ? LGLSXP : r_type;
      break;
    }
        
        // bool - true
      case rapidjson::kTrueType: {
        out[i] = array[i].GetBool();
        r_type = LGLSXP > r_type ? LGLSXP : r_type;
        break;
      }
        
        // string
      case rapidjson::kStringType: {
        out[i] = Rcpp::String(array[i].GetString());
        r_type = STRSXP > r_type ? STRSXP : r_type;
        break;
      }
        
        // numeric
      case rapidjson::kNumberType: {
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
      case rapidjson::kNullType: {
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

  inline SEXP simplify_matrix(
      Rcpp::List& out,
      R_xlen_t& n_col, 
      R_xlen_t& n_row,
      int& r_type
  ) {
    
    switch( r_type ) {
    case INTSXP: {
      
      // if( by == "col") {
      //   Rcpp::IntegerMatrix mat( n_col, n_row ); // swapped
      // 
      //   for(int i = 0; i < n_col; i++ ) {
      //     Rcpp::IntegerVector this_vec = out[i];
      //     mat( Rcpp::_, i ) = this_vec;
      //   }
      //   return mat;
      // } else {
      
      Rcpp::IntegerMatrix mat( n_row, n_col );
      R_xlen_t i;
      R_xlen_t j;
      
      for( i = 0; i < n_row; i++ ) {
        Rcpp::IntegerVector this_vec = out[i];
        for( j = 0; j < n_col; j++ ) {
          R_xlen_t this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
      // }
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
      R_xlen_t i;
      R_xlen_t j;
      
      for( i = 0; i < n_row; i++ ) {
        Rcpp::NumericVector this_vec = out[i];
        for( j = 0; j < n_col; j++ ) {
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
      R_xlen_t i;
      R_xlen_t j;
      
      for( i = 0; i < n_row; i++ ) {
        Rcpp::LogicalVector this_vec = out[i];
        for( j = 0; j < n_col; j++ ) {
          bool this_val = this_vec[j];
          mat( i, j ) = this_val;
        }
      }
      return mat;
      // }
    }
    case VECSXP: {
      // list / data.frame
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
      R_xlen_t i;
      R_xlen_t j;
      
      for( i = 0; i < n_row; i++ ) {
        Rcpp::StringVector this_vec = out[i];
        for( j = 0; j < n_col; j++ ) {
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
      Rcpp::List& array_of_array
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
      R_xlen_t n_col = *array_lengths.begin();  // only one sizez
      R_xlen_t n_row = n;
      
      return jsonify::from_json::simplify_matrix( array_of_array, n_col, n_row, r_type );
    } else {
      return array_of_array;
    }
  }

  // takes a list element and converts it to the correct type
  // only works with single-elements (vectors)
  inline void list_to_vector(
      Rcpp::List& columns,
      std::string& this_name,
      int& r_type,
      int& struct_type, // 1 vector, 2 matrix, 3 list
      bool fill_na
  ) {
    
    Rcpp::List lst = columns[ this_name.c_str() ];
    R_xlen_t n_rows = lst.size();
    R_xlen_t i;
    
    // TODO; is this needed?
    if( n_rows > 0 ) {
      
      // if struct_type == 2; the result is a matrix
      // need the dimensions...
      if( struct_type == 2 ) {
        // i.e., the entire list element is one matrix.
        // so n_rows remains
        // n_cols is the length of the first list element
        R_xlen_t n_cols = get_sexp_length( lst[0] );
        
        switch( r_type ) {
        case LGLSXP: {
          Rcpp::LogicalMatrix m( n_rows, n_cols );
          for( i = 0; i < n_rows; i++ ) {
            Rcpp::LogicalVector v = Rcpp::as< Rcpp::LogicalVector >( lst[i] );
            m( i, Rcpp::_ ) = v;
          }
          columns[ this_name ] = m;
          break;
        }
        case INTSXP: {
          Rcpp::IntegerMatrix m( n_rows, n_cols );
          for( i = 0; i < n_rows; i++ ) {
            Rcpp::IntegerVector v = Rcpp::as< Rcpp::IntegerVector >( lst[i] );
            m( i, Rcpp::_ ) = v;
          }
          columns[ this_name ] = m;
          break;
        }
        case REALSXP: {
          Rcpp::NumericMatrix m( n_rows, n_cols );
          for( i = 0; i < n_rows; i++ ) {
            Rcpp::NumericVector v = Rcpp::as< Rcpp::NumericVector >( lst[i] );
            m( i, Rcpp::_ ) = v;
          }
          columns[ this_name ] = m;
          break;
        }
        case STRSXP: {
          Rcpp::StringMatrix m( n_rows, n_cols );
          for( i = 0; i < n_rows; i++ ) {
            Rcpp::StringVector v = Rcpp::as< Rcpp::StringVector >( lst[i] );
            m( i, Rcpp::_ ) = v;
          }
          columns[ this_name ] = m;
          break;
        }
        default: {
          Rcpp::stop("jsonify - invalid matrix-column");
        }
        }
      }
      
      // if( struct_type == 3 ) {
      // // it's a list; can it be simplified to a data.frame?
      //   
      // }
      
      if( struct_type == 1 ) {
        
        switch( r_type ) {
        case LGLSXP: {
          Rcpp::LogicalVector lv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            if( Rf_isNull( lst[i] ) ) {
              lv[i] = NA_LOGICAL;
            } else {
              lv[i] = lst[i];
            }
          }
          columns[ this_name ] = lv;
          break;
        }
        case INTSXP: {
          Rcpp::IntegerVector iv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            if( Rf_isNull( lst[i] ) ) {
              iv[i] = NA_INTEGER;
            } else {
              iv[i] = lst[i];
            }
          }
          columns[ this_name ] = iv;
          break;
        }
        case REALSXP: {
          Rcpp::NumericVector nv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            if( Rf_isNull( lst[i] ) ) {
              nv[i] = NA_REAL;
            } else {
              nv[i] = lst[i];
            }
          }
          columns[ this_name ] = nv;
          break;
        }
        case STRSXP: {
          Rcpp::StringVector sv( n_rows );
          for( i = 0; i < n_rows; i++ ) {
            if( Rf_isNull( lst[i] ) ) {
              sv[i] = NA_STRING;
            } else {
              SEXP s = lst[i];
              
              Rcpp::StringVector x = Rcpp::as< Rcpp::StringVector >( s );
              sv[i] = x[0];
            }
          }
          columns[ this_name ] = sv;
          break;
        }
        case NILSXP: {
        if( !fill_na ) {
          // if we are filling with NAs, the column will already exist as NAs
          // (from append_new_column_fill_na)
          Rcpp::Nullable< Rcpp::List >lst_null( n_rows );
          columns[ this_name ] = lst_null;
        }
          break;
        }
        default: {
          Rcpp::stop("jsonify - vector-column not found");
        }
        }
      }
    } else {
      // nrows == 0; empty vector/list?
    }
  }

  inline SEXP simplify_dataframe_fill_na(
      Rcpp::List& out,
      R_xlen_t& doc_len
  ) {
    
    // the number of rows is equal to the number of list elements?
    // the number of columns is equal to the unique names
    R_xlen_t n_rows = out.size();
    R_xlen_t i, j;
    
    // initialise a new list for each column
    // keep track of it's type
    Rcpp::List columns;        // will grow when a new column name is found
    
    std::unordered_map< std::string, int > column_types;
    std::unordered_map< std::string, int > column_structs; // int : 1 == vector element, 2 == matrix, 3 == list;
    std::unordered_map< std::string, int > column_lengths;
    
    int struct_type;
    R_xlen_t sexp_length;
    int tp;
    int st;
    R_xlen_t ln;
    
    Rcpp::StringVector list_names;
    std::vector< std::string > column_names;
    bool new_column = true;
    
    
    for( i = 0; i < n_rows; i++ ) {
      // iterating list elements
      Rcpp::List this_list = out[i];

       if( !Rf_isNull( this_list.names() ) ) {
         list_names = this_list.names();
       }

      R_xlen_t list_size = this_list.size();
      
      if( list_names.size() != list_size || list_size == 0 ) {
       return out;
      }
      
      // Iterate over names??
      for( j = 0; j < list_size; j++ ) { 
        const char* this_name = list_names[j];
            
        // does this_name exist in the vector of names?
        if( std::find( column_names.begin(), column_names.end(), this_name ) != column_names.end() ) {
          // it exists in the vector
          new_column = false;
        } else {
          new_column = true;
          // add it to the vector
          column_names.push_back( this_name );
          append_new_column_fill_na( columns, this_name, n_rows );
        }

        SEXP this_elem = this_list[ this_name ]; 
        sexp_length = get_sexp_length( this_elem );
        
        int this_type = TYPEOF( this_elem );
        bool is_matrix = Rf_isMatrix( this_elem );
        
        
        if( sexp_length > 1 && this_type != VECSXP && !is_matrix ) {
          // the object is more than a scalar, but not a list or matrix
          struct_type = 3; // matrix
        } else if ( this_type == VECSXP || is_matrix) {
          // the object is a list
          struct_type = 3; // list
        } else { 
          struct_type = 1; // scalar-vector
        }
        
        tp = column_value( column_types, this_name );
        st = column_value( column_structs, this_name );
        ln = column_value( column_lengths, this_name );
        
        if( new_column ) { 
          column_types[ this_name ] = this_type;
          column_structs[ this_name ] = struct_type;
          column_lengths[ this_name ] = sexp_length;
        }

        if( i > 0 && st >= 0 ) {
          // onto the second row
          
          // if this struct_type is different to the previous one, make it a list;
          // OR, if it's going to be a matrix, but the type is different
          if( ( struct_type != st || sexp_length != ln ) || ( tp != this_type && struct_type == 2 ) ) {
            column_structs[ this_name ] = 3;
          }
          
          if( sexp_length > ln ) {
            column_lengths[ this_name ] = sexp_length;
          }
          
          if( this_type > tp ) {
            column_types[ this_name ] = this_type;
          }
          
        }
        
        // put the element in the correct column slot
        insert_column_value( columns, this_name, this_elem, i );
      } // for j
    }  // for i
    
    for( auto& it: column_types ) {
      
      std::string this_name = it.first;
      int r_type = it.second;
      struct_type = column_value( column_structs, this_name.c_str() );
      if( struct_type == 3 ) {
        // can it be a data.frame?
        Rcpp::List lst = columns[ this_name ];
        columns[ this_name ] = simplify_dataframe_fill_na( lst, doc_len );
      } else {
        list_to_vector( columns, this_name, r_type, struct_type, true );  // true: fill_na
      }
    }
    
    columns.attr("class") = "data.frame";
    columns.attr("row.names") = Rcpp::seq(1, n_rows);
    
    return columns;
  }

  // iff all the column lengths are the same, and > 1, the whole column can become a matrix
  // iff all the column lenghts are the same, and == 1, the whole column is a vector
  // iff any column lenghts are different, it's a list
  inline SEXP simplify_dataframe(
      Rcpp::List& out,
      R_xlen_t& doc_len
  ) {
    
    // the number of rows is equal to the number of list elements?
    // the number of columns is equal to the unique names
    R_xlen_t n_rows = out.size();
    R_xlen_t i, j;
    
    // initialise a new list for each column
    // keep track of it's type
    Rcpp::List columns;        // will grow when a new column name is found
    
    std::unordered_map< std::string, int > column_types;
    std::unordered_map< std::string, int > column_structs; // int : 1 == vector element, 2 == matrix, 3 == list;
    std::unordered_map< std::string, int > column_lengths;
    
    int struct_type;
    R_xlen_t sexp_length;
    int tp;
    int st;
    R_xlen_t ln;
    
    Rcpp::StringVector list_names;
    
    for( i = 0; i < n_rows; i++ ) {
      // iterating list elements
      Rcpp::List this_list = out[i];
      if( i == 0 ) {
        if( !Rf_isNull( this_list.names() ) ) {
          list_names = this_list.names();
        }
      }
      R_xlen_t list_size = this_list.size();
      
      if( list_names.size() != list_size || list_size == 0 ) {
        return out;
      }
      
      // Iterate over names??
      for( j = 0; j < list_size; j++ ) { 
        const char* this_name = list_names[j];
        Rcpp::StringVector these_names = this_list.names();
        R_xlen_t found_name = where_is( this_name, these_names );
        
        if( found_name == -1 ) {
          // can't simplify
          return out;
        }
        
        SEXP this_elem = this_list[ this_name ]; 
        sexp_length = get_sexp_length( this_elem );
        
        int this_type = TYPEOF( this_elem );
        bool is_matrix = Rf_isMatrix( this_elem );
        
        if( sexp_length > 1 && this_type != VECSXP && !is_matrix ) {
          // the object is more than a scalar, but not a list or matrix
          struct_type = 2; // matrix
        } else if ( ( i == 0 && this_type == VECSXP ) || is_matrix) {
          // the object is a list
          struct_type = 3; // list
        } else { 
          struct_type = 1; // scalar-vector
        }
        
        tp = column_value( column_types, this_name );
        st = column_value( column_structs, this_name );
        ln = column_value( column_lengths, this_name );
        
        //if( i == 0 && tp >= 0 ) {
          // on the first row, but the column already exists
          //return out;
        //}
        
        // only add column types if we're on the first 'row'
        if( i == 0 ) {
          column_types[ this_name ] = this_type;
          column_structs[ this_name ] = struct_type;
          column_lengths[ this_name ] = sexp_length;
          
          append_new_column( columns, this_name, n_rows );
        }
        
        if( tp == -1 && i > 0 ) {
          // can't simplify because new column names
          return out;
        }
        
        if( i > 0 && st >= 0 ) {
          // onto the second row
          
          // if this struct_type is different to the previous one, make it a list;
          // OR, if it's going to be a matrix, but the type is different
          if( ( struct_type != st || sexp_length != ln ) || ( tp != this_type && struct_type == 2 ) ) {
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
      
      std::string this_name = it.first;
      int r_type = it.second;
      struct_type = column_value( column_structs, this_name.c_str() );
      if( struct_type == 3 ) {
        // can it be a data.frame?
        Rcpp::List lst = columns[ this_name ];
        columns[ this_name ] = simplify_dataframe( lst, doc_len );
      } else {
        list_to_vector( columns, this_name, r_type, struct_type, false );  // false : fill_na
      }
    }
    
    //columns.attr("names") = names;
    columns.attr("class") = "data.frame";
    columns.attr("row.names") = Rcpp::seq(1, n_rows);
    
    return columns;
  }

} // from_json
} // jsonify

#endif