#ifndef R_JSONIFY_WRITERS_COMPLEX_H
#define R_JSONIFY_WRITERS_COMPLEX_H

#include <Rcpp.h>
#include "jsonify/to_json/utils.hpp"
#include "jsonify/to_json/dates/dates.hpp"
#include "jsonify/to_json/writers/simple.hpp"
#include <math.h>

using namespace rapidjson;

namespace jsonify {
namespace writers {
namespace complex {

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
          int ele = 0;
          jsonify::writers::simple::write_value( writer, s, ele );
        } else {
          jsonify::writers::simple::write_value( writer, lvls, unbox );
        }
      } else {
        jsonify::writers::simple::write_value( writer, iv, unbox, numeric_dates, factors_as_string );
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
      jsonify::writers::simple::write_value( writer, nv, row, digits, numeric_dates );
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
          int ele = 0;
          jsonify::writers::simple::write_value( writer, s, ele );
        } else {
          int this_int = iv[ row ];
          const char * this_char = lvls[ this_int -1 ];
          writer.String( this_char );
        }
        
      } else {
        jsonify::writers::simple::write_value( writer, iv, row, numeric_dates, factors_as_string );
      }
      break;
    }
    case LGLSXP: {
      Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
      jsonify::writers::simple::write_value( writer, lv, row );
      break;
    }
    default: {
      Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
      jsonify::writers::simple::write_value( writer, sv, row );
      break;
    }
    }
  }

  template< typename Writer >
  inline void write_value( Writer& writer, SEXP list_element, bool unbox = false, 
                           int digits = -1, bool numeric_dates = true,
                           bool factors_as_string = true, std::string by = "row", 
                           int row = -1   // for when we are recursing into a row of a data.frame
                             ) {
    
    int i, df_col, df_row;
    
    if( Rf_isNull( list_element ) ) {
      writer.StartObject();
      writer.EndObject();
      return;
    } 
    
    if( Rf_isMatrix( list_element ) ) {
      
      switch( TYPEOF( list_element ) ) {
      case REALSXP: {
        Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( list_element );
        return jsonify::writers::simple::write_value( writer, nm, unbox, digits, by );
        break;
      }
      case INTSXP: {
        Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( list_element );
        return jsonify::writers::simple::write_value( writer, im, unbox, by );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalMatrix lm = Rcpp::as< Rcpp::LogicalMatrix >( list_element );
        return jsonify::writers::simple::write_value( writer, lm, unbox, by );
        break;
      }
      default :{
        Rcpp::StringMatrix sm = Rcpp::as< Rcpp::StringMatrix >( list_element );
        return jsonify::writers::simple::write_value( writer, sm, unbox, by );
        break;
      }
      }
    } else if ( Rf_inherits( list_element, "data.frame" ) ) {
      
      Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( list_element );
      int n_cols = df.ncol();
      int n_rows = df.nrows();
      Rcpp::StringVector column_names = df.names();
      
      if ( by == "column") {
        writer.StartObject();
        
        for( df_col = 0; df_col < n_cols; df_col++ ) {

          const char *h = column_names[ df_col ];
          writer.String( h );
          SEXP this_vec = df[ h ];
          
          switch( TYPEOF( this_vec ) ) {
          case VECSXP: {
            write_value( writer, this_vec, unbox, digits, numeric_dates, factors_as_string, by );
            break;
          }
          default: {
            switch_vector( writer, this_vec, unbox, digits, numeric_dates, factors_as_string );
          }
          }
        }
        writer.EndObject();
        
      } else { // by == "row"
        
        if ( row >= 0 ) {
          
          for( df_col = 0; df_col < n_cols; df_col++ ) {
            
            writer.StartObject();
            
            const char *h = column_names[ df_col ];
            writer.String( h );
            SEXP this_vec = df[ h ];
            
            switch( TYPEOF( this_vec ) ) {
            case VECSXP: {
              Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
              write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by, row );
              break;
            }
            default: {
              switch_vector( writer, this_vec, unbox, digits, numeric_dates, factors_as_string, row );
            }
            } // end switch
            writer.EndObject();
          } // end for
          
        } else {
          
          writer.StartArray();
          
          for( df_row = 0; df_row < n_rows; df_row++ ) {
            writer.StartObject();
            
            for( df_col = 0; df_col < n_cols; df_col++ ) {
              
              const char *h = column_names[ df_col ];
              writer.String( h );
              SEXP this_vec = df[ h ];
              
              switch( TYPEOF( this_vec ) ) {
              case VECSXP: {
                Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
                write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by, df_row );
                break;
              }
              default: {
                switch_vector( writer, this_vec, unbox, digits, numeric_dates, factors_as_string, df_row );
              }
              }
            }
            writer.EndObject();
          } // end for
          writer.EndArray();
        } // end if
      }
      
    } else {
      
      switch( TYPEOF( list_element ) ) {
      
      case VECSXP: {
        // the case where the list item is a row of a data.frame
        // ISSUE #32
        
        Rcpp::List temp_lst = Rcpp::as< Rcpp::List >( list_element );
        bool has_names;
        
        has_names = temp_lst.hasAttribute("names");
        
        Rcpp::List lst(1);
        if( row >= 0 ) {   // we came in from a data.frame, going by-row
          // ISSUE 32
          lst[0] = temp_lst[ row ];
          
          if( temp_lst.hasAttribute("names") ) {
            Rcpp::CharacterVector templstnames = temp_lst.names();
            const char* this_name = templstnames[ row ];
            lst.names() = this_name;
          }

          write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );  
          
        } else {
          lst = temp_lst;

          int n = lst.size();
          
          if ( n == 0 ) {
            writer.StartArray();
            writer.EndArray();
            break;
          }
          
          // LIST NAMES
          Rcpp::IntegerVector int_names = Rcpp::seq(1, lst.size());
          Rcpp::CharacterVector list_names = Rcpp::as< Rcpp::CharacterVector >( int_names );
          has_names = lst.hasAttribute("names");
          
          if ( has_names ) {
            Rcpp::CharacterVector temp_names = lst.names();
            for( i = 0; i < n; i++ ) {
              list_names[i] = temp_names[i] == "" ? list_names[i] : temp_names[i];
            }
          }
          // END LIST NAMES
          
          jsonify::utils::writer_starter( writer, has_names );
          
          for ( i = 0; i < n; i++ ) {
            
            SEXP recursive_list = lst[ i ];
            if ( has_names ) {
              const char *s = list_names[ i ];
              writer.String( s );
            }
            write_value( writer, recursive_list, unbox, digits, numeric_dates, factors_as_string, by );
          }
        jsonify::utils::writer_ender( writer, has_names );
        } // end if (by row)
        break;
      }
        
      case REALSXP: {
        Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_element );
        jsonify::writers::simple::write_value( writer, nv, unbox, digits, numeric_dates );
        break;
      }
      case INTSXP: { 
        Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_element );
        jsonify::writers::simple::write_value( writer, iv, unbox, numeric_dates, factors_as_string );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
        jsonify::writers::simple::write_value( writer, lv, unbox );
        break;
      }
      case LISTSXP: {} // lists of dotted paires
      case LANGSXP: {   // language constructs (special lists)
        Rcpp::Pairlist s = Rcpp::as< Rcpp::Pairlist >( list_element );
        Rcpp::List l = Rcpp::as< Rcpp::List >( s );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      case CLOSXP: {}   // closures
      case BUILTINSXP: {}
      case SPECIALSXP: {}
      case ENVSXP: {}
      case FUNSXP: {
        Rcpp::List l = Rcpp::as< Rcpp::List >( list_element );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      default: {
        Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( list_element );
        jsonify::writers::simple::write_value( writer, sv, unbox );
        break;
      }
      }
    }
  }

} // namespace complex
} // namespace writers
} // namespace jsonify

#endif
