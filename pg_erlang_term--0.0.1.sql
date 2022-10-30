\echo Use "CREATE EXTENSION pg_erlang_term" to load this file.

CREATE FUNCTION erlang_term_encode(text) RETURNS bytea
AS '$libdir/pg_erlang_term'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION erlang_term_decode(bytea) RETURNS text
AS '$libdir/pg_erlang_term'
LANGUAGE C IMMUTABLE STRICT;
