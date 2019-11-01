#include <Rcpp.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <fstream>

#include "jsonify/from_json/api.hpp"

#include <Rcpp.h>

rapidjson::Document buffer_string( const char* file, const char* mode, int buffer_size = 1024 ) {
  
  FILE* fp = fopen(file, mode );
  char readBuffer[ buffer_size ];
  
  rapidjson::FileReadStream is(fp, readBuffer, sizeof( readBuffer ) );
  
  rapidjson::Document d;
  d.ParseStream( is );
  fclose(fp);
  
  return d;
}

// [[Rcpp::export]]
SEXP rcpp_read_json_file(
  const char* file,
  const char* mode,
  bool& simplify,
  bool& fill_na,
  int buffer_size = 1024
) {
  rapidjson::Document d = buffer_string( file, mode, buffer_size );
  return jsonify::api::from_json( d, simplify, fill_na );
}