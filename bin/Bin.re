open Migrate_parsetree;

let () =
  Migrate_parsetree.Driver.register(
    ~name="get_in_ppx",
    ~args=[],
    Migrate_parsetree.Versions.ocaml_402,
    Get_in_ppx.mapper,
  );

Driver.run_as_ppx_rewriter();