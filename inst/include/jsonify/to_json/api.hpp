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
      Rcpp::DataFrame& df,
      bool unbox = false,
      int digits = -1,
      bool numeric_dates = true,
      bool factors_as_string = true,
      std::string by = "row"
  ) {
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
  }

  inline Rcpp::StringVector to_ndjson(
      Rcpp::LogicalMatrix mat,
      bool unbox = false,
      std::string by = "row"
  ) {
    
    R_xlen_t n_row = mat.nrow();
    R_xlen_t n_col = mat.ncol();
    R_xlen_t i;
    
    std::ostringstream os; // for storing the final string of ndjson
    
    if( by == "row" ) {
      
      for( i = 0; i < n_row; ++i ) {
        // create new stream each row
        Rcpp::LogicalVector v = mat( i, Rcpp::_ );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else if ( by == "column" ) {
      
      for( i = 0; i < n_col; ++i ) {
        
        Rcpp::LogicalVector v = mat( Rcpp::_, i );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else {
      Rcpp::stop("jsonify - expecting matrix operatinos by row or column");
    }
    Rcpp::StringVector sv = os.str();
    return sv;
  }

  inline Rcpp::StringVector to_ndjson(
      Rcpp::IntegerMatrix mat,
      bool unbox = false,
      std::string by = "row"
  ) {
    
    R_xlen_t n_row = mat.nrow();
    R_xlen_t n_col = mat.ncol();
    R_xlen_t i;
    
    std::ostringstream os; // for storing the final string of ndjson
    
    if( by == "row" ) {
      
      for( i = 0; i < n_row; ++i ) {
        // create new stream each row
        Rcpp::IntegerVector v = mat( i, Rcpp::_ );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else if ( by == "column" ) {
      
      for( i = 0; i < n_col; ++i ) {
        
        Rcpp::IntegerVector v = mat( Rcpp::_, i );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else {
      Rcpp::stop("jsonify - expecting matrix operatinos by row or column");
    }
    Rcpp::StringVector sv = os.str();
    return sv;
  }
  
  
  inline Rcpp::StringVector to_ndjson(
    Rcpp::NumericMatrix mat,
    bool unbox = false,
    int digits = -1,
    std::string by = "row"
  ) {
    
    R_xlen_t n_row = mat.nrow();
    R_xlen_t n_col = mat.ncol();
    R_xlen_t i;
    
    std::ostringstream os; // for storing the final string of ndjson
    
    if( by == "row" ) {
      
      for( i = 0; i < n_row; ++i ) {
        // create new stream each row
        Rcpp::NumericVector v = mat( i, Rcpp::_ );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox, digits );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else if ( by == "column" ) {
      
      for( i = 0; i < n_col; ++i ) {
        
        Rcpp::NumericVector v = mat( Rcpp::_, i );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox, digits );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else {
      Rcpp::stop("jsonify - expecting matrix operatinos by row or column");
    }
    Rcpp::StringVector sv = os.str();
    return sv;
  }

  inline Rcpp::StringVector to_ndjson(
      Rcpp::StringMatrix mat,
      bool unbox = false,
      std::string by = "row"
  ) {
    
    R_xlen_t n_row = mat.nrow();
    R_xlen_t n_col = mat.ncol();
    R_xlen_t i;
    
    std::ostringstream os; // for storing the final string of ndjson
    
    if( by == "row" ) {
      
      for( i = 0; i < n_row; ++i ) {
        // create new stream each row
        Rcpp::StringVector v = mat( i, Rcpp::_ );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else if ( by == "column" ) {
      
      for( i = 0; i < n_col; ++i ) {
        
        Rcpp::StringVector v = mat( Rcpp::_, i );
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, v, unbox );
        
        os << sb.GetString();
        os << '\n';
      }
      
    } else {
      Rcpp::stop("jsonify - expecting matrix operatinos by row or column");
    }
    Rcpp::StringVector sv = os.str();
    return sv;
  }

  inline Rcpp::StringVector to_ndjson(
    SEXP obj,
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
    switch( TYPEOF( obj ) ) {
    case LGLSXP: {
      if( !Rf_isMatrix( obj ) ) {
        Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( obj );
        std::ostringstream os; // for storing the final string of ndjson
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, lv, unbox );
        
        os << sb.GetString();
        os << '\n';
        Rcpp::StringVector res = os.str();
        return res;
      } else {
        Rcpp::LogicalMatrix lm = Rcpp::as< Rcpp::LogicalMatrix >( obj );
        return to_ndjson( lm, unbox, by );
      }
    }
    case INTSXP: {
      if( !Rf_isMatrix( obj ) ) {
        Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( obj );
        std::ostringstream os; // for storing the final string of ndjson
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, iv, unbox );
        
        os << sb.GetString();
        os << '\n';
        Rcpp::StringVector res = os.str();
        return res;
      } else {
        Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( obj );
        return to_ndjson( im, unbox, by );
      }
    }
    case REALSXP: {
      if( !Rf_isMatrix( obj ) ) {
        Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( obj );
      
        std::ostringstream os; // for storing the final string of ndjson
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, nv, unbox, digits );
        
        os << sb.GetString();
        os << '\n';
        Rcpp::StringVector res = os.str();
        return res;
      } else {
        Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( obj );
        return to_ndjson( nm, unbox, digits, by );
      }
    }
    case STRSXP: {
      if( !Rf_isMatrix( obj ) ) {
        Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( obj );
      
        std::ostringstream os; // for storing the final string of ndjson
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer < rapidjson::StringBuffer > writer( sb );
        jsonify::writers::simple::write_value( writer, sv, unbox );
        
        os << sb.GetString();
        os << '\n';
        Rcpp::StringVector res = os.str();
        return res;
        
      } else {
        Rcpp::StringMatrix sm = Rcpp::as< Rcpp::StringMatrix >( obj );
        return to_ndjson( sm, unbox, by );
      }
    }
    case VECSXP: {
      if( Rf_inherits( obj, "data.frame") ) {
        Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( obj );
        return to_ndjson( df, unbox, digits, numeric_dates, factors_as_string, by );
      } else {
        // list
        Rcpp::List lst = Rcpp::as< Rcpp::List >( obj );
        R_xlen_t n = lst.size();
        R_xlen_t i;
        for( i = 0; i < n; ++i ) {
          SEXP s = lst[ i ];
          return to_ndjson( s, unbox, digits, numeric_dates, factors_as_string, by );
        }
      }
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
