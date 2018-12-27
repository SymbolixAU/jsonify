#ifndef JSONIFY_DATES_H
#define JSONIFY_DATES_H

#include <Rcpp.h>
#include <boost/date_time.hpp>
#include "jsonify/utils.hpp"

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
    
    //boost::local_time::tz_database tz_db;
    //tz_db.load_from_file( "../../boost/libs/date_time/data/date_time_zonespec.csv" );
    
    
    // Rcpp::StringVector tz = nv.attr( "tzone" );
    // Rcpp::Rcout << "tz: " << tz << std::endl;
    // const char * str_tz = tz[0] ;
    // boost::local_time::time_zone_ptr this_tz = tz_db.time_zone_from_region(str_tz);
    //boost::local_time::time_zone_ptr this_tz{ new boost::local_time::posix_time_zone("CET+1")};
    
    Rcpp::StringVector sv( n );
    
    for ( i = 0; i < n; i++ ) {
      Rcpp::Datetime d = nv[i];
      boost::gregorian::date dt( d.getYear(), d.getMonth(), d.getDay() );
      boost::posix_time::hours h( d.getHours() );
      boost::posix_time::minutes mins( d.getMinutes() );
      boost::posix_time::seconds sec( d.getSeconds() );
      boost::posix_time::time_duration td = h + mins + sec;
      
      //std::cout << "td " << td << std::endl;
      
      //boost::posix_time::ptime pt = boost::posix_time::ptime( dt, h );
      boost::posix_time::ptime pt = boost::posix_time::ptime( dt, td );
      
      // if there's a time zone, do this bit
      // https://theboostcpplibraries.com/boost.datetime-location-dependent-times 
      // boost::local_time::local_date_time ldt{ pt, this_tz };
      // boost::local_time::local_date_time ldt{ pt, this_tz };
      // // 
      // Rcpp::Rcout << "utc time: " << ldt.utc_time() << std::endl;
      // Rcpp::Rcout << "local time: " << ldt.local_time() << std::endl;
      
      
      std::string s = boost::posix_time::to_iso_extended_string( pt );
      sv[i] = s.c_str();
    }
    return sv;
  }


} // namespace format
} // namespace jsonify


#endif 