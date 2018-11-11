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
# jsonlite 9665.3574 9728.9917 10207.403 10091.683 10547.409 11003.576     5
#  jsonify  850.9892  884.6032  1109.209   885.407  1435.751  1489.296     5

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
#     expr       min        lq      mean    median        uq       max neval
# jsonlite 772.63428 777.04790 793.70125 781.21720 787.17005 850.43682     5
#  jsonify  67.77849  68.18133  71.75382  71.46636  73.25485  78.08806     5

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
    r_val <- jsonlite::fromJSON( js )
  },
  jsonify = {
    r_val <- jsonify::from_json( js )
  },
  times = 5
)
# Unit: seconds
#     expr       min        lq      mean    median        uq       max neval
# jsonlite 118.48090 126.06599 141.32816 142.35451 150.73256 169.00684     5
#  jsonify  19.58654  20.38357  25.90293  20.69855  30.20221  38.64381     5


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

