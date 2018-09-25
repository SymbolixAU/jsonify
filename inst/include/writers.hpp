#ifndef R_JSONIFY_WRITERS_H
#define R_JSONIFY_WRITERS_H

#include <Rcpp.h>
#include "utils.hpp"
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
      bool l = lv[i];             // required for logical vectors
      write_value( writer, l );
    }
    writer.EndArray();
  }
  
  template <typename Writer, typename T>
  inline void write_value( Writer& writer, T& t, int& n) {
    if ( n > 0 ) {
      write_value( writer, t );
    } else {
      write_value( writer, t[0] );
    }
  }

  template< typename Writer>
  inline void write_value( Writer& writer, SEXP list_element ) {
    
    int n_elements;
    
    switch( TYPEOF( list_element ) ) {
    case VECSXP: {
      
      //Rcpp::Rcout << "list element list " << std::endl;
      Rcpp::List lst = Rcpp::as< Rcpp::List >( list_element );
      int n = lst.size();
      
      // LIST NAMES
      Rcpp::IntegerVector int_names = Rcpp::seq(1, lst.size());
      Rcpp::CharacterVector list_names = Rcpp::as< Rcpp::CharacterVector >( int_names );
      bool has_names = Rf_isNull(lst.names()) ? false : true;
      
      if ( has_names ) {
        Rcpp::CharacterVector temp_names = lst.names();
        //Rcpp::Rcout << "has names: " << temp_names << std::endl;
        //list_names = lst.names();
        for( int i = 0; i < n; i++ ) {
          list_names[i] = temp_names[i] == "" ? list_names[i] : temp_names[i];
        }
      }
      // END LIST NAMES

      jsonify::utils::writer_starter( writer, has_names );
      
      for ( int i = 0; i < n; i++ ) {
        SEXP recursive_list = lst[ i ];
        if ( has_names ) {
          const char *s = list_names[ i ];
          write_value( writer, s );
        }
        write_value( writer, recursive_list );
      }
      
      jsonify::utils::writer_ender( writer, has_names );

      break;
    }
    case REALSXP: {
      //Rcpp::Rcout << "list element REAL " << std::endl;
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_element );
      n_elements = nv.size();
      write_value( writer, nv, n_elements);
      break;
    }
    case INTSXP: { 
      //Rcpp::Rcout << "list element INT " << std::endl;
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_element );
      n_elements = iv.size();
      write_value( writer, iv, n_elements);
      break;
    }
    case LGLSXP: {
      //Rcpp::Rcout << "list element LGL " << std::endl;
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
      n_elements = lv.size();
      write_value( writer, lv, n_elements);
      break;
    }
    default: {
      //Rcpp::Rcout << "list element default " << std::endl;
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( list_element );
      n_elements = sv.size();
      write_value( writer, sv, n_elements);
      break;
    }
    }
  }

} // namespace writers
} // namespace jsonify

#endif
