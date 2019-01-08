#ifndef R_JSONIFY_VECTORS_H
#define R_JSONIFY_VECTORS_H

#include <Rcpp.h>
#include "jsonify/jsonify.hpp"
#include "jsonify/to_json/writers/simple.hpp"

using namespace rapidjson;

namespace jsonify {
namespace vectors {

  // inline Rcpp::StringVector to_json( Rcpp::NumericVector& nv, bool unbox = false, int digits = -1 ) {
  //   // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
  //   return jsonify::api::to_json( nv, unbox, digits );
  // }
  // 
  // inline Rcpp::StringVector to_json( Rcpp::StringVector& sv, bool unbox = false ) {
  //   // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
  //   return jsonify::api::to_json( sv, unbox );
  // }
  // 
  // inline Rcpp::StringVector to_json( Rcpp::IntegerVector& iv, bool unbox = false ) {
  //   // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
  //   return jsonify::api::to_json( iv, unbox );
  // }
  // 
  // inline Rcpp::StringVector to_json( Rcpp::LogicalVector& lv, bool unbox = false ) {
  //   // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
  //   return jsonify::api::to_json( lv, unbox );
  // }
  // 
  // inline Rcpp::StringVector to_json( SEXP& lst, bool unbox = false, int digits = -1 ) {
  //   // Rcpp::warning("namespace jsonify::vectors is deprecated. Use jsonify::api instead");
  //   return jsonify::api::to_json( lst, unbox, digits );
  // }

  template < typename Writer >
  inline void switch_vector( Writer& writer, SEXP this_vec, bool unbox, 
                             int digits, bool numeric_dates, 
                             bool factors_as_string ) {
    
    switch( TYPEOF( this_vec ) ) {
    case REALSXP: {
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
      jsonify::writers::simple::write_value( writer, nv, unbox, digits, numeric_dates );
      break;
    }
    case INTSXP: {
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
      if ( factors_as_string && Rf_isFactor( this_vec ) ) {
        Rcpp::CharacterVector lvls = iv.attr( "levels" );
        if( lvls.length() == 0 ) {
          // no levels - from NA_character_ vector
          
          Rcpp::StringVector s(1);
          s[0] = NA_STRING;
          jsonify::writers::simple::write_value( writer, s, 0, unbox );
        } else {
          jsonify::writers::simple::write_value( writer, lvls, unbox );
        }
      } else {
        jsonify::writers::simple::write_value( writer, iv, unbox, numeric_dates );
      }
      break;
    }
    case LGLSXP: {
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
      jsonify::writers::simple::write_value( writer, lv, unbox );
      break;
    }
    default: {
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
      jsonify::writers::simple::write_value( writer, sv, unbox );
      break;
    }
    }
  }
  
  // working by-row, so we only use a single element of each vector
  template < typename Writer >
  inline void switch_vector( Writer& writer, SEXP this_vec, bool unbox, 
                             int digits, bool numeric_dates, 
                             bool factors_as_string, int row) {
    
    switch( TYPEOF( this_vec ) ) {
    case REALSXP: {
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
      jsonify::writers::simple::write_value( writer, nv, row, unbox, digits, numeric_dates );
      break;
    }
    case INTSXP: {
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
      if( factors_as_string && Rf_isFactor( this_vec ) ) {
        Rcpp::CharacterVector lvls = iv.attr("levels");
        if ( lvls.length() == 0 ) {
          // no levls - from NA_character_ vector
          
          Rcpp::StringVector s(1);
          s[0] = NA_STRING;
          jsonify::writers::simple::write_value( writer, s, 0, unbox );
        } else {
          int this_int = iv[ row ];
          const char * this_char = lvls[ this_int -1 ];
          jsonify::writers::simple::write_value( writer, this_char );
        }
        
      } else {
        jsonify::writers::simple::write_value( writer, iv, row, unbox, numeric_dates, factors_as_string );
      }
      break;
    }
    case LGLSXP: {
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
      jsonify::writers::simple::write_value( writer, lv, row, unbox );
      break;
    }
    default: {
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
      jsonify::writers::simple::write_value( writer, sv, row, unbox );
      break;
    }
    }
  }


} // namespace vectors
} // namespace jsonify

#endif
