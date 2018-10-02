#ifndef R_JSONIFY_VALIDATE_H
#define R_JSONIFY_VALIDATE_H

#include <Rcpp.h>
#include "rapidjson/document.h"

namespace jsonify {
namespace validate {

  inline bool validate_json( rapidjson::Document& d, const char* geojson ) {
    d.Parse( geojson );
    bool res = true;
    if( d.Parse( geojson ).HasParseError() ) {
      res = false;
    }
    return res;
  }

  inline bool validate_json( const char* geojson ) {
    rapidjson::Document doc;
    return validate_json( doc, geojson );
  }

} // namespace validate
} // namespace jsonify

#endif
