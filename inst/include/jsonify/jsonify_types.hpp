#ifndef R_JSONIFY_TYPES_H
#define R_JSONIFY_TYPES_H

#include "rapidjson/document.h"

#include <RcppCommon.h>

namespace Rcpp {

template< typename T > SEXP wrap( const rapidjson::Value& obj );

namespace traits {

} // traits
} // Rcpp




#include <Rcpp.h>



namespace Rcpp {

template< typename T >
SEXP wrap( const rapidjson::Value& obj) {

  const int RTYPE = Rcpp::traits::r_sexptype_traits< T >::rtype;
  
  return Rcpp::wrap< RTYPE >( obj.Get< T >() );
}


namespace traits {


} // traits
} // Rcpp



#endif