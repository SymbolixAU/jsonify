#ifndef R_JSONIFY_WRITERS_UTILS_H
#define R_JSONIFY_WRITERS_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]

#include "rapidjson/writer.h"

namespace jsonify {
namespace utils {

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


  inline Rcpp::StringVector finalise_json( rapidjson::StringBuffer& sb ) {
    Rcpp::StringVector js = sb.GetString();
    js.attr("class") = "json";
    return js;
  }

  inline bool should_unbox( int n, bool unbox ) {
    return ( unbox && n == 1 );
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
