#ifndef R_JSONIFY_TYPES_H
#define R_JSONIFY_TYPES_H

#include "rapidjson/document.h"

#include <RcppCommon.h>

namespace Rcpp {

  // template<> SEXP wrap( std::basic_string& obj );
  template< typename T > SEXP wrap( const rapidjson::Value& obj );

namespace traits {

} // traits
} // Rcpp

#include <Rcpp.h>

namespace Rcpp {

  template< typename T >
  SEXP wrap( const rapidjson::Value& obj) {
    auto b = obj.Get< T >();
    return Rcpp::wrap( b );
  }


namespace traits {


} // traits
} // Rcpp



#endif