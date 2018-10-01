#ifndef R_JSONIFY_WRITERS_H
#define R_JSONIFY_WRITERS_H

#include <Rcpp.h>
#include "utils.hpp"

// [[Rcpp::depends(rapidjsonr)]]

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

namespace jsonify {
namespace writers {

  template <typename Writer>
  inline void write_value( Writer& writer, const char* value ) {
    writer.String( value );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, int& value ) {
     if( std::isnan( value ) ) {
      writer.Null();
    } else {
      writer.Int( value );
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, double& value ) {
    if(std::isnan( value ) ) {
      writer.Null();
    } else if ( std::isinf( value ) ) {
      
      std::string str = std::to_string( value );
      // https://stackoverflow.com/a/14744016/5977215
      if ( str[0] == '-') { 
        str[1] = toupper( str[1] );
      } else {
        str[0] = toupper(str[0]);
      }
      writer.String( str.c_str() );
    } else {
      writer.Double( value );
    }
  }
  
  template< typename Writer> 
  inline void write_value( Writer& writer, bool& value ) {
    writer.Bool( value );
  }
  
  template< typename Writer>
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv ) {
    int n = nv.size();
    jsonify::utils::should_start_array( writer, n );
    for ( int i = 0; i < n; i++ ) {
      if( Rcpp::NumericVector::is_na( nv[i] ) ) {
        writer.Null();
      } else {
        write_value( writer, nv[i] );
      }
    }
    jsonify::utils::should_end_array( writer, n );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv ) {
    int n = iv.size();
    jsonify::utils::should_start_array( writer, n );
    for ( int i = 0; i < n; i++ ) {
      if( Rcpp::IntegerVector::is_na( iv[i] ) ) {
        //write_value( writer, "NA" );
        writer.Null();
      } else {
        write_value( writer, iv[i] );
      }
    }
    jsonify::utils::should_end_array( writer, n );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::StringVector& sv ) {
    int n = sv.size();
    jsonify::utils::should_start_array( writer, n );
    for ( int i = 0; i < n; i++ ) {
      if (Rcpp::StringVector::is_na( sv[i] ) ) {
        writer.Null();
      } else{
        write_value( writer, sv[i] );
      }
    }
    jsonify::utils::should_end_array( writer, n );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv ) {
    int n = lv.size();
    jsonify::utils::should_start_array( writer, n );
    for ( int i = 0; i < lv.size(); i++ ) {
      if (Rcpp::LogicalVector::is_na( lv[i] ) ) {
        //Rcpp::Rcout << "NA logical found" << std::endl;
        // write_value( writer, "NA" );
        writer.Null();
      } else {
        bool l = lv[i];             // required for logical vectors
        write_value( writer, l );
      }
    }
    jsonify::utils::should_end_array( writer, n );
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
  inline void write_value( Writer& writer, SEXP& list_element ) {
    
    int n_elements;
    if( Rf_isNull( list_element ) ) {
      writer.StartObject();
      writer.EndObject();
      return;
    } 
    
    switch( TYPEOF( list_element ) ) {
    case VECSXP: {
      Rcpp::List lst = Rcpp::as< Rcpp::List >( list_element );
      int n = lst.size();
      
      if ( n == 0 ) {
        writer.StartArray();
        writer.EndArray();
        break;
      }
      
      // LIST NAMES
      Rcpp::IntegerVector int_names = Rcpp::seq(1, lst.size());
      Rcpp::CharacterVector list_names = Rcpp::as< Rcpp::CharacterVector >( int_names );
      bool has_names = Rf_isNull(lst.names()) ? false : true;
      
      if ( has_names ) {
        Rcpp::CharacterVector temp_names = lst.names();
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
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_element );
      n_elements = nv.size();
      write_value( writer, nv, n_elements);
      break;
    }
    case INTSXP: { 
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_element );
      n_elements = iv.size();
      write_value( writer, iv, n_elements);
      break;
    }
    case LGLSXP: {
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
      n_elements = lv.size();
      write_value( writer, lv, n_elements);
      break;
    }
    default: {
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
