
[![Travis build
status](https://travis-ci.org/SymbolixAU/jsonify.svg?branch=master)](https://travis-ci.org/SymbolixAU/jsonify)
[![Coverage
status](https://codecov.io/gh/SymbolixAU/jsonify/branch/master/graph/badge.svg)](https://codecov.io/github/SymbolixAU/jsonify?branch=master)

# jsonify

jsonify converts R objects to JSON.

### Aren’t there already data.frame to JSON converters?

Yep.

### So why did you build this one?

Because I wanted it available at the source ( C++ ) level for
integrating into other packages.

### Is it fast

It does alright

``` r

library(microbenchmark)
library(jsonlite)

n <- 1e7
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
)

microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( df )
  },
  jsonify = {
    js <- jsonify::to_json( df )
  },
  times = 3
)
#  Unit: seconds
#       expr       min        lq     mean    median       uq      max neval
#   jsonlite 22.585821 26.050425 29.05018 29.515029 32.28236 35.04969     3
#    jsonify  9.257935  9.315703 11.57512  9.373472 12.73372 16.09396     3

n <- 1e6
x <- rnorm(n = n)
microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( x )
  },
  jsonify = {
    js <- jsonify::to_json( x )
  },
  times = 5
)
#  Unit: milliseconds
#       expr      min       lq     mean   median       uq      max neval
#   jsonlite 136.7371 138.4160 146.2242 147.6025 153.9232 154.4420     5
#    jsonify 150.4853 150.8432 152.1658 151.5055 153.3850 154.6102     5

n <- 1e4
x <- list(
  x = rnorm(n = n)
  , y = list(x = rnorm(n = n))
  , z = list( list( x = rnorm(n = n)))
  , xx = rnorm(n = n)
  , yy = data.frame(
      id = 1:n
      , value = sample(letters, size = n, replace = T)
      , val2 = rnorm(n = n)
      , log = sample(c(T,F), size = n, replace = T)
    )
)

microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( x )
  },
  jsonify = {
    js <- jsonify::to_json( x )
  },
  times = 5
)
#  Unit: milliseconds
#       expr       min        lq     mean    median        uq      max neval
#   jsonlite 20.686503 20.816496 21.82861 20.948203 21.741090 24.95077     5
#    jsonify  7.655508  8.037006  8.64225  8.226437  8.367374 10.92492     5
```

### There’s no ‘Date’ type in JSON, how have you handled this?

At its core `Dates` in R are numeric, so they’re treated as numbers when
converted to JSON. However, I’ve given the option to the user to coerce
to character through the `numeric_dates` argument.

**This argument only works for data.frames and vectors, not lists**

``` r
df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df )
#  [{"dte":17532.0}]

df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [{"dte":"2018-01-01"}]

df <- data.frame(dte = as.POSIXct("2018-01-01 01:01:01"))
jsonify::to_json( df )
#  [{"dte":1514728861.0}]

df <- data.frame(dte = as.POSIXct("2018-01-01 01:01:01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [{"dte":"2018-01-01 01:01:01"}]
```

### And `POSIXct` and `POSIXlt`?

`POSIXct` work the same as `Date`

``` r
df <- data.frame(dte = as.POSIXct("2018-01-01 10:00:00"))
jsonify::to_json( df )
#  [{"dte":1514761200.0}]
jsonify::to_json( df, numeric_dates = FALSE)
#  [{"dte":"2018-01-01 10:00:00"}]
```

`POSIXlt` will return each component of the date-time

``` r
x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
jsonify::to_json( x )
#  {"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}
jsonify::to_json( x, numeric_dates = FALSE)
#  ["2018-01-01 01:00:00"]
```

### Why doesn’t `numeric_dates` work for lists?

Because the purpose of this library is speed. A lot of overhead is
incurred iterating over a list to find and convert objects from one type
to another.

### What do you mean by ‘available at the source’ ?

I want to be able to call the C++ code from another package, without
going to & from R. Therefore, the C++ code is imlemented in headers, so
you can “link to” it in your own package.

For example, the `LinkingTo` section in DESCRIPTION will look something
like

``` r
LinkingTo: 
    Rcpp,
    jsonify
```

And in a c++ source file so you can `#include` the header and use the
available functions

``` cpp
#include "jsonify.hpp"
// [[Rcpp::depends(jsonify)]]

Rcpp::StringVector my_json( Rcpp::DataFrame df ) {
  return jsonify::to_json( df );
}
```

### Can I call it from R if I want to?

Yes.

``` r
df <- data.frame(
  id = 1:3
  , val = letters[1:3]
  , stringsAsFactors = FALSE 
  )
jsonify::to_json( df )
#  [{"id":1,"val":"a"},{"id":2,"val":"b"},{"id":3,"val":"c"}]
```

### How do I install it?

Install the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("SymbolixAU/jsonify")
```
