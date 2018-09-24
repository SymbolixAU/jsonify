#ifndef R_JSONIFY_H
#define R_JSONIFY_H

#include <Rcpp.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "writers.hpp"

namespace jsonify {

  template <typename Writer>
  inline void dataframe_cell( Writer& writer, SEXP& this_vec, int row) {
    // get teh type of vec, then get the cell value
    // then WRITE that value?
    // if it's a single element, it can be written, no problemo
    // if it's a list, it needs to be iterated into...
    
    switch( TYPEOF( this_vec ) ) {
    case VECSXP: {
      // list
      //Rcpp::Rcout << "list cel: " << std::endl;
      Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
      SEXP s = lst[ row ];
      jsonify::writers::write_value( writer, s );
      break;
    }
    case REALSXP: {
      // real
      // from a data.frame, if the 'cell' is not a list, it's a single value
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
      double n = nv[ row ];
      //Rcpp::Rcout << "nv cell: " << nv << std::endl;
      jsonify::writers::write_value( writer, n );
      break;
    }
    case INTSXP: { 
      // int
      Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
      int i = iv[ row ];
      jsonify::writers::write_value( writer, i );
      break;
    }
    case LGLSXP: {
      // logical
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
      bool l = lv[ row ];
      jsonify::writers::write_value( writer, l );
      break;
    }
    default: {
      // string / other
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
      const char *s = sv[ row ];
      jsonify::writers::write_value( writer, s );
      break;
    }
    }
  }

  
  inline Rcpp::StringVector to_json( Rcpp::DataFrame& df ) {
    
    rapidjson::StringBuffer sb;
    rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    
    size_t n_cols = df.ncol();
    size_t n_rows = df.nrows();
    int i, j;
    Rcpp::StringVector column_names = df.names();
    
    writer.StartArray();
    
    for( i = 0; i < n_rows; i++ ) {
      writer.StartObject();
      for( j = 0; j < n_cols; j++ ) {
        const char *h = column_names[ j ];
        //const char *this_type = col_classes[ j ];
        
        jsonify::writers::write_value( writer, h );
        
        SEXP this_vec = df[ h ];
        dataframe_cell( writer, this_vec, i );
        
      }
      writer.EndObject();
    }
    writer.EndArray();
    
    Rcpp::StringVector js = sb.GetString();
    return js;
  }

} // namespace jsonify

#endif
