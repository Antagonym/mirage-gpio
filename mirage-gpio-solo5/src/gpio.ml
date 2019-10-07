(* I can't OCAML, so this is mostly just a bunch of cargo culting. If anything
 * seems really weird about this code, that's probably the reason. Feel free
 * to fix stuff if you know betther than me. *)

open Mirage_gpio
open OS.Solo5

(* Make the compiler STFU *)
type 'a io = 'a Lwt.t

type t = {
    name: string;
}

external solo5_gpio_mode:
    int -> int -> unit = "mirage_solo5_gpio_mode"
external solo5_gpio_write:
    int -> int -> unit = "mirage_solo5_gpio_write"
external solo5_gpio_read:
    int -> int = "mirage_solo5_gpio_read"

let disconnect _id =
    Lwt.return_unit

let connect name =
    Lwt.return ({ name })


let set_mode t pin mode =
    (* Make the compiler STFU *)
    let name = t.name in
    solo5_gpio_mode pin mode

let write t pin value =
    (* Make the compiler STFU *)
    let name = t.name in
    solo5_gpio_write pin value

let read t pin =
    (* Make the compiler STFU *)
    let name = t.name in
    solo5_gpio_read pin

(* vim: set sw=4 ts=4 sts=4 expandtab: *)
