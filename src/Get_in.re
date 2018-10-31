
let fail = (loc, txt) => raise(Location.Error(Location.error(~loc, txt)));

let get_mapper = Parsetree.{
  ...Ast_mapper.default_mapper,
  expr: (mapper, expr) => {
    switch (expr.pexp_desc) {
      | Pexp_apply({pexp_loc, pexp_desc: Pexp_ident({txt: Lident("#??")})}, [("", arg), ("", name)]) =>
        switch (name.pexp_desc) {
          | Pexp_ident({txt: Lident(_)}) => [%expr switch ([%e mapper.expr(mapper, arg)]) {
            | None => None
            | Some(arg) => arg##[%e name]
          }]
          | _ => fail(pexp_loc, "#? must have a literal string after it")
        }
      | Pexp_apply({pexp_loc, pexp_desc: Pexp_ident({txt: Lident("#?")})}, [("", arg), ("", name)]) =>
        switch (name.pexp_desc) {
          | Pexp_ident({txt: Lident(_)}) => [%expr switch ([%e mapper.expr(mapper, arg)]) {
            | None => None
            | Some(arg) => Some(arg##[%e name])
          }]
          | _ => fail(pexp_loc, "#? must have a literal string after it")
        }
      | _ => Ast_mapper.default_mapper.expr(mapper, expr)
    }
  }
};

let mapper = _argv =>
  Parsetree.{
    ...Ast_mapper.default_mapper,
    /* TODO throw error on structure items */
    expr: (mapper, expr) =>
      switch expr.pexp_desc {
      | Pexp_extension(({txt: "get_in", loc}, PStr([{pstr_desc: Pstr_eval(expr, attributes)}]))) => {
        get_mapper.expr(get_mapper, expr)
      }
      | _ => Ast_mapper.default_mapper.expr(mapper, expr)
      }
  };

let () = Ast_mapper.run_main(mapper);