#ifndef R_JSONIFY_WRITERS_H
#define R_JSONIFY_WRITERS_H

#include <Rcpp.h>
//#include "listwriter.hpp"

namespace jsonify {
namespace writers {

  template <typename Writer>
  inline void write_value( Writer& writer, const char* value ) {
    writer.String( value );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, int& value ) {
    writer.Int( value );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, double& value ) {
    writer.Double( value );
  }
  
  template< typename Writer> 
  inline void write_value( Writer& writer, bool& value ) {
    writer.Bool( value );
  }
  
  template< typename Writer>
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv ) {
    writer.StartArray();
    for ( int i = 0; i < nv.size(); i++ ) {
      write_value( writer, nv[i] );
    }
    writer.EndArray();
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv ) {
    writer.StartArray();
    for ( int i = 0; i < iv.size(); i++ ) {
      write_value( writer, iv[i] );
    }
    writer.EndArray();
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::StringVector& sv ) {
    writer.StartArray();
    for ( int i = 0; i < sv.size(); i++ ) {
      write_value( writer, sv[i] );
    }
    writer.EndArray();
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv ) {
    writer.StartArray();
    for ( int i = 0; i < lv.size(); i++ ) {
      write_value( writer, lv[i] );
    }
    writer.EndArray();
  }

