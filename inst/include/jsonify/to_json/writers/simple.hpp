#ifndef R_JSONIFY_WRITERS_SIMPLE_H
#define R_JSONIFY_WRITERS_SIMPLE_H

#include <Rcpp.h>

using namespace rapidjson;

namespace jsonify {
namespace writers {
namespace simple {

  // ---------------------------------------------------------------------------
  // scalar values
  // ---------------------------------------------------------------------------
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
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::StringVector& sv, bool unbox = false ) {
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
  
  template< typename Writer>
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv, bool unbox = false, int digits = -1, bool numeric_dates = true ) {

    Rcpp::CharacterVector cls = jsonify::utils::getRClass( nv );
    
    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::date_to_string( nv );
      write_value( writer, sv, unbox );
      
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( nv );
      write_value( writer, sv, unbox );
      
    } else {
    
      int n = nv.size();
      // Rcpp::Rcout << "nv.size: " << n << std::endl;
      // Rcpp::Rcout << "unbox: " << unbox << std::endl; 
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
  }
  
  /*
   * For writing a single value of a vector
   */
  template< typename Writer >
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv, size_t row, bool unbox = false, int digits = -1, bool numeric_dates = true ) {

    // Rcpp::Rcout << "writing nv with row: " << row << std::endl;
    
    Rcpp::CharacterVector cls = jsonify::utils::getRClass( nv );
    
    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {

      Rcpp::StringVector sv = jsonify::dates::date_to_string( nv );
      write_value( writer, sv, row, unbox );
      
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( nv );
      write_value( writer, sv, row, unbox );
      
    } else {
      if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) {
        writer.Null();
      } else {
        double n = nv[ row ];
        write_value( writer, n, digits );
      }
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv, bool unbox = false, bool numeric_dates = true ) {
    
    Rcpp::CharacterVector cls = jsonify::utils::getRClass( iv );

    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {

      Rcpp::StringVector sv = jsonify::dates::date_to_string( iv );
      write_value( writer, sv, unbox );
      
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( iv );
      write_value( writer, sv, unbox );
      
    } else {
    
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
  }
  
  /*
   * For writing a single value of a vector
   */
  template< typename Writer >
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv, size_t row, 
                           bool unbox = false, bool numeric_dates = false ) {
    
    Rcpp::CharacterVector cls = jsonify::utils::getRClass( iv );
    
    
    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::date_to_string( iv );
      write_value( writer, sv, row, unbox );
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( iv );
      write_value( writer, sv, row, unbox );
      
    } else {
    
      if ( Rcpp::IntegerVector::is_na( iv[ row ] ) ) {
        writer.Null();
      } else {
        int i = iv[ row ];
        write_value( writer, i );
      }
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, bool unbox = false ) {
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
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, size_t row, bool unbox = false ) {
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
  inline void write_value( Writer& writer, Rcpp::IntegerMatrix& mat, bool unbox = false,
                           std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int n;
    int i;
    
    if ( by == "row" ) {
      n = mat.nrow();
      for ( i = 0; i < n; i++ ) {
        Rcpp::IntegerVector this_row = mat(i, Rcpp::_);
        write_value( writer, this_row, unbox );
      }
    } else { // by == "column"
      n = mat.ncol();
      for( i = 0; i < n; i++ ) {
        Rcpp::IntegerVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::NumericMatrix& mat, bool unbox = false, 
                           int digits = -1, std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    
    int n;
    int i;
    if ( by == "row" ) {
      n = mat.nrow();
      for ( i = 0; i < n; i++ ) {
        Rcpp::NumericVector this_row = mat(i, Rcpp::_);
        write_value( writer, this_row, unbox, digits );
      }
    } else { // by == "column"
      n = mat.ncol();
      for( i = 0; i < n; i++ ) {
        Rcpp::NumericVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox, digits );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::CharacterMatrix& mat, bool unbox = false,
                           std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int i;
    int n;
    
    if( by == "row" ) {
      n = mat.nrow();
      for ( i = 0; i < n; i++ ) {
        Rcpp::StringVector this_row = mat( i, Rcpp::_ );
        write_value( writer, this_row, unbox );
      }
    } else { // by == column
      n = mat.ncol();
      for ( i = 0; i < n; i++ ) {
        Rcpp::StringVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::LogicalMatrix& mat, bool unbox = false, 
                           std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int i;
    int n;
    
    if( by == "row" ) {
      n = mat.nrow();
      
      for ( i = 0; i < n; i++ ) {
        Rcpp::LogicalVector this_row = mat(i, Rcpp::_);
        write_value( writer, this_row, unbox );
      }
    } else { // by == "column;
      n = mat.ncol();
      
      for( i = 0; i < n; i++ ) {
        Rcpp::LogicalVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox);
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }

} // namespace simple
} // namespace writers
} // namespace jsonify

#endif
