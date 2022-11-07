`pg_erlang_term`
================

`pg_erlang_term` is a PostgreSQL extension that adds a new data type `erlang_term` for storing and working with arbitrary Erlang terms.

The terms are stored in the [Erlang External Term format](https://www.erlang.org/doc/apps/erts/erl_ext_dist.html) internally and are sent unmodified over the network when using binary input/output ([Postgrex](https://hexdocs.pm/postgrex/readme.html) does this). 

Installing
----------
`pg_erlang_term` depends on the [`ei`](https://www.erlang.org/doc/man/ei.html) library which is included in Erlang/OTP.

Build and install `pg_erlang_term` like so:
```shell
$ make
$ sudo make install
```

and optionally:
```shell
$ make installcheck
```

and then in PostgreSQL:
```sql
CREATE EXTENSION pg_erlang_term;
```

Limitations
-----------

The textual representation of Erlang terms is currently handled by `ei_s_print_term` for decoding and `ei_x_format` for encoding.

These functions have some limitations and may not be able represent all kinds of Erlang terms.

**Note that Ecto+Postgrex uses binary input/output which does not have these problems.**
