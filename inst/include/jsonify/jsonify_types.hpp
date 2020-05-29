#ifndef R_JSONIFY_TYPES_H
#define R_JSONIFY_TYPES_H

#include "rapidjson/document.h"

#include <RcppCommon.h>

namespace Rcpp {

  template< typename T > SEXP wrap( const rapidjson::Value& obj );
  // template< typename T > bool isType( const rapidjson::Value& obj );
  // //template< typename T, typename N > T getType( const N& obj );
  // rapidjson::Type getType( const rapidjson::Value& obj );
  // R_xlen_t getSize( const rapidjson::Value& obj );
  // rapidjson::Value::ConstArray getArray( const rapidjson::Value& obj );
  // rapidjson::Value::ConstObject getObject( const rapidjson::Value& obj );

namespace traits {

} // traits
} // Rcpp

#include <Rcpp.h>

namespace Rcpp {

  template< typename T >
  SEXP wrap( const rapidjson::Value& obj ) {
    auto b = obj.Get< T >();
    return Rcpp::wrap( b );
  }
  
  // template< typename T >
  // bool isType( const rapidjson::Value& obj ) {
  //   return obj.Is< T >();
  // }
  // 
  // // template< typename T, typename N > T getType( const N& object ) {
  // //   
  // // }
  // 
  // inline rapidjson::Type getType( const rapidjson::Value& obj ) {
  //   return obj.GetType();
  // }
  // 
  // inline R_xlen_t getSize( const rapidjson::Value& obj ) {
  //   return obj.Size();
  // }


namespace traits {


} // traits
} // Rcpp

template < typename T > 
inline R_xlen_t jsonLength( T& obj ) {
  return obj.Size();
}

template< typename T >
inline rapidjson::Value::ConstArray getArray( T& obj ) {
  return obj.GetArray();  
}

template< typename T >
inline rapidjson::Value::ConstObject getObject( T& obj ) {
  return obj.GetObject();
}

template< typename T >
inline int getType( const T& obj ) {
  return obj.GetType();
}

template< typename T >
inline bool getBool( T& obj ) {
  return obj.GetBool();
}

// template< typename T >
// inline const char* getChar( T& obj ) {
//   return obj.GetString();
// }

template< typename T >
inline double getDouble( T& obj ) {
  return obj.GetDouble();
}

template< typename T >
inline int getInt( T& obj ) {
  return obj.GetInt();
}

#endif