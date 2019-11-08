#ifndef JSONIFY_TO_JSON_API_H
#define JSONIFY_TO_JSON_API_H

#include <Rcpp.h>
#include "jsonify/to_json/utils.hpp"
#include "jsonify/to_json/writers/complex.hpp"

namespace jsonify {
namespace api {

  inline Rcpp::StringVector to_json(
    SEXP lst, 
    bool unbox = false, 
    int digits = -1, 
    bool numeric_dates = true, 
    bool factors_as_string = true, 
    std::string by = "row"
  ) {
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::complex::write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
    return jsonify::utils::finalise_json( sb );
  }

  inline Rcpp::StringVector to_ndjson(
    SEXP lst, 
    bool unbox = false, 
    int digits = -1, 
    bool numeric_dates = true, 
    bool factors_as_string = true, 
    std::string by = "row"
  ) {
        
    // TODO
    // the way ndjson is created depends on the type of input object
    // a list will be element-wise
    // data.frame / matrix will be whatever 'by' is set
    // other cases not handled?
    switch( TYPEOF( lst ) ) {
    case LGLSXP: {
        if( !Rf_isMatrix( lst ) ) {
        Rcpp::stop("jsonify - expecting a matrix, data.frame or list");
    } else {
        Rcpp::LogicalMatrix im = Rcpp::as< Rcpp::LogicalMatrix >( lst );   
    }
    break;
    }
    case INTSXP: {
      if( !Rf_isMatrix( lst ) ) {
        Rcpp::stop("jsonify - expecting a matrix, data.frame or list");
      } else {
        Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( lst );   
      }
      break;
    }
    case REALSXP: {
        if( !Rf_isMatrix( lst ) ) {
          Rcpp::stop("jsonify - expecting a matrix, data.frame or list");
        } else {
          Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( lst );   
        }
        break;
    }
    case STRSXP: {
        if( !Rf_isMatrix( lst ) ) {
          Rcpp::stop("jsonify - expecting a matrix, data.frame or list");
        } else {
          Rcpp::StringMatrix im = Rcpp::as< Rcpp::StringMatrix >( lst );   
        }
        break;
    }
    case VECSXP: {
     if( Rf_inherits( lst, "data.frame") ) {
        Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( lst );
        
    } else {
        // list
    }
      break;
    }
    default: {
      Rcpp::stop("jsonify - expecting a matrix, data.frame or list")
      }
    
    }
        
        
        
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::complex::write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
    return jsonify::utils::finalise_json( sb );
  }


} // namespace api
} // namespace jsonify

#endif
