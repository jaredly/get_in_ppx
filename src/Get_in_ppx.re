open Ast_402;

let fail = (loc, txt) => raise(Location.Error(Location.error(~loc, txt)));

let get_mapper =
  Parsetree.{
    ...Ast_mapper.default_mapper,
    expr: (mapper, expr) =>
      switch (expr.pexp_desc) {
      | Pexp_apply(
          {pexp_loc, pexp_desc: Pexp_ident({txt: Lident("#??")})},
          [("", arg), ("", name)],
        ) =>
        switch (name.pexp_desc) {
        | Pexp_ident({txt: Lident(_)}) => {
            Parsetree.pexp_desc:
              [@implicit_arity]
              Parsetree.Pexp_match(
                mapper.expr(mapper, arg),
                [
                  {
                    Parsetree.pc_lhs: {
                      Parsetree.ppat_desc:
                        [@implicit_arity]
                        Parsetree.Ppat_construct(
                          {
                            Asttypes.txt: Longident.Lident("None"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          None,
                        ),
                      Parsetree.ppat_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.ppat_attributes: [],
                    },
                    Parsetree.pc_guard: None,
                    Parsetree.pc_rhs: {
                      Parsetree.pexp_desc:
                        [@implicit_arity]
                        Parsetree.Pexp_construct(
                          {
                            Asttypes.txt: Longident.Lident("None"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          None,
                        ),
                      Parsetree.pexp_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.pexp_attributes: [],
                    },
                  },
                  {
                    Parsetree.pc_lhs: {
                      Parsetree.ppat_desc:
                        [@implicit_arity]
                        Parsetree.Ppat_construct(
                          {
                            Asttypes.txt: Longident.Lident("Some"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          Some({
                            Parsetree.ppat_desc:
                              Parsetree.Ppat_tuple([
                                {
                                  Parsetree.ppat_desc:
                                    Parsetree.Ppat_var({
                                      Asttypes.txt: "arg",
                                      Asttypes.loc:
                                        Pervasives.(^)(
                                          Ast_helper.default_loc,
                                        ),
                                    }),
                                  Parsetree.ppat_loc:
                                    Pervasives.(^)(Ast_helper.default_loc),
                                  Parsetree.ppat_attributes: [],
                                },
                              ]),
                            Parsetree.ppat_loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                            Parsetree.ppat_attributes: [],
                          }),
                        ),
                      Parsetree.ppat_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.ppat_attributes: [
                        (
                          {
                            Asttypes.txt: "explicit_arity",
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          Parsetree.PStr([]),
                        ),
                      ],
                    },
                    Parsetree.pc_guard: None,
                    Parsetree.pc_rhs: {
                      Parsetree.pexp_desc:
                        [@implicit_arity]
                        Parsetree.Pexp_apply(
                          {
                            Parsetree.pexp_desc:
                              Parsetree.Pexp_ident({
                                Asttypes.txt: Longident.Lident("##"),
                                Asttypes.loc:
                                  Pervasives.(^)(Ast_helper.default_loc),
                              }),
                            Parsetree.pexp_loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                            Parsetree.pexp_attributes: [],
                          },
                          [
                            (
                              "",
                              {
                                Parsetree.pexp_desc:
                                  Parsetree.Pexp_ident({
                                    Asttypes.txt: Longident.Lident("arg"),
                                    Asttypes.loc:
                                      Pervasives.(^)(Ast_helper.default_loc),
                                  }),
                                Parsetree.pexp_loc:
                                  Pervasives.(^)(Ast_helper.default_loc),
                                Parsetree.pexp_attributes: [],
                              },
                            ),
                            ("", name),
                          ],
                        ),
                      Parsetree.pexp_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.pexp_attributes: [],
                    },
                  },
                ],
              ),
            Parsetree.pexp_loc: Pervasives.(^)(Ast_helper.default_loc),
            Parsetree.pexp_attributes: [],
          }

        | _ => fail(pexp_loc, "#? must have a literal string after it")
        }
      | Pexp_apply(
          {pexp_loc, pexp_desc: Pexp_ident({txt: Lident("#?")})},
          [("", arg), ("", name)],
        ) =>
        switch (name.pexp_desc) {
        | Pexp_ident({txt: Lident(_)}) => {
            Parsetree.pexp_desc:
              [@implicit_arity]
              Parsetree.Pexp_match(
                mapper.expr(mapper, arg),
                [
                  {
                    Parsetree.pc_lhs: {
                      Parsetree.ppat_desc:
                        [@implicit_arity]
                        Parsetree.Ppat_construct(
                          {
                            Asttypes.txt: Longident.Lident("None"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          None,
                        ),
                      Parsetree.ppat_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.ppat_attributes: [],
                    },
                    Parsetree.pc_guard: None,
                    Parsetree.pc_rhs: {
                      Parsetree.pexp_desc:
                        [@implicit_arity]
                        Parsetree.Pexp_construct(
                          {
                            Asttypes.txt: Longident.Lident("None"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          None,
                        ),
                      Parsetree.pexp_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.pexp_attributes: [],
                    },
                  },
                  {
                    Parsetree.pc_lhs: {
                      Parsetree.ppat_desc:
                        [@implicit_arity]
                        Parsetree.Ppat_construct(
                          {
                            Asttypes.txt: Longident.Lident("Some"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          Some({
                            Parsetree.ppat_desc:
                              Parsetree.Ppat_tuple([
                                {
                                  Parsetree.ppat_desc:
                                    Parsetree.Ppat_var({
                                      Asttypes.txt: "arg",
                                      Asttypes.loc:
                                        Pervasives.(^)(
                                          Ast_helper.default_loc,
                                        ),
                                    }),
                                  Parsetree.ppat_loc:
                                    Pervasives.(^)(Ast_helper.default_loc),
                                  Parsetree.ppat_attributes: [],
                                },
                              ]),
                            Parsetree.ppat_loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                            Parsetree.ppat_attributes: [],
                          }),
                        ),
                      Parsetree.ppat_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.ppat_attributes: [
                        (
                          {
                            Asttypes.txt: "explicit_arity",
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          Parsetree.PStr([]),
                        ),
                      ],
                    },
                    Parsetree.pc_guard: None,
                    Parsetree.pc_rhs: {
                      Parsetree.pexp_desc:
                        [@implicit_arity]
                        Parsetree.Pexp_construct(
                          {
                            Asttypes.txt: Longident.Lident("Some"),
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          Some({
                            Parsetree.pexp_desc:
                              Parsetree.Pexp_tuple([
                                {
                                  Parsetree.pexp_desc:
                                    [@implicit_arity]
                                    Parsetree.Pexp_apply(
                                      {
                                        Parsetree.pexp_desc:
                                          Parsetree.Pexp_ident({
                                            Asttypes.txt:
                                              Longident.Lident("##"),
                                            Asttypes.loc:
                                              Pervasives.(^)(
                                                Ast_helper.default_loc,
                                              ),
                                          }),
                                        Parsetree.pexp_loc:
                                          Pervasives.(^)(
                                            Ast_helper.default_loc,
                                          ),
                                        Parsetree.pexp_attributes: [],
                                      },
                                      [
                                        (
                                          "",
                                          {
                                            Parsetree.pexp_desc:
                                              Parsetree.Pexp_ident({
                                                Asttypes.txt:
                                                  Longident.Lident("arg"),
                                                Asttypes.loc:
                                                  Pervasives.(^)(
                                                    Ast_helper.default_loc,
                                                  ),
                                              }),
                                            Parsetree.pexp_loc:
                                              Pervasives.(^)(
                                                Ast_helper.default_loc,
                                              ),
                                            Parsetree.pexp_attributes: [],
                                          },
                                        ),
                                        ("", name),
                                      ],
                                    ),
                                  Parsetree.pexp_loc:
                                    Pervasives.(^)(Ast_helper.default_loc),
                                  Parsetree.pexp_attributes: [],
                                },
                              ]),
                            Parsetree.pexp_loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                            Parsetree.pexp_attributes: [],
                          }),
                        ),
                      Parsetree.pexp_loc:
                        Pervasives.(^)(Ast_helper.default_loc),
                      Parsetree.pexp_attributes: [
                        (
                          {
                            Asttypes.txt: "explicit_arity",
                            Asttypes.loc:
                              Pervasives.(^)(Ast_helper.default_loc),
                          },
                          Parsetree.PStr([]),
                        ),
                      ],
                    },
                  },
                ],
              ),
            Parsetree.pexp_loc: Pervasives.(^)(Ast_helper.default_loc),
            Parsetree.pexp_attributes: [],
          }

        | _ => fail(pexp_loc, "#? must have a literal string after it")
        }
      | _ => Ast_mapper.default_mapper.expr(mapper, expr)
      },
  };

let mapper = 
  Parsetree.{
    ...Ast_mapper.default_mapper,
    expr: (mapper, expr) =>
      switch (expr.pexp_desc) {
      | Pexp_extension((
          {txt: "get_in", loc},
          PStr([{pstr_desc: Pstr_eval(expr, attributes)}]),
        )) =>
        get_mapper.expr(get_mapper, expr)
      | _ => Ast_mapper.default_mapper.expr(mapper, expr)
      },
  };