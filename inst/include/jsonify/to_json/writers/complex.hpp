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

  template< typename Writer >
  inline void write_value( Writer& writer, SEXP list_element, bool unbox = false, 
                           int digits = -1, bool numeric_dates = true,
                           bool factors_as_string = true, std::string by = "row", 
                           int row = -1   // for when we are recursing into a row of a data.frame
  )
  {
    
    // Rcpp::Rcout << "entered function" << std::endl;
    // Rcpp::Rcout << "with row: " << row << std::endl;
    
    size_t i, j;
    
    size_t df_col, df_row;
    
    if( Rf_isNull( list_element ) ) {
      // Rcpp::Rcout << "empty" << std::endl;
      writer.StartObject();
      writer.EndObject();
      return;
    } 
    
    if( Rf_isMatrix( list_element ) ) {
      // Rcpp::Rcout << "matrix" << std::endl;
      
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
      
      // Rcpp::Rcout << "writing data.frame" << std::endl;
      
      Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( list_element );
      size_t n_cols = df.ncol();
      size_t n_rows = df.nrows();
      Rcpp::StringVector column_names = df.names();
      
      //writer.StartArray();
      
      if ( by == "column") {
        // Rcpp::Rcout << "column" << std::endl;
        writer.StartObject();
        
        for( df_col = 0; df_col < n_cols; df_col++ ) {
          
          
          const char *h = column_names[ df_col ];
          jsonify::writers::simple::write_value( writer, h );
          SEXP this_col = df[ h ];
          
          switch( TYPEOF( this_col ) ) {
          case VECSXP: {
            //Rcpp::List lst = Rcpp::as< Rcpp::List >( this_col );
            write_value( writer, this_col, unbox, digits, numeric_dates, factors_as_string, by );
            break;
          }
          case REALSXP: {
            Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_col );
            jsonify::writers::simple::write_value( writer, nv, unbox, digits, numeric_dates );
            break;
          }
          case INTSXP: {
            Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_col );
            jsonify::writers::simple::write_value( writer, iv, unbox, numeric_dates );
            break;
          }
          case LGLSXP: {
            Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_col );
            jsonify::writers::simple::write_value( writer, lv, unbox );
            break;
          }
          default: {
            Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_col );
            jsonify::writers::simple::write_value( writer, sv, unbox );
            break;
          }
          }
        }
        writer.EndObject();
        
      } else { // by == "row"
        
        // TODO( if the row is supplied, it's because we're iterating into another data.frame )??
        // so we don't loop over rows, but instead just extract the row of interest?
        // AND we are in the 'data.frame' section, so we know (?) the row value will work
        if ( row >= 0 ) {
          
          // Rcpp::Rcout << "row specified " << std::endl;
          // Rcpp::Rcout << "row: " << row << std::endl;
          
          for( df_col = 0; df_col < n_cols; df_col++ ) {
            
            writer.StartObject();
            
            const char *h = column_names[ df_col ];
            jsonify::writers::simple::write_value( writer, h );
            SEXP this_vec = df[ h ];
            
            switch( TYPEOF( this_vec ) ) {
            case VECSXP: {
              //Rcpp::Rcout << "writing list" << std::endl;
              Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
              // need to iterate over lst...
              // int k = lst.size();
              // Rcpp::Rcout << "k: " << k << std::endl;
              // TOOD( we've got a list, but, we only want the ith row of that list )
              // So we need to have another method which accepts a list, and a ROW value, 
              // and iterates over the list (cols), and only picks out the ith row
              
              //Rcpp::Rcout << "row: " << row << std::endl;
              
              write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by, row );
              //write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
              // for ( int k = 0; k <= lst.size(); k++ ) {
              //   SEXP s = lst[ k ];
              //   write_value( writer, s, unbox, digits, numeric_dates, factors_as_string, by );
              // }
              break;
            }
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
                jsonify::writers::simple::write_value( writer, iv, row, unbox, numeric_dates );
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
            } // end swithc
            writer.EndObject();
          } // end for
          
        } else {
          
          // Rcpp::Rcout << "row not specified" << std::endl;
          writer.StartArray();
          
          for( df_row = 0; df_row < n_rows; df_row++ ) {
            writer.StartObject();
            
            // Rcpp::Rcout << "df_row: " << df_row << std::endl;
            
            for( df_col = 0; df_col < n_cols; df_col++ ) {
              
              const char *h = column_names[ df_col ];
              jsonify::writers::simple::write_value( writer, h );
              SEXP this_vec = df[ h ];
              
              switch( TYPEOF( this_vec ) ) {
              case VECSXP: {
                // Rcpp::Rcout << "writing list" << std::endl;
                Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
                // need to iterate over lst...
                // int k = lst.size();
                // Rcpp::Rcout << "k: " << k << std::endl;
                // TOOD( we've got a list, but, we only want the ith row of that list )
                // So we need to have another method which accepts a list, and a ROW value, 
                // and iterates over the list (cols), and only picks out the ith row
                
                // Rcpp::Rcout << "df_row: " << df_row << std::endl;
                //if( row >= 0 ) {
                //  Rcpp::List lst_interior = lst[ row ];
                //  write_value( writer, lst_interior, unbox, digits, numeric_dates, factors_as_string, by );
                //} else {
                write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by, df_row );
                //}
                //write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
                // for ( int k = 0; k <= lst.size(); k++ ) {
                //   SEXP s = lst[ k ];
                //   write_value( writer, s, unbox, digits, numeric_dates, factors_as_string, by );
                // }
                break;
              }
              case REALSXP: {
                Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
                jsonify::writers::simple::write_value( writer, nv, df_row, unbox, digits, numeric_dates );
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
                    int this_int = iv[ df_row ];
                    const char * this_char = lvls[ this_int -1 ];
                    jsonify::writers::simple::write_value( writer, this_char );
                  }
                  
                } else {
                  jsonify::writers::simple::write_value( writer, iv, df_row, unbox, numeric_dates );
                }
                break;
              }
              case LGLSXP: {
                Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
                jsonify::writers::simple::write_value( writer, lv, df_row, unbox );
                break;
              }
              default: {
                Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
                jsonify::writers::simple::write_value( writer, sv, df_row, unbox );
                break;
              }
              }
            }
            writer.EndObject();
          } // end for
          writer.EndArray();
        } // end if
      }
      //writer.EndArray();
      
    } else {
      
      // Rcpp::Rcout << "list element " << std::endl;
      switch( TYPEOF( list_element ) ) {
      
      case VECSXP: {
        // Rcpp::Rcout << "list elelment is another list " << std::endl;
        // TODO( handle the case where the list item is a row of a data.frame)
        // ISSUE #32
        
        Rcpp::List temp_lst = Rcpp::as< Rcpp::List >( list_element );
        bool has_names;
        
        has_names = temp_lst.hasAttribute("names");
        
        Rcpp::List lst(1);
        if( row >= 0 ) {   // we came in from a data.frame, going by-row
          // if operating 'by row' on a list, we need to convert the lsit elements in the
          // row to a vector, but some how maintain the 'list-iness' of it. 
          lst = temp_lst[ row ];

          write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );  
          // Rcpp::Rcout << "lst.size: " << lst.size() << std::endl;
          // Rcpp::CharacterVector temp_list_names;
          // 
          // if (has_names ) {
          //   Rcpp::Rcout << "has names: " << has_names << std::endl;
          //   temp_list_names = temp_lst.names();
          //   Rcpp::Rcout << temp_list_names << std::endl;
          //   //std::string t = temp_list_names[ row ];
          //   //lst.names() = temp_list_names[ row ];
          //   //lst.names() = "test";
          //   Rcpp::Rcout << "assigned names " << std::endl;
          // }
          // 
          // // TODO( rather than access lst[0], we need to recurse into the list again )
          // // because it might be a list of lists of lists of lists...
          // // but keeping the 'row' structure
          // // so, we need to loop through each lst element, and call write_values
          // // but, as we've already accessed the correct 'row' from temp_list[ row ]
          // // can we just 'write_values as per usual?
          // 
          // int t = TYPEOF( lst[0] );
          // Rcpp::Rcout << "list element types: " << t << std::endl;
          // 
          // switch( TYPEOF( lst[0] ) ) { // in a data.frame, by-row, all liste elements will be the same type?
          // case INTSXP: {
          //   Rcpp::Rcout << "creating integer vector" << std::endl;
          //   Rcpp::Rcout << "lst.size() " << lst.size() << std::endl;
          //   Rcpp::IntegerVector iv( lst.size() );
          //   // TODO( if it's a recursive list... )
          //   Rcpp::List innerList = lst[0];
          //   Rcpp::Rcout << "innerlist size: " << innerList.size() << std::endl;
          //   for( i = 0; i < lst.size(); i++ ) {
          //     iv[i] = lst[i];
          //   }
          //   Rcpp::Rcout << iv << std::endl;
          //   Rcpp::List lst_iv(1);
          //   lst_iv[0] = iv;
          //   Rcpp::Rcout << "temp_list_names " << std::endl;
          //   Rcpp::Rcout << "row: " << row << std::endl;
          //   if( has_names ) {
          //     const char* this_name = temp_list_names[ row ];
          //     lst_iv.names() = this_name;
          //   }
          //   write_value( writer, lst_iv, unbox, digits, numeric_dates, factors_as_string, by );
          //   break;
          // }
          // case REALSXP: {
          //   Rcpp::NumericVector nv( lst.size() );
          //   for( i = 0; i < lst.size(); i++ ) {
          //     nv[i] = lst[i];
          //   }
          //   //lst[0] = nv;
          //   Rcpp::List lst_nv(1);
          //   lst_nv[0] = nv;
          //   if( has_names ) {
          //     const char* this_name = temp_list_names[ row ];
          //     lst_nv.names() = this_name;
          //   }
          //   write_value( writer, lst_nv, unbox, digits, numeric_dates, factors_as_string, by );
          //   break;
          // }
          // default: {
          //   Rcpp::StringVector sv( lst.size() );
          //   for( i = 0; i < lst.size(); i++ ) {
          //     std::string s = lst[i];
          //     sv[i] = s;
          //   }
          //   Rcpp::List lst_sv(1);
          //   lst_sv[0] = sv;
          //   if( has_names ) {
          //     const char* this_name = temp_list_names[ row ];
          //     lst_sv.names() = this_name;
          //   }
          //   write_value( writer, lst_sv, unbox, digits, numeric_dates, factors_as_string, by );
          //   break;
          // }
          // }
          
          //Rcpp::StringVector nmes = lst.names();
          //Rcpp::Rcout << nmes << std::endl;
          //unbox = true;
        } else {
          lst = temp_lst;
        //}
          size_t n = lst.size();
          
          if ( n == 0 ) {
            writer.StartArray();
            writer.EndArray();
            break;
          }
          
          // LIST NAMES
          Rcpp::IntegerVector int_names = Rcpp::seq(1, lst.size());
          Rcpp::CharacterVector list_names = Rcpp::as< Rcpp::CharacterVector >( int_names );
          //bool has_names = Rf_isNull(lst.names()) ? false : true;
          has_names = lst.hasAttribute("names");
          
          
          if ( has_names ) {
            Rcpp::CharacterVector temp_names = lst.names();
            Rcpp::Rcout << "temp_names: " << temp_names << std::endl;
            for( i = 0; i < n; i++ ) {
              list_names[i] = temp_names[i] == "" ? list_names[i] : temp_names[i];
            }
          }
          // END LIST NAMES
          
          jsonify::utils::writer_starter( writer, has_names );
          
          for ( i = 0; i < n; i++ ) {
            
            // Rcpp::Rcout << "list loop i - " << i << ", n: " << n << std::endl;
            
            SEXP recursive_list = lst[ i ];
            if ( has_names ) {
              const char *s = list_names[ i ];
              jsonify::writers::simple::write_value( writer, s );
            }
            //Rcpp::Rcout << "recursive list " << std::endl;
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
        jsonify::writers::simple::write_value( writer, iv, unbox, numeric_dates );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
        jsonify::writers::simple::write_value( writer, lv, unbox );
        break;
      }
      case LISTSXP: { // lists of dotted paires
        Rcpp::Pairlist s = Rcpp::as< Rcpp::Pairlist >( list_element );
        Rcpp::List l = Rcpp::as< Rcpp::List >( s );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      case LANGSXP: {   // language constructs (special lists)
        Rcpp::Pairlist s = Rcpp::as< Rcpp::Pairlist >( list_element );
        Rcpp::List l = Rcpp::as< Rcpp::List >( s );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      case CLOSXP: {   // closures
        Rcpp::List l = Rcpp::as< Rcpp::List >( list_element );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      case BUILTINSXP: {
        Rcpp::List l = Rcpp::as< Rcpp::List >( list_element );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      case SPECIALSXP: {
        Rcpp::List l = Rcpp::as< Rcpp::List >( list_element );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
      case ENVSXP: {
        Rcpp::List l = Rcpp::as< Rcpp::List >( list_element );
        write_value( writer, l, unbox, digits, numeric_dates, factors_as_string, by );
        break;
      }
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
