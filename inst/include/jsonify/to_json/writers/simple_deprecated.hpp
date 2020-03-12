#ifndef R_JSONIFY_WRITERS_SIMPLE_DEPRECATED_H
#define R_JSONIFY_WRITERS_SIMPLE_DEPRECATED_H

#include <Rcpp.h>
#include "jsonify/to_json/utils.hpp"
#include "jsonify/to_json/writers/scalars.hpp"

using namespace rapidjson;

namespace jsonify {
namespace writers {
namespace simple {


// ---------------------------------------------------------------------------
// vectors
// ---------------------------------------------------------------------------

/*
 * for writing a single value of a vector
 */
template <typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::StringVector& sv, 
    int row
) {
  
  if ( Rcpp::StringVector::is_na( sv[ row ] ) ) {
    writer.Null();
  } else {
    const char *s = sv[ row ];
    jsonify::writers::scalars::write_value( writer, s );
  }
}

/*
 * For writing a single value of a vector
 */
template< typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::NumericVector& nv, 
    int row, 
    int digits, 
    bool numeric_dates
) {
  
  Rcpp::CharacterVector cls = jsonify::utils::getRClass( nv );
  
  if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {
    
    Rcpp::StringVector sv = jsonify::dates::date_to_string( nv );
    write_value( writer, sv, row );
    
  } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
    
    Rcpp::StringVector sv = jsonify::dates::posixct_to_string( nv );
    write_value( writer, sv, row );
    
  } else {
    if ( Rcpp::NumericVector::is_na( nv[ row ] ) ) {
      writer.Null();
    } else {
      double n = nv[ row ];
      jsonify::writers::scalars::write_value( writer, n, digits );
    }
  }
}


/*
 * For writing a single value of a vector
 */
template< typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::IntegerVector& iv, 
    int row, 
    bool numeric_dates, 
    bool factors_as_string
) {
  
  Rcpp::CharacterVector cls = jsonify::utils::getRClass( iv );
  
  
  if( !numeric_dates && jsonify::dates::is_in( "Date", cls ) ) {
    
    Rcpp::StringVector sv = jsonify::dates::date_to_string( iv );
    write_value( writer, sv, row );
  } else if ( !numeric_dates && jsonify::dates::is_in( "POSIXt", cls ) ) {
    
    Rcpp::StringVector sv = jsonify::dates::posixct_to_string( iv );
    write_value( writer, sv, row );
    
  } else if ( factors_as_string && Rf_isFactor( iv ) ) {
    
    Rcpp::StringVector lvls = iv.attr( "levels" );
    
    if (lvls.length() == 0 && iv.length() == 0 ) {
      writer.StartArray();
      writer.EndArray();
    } else if( lvls.length() == 0 ) {
      // no level s- from NA_character_ vector
      Rcpp::StringVector s(1);
      s[0] = NA_STRING;
      int ele = 0;
      write_value( writer, s, ele );
    } else {
      Rcpp::StringVector str = Rcpp::as< Rcpp::StringVector >( iv );
      write_value( writer, str, row );
    }
    
  } else {
    
    if ( Rcpp::IntegerVector::is_na( iv[ row ] ) ) {
      writer.Null();
    } else {
      int i = iv[ row ];
      jsonify::writers::scalars::write_value( writer, i );
    }
  }
}


template < typename Writer >
inline void write_value( Writer& writer, Rcpp::LogicalVector& lv, int row ) {
  if ( Rcpp::LogicalVector::is_na( lv[ row ] ) ) { 
    writer.Null();
  } else {
    bool l = lv[ row ];
    jsonify::writers::scalars::write_value( writer, l );
  }
}


// ---------------------------------------------------------------------------
// matrix values
// ---------------------------------------------------------------------------

template < typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::IntegerMatrix& mat,
    int& row, 
    bool unbox = false
) {
  
  Rcpp::IntegerVector this_row = mat(row, Rcpp::_);
  write_value( writer, this_row, unbox, true, true );  // true, true : numeric_dates, factors_as_string
}


template < typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::NumericMatrix& mat, 
    int& row, 
    bool unbox = false
) {
  
  Rcpp::NumericVector this_row = mat(row, Rcpp::_);
  write_value( writer, this_row, unbox, true, true );  // true, true : numeric_dates, factors_as_string
  
}


template < typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::StringMatrix& mat, 
    int& row, 
    bool unbox = false
) {
  
  Rcpp::StringVector this_row = mat(row, Rcpp::_);
  write_value( writer, this_row, unbox );
}


template < typename Writer >
inline void write_value(
    Writer& writer, 
    Rcpp::LogicalMatrix& mat, 
    int& row, 
    bool unbox = false
) {
  
  Rcpp::LogicalVector this_row = mat(row, Rcpp::_);
  write_value( writer, this_row, unbox );
}


/*
 * template for R SEXPs for single-row from a vector
 */
template < typename Writer >
inline void write_value(
    Writer& writer, 
    SEXP sexp, 
    int row, 
    int digits, 
    bool numeric_dates, 
    bool factors_as_string
) {
  
  switch( TYPEOF( sexp ) ) {
  case REALSXP: {
    if( Rf_isMatrix( sexp ) ) {
    Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( sexp );
    write_value( writer, nm, row  );
  } else {
    Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( sexp );
    write_value( writer, nv, row, digits, numeric_dates );
  }
  break;
  }
  case INTSXP: {
    if( Rf_isMatrix( sexp ) ) {
    Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( sexp );
    write_value( writer, im, row );
  } else {
    Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( sexp );
    // TODO( do we need factors_as_string here, or will it be sorted by the time it comes to this step?)
    write_value( writer, iv, row, numeric_dates, factors_as_string );
  }
  break;
  }
  case LGLSXP: {
    if( Rf_isMatrix( sexp ) ) {
    Rcpp::LogicalMatrix lm = Rcpp::as< Rcpp::LogicalMatrix >( sexp );
    write_value( writer, lm, row );
  } else {
    Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( sexp );
    write_value( writer, lv, row );
  }
  break;
  }
  case STRSXP: {
    if( Rf_isMatrix( sexp ) ) {
    Rcpp::StringMatrix sm = Rcpp::as< Rcpp::StringMatrix >( sexp );
    write_value( writer, sm, row );
  } else {
    Rcpp::StringVector sv = Rcpp::as< Rcpp::StringVector >( sexp );
    write_value( writer, sv, row );
  }
  break;
  }
  default: {
    Rcpp::stop("Unknown R object type");
  }
  }
}