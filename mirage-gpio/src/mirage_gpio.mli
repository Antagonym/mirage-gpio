(** GPIO device signatures. *)

(** Operations on a GPIO device. *)
module type S = sig
    include Mirage_device.S

    val set_mode: t -> int -> int -> unit
    (** [mode pin mode] sets the mode of the GPIO pin number [pin] to [mode],
     * where [mode] may be 0 (input mode) or 1 (output mode). Attempting to set
     * any other mode will have no effect. *)

    val write: t -> int -> int -> unit
    (** [write pin value] sets the value of the GPIO pin number [pin] to
     * [value], where [value] may be 0 (LOW) or 1 (HIGH). Attempting to set any
     * other value will have no effect.
     * Note that this assumes that the pin is in output mode. If it is not, this
     * function has undefined behavior. *)

    val read: t -> int -> int
    (** [read pin] returns the value of the GPIO pin number [pin], where the
     * returned value may be 0 (LOW) or 1 (HIGH).
     * Note that this assumes that the pin is in input mode. If it is not, this
     * function has undefined behavior. *)

end

(* vim: set sw=4 ts=4 sts=4 expandtab: *)
