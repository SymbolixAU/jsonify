## v1.0

* empty list names handled [issue 53](https://github.com/SymbolixAU/jsonify/issues/53)
* `from_json()` method from converting JSON to R objects

## v0.2.1

* `as.json()` method
* Bug fix: AsIs data.frame inside another data.frame [issue #38](https://github.com/SymbolixAU/jsonify/issues/38)
* BH -DBOOST_NO_AUTO_PTR flag

## v0.2.0

* by column | row option
* factors_as_string argument
* Support for LANGSXP objects
* Dates are handled in c++, and now supports nested lists
* c++ API restructured and simplified
* `print.json()` method to print json class objects
* `minify_json()` function to remove indentation
* `pretty_json()` function to add indentation
* `digits` argument to specify the number of digits to which numeric values will be rounded

## v0.1.2

* `unbox` argument for `to_json()`
* Added a `NEWS.md` file to track changes to the package.
