#ifndef JSONIFY_FORMAT_DATES_H
#define JSONIFY_FORMAT_DATES_H

#include <Rcpp.h>
#include <boost/date_time.hpp>

// [[Rcpp::depends(BH)]]

namespace jsonify {
namespace format {

  inline Rcpp::StringVector date_to_string( SEXP v, int n ) {
    
    int i;
    Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( v );
    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Date d = nv[i];
      boost::gregorian::date gd = boost::gregorian::date(d.getYear(), d.getMonth(), d.getDay());
      std::string s = boost::gregorian::to_iso_extended_string( gd );
      sv[i] = s.c_str();
    }
    return sv;
  }
  
  inline Rcpp::StringVector posixct_to_string( SEXP v, int n ) {
    
    int i;
    Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( v );
    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Datetime d = nv[i];
      boost::gregorian::date dt( d.getYear(), d.getMonth(), d.getDay() );
      boost::posix_time::hours h( d.getHours() );
      boost::posix_time::ptime pt = boost::posix_time::ptime( dt, h );
      std::string s = boost::posix_time::to_iso_extended_string( pt );
      sv[i] = s.c_str();
    }
    return sv;
  }
  

} // namespace format
} // namespace jsonify


#endif 