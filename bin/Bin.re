open Migrate_parsetree;

let protectx = (x, ~finally, ~f) =>
  switch (f(x)) {
  | y =>
    finally(x);
    y;
  | exception e =>
    finally(x);
    raise(e);
  };

let with_file_in = (fn, ~f) =>
  protectx(open_in_bin(fn), ~finally=close_in, ~f);

let with_file_out = (fn, ~f) =>
  protectx(open_out_bin(fn), ~finally=close_out, ~f);

module Target = OCaml_402;

let run = (mapper, infile, outfile) => {
  Location.input_name := infile;
  with_file_in(infile, ~f=ic => switch (Ast_io.from_channel(ic)) {
    | Ok((fn, Ast_io.Intf((module V), signature))) =>
      let migrated = Versions.migrate((module V), (module Target)).copy_signature(signature);
      let mapped = mapper.Ast_402.Ast_mapper.signature(mapper, migrated);
      let back = Versions.migrate((module Target), (module V)).copy_signature(mapped);
      with_file_out(outfile, ~f=oc => {
        Ast_io.to_channel(oc, fn, Ast_io.Intf((module V), back))
      })
    | Ok((fn, Ast_io.Impl((module V), str))) =>
      let migrated = Versions.migrate((module V), (module Target)).copy_structure(str);
      let mapped = mapper.Ast_402.Ast_mapper.structure(mapper, migrated);
      let back = Versions.migrate((module Target), (module V)).copy_structure(mapped);
      with_file_out(outfile, ~f=oc => {
        Ast_io.to_channel(oc, fn, Ast_io.Impl((module V), back))
      })
    | Error(Ast_io.Unknown_version(_)) =>
      Location.raise_errorf(
        ~loc=Location.in_file(infile),
        "File is a binary ast for an unknown version of OCaml",
      )
    | Error(Ast_io.Not_a_binary_ast(prefix_read_from_file)) =>
      Location.raise_errorf(
        ~loc=Location.in_file(infile),
        "File is a binary ast",
      )
  })
}

switch (Sys.argv) {
  | [|_, infile, outfile|] => run(Get_in_ppx.mapper, Sys.argv[1], Sys.argv[2])
  | _ => print_endline("Usage: ppx <infile> <outfile>")
  exit(1)
}


// let () =
//   Migrate_parsetree.Driver.register(
//     ~name="get_in_ppx",
//     ~args=[],
//     Migrate_parsetree.Versions.ocaml_402,
//     Get_in_ppx.mapper,
//   );

// Driver.run_as_ppx_rewriter();