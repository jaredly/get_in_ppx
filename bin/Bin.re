
switch (Sys.argv) {
  | [|_, infile, outfile|] => Ast_multi.run(
    Migrate_parsetree.Versions.ocaml_402,
    Get_in_ppx.mapper.signature(Get_in_ppx.mapper),
    Get_in_ppx.mapper.structure(Get_in_ppx.mapper),
    infile,
    outfile
  )
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