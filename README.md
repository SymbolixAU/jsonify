
[![CRAN\_Status\_Badge](http://www.r-pkg.org/badges/version/jsonify)](https://CRAN.R-project.org/package=jsonify)
![downloads](http://cranlogs.r-pkg.org/badges/grand-total/jsonify)
[![CRAN RStudio mirror
downloads](http://cranlogs.r-pkg.org/badges/jsonify)](https://CRAN.R-project.org/package=jsonify)
[![Travis build
status](https://travis-ci.org/SymbolixAU/jsonify.svg?branch=master)](https://travis-ci.org/SymbolixAU/jsonify)
[![Coverage
status](https://codecov.io/gh/SymbolixAU/jsonify/branch/master/graph/badge.svg)](https://codecov.io/github/SymbolixAU/jsonify?branch=master)

# jsonify

jsonify converts R objects to JSON.

### There are already JSON converters, why did you build this one?

Because I wanted it available at the source ( C++ ) level for
integrating into other packages.

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
  , stringsAsFactors = FALSE
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
#      expr       min        lq      mean    median        uq       max neval
#  jsonlite 11.243452 13.174676 14.721483 15.105899 16.460498 17.815098     3
#   jsonify  4.607378  4.696038  5.419219  4.784698  5.825139  6.865581     3

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
#  jsonlite 19.226906 19.488125 20.310911 19.768292 21.142979 21.928253     5
#   jsonify  7.638329  7.715288  7.944044  7.879122  7.924961  8.562521     5
   
```

### There’s no `Date` type in JSON, how have you handled this?

At its core `Dates` in R are numeric, so they are treated as numbers
when converted to JSON. However, the user can coerce to character
through the `numeric_dates` argument.

``` r
df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df )
#  [{"dte":17532.0}]

df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [{"dte":"2018-01-01"}]
```

### And `POSIXct` and `POSIXlt`?

``` r

jsonify::to_json( as.POSIXct("2018-01-01 10:00:00") )
#  [1514761200.0]
jsonify::to_json( as.POSIXct("2018-01-01 10:00:00"), numeric_dates = FALSE)
#  ["2017-12-31T23:00:00"]
```

Note here that **POSIXct** values are returned in UTC timezone.

**POSIXlt** will return each component of the date-time

``` r
x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
jsonify::to_json( x )
#  {"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}

jsonify::to_json( x, numeric_dates = FALSE)
#  {"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}
```

### What about lists?

~~The purpose of this library is speed. A lot of overhead is incurred
iterating over a list to find and convert objects from one type to
another.~~

For v0.2.0 I’ve managed to get the date handling at the c++ level, so
there’s no penalty for recursing through the list to coerce to
character.

Therefore, lists will work too

``` r
l <- list(
  dte = as.Date("2018-01-01")
  , psx = seq(as.POSIXct("2018-01-01 13:00:00"), as.POSIXct("2018-01-05 13:00:00"), length.out = 5)
  , df = data.frame(psx = seq(as.POSIXct("2018-01-01 13:00:00"), as.POSIXct("2018-01-05 13:00:00"), length.out = 5))
)
jsonify::to_json( l )
#  {"dte":[17532.0],"psx":[1514772000,1514858400,1514944800,1515031200,1515117600],"df":[{"psx":1514772000},{"psx":1514858400},{"psx":1514944800},{"psx":1515031200},{"psx":1515117600}]}
```

``` r
js <- jsonify::to_json( l, numeric_dates = FALSE )
jsonify::pretty_json( js )
#  {
#      "dte": [
#          "2018-01-01"
#      ],
#      "psx": [
#          "2018-01-01T02:00:00",
#          "2018-01-02T02:00:00",
#          "2018-01-03T02:00:00",
#          "2018-01-04T02:00:00",
#          "2018-01-05T02:00:00"
#      ],
#      "df": [
#          {
#              "psx": "2018-01-01T02:00:00"
#          },
#          {
#              "psx": "2018-01-02T02:00:00"
#          },
#          {
#              "psx": "2018-01-03T02:00:00"
#          },
#          {
#              "psx": "2018-01-04T02:00:00"
#          },
#          {
#              "psx": "2018-01-05T02:00:00"
#          }
#      ]
#  }
```

And it’s still
fast

``` r
dtes <- seq(as.Date("2018-01-01"), as.Date("2019-01-01"), length.out = 365)
psx <- seq(as.POSIXct("2018-01-01"), as.POSIXct("2019-01-01"), length.out = 365)
n <- 1e5

lst <- list(
  x = sample(dtes, size = n, replace = T)
  , y = list(
    ya = sample(dtes, size = n, replace = TRUE)
    , yb = rnorm(n = n)
    , yx = list( sample(dtes, size = n, replace = T ) )
  )
  , p = psx
)

library( microbenchmark )

microbenchmark(
  jsonify1 = {
    jsonify::to_json( lst, numeric_dates = TRUE )
  },
  jsonify2 = {
    jsonify::to_json( lst, numeric_dates = FALSE )
  },
  jsonlite = {
    jsonlite::toJSON( lst )
  },
  times = 3
)
#  Unit: milliseconds
#       expr       min        lq      mean    median        uq       max
#   jsonify1  130.4760  136.5254  176.1251  142.5748  198.9496  255.3243
#   jsonify2  678.8992  730.5589  928.0832  782.2186 1052.6752 1323.1318
#   jsonlite 1849.3437 1897.8639 2028.3890 1946.3841 2117.9116 2289.4391
#   neval
#       3
#       3
#       3
```

### That output looks nice, is that `pretty_json()` function new?

Yep, it’s a new feature in v0.2.0

### What do you mean by “available at the source” ?

I want to be able to call the C++ code from another package, without
going to & from R. Therefore, the C++ code is implemented in headers, so
you can “link to” it in your own package.

For example, the `LinkingTo` section in **DESCRIPTION** will look
something like

``` yaml
LinkingTo: 
    Rcpp,
    jsonify
```

And in a c++ source file you can `#include` the header and use the
available functions

``` cpp
// [[Rcpp::depends(jsonify)]]
#include "jsonify/jsonify.hpp"

Rcpp::StringVector my_json( Rcpp::DataFrame df ) {
  return jsonify::api::to_json( df );
}
```

### Can I call it from R if I want to?

Yes. Just like the examples in this readme use `to_json()`

``` r
df <- data.frame(
  id = 1:3
  , val = letters[1:3]
  )
jsonify::to_json( df )
#  [{"id":1,"val":1},{"id":2,"val":2},{"id":3,"val":3}]
```

### Why are there numbers instead of letters?

Because `factors` are a thing in R. And `jsonify` treats them as
factors.

### Oh yeah, I forget about factors…

So do a lot of people, especially when working with `tibbles`, which
sets `stringsAsFactors = FALSE` by default (the opposite of base R)

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

Get the latest release version from CRAN

``` r
install.packages("jsonify")
```

Or the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("SymbolixAU/jsonify")
```
