let load_file = ((kind, fn)) =>
  with_file_in(fn, ~f=ic =>
    switch (Ast_io.from_channel(ic)) {
    | [@implicit_arity] Ok(fn, [@implicit_arity] Ast_io.Intf((module V), sg)) =>
      check_kind(fn, ~expected=kind, ~got=Kind_intf);
      Location.input_name := fn;
      /* We need to convert to the current version in order to interpret the cookies using
         [Ast_mapper.drop_ppx_context_*] from the compiler */
      (
        fn,
        Intf(
          migrate((module V), (module OCaml_current)).copy_signature(sg),
        ),
      );
    | [@implicit_arity] Ok(fn, [@implicit_arity] Ast_io.Impl((module V), st)) =>
      check_kind(fn, ~expected=kind, ~got=Kind_impl);
      Location.input_name := fn;
      (
        fn,
        Impl(
          migrate((module V), (module OCaml_current)).copy_structure(st),
        ),
      );
    | Error(Ast_io.Unknown_version(_)) =>
      Location.raise_errorf(
        ~loc=Location.in_file(fn),
        "File is a binary ast for an unknown version of OCaml",
      )
    | Error(Ast_io.Not_a_binary_ast(prefix_read_from_file)) =>
      /* To test if a file is a binary AST file, we have to read the first few bytes of
         the file.
         If it is not a binary AST, we have to parse these bytes and the rest of the file
         as source code. To do that, we prefill the lexbuf buffer with what we read from
         the file to do the test. */
      let lexbuf = Lexing.from_channel(ic);
      let len = String.length(prefix_read_from_file);
      String.blit(prefix_read_from_file, 0, lexbuf.Lexing.lex_buffer, 0, len);
      lexbuf.Lexing.lex_buffer_len = len;
      lexbuf.Lexing.lex_curr_p = {
        Lexing.pos_fname: fn,
        pos_lnum: 1,
        pos_bol: 0,
        pos_cnum: 0,
      };
      Location.input_name := fn;
      let kind =
        switch (kind) {
        | Kind_impl => Kind_impl
        | Kind_intf => Kind_intf
        | Kind_unknown => guess_file_kind(fn)
        };

      switch (kind) {
      | Kind_impl => (fn, Impl(Parse.implementation(lexbuf)))
      | Kind_intf => (fn, Intf(Parse.interface(lexbuf)))
      | Kind_unknown =>
        Location.raise_errorf(
          ~loc=Location.in_file(fn),
          "I can't decide whether %s is an implementation or interface file",
          fn,
        )
      };
    }
  );

let with_output = (~bin=?, output, ~f) =>
  switch (output) {
  | None =>
    switch (bin) {
    | Some(bin) => set_binary_mode_out(stdout, bin)
    | None => ()
    };
    f(stdout);
  | Some(fn) => with_file_out(fn, ~f)
  };

type output_mode =
  | Pretty_print
  | Dump_ast
  | Null;

let process_file = (~config, ~output, ~output_mode, ~embed_errors, file) => {
  let (fn, ast) = load_file(file);
  let ast =
    switch (ast) {
    | Intf(sg) =>
      let sg = Ast_mapper.drop_ppx_context_sig(~restore=true, sg);
      let sg =
        try (
          rewrite_signature(config, (module OCaml_current), sg)
          |> migrate_some_signature((module OCaml_current))
        ) {
        | exn when embed_errors =>
          switch (Migrate_parsetree_compiler_functions.error_of_exn(exn)) {
          | None => raise(exn)
          | Some(error) => [
              Ast_helper.Sig.extension(
                ~loc=Location.none,
                Ast_mapper.extension_of_error(error),
              ),
            ]
          }
        };

      [@implicit_arity]
      Intf(
        sg,
        Ast_mapper.add_ppx_context_sig(~tool_name=config.tool_name, sg),
      );
    | Impl(st) =>
      let st = Ast_mapper.drop_ppx_context_str(~restore=true, st);
      let st =
        try (
          rewrite_structure(config, (module OCaml_current), st)
          |> migrate_some_structure((module OCaml_current))
        ) {
        | exn when embed_errors =>
          switch (Migrate_parsetree_compiler_functions.error_of_exn(exn)) {
          | None => raise(exn)
          | Some(error) => [
              Ast_helper.Str.extension(
                ~loc=Location.none,
                Ast_mapper.extension_of_error(error),
              ),
            ]
          }
        };

      [@implicit_arity]
      Impl(
        st,
        Ast_mapper.add_ppx_context_str(~tool_name=config.tool_name, st),
      );
    };

  switch (output_mode) {
  | Dump_ast =>
    with_output(
      ~bin=true,
      output,
      ~f=oc => {
        let ast =
          switch (ast) {
          | [@implicit_arity] Intf(_, sg) =>
            [@implicit_arity] Ast_io.Intf((module OCaml_current), sg)
          | [@implicit_arity] Impl(_, st) =>
            [@implicit_arity] Ast_io.Impl((module OCaml_current), st)
          };

        Ast_io.to_channel(oc, fn, ast);
      },
    )
  | Pretty_print =>
    with_output(
      output,
      ~f=oc => {
        let ppf = Format.formatter_of_out_channel(oc);
        switch (ast) {
        | [@implicit_arity] Intf(sg, _) => Pprintast.signature(ppf, sg)
        | [@implicit_arity] Impl(st, _) => Pprintast.structure(ppf, st)
        };
        Format.pp_print_newline(ppf, ());
      },
    )
  | Null => ()
  };
};
