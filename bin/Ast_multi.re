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

let run
// : (Versions.ocaml_version('types), Versions.get_mapper('types), string, string) => unit
 = (version, map_sig, map_str, infile, outfile) => {
  Location.input_name := infile;
  with_file_in(infile, ~f=ic => switch (Ast_io.from_channel(ic)) {
    | Ok((fn, Ast_io.Intf((module V), signature))) =>
      // module Version = (val version: Versions.OCaml_version with type Ast.Ast_mapper.mapper = 'mapper);
      let migrated = Versions.migrate((module V), version).copy_signature(signature);
      // let mapped = Version.Ast.map_signature(mapper, migrated);
      let mapped = map_sig(migrated);
      let back = Versions.migrate(version, (module V)).copy_signature(mapped);
      with_file_out(outfile, ~f=oc => {
        Ast_io.to_channel(oc, fn, Ast_io.Intf((module V), back))
      })
    | Ok((fn, Ast_io.Impl((module V), str))) =>
      let migrated = Versions.migrate((module V), version).copy_structure(str);
      let mapped = map_str(migrated);
      let back = Versions.migrate(version, (module V)).copy_structure(mapped);
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
