#ifndef R_JSONIFY_DATAFRAME_H
#define R_JSONIFY_DATAFRAME_H

#include <Rcpp.h>

#include "jsonify/to_json/writers/simple.hpp"
#include "jsonify/to_json/writers/complex.hpp"
#include "jsonify/to_json/writers.hpp"

using namespace rapidjson;

// TODO( remove this once spatialwidget v0.2 is on CRAN)

namespace jsonify {
namespace dataframe {

  // keeping these dataframe_cell functions so they can be called directly by other libraries
  //, e.g., geojsonsf
  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP this_vec, int row,
                              int digits, bool numeric_dates, 
                              bool factors_as_string ) {

    jsonify::writers::simple::write_value( writer, this_vec, row, digits, numeric_dates, factors_as_string );
    
    // switch( TYPEOF( this_vec ) ) {
    // case VECSXP: {
    //   Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
    //   SEXP s = lst[ row ];
    //   jsonify::writers::simple::write_value( writer, s, unbox, digits );
    //   break;
    // }
    // case REALSXP: {
    //   Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
    //   if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) {
    //     writer.Null();
    //   } else {
    //     double n = nv[ row ];
    //     jsonify::writers::simple::write_value( writer, n, digits );
    //   }
    //   break;
    // }
    // case INTSXP: {
    //   Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
    //   if ( Rcpp::IntegerVector::is_na( iv[ row ] ) ) {
    //     writer.Null();
    //   } else {
    //     int i = iv[ row ];
    //     jsonify::writers::simple::write_value( writer, i );
    //   }
    //   break;
    // }
    // case LGLSXP: {
    //   Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
    //   if ( Rcpp::LogicalVector::is_na( lv[ row ] ) ) {
    //     writer.Null();
    //   } else {
    //     bool l = lv[ row ];
    //     jsonify::writers::simple::write_value( writer, l );
    //   }
    //   break;
    // }
    // default: {
    //   Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
    //   if ( Rcpp::StringVector::is_na( sv[ row ] ) ) {
    //     writer.Null();
    //   } else {
    //     const char *s = sv[ row ];
    //     jsonify::writers::simple::write_value( writer, s );
    //   }
    //   break;
    // }
    // }
  }
  
  // template <typename Writer>
  // inline void dataframe_cell( Writer& writer, SEXP this_vec, size_t row,
  //                             bool unbox, int digits, bool numeric_dates, 
  //                             bool factors_as_string ) {
  //   
  //   jsonify::writers::simple::write_value( writer, this_vec, row, unbox, digits, numeric_dates, factors_as_string );
  // }

  // overload for when you dont' specify 'unbox'
  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP this_vec, int row ) {
    dataframe_cell( writer, this_vec, row, -1, false, true );
  }
  
  // overload for when you dont' specify 'unbox'
  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP this_vec, size_t row ) {
    int i = row;
    dataframe_cell( writer, this_vec, i, -1, false, true);
  }

  inline Rcpp::StringVector to_json( Rcpp::DataFrame& df, bool unbox = false, int digits = -1 ) {
    // Rcpp::warning("namespace jsonify::dataframe is deprecated. Use jsonify::api instead");
    //SEXP s = df;
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::complex::write_value( writer, df, unbox, digits );
    return jsonify::utils::finalise_json( sb );
  }

} // namespace dataframe
} // namespace jsonify

#endif
