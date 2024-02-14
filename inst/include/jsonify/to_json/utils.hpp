#ifndef R_JSONIFY_WRITERS_UTILS_H
#define R_JSONIFY_WRITERS_UTILS_H

#include <Rcpp.h>

#include "rapidjson/writer.h"

namespace jsonify {
namespace utils {

  // true if `vec` contains `target`; else false
  inline bool contains(const Rcpp::CharacterVector& vec, const std::string& target) {
    for (int i = 0; i < vec.size(); ++i) {
      if (Rcpp::as<std::string>(vec[i]) == target) {
        return true;
      }
    }
    return false;
  }

  template < int RTYPE >
  inline Rcpp::CharacterVector rClass( Rcpp::Vector< RTYPE > v ) {
    if( Rf_isNull( v.attr("class")) ) {
      return "";
    }
    return v.attr("class");
  }
  
  inline Rcpp::CharacterVector getRClass( SEXP obj ) {
    switch( TYPEOF( obj ) ) {
    case REALSXP:
      return rClass< REALSXP >( obj );
    case VECSXP:
      return rClass< VECSXP >( obj );
    case INTSXP:
      return rClass< INTSXP >( obj );
    }
    return "";
  }
  
  inline void factors_to_string( Rcpp::DataFrame& df ) {
    R_xlen_t df_col;
    R_xlen_t n_cols;
    n_cols = df.ncol();
    Rcpp::StringVector column_names = df.names();
    
    for( df_col = 0; df_col < n_cols; ++df_col ) {
      const char * col_name = column_names[ df_col ];
      if( Rf_isFactor( df[ col_name ] ) ) {
        df[ col_name ] = Rcpp::as< Rcpp::StringVector >( df[ col_name ] );
      }
    }
  }

  inline Rcpp::StringVector finalise_json( rapidjson::StringBuffer& sb ) {
    Rcpp::StringVector js = Rcpp::StringVector::create( Rcpp::String( sb.GetString() ) );
    
    js.attr("class") = "json";
    return js;
  }

  template< typename RTYPE >
  inline bool should_unbox( RTYPE x, int n, bool unbox ) {
    Rcpp::CharacterVector klasses = rClass( x );
    return ( unbox && n == 1 && !contains(klasses, "AsIs") );
  }
  
  template< typename Writer >
  inline void writer_starter( Writer& writer, bool& has_names, bool& in_data_frame ) {
    if ( has_names ) {
      writer.StartObject();
    } else if( !in_data_frame ) {
      writer.StartArray();
    }
  }
  template< typename Writer >
  inline void writer_ender( Writer& writer, bool& has_names, bool& in_data_frame ) {
    if ( has_names ) {
      writer.EndObject();
    } else if( !in_data_frame ) {
      writer.EndArray();
    }
  }
  
  template< typename Writer >
  inline void start_array( Writer& writer, bool& unbox ) {
    if( !unbox ) {
      writer.StartArray();
    }
  }
  
  template< typename Writer >
  inline void end_array( Writer& writer, bool& unbox ) {
    if( !unbox ) {
      writer.EndArray();
    }
  }
  
} // namespace utils
} // namespace jsonify



#endif 
