#ifndef R_JSONIFY_WRITERS_H
#define R_JSONIFY_WRITERS_H

#include <Rcpp.h>
#include "jsonify/to_json/utils.hpp"
#include "jsonify/to_json/dates/dates.hpp"
#include <math.h>

using namespace rapidjson;

namespace jsonify {
namespace writers {

  // ---------------------------------------------------------------------------
  // scalar values
  // ---------------------------------------------------------------------------
  template <typename Writer>
  inline void write_value( Writer& writer, const char* value ) {
    writer.String( value );
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, int& value ) {
     if( std::isnan( value ) ) {
      writer.Null();
    } else {
      writer.Int( value );
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, double& value, int digits = -1 ) {
    
    if(std::isnan( value ) ) {
      writer.Null();
    } else if ( std::isinf( value ) ) {
      
      std::string str = std::to_string( value );
      // https://stackoverflow.com/a/14744016/5977215
      if ( str[0] == '-') { 
        str[1] = toupper( str[1] );
      } else {
        str[0] = toupper(str[0]);
      }
      writer.String( str.c_str() );
    } else {
      
      if ( digits >= 0 ) {
        double e = std::pow( 10.0, digits );
        value = round( value * e ) / e;
      }
      writer.Double( value );
    }
  }
  
  template< typename Writer> 
  inline void write_value( Writer& writer, bool& value, int digits = -1 ) {
    writer.Bool( value );
  }
  
  // ---------------------------------------------------------------------------
  // vectors
  // ---------------------------------------------------------------------------
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::StringVector& sv, bool unbox = false ) {
    int n = sv.size();
    bool will_unbox = jsonify::utils::should_unbox( n, unbox );
    jsonify::utils::start_array( writer, will_unbox );
    
    for ( int i = 0; i < n; i++ ) {
      if (Rcpp::StringVector::is_na( sv[i] ) ) {
        writer.Null();
      } else{
        write_value( writer, sv[i] );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  /*
   * for writing a single value of a vector
   */
  template <typename Writer >
  inline void write_value( Writer& writer, Rcpp::StringVector& sv, size_t row, bool unbox = false ) {
    if ( Rcpp::StringVector::is_na( sv[ row ] ) ) {
      writer.Null();
    } else {
      const char *s = sv[ row ];
      write_value( writer, s );
    }
  }
  
  template< typename Writer>
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv, bool unbox = false, int digits = -1, bool numeric_dates = true ) {

    Rcpp::CharacterVector cls = jsonify::utils::getRClass( nv );
    
    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::date_to_string( nv );
      write_value( writer, sv, unbox );
      
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( nv );
      write_value( writer, sv, unbox );
      
    } else {
    
      int n = nv.size();
      Rcpp::Rcout << "nv.size: " << n << std::endl;
      Rcpp::Rcout << "unbox: " << unbox << std::endl; 
      bool will_unbox = jsonify::utils::should_unbox( n, unbox );
      
      jsonify::utils::start_array( writer, will_unbox );
    
      for ( int i = 0; i < n; i++ ) {
        if( Rcpp::NumericVector::is_na( nv[i] ) ) {
          writer.Null();
        } else {
          write_value( writer, nv[i], digits );
        }
      }
    jsonify::utils::end_array( writer, will_unbox );
    }
  }
  
  /*
   * For writing a single value of a vector
   */
  template< typename Writer >
  inline void write_value( Writer& writer, Rcpp::NumericVector& nv, size_t row, bool unbox = false, int digits = -1, bool numeric_dates = true ) {

    Rcpp::Rcout << "writing nv with row: " << row << std::endl;
    
    Rcpp::CharacterVector cls = jsonify::utils::getRClass( nv );
    
    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {

      Rcpp::StringVector sv = jsonify::dates::date_to_string( nv );
      write_value( writer, sv, row, unbox );
      
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( nv );
      write_value( writer, sv, row, unbox );
      
    } else {
      if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) {
        writer.Null();
      } else {
        double n = nv[ row ];
        write_value( writer, n, digits );
      }
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv, bool unbox = false, bool numeric_dates = true ) {
    
    Rcpp::CharacterVector cls = jsonify::utils::getRClass( iv );

    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {

      Rcpp::StringVector sv = jsonify::dates::date_to_string( iv );
      write_value( writer, sv, unbox );
      
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( iv );
      write_value( writer, sv, unbox );
      
    } else {
    
      int n = iv.size();
      bool will_unbox = jsonify::utils::should_unbox( n, unbox );
      jsonify::utils::start_array( writer, will_unbox );
      
      for ( int i = 0; i < n; i++ ) {
        if( Rcpp::IntegerVector::is_na( iv[i] ) ) {
          writer.Null();
        } else {
          write_value( writer, iv[i] );
        }
      }
      jsonify::utils::end_array( writer, will_unbox );
    }
  }
  
  /*
   * For writing a single value of a vector
   */
  template< typename Writer >
  inline void write_value( Writer& writer, Rcpp::IntegerVector& iv, size_t row, 
                           bool unbox = false, bool numeric_dates = false ) {
    
    Rcpp::CharacterVector cls = jsonify::utils::getRClass( iv );
    
    
    if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::date_to_string( iv );
      write_value( writer, sv, row, unbox );
    } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
      
      Rcpp::StringVector sv = jsonify::dates::posixct_to_string( iv );
      write_value( writer, sv, row, unbox );
      
    } else {
    
      if ( Rcpp::IntegerVector::is_na( iv[ row ] ) ) {
        writer.Null();
      } else {
        int i = iv[ row ];
        write_value( writer, i );
      }
    }
  }
  
