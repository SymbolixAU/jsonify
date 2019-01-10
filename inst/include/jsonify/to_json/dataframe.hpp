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

  }

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
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    jsonify::writers::complex::write_value( writer, df, unbox, digits );
    return jsonify::utils::finalise_json( sb );
  }

} // namespace dataframe
} // namespace jsonify

#endif
