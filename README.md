
[![Travis build status](https://travis-ci.org/SymbolixAU/jsonify.svg?branch=master)](https://travis-ci.org/SymbolixAU/jsonify) [![Coverage status](https://codecov.io/gh/SymbolixAU/jsonify/branch/master/graph/badge.svg)](https://codecov.io/github/SymbolixAU/jsonify?branch=master)

jsonify
=======

jsonify converts R objects to JSON.

### Aren't there already data.frame to JSON converters?

Yep.

### So why did you build this one?

Because I wanted it available at the source ( C++ ) level for integrating into other packages.

### Is it fast?

It does alright

``` r

library(microbenchmark)
library(jsonlite)

n <- 5e6
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

# Unit: seconds
#      expr       min        lq      mean    median        uq      max neval
#  jsonlite 13.422896 13.767812 15.178853 14.112728 16.056832 18.00094     3
#   jsonify  7.152847  7.238169  7.294133  7.323491  7.364776  7.40606     3

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

# Unit: milliseconds
#      expr       min        lq      mean    median        uq       max neval
#  jsonlite 21.806105 21.911735 22.143469 21.943781 22.499527 22.556196     5
#   jsonify  8.009139  8.109407  8.540984  8.117278  8.874733  9.594362     5
```

### There's no 'Date' type in JSON, how have you handled this?

At its core `Dates` in R are numeric, so they're treated as numbers when converted to JSON. However, I've given the option to the user to coerce to character through the `numeric_dates` argument.

**This argument only works for data.frames and vectors, not lists**

``` r
df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df )
#  [1] "[{\"dte\":17532.0}]"
#  attr(,"class")
#  [1] "json"

df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [1] "[{\"dte\":\"2018-01-01\"}]"
#  attr(,"class")
#  [1] "json"

df <- data.frame(dte = as.POSIXct("2018-01-01 01:01:01"))
jsonify::to_json( df )
#  [1] "[{\"dte\":1514728861.0}]"
#  attr(,"class")
#  [1] "json"

df <- data.frame(dte = as.POSIXct("2018-01-01 01:01:01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [1] "[{\"dte\":\"2018-01-01 01:01:01\"}]"
#  attr(,"class")
#  [1] "json"
```

### And `POSIXct` and `POSIXlt`?

`POSIXct` work the same as `Date`

``` r
df <- data.frame(dte = as.POSIXct("2018-01-01 10:00:00"))
jsonify::to_json( df )
#  [1] "[{\"dte\":1514761200.0}]"
#  attr(,"class")
#  [1] "json"
jsonify::to_json( df, numeric_dates = FALSE)
#  [1] "[{\"dte\":\"2018-01-01 10:00:00\"}]"
#  attr(,"class")
#  [1] "json"
```

`POSIXlt` will return each component of the date-time

``` r
x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
jsonify::to_json( x )
#  [1] "{\"sec\":[0.0],\"min\":[0],\"hour\":[1],\"mday\":[1],\"mon\":[0],\"year\":[118],\"wday\":[1],\"yday\":[0],\"isdst\":[0]}"
#  attr(,"class")
#  [1] "json"
jsonify::to_json( x, numeric_dates = FALSE)
#  [1] "[\"2018-01-01 01:00:00\"]"
#  attr(,"class")
#  [1] "json"
```

### Why doesn't `numeric_dates` work for lists?

Because the purpose of this library is speed. A lot of overhead is incurred iterating over a list to find and convert objects from one type to another.

### What do you mean by 'available at the source' ?

I want to be able to call the C++ code from another package, without going to & from R. Therefore, the C++ code is implemented in headers, so you can "link to" it in your own package.

For example, the `LinkingTo` section in DESCRIPTION will look something like

``` r
LinkingTo: 
    Rcpp,
    jsonify
```

And in a c++ source file you can `#include` the header and use the available functions

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
#  [1] "[{\"id\":1,\"val\":\"a\"},{\"id\":2,\"val\":\"b\"},{\"id\":3,\"val\":\"c\"}]"
#  attr(,"class")
#  [1] "json"
```

### How do I install it?

Install the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("SymbolixAU/jsonify")
```
