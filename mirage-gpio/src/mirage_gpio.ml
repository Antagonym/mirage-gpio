module type S = sig
    include Mirage_device.S
    val set_mode: t -> int -> int -> unit
    val write: t -> int -> int -> unit
    val read: t -> int -> int
end

(* vim: set sw=4 ts=4 sts=4 expandtab: *)