  template <typename Writer>
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, bool unbox = false ) {
    int n = lv.size();
    bool will_unbox = jsonify::utils::should_unbox( n, unbox );
    jsonify::utils::start_array( writer, will_unbox );
    
    for ( int i = 0; i < n; i++ ) {
      if (Rcpp::LogicalVector::is_na( lv[i] ) ) {
        writer.Null();
      } else {
        bool l = lv[i];             // required for logical vectors
        write_value( writer, l );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, size_t row, bool unbox = false ) {
    if ( Rcpp::LogicalVector::is_na( lv[ row ] ) ) { 
      writer.Null();
    } else {
      bool l = lv[ row ];
      write_value( writer, l );
    }
  }
  
  // ---------------------------------------------------------------------------
  // matrix values
  // ---------------------------------------------------------------------------
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::IntegerMatrix& mat, bool unbox = false,
                           std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int n;
    int i;
    
    if ( by == "row" ) {
      n = mat.nrow();
      for ( i = 0; i < n; i++ ) {
        Rcpp::IntegerVector this_row = mat(i, Rcpp::_);
        write_value( writer, this_row, unbox );
      }
    } else { // by == "column"
      n = mat.ncol();
      for( i = 0; i < n; i++ ) {
        Rcpp::IntegerVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::NumericMatrix& mat, bool unbox = false, 
                           int digits = -1, std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    
    int n;
    int i;
    if ( by == "row" ) {
      n = mat.nrow();
      for ( i = 0; i < n; i++ ) {
        Rcpp::NumericVector this_row = mat(i, Rcpp::_);
        write_value( writer, this_row, unbox, digits );
      }
    } else { // by == "column"
      n = mat.ncol();
      for( i = 0; i < n; i++ ) {
        Rcpp::NumericVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox, digits );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::CharacterMatrix& mat, bool unbox = false,
                           std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int i;
    int n;
    
    if( by == "row" ) {
      n = mat.nrow();
      for ( i = 0; i < n; i++ ) {
        Rcpp::StringVector this_row = mat( i, Rcpp::_ );
        write_value( writer, this_row, unbox );
      }
    } else { // by == column
      n = mat.ncol();
      for ( i = 0; i < n; i++ ) {
        Rcpp::StringVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox );
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }
  
  
  template < typename Writer >
  inline void write_value( Writer& writer, Rcpp::LogicalMatrix& mat, bool unbox = false, 
                           std::string by = "row" ) {
    
    bool will_unbox = false;
    jsonify::utils::start_array( writer, will_unbox );
    int i;
    int n;
    
    if( by == "row" ) {
      n = mat.nrow();
      
      for ( i = 0; i < n; i++ ) {
        Rcpp::LogicalVector this_row = mat(i, Rcpp::_);
        write_value( writer, this_row, unbox );
      }
    } else { // by == "column;
      n = mat.ncol();
      
      for( i = 0; i < n; i++ ) {
        Rcpp::LogicalVector this_col = mat( Rcpp::_, i );
        write_value( writer, this_col, unbox);
      }
    }
    jsonify::utils::end_array( writer, will_unbox );
  }

  // ---------------------------------------------------------------------------
  // List
  // ---------------------------------------------------------------------------
  
  // // When a colunn of a data.frame is a list, and we're parsing by row
  // template< typename Writer >
  // inline void write_value( Writer& writer, Rcpp::List lst, size_t row, bool unbox = false, 
  //   int digits = -1, bool numeric_dates = true, bool factors_as_string = true, 
  //   std::string by = "row" ) {
  //     size_t n, i;
  //     n = lst.size();
  //     
  //     for( i = 0; i < n; i++ ) {
  //       
  //       SEXP this_vec = lst[ i ];
  //       
  //       switch( TYPEOF( this_vec ) ) {
  //       case VECSXP: {
  //         Rcpp::Rcout << "writing list" << std::endl;
  //         Rcpp::List recursive_list = Rcpp::as< Rcpp::List >( this_vec );
  //         // TODO
  //         // If it's a recursive list, we no longer use the row?
  //         write_value( writer, recursive_list, unbox, digits, numeric_dates, factors_as_string, by );
  //         break;
  //       }
  //       case REALSXP: {
  //         Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
  //         write_value( writer, nv, row, unbox, digits, numeric_dates );
  //         break;
  //       }
  //       case INTSXP: {
  //         Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_vec );
  //         if( factors_as_string && Rf_isFactor( this_vec ) ) {
  //           Rcpp::CharacterVector lvls = iv.attr("levels");
  //           if ( lvls.length() == 0 ) {
  //             // no levls - from NA_character_ vector
  //             
  //             Rcpp::StringVector s(1);
  //             s[0] = NA_STRING;
  //             write_value( writer, s, 0, unbox );
  //           } else {
  //             int this_int = iv[ row ];
  //             const char * this_char = lvls[ this_int -1 ];
  //             write_value( writer, this_char );
  //           }
  //           
  //         } else {
  //           write_value( writer, iv, row, unbox, numeric_dates );
  //         }
  //         break;
  //       }
  //       case LGLSXP: {
  //         Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
  //         write_value( writer, lv, row, unbox );
  //         break;
  //       }
  //       default: {
  //         Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
  //         write_value( writer, sv, row, unbox );
  //         break;
  //       }
  //       }
  //       }
  //     }
  
  template< typename Writer >
  inline void write_value( Writer& writer, SEXP list_element, bool unbox = false, 
                           int digits = -1, bool numeric_dates = true,
                           bool factors_as_string = true, std::string by = "row", 
                           int row = -1   // for when we are recursing into a row of a data.frame
                          )
    {
    
    Rcpp::Rcout << "entered function" << std::endl;
    
    size_t i, j;
    
    size_t df_col, df_row;
    
    if( Rf_isNull( list_element ) ) {
      Rcpp::Rcout << "empty" << std::endl;
      writer.StartObject();
      writer.EndObject();
      return;
    } 
    
    if( Rf_isMatrix( list_element ) ) {
      Rcpp::Rcout << "matrix" << std::endl;
      
      switch( TYPEOF( list_element ) ) {
      case REALSXP: {
        Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( list_element );
        return write_value( writer, nm, unbox, digits, by );
        break;
      }
      case INTSXP: {
        Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( list_element );
        return write_value( writer, im, unbox, by );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalMatrix lm = Rcpp::as< Rcpp::LogicalMatrix >( list_element );
        return write_value( writer, lm, unbox, by );
        break;
      }
      default :{
        Rcpp::StringMatrix sm = Rcpp::as< Rcpp::StringMatrix >( list_element );
        return write_value( writer, sm, unbox, by );
        break;
      }
      }
    } else if ( Rf_inherits( list_element, "data.frame" ) ) {

      Rcpp::Rcout << "writing data.frame" << std::endl;
      
      Rcpp::DataFrame df = Rcpp::as< Rcpp::DataFrame >( list_element );
      size_t n_cols = df.ncol();
      size_t n_rows = df.nrows();
      Rcpp::StringVector column_names = df.names();
      
      //writer.StartArray();
      
      if ( by == "column") {
        Rcpp::Rcout << "column" << std::endl;
        writer.StartObject();
        
        for( df_col = 0; df_col < n_cols; df_col++ ) {
          
          
          const char *h = column_names[ df_col ];
          write_value( writer, h );
          SEXP this_col = df[ h ];
          
          switch( TYPEOF( this_col ) ) {
          case VECSXP: {
            //Rcpp::List lst = Rcpp::as< Rcpp::List >( this_col );
            write_value( writer, this_col, unbox, digits, numeric_dates, factors_as_string, by );
            break;
          }
          case REALSXP: {
            Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_col );
            write_value( writer, nv, unbox, digits, numeric_dates );
            break;
          }
          case INTSXP: {
            Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( this_col );
            write_value( writer, iv, unbox, numeric_dates );
            break;
          }
          case LGLSXP: {
            Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_col );
            write_value( writer, lv, unbox );
            break;
          }
          default: {
            Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_col );
            write_value( writer, sv, unbox );
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
          
          Rcpp::Rcout << "row specified " << std::endl;
          Rcpp::Rcout << "row: " << row << std::endl;
          
          for( df_col = 0; df_col < n_cols; df_col++ ) {
            
            writer.StartObject();
            
            const char *h = column_names[ df_col ];
            write_value( writer, h );
            SEXP this_vec = df[ h ];
            
            switch( TYPEOF( this_vec ) ) {
            case VECSXP: {
              Rcpp::Rcout << "writing list" << std::endl;
              Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
              // need to iterate over lst...
              // int k = lst.size();
              // Rcpp::Rcout << "k: " << k << std::endl;
              // TOOD( we've got a list, but, we only want the ith row of that list )
              // So we need to have another method which accepts a list, and a ROW value, 
              // and iterates over the list (cols), and only picks out the ith row
              
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
              write_value( writer, nv, row, unbox, digits, numeric_dates );
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
                  write_value( writer, s, 0, unbox );
                } else {
                  int this_int = iv[ row ];
                  const char * this_char = lvls[ this_int -1 ];
                  write_value( writer, this_char );
                }
                
              } else {
                write_value( writer, iv, row, unbox, numeric_dates );
              }
              break;
            }
            case LGLSXP: {
              Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
              write_value( writer, lv, row, unbox );
              break;
            }
            default: {
              Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
              write_value( writer, sv, row, unbox );
              break;
            }
            } // end swithc
            writer.EndObject();
          } // end for
          
        } else {
          
          Rcpp::Rcout << "row not specified" << std::endl;
          writer.StartArray();
        
          for( df_row = 0; df_row < n_rows; df_row++ ) {
            writer.StartObject();
            
            Rcpp::Rcout << "df_row: " << df_row << std::endl;
          
            for( df_col = 0; df_col < n_cols; df_col++ ) {
              
              const char *h = column_names[ df_col ];
              write_value( writer, h );
              SEXP this_vec = df[ h ];
              
              switch( TYPEOF( this_vec ) ) {
              case VECSXP: {
                Rcpp::Rcout << "writing list" << std::endl;
                Rcpp::List lst = Rcpp::as< Rcpp::List >( this_vec );
                // need to iterate over lst...
                // int k = lst.size();
                // Rcpp::Rcout << "k: " << k << std::endl;
                // TOOD( we've got a list, but, we only want the ith row of that list )
                // So we need to have another method which accepts a list, and a ROW value, 
                // and iterates over the list (cols), and only picks out the ith row
                
                write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by, df_row );
                
                //write_value( writer, lst, unbox, digits, numeric_dates, factors_as_string, by );
                // for ( int k = 0; k <= lst.size(); k++ ) {
                //   SEXP s = lst[ k ];
                //   write_value( writer, s, unbox, digits, numeric_dates, factors_as_string, by );
                // }
                break;
              }
              case REALSXP: {
                Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( this_vec );
                write_value( writer, nv, df_row, unbox, digits, numeric_dates );
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
                    write_value( writer, s, 0, unbox );
                  } else {
                    int this_int = iv[ df_row ];
                    const char * this_char = lvls[ this_int -1 ];
                    write_value( writer, this_char );
                  }
  
                } else {
                  write_value( writer, iv, df_row, unbox, numeric_dates );
                }
                break;
              }
              case LGLSXP: {
                Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( this_vec );
                write_value( writer, lv, df_row, unbox );
                break;
              }
              default: {
                Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( this_vec );
                write_value( writer, sv, df_row, unbox );
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
    
      Rcpp::Rcout << "list element " << std::endl;
      switch( TYPEOF( list_element ) ) {
      
      case VECSXP: {
        Rcpp::List lst = Rcpp::as< Rcpp::List >( list_element );
        size_t n = lst.size();
        
        if ( n == 0 ) {
          writer.StartArray();
          writer.EndArray();
          break;
        }
        
        // LIST NAMES
        Rcpp::IntegerVector int_names = Rcpp::seq(1, lst.size());
        Rcpp::CharacterVector list_names = Rcpp::as< Rcpp::CharacterVector >( int_names );
        bool has_names = Rf_isNull(lst.names()) ? false : true;
        
        if ( has_names ) {
          Rcpp::CharacterVector temp_names = lst.names();
          for( i = 0; i < n; i++ ) {
            list_names[i] = temp_names[i] == "" ? list_names[i] : temp_names[i];
          }
        }
        // END LIST NAMES
        
        jsonify::utils::writer_starter( writer, has_names );
        
        for ( i = 0; i < n; i++ ) {
          
          Rcpp::Rcout << "list loop i - " << i << ", n: " << n << std::endl;
          
          SEXP recursive_list = lst[ i ];
          if ( has_names ) {
            const char *s = list_names[ i ];
            write_value( writer, s );
          }
          Rcpp::Rcout << "recursive list " << std::endl;
          write_value( writer, recursive_list, unbox, digits, numeric_dates, factors_as_string, by );
        }
        jsonify::utils::writer_ender( writer, has_names );
        break;
      }
        
      case REALSXP: {
        Rcpp::Rcout << "REALSXP" << std::endl;
        Rcpp::Rcout << "row: " << row << std::endl;
        Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( list_element );
        // if ( row >= 0 ) {
        //    write_value( writer, nv, row, unbox, digits, numeric_dates );
        // } else {
          write_value( writer, nv, unbox, digits, numeric_dates );
        // }

        break;
      }
      case INTSXP: { 
        Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( list_element );
        write_value( writer, iv, unbox, numeric_dates );
        break;
      }
      case LGLSXP: {
        Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( list_element );
        write_value( writer, lv, unbox );
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
        write_value( writer, sv, unbox );
        break;
      }
      }
    }
  }

} // namespace writers
} // namespace jsonify

#endif
