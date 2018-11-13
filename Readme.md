# get_in ppx

An exploration of the ["safe call operators" pull-request](https://github.com/facebook/reason/pull/2142), implemented as a ppx.

## Installation

`npm i get_in_ppx`

bsconfig.json

```
"ppx-flags": ["get_in_ppx/ppx"]
```

## Operators
This ppx includes two operators, that are valid within the `[%get_in ]` form.

- `#??` is to be used when *both sides* are optional. E.g. the object on the left is optional, and the attribute you're getting out is also optional. e.g. `option({. "attr": option(int)})`
- `#?` is to be used when *only the object* is optional, but the attribute you're getting out is not. e.g. `option({. "attr": int})`

## Usage

```
/* some data with optional attributes in javascript objects (e.g. from graphql) */
let one = Some({"two": Some({"three": 4})});

let x: option(int) = [%get_in one#??two#?three];
```