  template< typename Writer>
  inline void write_value( Writer& writer, SEXP list_element ) {
    
    switch( TYPEOF( list_element ) ) {
    case VECSXP: {
      //Rcpp::Rcout << "list element list " << std::endl;
      Rcpp::List lst = Rcpp::as< Rcpp::List >( list_element );
      int n = lst.size();
      
      Rcpp::IntegerVector int_names = Rcpp::seq(1, lst.size());
      Rcpp::CharacterVector list_names = Rcpp::as< Rcpp::CharacterVector >( int_names );
      bool has_names = Rf_isNull(lst.names()) ? false : true;
      
      if ( has_names ) {
        Rcpp::CharacterVector temp_names = lst.names();
        //list_names = lst.names();
        for( int i = 0; i < n; i++ ) {
          list_names[i] = temp_names[i] == "" ? list_names[i] : temp_names[i];
        }
      }
      //Rcpp::Rcout << "list names: " << list_names << std::endl;
      // IF NO NAMES, it's an array of arrays
      // if named, it's an object, some of which are named 
      
      if ( n > 1 ) {
        if ( has_names ) {
          writer.StartObject();
        } else {
          writer.StartArray();
        }
      }
      for ( int i = 0; i < n; i++ ) {
        SEXP recursive_list = lst[ i ];
        if ( has_names ) {
          const char *s = list_names[ i ];
          write_value( writer, s );
        }
        write_value( writer, recursive_list );
      }
      if ( n > 1 ) {
        if ( has_names ) {
          writer.EndObject();
        } else {
          writer.EndArray();
        }
      }
      // list
      break;
    }
    case REALSXP: {
      //Rcpp::Rcout << "list element REAL " << std::endl;
      // real
      // IF n_elemnts > 0; vector : else value
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_element );
      int n = nv.size();
      if ( n > 0 ) {
        write_value( writer, nv );
      } else {
        write_value( writer, nv[0] );
      }
      break;
    }
    case INTSXP: { 
      //Rcpp::Rcout << "list element INT " << std::endl;
      // int
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_element );
      int n = iv.size();
      if ( n > 0 ) {
        write_value( writer, iv );
      } else {
        write_value( writer, iv[0] );
      }
      break;
    }
    case LGLSXP: {
      //Rcpp::Rcout << "list element LGL " << std::endl;
      // logical
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
      int n = lv.size();
      if ( n > 0 ) {
        write_value( writer, lv );
      } else {
        write_value( writer, lv[0] );
      }
      break;
    }
    default: {
      //Rcpp::Rcout << "list element default " << std::endl;
      // string
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( list_element );
      int n = sv.size();
      if ( n > 0 ) {
        write_value( writer, sv );
      } else {
        const char *s = sv[0];
        write_value( writer, s );
      }
      break;
    }
    }
  }

  template <typename Writer>
  inline void should_start_array( Writer& writer, int n_elements ) {
    if( n_elements > 1 ) {
      writer.StartArray();
    }
  }
  
  template <typename Writer>
  inline void should_end_array( Writer& writer, int n_elements ) {
    if( n_elements > 1 ) {
      writer.EndArray();
    }
  }
  
  // template <typename Writer>
  // inline void write_list( Writer& writer, Rcpp::List& lst, int row, int& lst_counter) {
  //   
  //   SEXP list_vec = lst[ row ];
  //   
  //   switch( TYPEOF( list_vec ) ) {
  //   case VECSXP: {
  //     Rcpp::Rcout << "list vecsxp" << std::endl;
  //     Rcpp::List lst = Rcpp::as< Rcpp::List >( list_vec);
  //     //Rcpp::List lst2 = lst[ lst_counter ];
  //     //write_list( writer, lst2, 0, lst_counter);
  //     //write_value( writer, lst, 0, lst_counter );
  //     //lst_counter++;
  //     break;
  //   }
  //   case REALSXP: {
  //     Rcpp::Rcout << "real vecsxp" << std::endl;
  //     Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_vec );
  //     Rcpp::Rcout << nv.size() << std::endl;
  //     for (int i = 0; i < nv.size(); i++) {
  //       write_value( writer, nv[i] );
  //     }
  //     break;
  //   }
  //   case INTSXP: {
  //     Rcpp::Rcout << "int vecsxp" << std::endl;
  //     Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_vec );
  //     Rcpp::Rcout << iv.size() << std::endl;
  //     for (int i = 0; i < iv.size(); i++) {
  //       write_value( writer, iv[i] );
  //     }
  //     break;
  //   }
  //   case LGLSXP: {
  //     Rcpp::Rcout << "lgl vecsxp" << std::endl;
  //     Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_vec );
  //     Rcpp::Rcout << lv.size() << std::endl;
  //     for (int i = 0; i < lv.size(); i++) {
  //       write_value( writer, lv[i] );
  //     }
  //     break;
  //   }
  //   default: {
  //     Rcpp::Rcout << "str vecsxp" << std::endl;
  //     Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( list_vec );
  //     Rcpp::Rcout << sv.size() << std::endl;
  //     for (int i = 0; i < sv.size(); i++) {
  //       write_value( writer, sv[i] );
  //     }
  //     break;
  //   }
  //   }
  // }

  // TODO
  // - If the row of a data.frame is a vector, ned to outptu all elements in ARRAY
  // - IFF the vector (REALSXP/INTSXP, etc) cases have been 'recursed' into, they need to 
  // - be iterated over into an array
  // template <typename Writer>
  // inline void write_value( Writer& writer, SEXP& this_vec ) {

    //int list_counter = 0;
    
    // switch( TYPEOF( this_vec ) ) {
    // case VECSXP: {
    //   //Rcpp::Rcout << "VECSXP" << std::endl;
    //   int list_counter = 0;
    //   Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
    //   lst = lst[ row ];
    //   Rcpp::Rcout << "list size: " << lst.size() << std::endl;
    //   //row = list_counter;
    //   // for (Rcpp::List::iterator it = lst.begin(); it != lst.end(); it++) {
    //   //   SEXP recursive_vec = *it;
    //   //   write_value( writer, recursive_vec, row);
    //   //   list_counter++;
    //   // }
    //   // if( lst.size() > 0 ) {
    //   //   writer.StartObject();
    //   // }
    //   for (int i = 0; i < lst.size(); i++) {
    //     //writer.StartArray();
    //     writer.StartObject();
    //     SEXP recursive_vec = lst[ i ];
    //     write_value( writer, recursive_vec );
    //     writer.EndObject();
    //     //writer.EndArray();
    //   }
    //   // if( lst.size() > 0 ) {
    //   //   writer.EndObject();
    //   // }
    //   
    //   //write_value( writer, recursive_vec, 0);
    //   
    //   //write_list( writer, lst, row, list_counter );
    //   //list_counter++;
    //   // The list is the entire column. 
    //   // I Need to extract the column
    //   
    //   break;
    // }
    // case REALSXP: {
    //   Rcpp::Rcout << "REALSXP" << std::endl;
    //   Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
    //   Rcpp::Rcout << "nv: " << nv << std::endl;
    // 
    //   // Rcpp::Rcout << "has rescursed: " << has_recursed << std::endl;
    //   // 
    //   // if( has_recursed ){ 
    //   //   writer.StartArray();
    //   //   for (int i = 0; i < nv.size(); i++) {
    //   //     write_value( writer, nv[i] );
    //   //   }
    //   //   writer.EndArray();
    //   // } else {
    //   //   write_value( writer, nv[ row ]);
    //   // }
    //   write_value( writer, nv[ row ]);
    // 
    //   break;
    // }
    // case INTSXP: {
    //   Rcpp::Rcout << "INTSXP" << std::endl;
    //   Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
    //   write_value( writer, iv[ row ] );
    //   break;
    // }
    // case LGLSXP: {
    //   Rcpp::Rcout << "LGLSXP" << std::endl;
    //   Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
    //   //bool b = lv[ row ];
    //   write_value( writer, lv[ row ]);
    //   break;
    // }
    // default: {
    //   Rcpp::Rcout << "default" << std::endl;
    //   Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
    //   const char *s = sv[ row ];
    //   write_value( writer, s );
    //   break;
    // }
    // }
  //}

} // namespace writers
} // namespace jsonify

#endif
