#!/usr/bin/env ocaml
#use "topfind"
#require "topkg"
#require "ocb-stubblr.topkg"
open Topkg
open Ocb_stubblr_topkg

let opams = [
  Pkg.opam_file "opam" ~lint_deps_excluding:(Some ["ocaml-freestanding"])
]

let nowhere ?force ?built ?cond ?exts ?dst _ = Pkg.nothing

let () =
  Pkg.describe ~build:(Pkg.build ~cmd()) ~opams "mirage-solo5" @@ fun c ->
  Ok [
    Pkg.mllib "lib/oS.mllib" ;
    Pkg.clib ~dllfield:nowhere "lib/libmirage-solo5_bindings.clib";
    (* Should be lib/pkgconfig/ but workaround ocaml/opam#2153 *)
    Pkg.share_root ~dst:"pkgconfig/" "lib/bindings/mirage-solo5.pc"
  ]
