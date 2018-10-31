# get_in ppx

An exploration of the ["safe call operators" pull-request](https://github.com/facebook/reason/pull/2142), implemented as a ppx.

## Installation

`npm i jaredly/get_in

## Usage

```
/* some data with optional attributes in javascript objects (e.g. from graphql) */
let one = Some({"two": Some({"three": 4})});

let x: option(int) = [%get_in one#??two#?three];
```


