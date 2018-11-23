#ifndef R_JSONIFY_DATAFRAME_H
#define R_JSONIFY_DATAFRAME_H

#include <Rcpp.h>

#include "jsonify/jsonify.hpp"

using namespace rapidjson;

namespace jsonify {
namespace dataframe {

  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP& this_vec, size_t& row, bool unbox = false ) {
    
    switch( TYPEOF( this_vec ) ) {
    case VECSXP: {
      Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
      SEXP s = lst[ row ];
      jsonify::writers::write_value( writer, s, unbox );
      break;
    }
    case REALSXP: {
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
      if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) { 
        writer.Null();
      } else {
        double n = nv[ row ];
        jsonify::writers::write_value( writer, n );
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

  inline Rcpp::StringVector to_json( Rcpp::DataFrame& df, bool unbox = false ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    
    size_t n_cols = df.ncol();
    size_t n_rows = df.nrows();
    size_t i, j;
    Rcpp::StringVector column_names = df.names();
    
    writer.StartArray();
    
    for( i = 0; i < n_rows; i++ ) {
      writer.StartObject();
      for( j = 0; j < n_cols; j++ ) {
        const char *h = column_names[ j ];
        
        jsonify::writers::write_value( writer, h );
        
        SEXP this_vec = df[ h ];
        dataframe_cell( writer, this_vec, i, unbox );
        
      }
      writer.EndObject();
    }
    writer.EndArray();
    
    return jsonify::utils::finalise_json( sb );
  }

} // namespace dataframe
} // namespace jsonify

#endif
