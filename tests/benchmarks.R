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
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 1.353150 1.534014 1.548655 1.545189 1.584838 1.726087     5
#  jsonify 1.456572 1.460230 1.507303 1.463506 1.524051 1.632157     5

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
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 7.986898 8.023153 8.303378 8.145451 8.295069 9.066318     5
#  jsonify 2.223123 2.531027 2.736762 2.746553 2.866034 3.317075     5

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
rm(list=ls()); gc()
set.seed(20191012)
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
    jlt <- jsonlite::toJSON( df )
  },
  jsonify = {
    jfy <- jsonify::to_json( df, digits = 4 )
  },
  times = 3
)
# Unit: seconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 19.87568 20.67037 24.10039 21.46506 26.21274 30.96042     3
#  jsonify 12.92456 14.22520 15.98015 15.52584 17.50794 19.49004     3

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
  times = 3
)
# Unit: seconds
#     expr      min      lq     mean   median        uq       max neval
# jsonlite 32.16290 34.9752 83.15379 37.78749 108.64924 179.51098     3
#  jsonify 23.04863 39.6811 53.73789 56.31357  69.08251  81.85146     3

rm( jlt, jfy ); gc()
microbenchmark(
  jsonlite = {
    jlt <- jsonlite::fromJSON( js_jlt )
  },
  jsonify = {
    jfy <- jsonify::from_json( js_jfy )
  },
  times = 3
)

# Unit: seconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 50.12151 58.74930 63.45008 67.37709 70.11437 72.85165     3
#  jsonify 52.16836 53.00044 62.65738 53.83251 67.90190 81.97128     3

ul <- "https://raw.githubusercontent.com/SymbolixAU/data/master/geojson/SA1_2016_ACT.json"
microbenchmark(
  
  jsonlite = {
    jlt <- jsonlite::fromJSON( ul )
  },
  jsonify = {
    jfy <- jsonify::from_json( ul )
  },
  times = 5
)
# Unit: seconds
#     expr      min       lq     mean   median       uq      max neval
# jsonlite 1.433624 1.486971 1.631894 1.537450 1.563000 2.138424     5
#  jsonify 1.510242 1.528668 1.920362 1.531584 1.877338 3.153977     5
