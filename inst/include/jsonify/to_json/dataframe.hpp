#ifndef R_JSONIFY_DATAFRAME_H
#define R_JSONIFY_DATAFRAME_H

#include <Rcpp.h>

#include "jsonify/to_json/writers.hpp"

using namespace rapidjson;

namespace jsonify {
namespace dataframe {

  // keeping these dataframe_cell functions so they can be called directly by other libraries
  //, e.g., geojsonsf
  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP& this_vec, size_t& row,
                              bool unbox, int digits ) {

    switch( TYPEOF( this_vec ) ) {
    case VECSXP: {
      Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
      SEXP s = lst[ row ];
      jsonify::writers::write_value( writer, s, unbox, digits );
      break;
    }
    case REALSXP: {
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
      if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) {
        writer.Null();
      } else {
        double n = nv[ row ];
        jsonify::writers::write_value( writer, n, digits );
      }
      break;
    }
    case INTSXP: {
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
      if ( Rcpp::IntegerVector::is_na( iv[ row ] ) ) {
        writer.Null();
      } else {
        int i = iv[ row ];
        jsonify::writers::write_value( writer, i );
      }
      break;
    }
    case LGLSXP: {
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
      if ( Rcpp::LogicalVector::is_na( lv[ row ] ) ) {
        writer.Null();
      } else {
        bool l = lv[ row ];
        jsonify::writers::write_value( writer, l );
      }
      break;
    }
    default: {
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
      if ( Rcpp::StringVector::is_na( sv[ row ] ) ) {
        writer.Null();
      } else {
        const char *s = sv[ row ];
        jsonify::writers::write_value( writer, s );
      }
      break;
    }
    }
  }

  // overload for when you dont' specify 'unbox'
  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP& this_vec, size_t& row ) {
    dataframe_cell( writer, this_vec, row, false, -1 );
  }

  inline Rcpp::StringVector to_json( Rcpp::DataFrame& df, bool unbox = false, int digits = -1 ) {
    // Rcpp::warning("namespace jsonify::dataframe is deprecated. Use jsonify::api instead");
    //SEXP s = df;
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::write_value( writer, df, unbox, digits );
    return jsonify::utils::finalise_json( sb );
  }

} // namespace dataframe
} // namespace jsonify

#endif
