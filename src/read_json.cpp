#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <fstream>

#include "jsonify/from_json/api.hpp"

#include <Rcpp.h>


rapidjson::Document buffer_string( const char* file, const char* mode, int buffer_size = 1024 ) {
  
  FILE* fp = fopen(file, mode );
  
  // this crashes for large files...
  //char readBuffer[ buffer_size ];
  // ISO C++ forbids variable length array ‘readBuffer’ [-Wvla]
  // https://stackoverflow.com/a/11379442/5977215
  char *readBuffer = new char[ buffer_size ];

  rapidjson::FileReadStream is(fp, readBuffer, sizeof( readBuffer ) );
  
  rapidjson::Document d;
  d.ParseStream( is );
  fclose(fp);
  
  delete[] readBuffer;
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

// [[Rcpp::export]]
SEXP rcpp_read_ndjson_file(
    const char* file,
    const char* mode,
    bool& simplify,
    bool& fill_na
) {
  
  // https://stackoverflow.com/a/51572325/5977215
  std::ifstream infile(file);
  std::ostringstream os;
  os << '[';
  if (infile.is_open()) {
    std::string line;
    while (getline(infile, line)) {
      // using printf() in all tests for consistency
      //printf("%s", line.c_str());
      os << line.c_str();
      os << ",";
      
    }
    infile.close();
  }
  // move head back one place to overwrite the final ','
  os.seekp(-1, os.cur);
  os << ']';
  std::string json = os.str();
  return jsonify::api::from_json( json.c_str(), simplify, fill_na );
}

