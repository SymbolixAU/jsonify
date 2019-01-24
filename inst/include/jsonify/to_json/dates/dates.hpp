#ifndef JSONIFY_DATES_H
#define JSONIFY_DATES_H

#include <Rcpp.h>
#include <boost/date_time.hpp>

// [[Rcpp::depends(BH)]]

namespace jsonify {
namespace dates {

  inline bool is_in( const char* x, Rcpp::CharacterVector v ) {
    int n = v.size();
    int i;
    for( i = 0; i < n; i++ ) {
      if( v[i] == x ) {
        return true;
      }
    }
    return false;
  }

  inline Rcpp::StringVector date_to_string( Rcpp::IntegerVector& iv ) {
    
    int i;
    int n = iv.size();
    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Date d = iv[i];
      boost::gregorian::date gd = boost::gregorian::date(d.getYear(), d.getMonth(), d.getDay());
      std::string s = boost::gregorian::to_iso_extended_string( gd );
      sv[i] = s.c_str();
    }
    return sv;
  }

  inline Rcpp::StringVector date_to_string( Rcpp::NumericVector& nv ) {
    
    int i;
    int n = nv.size();
    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Date d = nv[i];
      boost::gregorian::date gd = boost::gregorian::date(d.getYear(), d.getMonth(), d.getDay());
      std::string s = boost::gregorian::to_iso_extended_string( gd );
      sv[i] = s.c_str();
    }
    return sv;
  }

  inline Rcpp::StringVector posixct_to_string( Rcpp::IntegerVector& iv ) {
    
    int i;
    int n = iv.size();

    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Datetime d = iv[i];
      boost::gregorian::date dt( d.getYear(), d.getMonth(), d.getDay() );
      boost::posix_time::hours h( d.getHours() );
      boost::posix_time::minutes mins( d.getMinutes() );
      boost::posix_time::seconds sec( d.getSeconds() );
      boost::posix_time::time_duration td = h + mins + sec;
      
      boost::posix_time::ptime pt = boost::posix_time::ptime( dt, td );
      
      std::string s = boost::posix_time::to_iso_extended_string( pt );
      sv[i] = s.c_str();
    }
    return sv;
  }
  
  inline Rcpp::StringVector posixct_to_string( Rcpp::NumericVector& nv ) {
    
    int i;
    int n = nv.size();
    
    boost::local_time::tz_database tz_db;
    
    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Datetime d = nv[i];
      boost::gregorian::date dt( d.getYear(), d.getMonth(), d.getDay() );
      boost::posix_time::hours h( d.getHours() );
      boost::posix_time::minutes mins( d.getMinutes() );
      boost::posix_time::seconds sec( d.getSeconds() );
      boost::posix_time::time_duration td = h + mins + sec;
      
      boost::posix_time::ptime pt = boost::posix_time::ptime( dt, td );
      
      std::string s = boost::posix_time::to_iso_extended_string( pt );
      sv[i] = s.c_str();
    }
    return sv;
  }


} // namespace format
} // namespace jsonify


#endif 
