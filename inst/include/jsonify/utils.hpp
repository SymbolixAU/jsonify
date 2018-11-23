#ifndef R_JSONIFY_WRITERS_UTILS_H
#define R_JSONIFY_WRITERS_UTILS_H

#include <Rcpp.h>

// [[Rcpp::depends(rapidjsonr)]]

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace jsonify {
namespace utils {

  inline Rcpp::StringVector finalise_json( rapidjson::StringBuffer& sb ) {
    Rcpp::StringVector js = sb.GetString();
    js.attr("class") = "json";
    return js;
  }

  inline bool should_unbox( int n, bool unbox ) {
    return ( unbox && n == 1 );
  }
  
  template< typename Writer >
  inline void writer_starter( Writer& writer, bool& has_names ) {
    if ( has_names ) {
      writer.StartObject();
    } else {
      writer.StartArray();
    }
  }
  template< typename Writer >
  inline void writer_ender( Writer& writer, bool& has_names ) {
    if ( has_names ) {
      writer.EndObject();
    } else {
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
