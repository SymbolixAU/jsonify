#ifndef R_JSONIFY_LIST_WRITERS_H
#define R_JSONIFY_LIST_WRITERS_H

#include <Rcpp.h>
#include "writers.hpp"

namespace jsonify {
namespace listwriter {

  template< typename Writer>
  inline void write_value( Writer& writer, Rcpp::List& lst ) {
    // need to iterate in the list
    int n = lst.size();
    Rcpp::Rcout << "list size: " << n << std::endl;
    // lists need to again switch on type
    for( int i = 0; i < n; i++) {
      SEXP list_element = lst[ i ];
      //jsonify::writers::write_value( writer, list_element );
    }
  }

} // namespace listwirter
} // namespace jsonify



#endif 
