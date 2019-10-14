## TODO
## - lists
## - POSIXlt
## - POSIXct
## - Date


library(microbenchmark)
library(jsonlite)

n <- 1e6
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  #, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n), size = n, replace = T)
)

microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( df )
  },
  jsonify = {
    js <- jsonify::to_json( df )
  },
  times = 5
)
# Unit: seconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 1.793552 1.833454 2.267699 1.988986 2.270455 3.452051     5
#  jsonify 1.385660 1.418404 1.766797 1.540941 1.964592 2.524389     5

## numeric vector
n <- 1e7
x <- rnorm(n = n)

# to_json
microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( x )
  },
  jsonify = {
    js <- jsonify::to_json( x )
  },
  times = 5
)
# Unit: seconds
# expr      min       lq     mean   median       uq      max neval
# jsonlite 1.750814 1.894219 2.096893 2.034916 2.250064 2.554454     5
#  jsonify 1.732922 1.746326 1.856985 1.805632 1.820807 2.179239     5

# from_json
js <- jsonify::to_json( x )
microbenchmark(
  jsonlite = {
    jlt <- jsonlite::fromJSON( js )
  }, 
  jsonify = {
    jfy <- jsonify::from_json( js )
  }, 
  times = 5
)
# Unit: seconds
#     expr      min       lq     mean   median       uq       max neval
# jsonlite 8.996560 9.164908 9.628287 9.205365 10.00561 10.768996     5
#  jsonify 2.651533 2.809438 2.929623 2.967455  3.05825  3.161441     5

## numeric vector with NAs
n <- 1e6
x <- rnorm(n = n)
x[sample(1:n,size = n/3)] <- NA_real_

# to_json
microbenchmark(
  jsonlite = {
    jlt <- jsonlite::toJSON( x )
  },
  jsonify = {
    jfy <- jsonify::to_json( x )
  },
  times = 5
)
# Unit: milliseconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 131.0990 133.9365 144.5690 139.6863 142.1736 175.9497     5
#  jsonify 120.5843 124.8682 129.2087 126.3678 136.1451 138.0783     5

# from_json
js <- jsonify::to_json( x )
microbenchmark(
  jsonlite = {
    jlt <- jsonlite::fromJSON( js )
  },
  jsonify = {
    jfy <- jsonify::from_json( js )
  },
  times = 5
)
# Unit: milliseconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 748.6320 753.5103 760.6138 755.1885 763.4893 782.2489     5
#  jsonify 181.9075 182.8828 214.3332 209.5452 224.1336 273.1970     5

## mixture of NAs, NULLs, Infs etc
set.seed(20191012)
n <- 1e6
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  #, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n), size = n, replace = T)
)

df[sample(1:n, size = n / 3), 'id'] <- NA_integer_
df[sample(1:n, size = n / 3), 'val2'] <- NA_real_
df[sample(1:n, size = n / 3), 'log'] <- NA

# to_json
microbenchmark(
  jsonlite = {
    jlt <- jsonlite::toJSON( df )
  },
  jsonify = {
    jfy <- jsonify::to_json( df, digits = 4 )
  },
  times = 5
)
# Unit: seconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 30.29614 30.29614 30.29614 30.29614 30.29614 30.29614     1
#  jsonify 19.88902 19.88902 19.88902 19.88902 19.88902 19.88902     1

# from_json
js_jfy <- jsonify::to_json(df)
js_jlt <- jsonlite::toJSON(df)
microbenchmark(
  jsonlite = {
    jlt <- jsonlite::fromJSON( js_jlt , simplifyVector = FALSE)
  },
  jsonify = {
    jfy <- jsonify::from_json( js_jfy , simplify = FALSE)
  },
  times = 1
)
# Unit: seconds
#   expr      min       lq     mean   median       uq      max neval
# jsonlite 42.12359 77.00299 72.53358 78.70986 79.59812 85.23332     5
#  jsonify 22.17143 24.22952 27.90723 25.01322 33.13034 34.99164     5

microbenchmark(
  jsonlite = {
    jlt <- jsonlite::fromJSON( js_jlt )
  },
  jsonify = {
    jfy <- jsonify::from_json( js_jfy )
  },
  times = 1
)
