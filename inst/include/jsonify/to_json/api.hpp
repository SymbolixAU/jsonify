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

  // inline Rcpp::StringVector to_ndjson(
  //   Rcpp::DataFrame& df,
  //   bool unbox = false,
  //   int digits = -1,
  //   bool numeric_dates = true,
  //   bool factors_as_string = true,
  //   std::string by = "row"
  // ) {
  //   // loop over rows or columns, 
  // }

  inline Rcpp::StringVector to_ndjson(
    SEXP lst,
    bool unbox = false,
    int digits = -1,
    bool numeric_dates = true,
    bool factors_as_string = true,
    std::string by = "row"
  ) {

    // rapidjson::StringBuffer sb;
    // rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    // 
    // std::ostringstream os; // for storing the final string of ndjson
    
    // TODO
    // the way ndjson is created depends on the type of input object
    // a list will be element-wise
    // data.frame / matrix will be whatever 'by' is set
    // other cases not handled?
    //
    // given the rapidjson DOM doesn't accept '\n' characters,
    // I may have to iterate over the object and create one json object at a time
    // and output to an OSStream, append '\n', 
    // then at the end convert to StringVector.
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
        R_xlen_t n_row = df.nrow();
        R_xlen_t n_cols = df.ncol();
        R_xlen_t df_col;
        R_xlen_t row;
        Rcpp::StringVector column_names = df.names();
        bool in_data_frame = true;
        
        std::ostringstream os; // for storing the final string of ndjson
        
        if( by == "row" ) {
        
          for( row = 0; row < n_row; row++ ) {
            
            // create new stream each row
            rapidjson::StringBuffer sb;
            rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
            
            writer.StartObject();
            for( df_col = 0; df_col < n_cols; df_col++ ) {
              
              const char *h = column_names[ df_col ];
              writer.String( h );
              
              SEXP this_vec = df[ h ];
              
              switch( TYPEOF( this_vec ) ) {
              
              case VECSXP: {
                Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
                jsonify::writers::complex::write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by, row, in_data_frame );
                break;
              }
              default: {
                jsonify::writers::complex::switch_vector( writer, this_vec, unbox, digits, numeric_dates, factors_as_string, row );
              }
              } // end switch
              
            } // end for
            writer.EndObject();
            
            os << sb.GetString();
            os << '\n';
          }  // end for (row)
          
        } else {
          // by == "column"
          for( df_col = 0; df_col < n_cols; df_col++ ) {
            // create new stream each row
            rapidjson::StringBuffer sb;
            rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
            
            writer.StartObject();
            
            const char *h = column_names[ df_col ];
            writer.String( h );
            SEXP this_vec = df[ h ];
            jsonify::writers::complex::write_value( writer, this_vec, unbox, digits, numeric_dates, factors_as_string, by, -1, in_data_frame );
            
            writer.EndObject();
            
            os << sb.GetString();
            os << '\n';
          }  // end for (column)
          
        }
        
        Rcpp::StringVector sv = os.str();
        return sv;
        
    } else {
        // list
    }
      break;
    }
    default: {
      Rcpp::stop("jsonify - expecting a matrix, data.frame or list");
      }

    }
    
    return Rcpp::StringVector::create();

    //rapidjson::StringBuffer sb;
    //rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
    //jsonify::writers::complex::write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
    //return jsonify::utils::finalise_json( sb );
  }


} // namespace api
} // namespace jsonify

#endif
