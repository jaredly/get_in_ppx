// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

console.log("Hello, BuckleScript and Reason!");

var obj = {
  a: {
    b: "c"
  }
};

var match = obj.a;

var c = match !== undefined ? Caml_option.valFromOption(match).b : undefined;

if (!Caml_obj.caml_equal(c, "c")) {
  throw [
        Caml_builtin_exceptions.assert_failure,
        /* tuple */[
          "Demo.re",
          4,
          0
        ]
      ];
}

console.log("Done!");

exports.obj = obj;
exports.c = c;
/*  Not a pure module */