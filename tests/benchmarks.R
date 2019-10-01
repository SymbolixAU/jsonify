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
    r_val <- jsonlite::fromJSON( js )
  }, 
  jsonify = {
    r_val <- jsonify::from_json( js )
  }, 
  times = 5
)
# Unit: milliseconds
#     expr       min        lq      mean    median        uq       max neval
# jsonlite 9531.6450 9757.0702 9826.4883 9854.7296 9880.0933 10108.903     5
#  jsonify  895.7495  908.3581  932.9345  910.2298  934.1805  1016.155     5

## numeric vector with NAs
n <- 1e6
x <- rnorm(n = n)
x[sample(1:n,size = n/3)] <- NA_real_

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
# Unit: milliseconds
# expr      min       lq     mean   median       uq      max neval
# jsonlite 142.9747 160.1157 161.9641 163.9898 169.7590 172.9811     5
#  jsonify 127.8681 128.6028 129.6287 130.0981 130.2327 131.3419     5

# from_json
js <- jsonify::to_json( x )
microbenchmark(
  jsonlite = {
    r_val <- jsonlite::fromJSON( js )
  },
  jsonify = {
    r_val <- jsonify::from_json( js )
  },
  times = 5
)
# Unit: milliseconds
#   expr      min        lq      mean    median       uq       max neval
# jsonlite 773.2204 777.71724 800.19764 797.00264 821.4729 831.57496     5
#  jsonify  82.7567  82.85822  85.61328  85.67881  87.7332  89.03947     5

## mixture of NAs, NULLs, Infs etc
n <- 1e7
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
    js <- jsonlite::toJSON( df )
  },
  jsonify = {
    js <- jsonify::to_json( df )
  },
  times = 5
)
# Unit: seconds
#     expr       min        lq     mean   median       uq      max neval
# jsonlite 18.518717 23.558636 24.79996 24.47630 24.77023 32.67592     5
#  jsonify  9.265547  9.407197 13.29851 12.50761 17.08521 18.22700     5

# from_json
js <- jsonify::to_json(df)
microbenchmark(
  jsonlite = {
    r_val <- jsonlite::fromJSON( js , simplifyVector = FALSE)
  },
  jsonify = {
    r_val <- jsonify::from_json( js , simplifyDataFrame = FALSE)
  },
  times = 5
)
# Unit: seconds
#   expr      min       lq     mean   median       uq      max neval
# jsonlite 42.12359 77.00299 72.53358 78.70986 79.59812 85.23332     5
#  jsonify 22.17143 24.22952 27.90723 25.01322 33.13034 34.99164     5


## forcing 

library(microbenchmark)
library(jsonlite)

n <- 1e7
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  #, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n), size = n, replace = T)
)

microbenchmark(
  force = {
    js <- jsonify::to_json( df, force = TRUE )
  },
  dontforce = {
    js <- jsonify::to_json( df, force = FALSE )
  },
  times = 5
)
# Unit: seconds
#       expr      min       lq     mean   median       uq      max neval
#     force 9.889725 10.03111 10.23549 10.05032 10.38913 10.81715     5
# dontforce 9.981074 10.02546 10.16957 10.12311 10.25727 10.46095     5

