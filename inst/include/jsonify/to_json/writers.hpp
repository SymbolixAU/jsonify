#ifndef R_JSONIFY_WRITERS_H
#define R_JSONIFY_WRITERS_H

#include <Rcpp.h>
#include "jsonify/utils.hpp"
#include <math.h>

using namespace rapidjson;

namespace jsonify {
namespace writers {

  // ---------------------------------------------------------------------------
  // scalar values
  // ---------------------------------------------------------------------------
  template <typename Writer>
  inline void write_value( Writer& writer, const char* value, int digits = -1 ) {
    writer.String( value );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, int& value, int digits = -1 ) {
     if( std::isnan( value ) ) {
      writer.Null();
    } else {
      writer.Int( value );
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, double& value, int digits = -1 ) {
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
      
      if ( digits >= 0 ) {
        double e = std::pow( 10.0, digits );
        value = round( value * e ) / e;
      }
      writer.Double( value );
    }
  }
  
  template< typename Writer> 
  inline void write_value( Writer& writer, bool& value, int digits = -1 ) {
    writer.Bool( value );
  }
  
  // ---------------------------------------------------------------------------
  // vectors
  // ---------------------------------------------------------------------------
  
  template< typename Writer>
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv, bool unbox = false, int digits = -1 ) {
    int n = nv.size();
    bool will_unbox = jsonify::utils::should_unbox( n, unbox );
    
    jsonify::utils::start_array( writer, will_unbox );
    
    for ( int i = 0; i < n; i++ ) {
      if( Rcpp::NumericVector::is_na( nv[i] ) ) {
        writer.Null();
      } else {
        write_value( writer, nv[i], digits );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  /*
   * For writing a single value of a vector
   */
  template< typename Writer >
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv, size_t row, bool unbox = false, int digits = -1 ) {
    if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) {
      writer.Null();
    } else {
      double n = nv[ row ];
      write_value( writer, n, digits );
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv, bool unbox = false, int digits = -1 ) {
    int n = iv.size();
    bool will_unbox = jsonify::utils::should_unbox( n, unbox );
    jsonify::utils::start_array( writer, will_unbox );
    
    for ( int i = 0; i < n; i++ ) {
      if( Rcpp::IntegerVector::is_na( iv[i] ) ) {
        writer.Null();
      } else {
        write_value( writer, iv[i] );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  /*
   * For writing a single value of a vector
   */
  template< typename Writer >
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv, size_t row, bool unbox = false ) {
    if ( Rcpp::IntegerVector::is_na( iv[ row ] ) ) {
      writer.Null();
    } else {
      int i = iv[ row ];
      write_value( writer, i );
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::StringVector& sv, bool unbox = false, int digits = -1) {
    int n = sv.size();
    bool will_unbox = jsonify::utils::should_unbox( n, unbox );
    jsonify::utils::start_array( writer, will_unbox );
    
    for ( int i = 0; i < n; i++ ) {
      if (Rcpp::StringVector::is_na( sv[i] ) ) {
        writer.Null();
      } else{
        write_value( writer, sv[i] );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  /*
   * for writing a single value of a vector
   */
  template <typename Writer >
  inline void write_value( Writer& writer, Rcpp::StringVector& sv, size_t row, bool unbox = false ) {
    if ( Rcpp::StringVector::is_na( sv[ row ] ) ) {
      writer.Null();
    } else {
      const char *s = sv[ row ];
      write_value( writer, s );
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, bool unbox = false, int digits = -1 ) {
    int n = lv.size();
    bool will_unbox = jsonify::utils::should_unbox( n, unbox );
    jsonify::utils::start_array( writer, will_unbox );
    
    for ( int i = 0; i < n; i++ ) {
      if (Rcpp::LogicalVector::is_na( lv[i] ) ) {
        writer.Null();
      } else {
        bool l = lv[i];             // required for logical vectors
        write_value( writer, l );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, size_t row, bool unbox = false, int digits = -1) {
    if ( Rcpp::LogicalVector::is_na( lv[ row ] ) ) { 
      writer.Null();
    } else {
      bool l = lv[ row ];
      write_value( writer, l );
    }
  }
  
  // ---------------------------------------------------------------------------
  // matrix values
  // ---------------------------------------------------------------------------
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::IntegerMatrix& mat, bool unbox = false, int digits = -1 ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int n = mat.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::IntegerVector this_row = mat(i, Rcpp::_);
      write_value( writer, this_row, unbox, digits );
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::NumericMatrix& mat, bool unbox = false, int digits = -1 ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    
    int n = mat.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::NumericVector this_row = mat(i, Rcpp::_);
      write_value( writer, this_row, unbox, digits );
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::CharacterMatrix& mat, bool unbox = false, int digits = -1 ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int n = mat.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::StringVector this_row = mat(i, Rcpp::_);
      write_value( writer, this_row, unbox, digits );
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::LogicalMatrix& mat, bool unbox = false, int digits = -1 ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    
    int n = mat.nrow();
    int i;
    for ( i = 0; i < n; i++ ) {
      Rcpp::LogicalVector this_row = mat(i, Rcpp::_);
      write_value( writer, this_row, unbox, digits );
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  // ---------------------------------------------------------------------------
  // Template definition
  // ---------------------------------------------------------------------------
  template <typename Writer, typename T>
  inline void write_value( Writer& writer, T& t, int& n, bool unbox = false, int digits = -1 ) {
    if ( n > 0 ) {
      write_value( writer, t, unbox, digits );
    } else {
      write_value( writer, t[0], digits );
    }
  }

  // ---------------------------------------------------------------------------
  // List
  // ---------------------------------------------------------------------------
  template< typename Writer>
  inline void write_value( Writer& writer, SEXP& list_element, bool unbox = false, int digits = -1 ) {
    
    if( Rf_isNull( list_element ) ) {
      writer.StartObject();
      writer.EndObject();
      return;
    } 
    
    if( Rf_isMatrix( list_element ) ) {
      
      switch( TYPEOF( list_element ) ) {
      case REALSXP: {
        Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( list_element );
        return write_value( writer, nm, unbox, digits );
        break;
      }
      case INTSXP: {
        Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( list_element );
        return write_value( writer, im, unbox, digits );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalMatrix lm = Rcpp::as< Rcpp::LogicalMatrix >( list_element );
        return write_value( writer, lm, unbox, digits );
        break;
      }
      default :{
        Rcpp::StringMatrix sm = Rcpp::as< Rcpp::StringMatrix >( list_element );
        return write_value( writer, sm, unbox, digits );
        break;
      }
      }
      
    } else if ( Rf_inherits( list_element, "data.frame" ) ) {
      
      Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( list_element );
      size_t n_cols = df.ncol();
      size_t n_rows = df.nrows();
      size_t i, j;
      Rcpp::StringVector column_names = df.names();
      
      writer.StartArray();
      
      for( i = 0; i < n_rows; i++ ) {
        writer.StartObject();
        for( j = 0; j < n_cols; j++ ) {
          const char *h = column_names[ j ];
          
          write_value( writer, h );
          SEXP this_vec = df[ h ];
          
          switch( TYPEOF( this_vec ) ) {
          case VECSXP: {
            Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
            SEXP s = lst[ i ];
            write_value( writer, s, unbox, digits );
            break;
          }
          case REALSXP: {
            Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
            write_value( writer, nv, i, unbox, digits );
            break;
          }
          case INTSXP: { 
            Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
            write_value( writer, iv, i, unbox );
            break;
          }
          case LGLSXP: {
            Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
            write_value( writer, lv, i, unbox );
            break;
          }
          default: {
            Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
            write_value( writer, sv, i, unbox );
            break;
          }
          }
          
        }
        writer.EndObject();
      }
      writer.EndArray();
      
    } else {
    
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
          write_value( writer, recursive_list, unbox, digits );
        }
        
        jsonify::utils::writer_ender( writer, has_names );
        break;
      }
      case REALSXP: {
        Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_element );
        write_value( writer, nv, unbox, digits );
        break;
      }
      case INTSXP: { 
        Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_element );
        write_value( writer, iv, unbox );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
        write_value( writer, lv, unbox );
        break;
      }
      default: {
        Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( list_element );
        write_value( writer, sv, unbox );
        break;
      }
      }
    }
  }

} // namespace writers
} // namespace jsonify

#endif
