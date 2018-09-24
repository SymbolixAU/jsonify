#ifndef R_JSONIFY_WRITERS_UTILS_H
#define R_JSONIFY_WRITERS_UTILS_H

#include <Rcpp.h>

namespace jsonify {
namespace utils {

  template< typename Writer>
  inline void writer_starter( Writer& writer, int& n_elements ) {
    
  }
  
  template< typename Writer>
  inline void writer_starter( Writer& writer, int n_elements, bool& has_names ) {
    if ( n_elements > 1 ) {
      if ( has_names ) {
        writer.StartObject();
      } else {
        writer.StartArray();
      }
    }
  }
  template< typename Writer>
  inline void writer_ender( Writer& writer, int n_elements, bool& has_names ) {
    if ( n_elements > 1 ) {
      if ( has_names ) {
        writer.EndObject();
      } else {
        writer.EndArray();
      }
    }
  }
  

} // namespace utils
} // namespace jsonify



#endif 
