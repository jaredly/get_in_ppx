Js.log("Hello, BuckleScript and Reason!");
let obj = {"a": Some({"b": Some("c")})};
let c = [%get_in obj##a#??b];
assert(c == Some("c"));

// TODO support this
// let obj = {"Capital": "one"};
// let contents = [%get_in obj["Capital"]];
// assert(contents == "one");

Js.log("Done!")