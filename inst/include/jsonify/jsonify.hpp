#ifndef R_JSONIFY_H
#define R_JSONIFY_H

#include "rapidjson/document.h"

#include <RcppCommon.h>

namespace Rcpp {

  template< typename T > SEXP wrap( const rapidjson::Value::ValueType& obj );

namespace traits {

} // traits
} // Rcpp




#include <Rcpp.h>



namespace Rcpp {
  
  template< typename T >
  SEXP wrap( const rapidjson::Value::ValueType& obj) {
    const int RTYPE = Rcpp::traits::r_sexptype_traits< T >::rtype;
    
    return Rcpp::wrap< RTYPE >( obj.Get< T >() );
  }


namespace traits {


} // traits
} // Rcpp





#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "jsonify/to_json/api.hpp"
#include "jsonify/from_json/api.hpp"

#endif
