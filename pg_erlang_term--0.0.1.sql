\echo Use "CREATE EXTENSION pg_erlang_term" to load this file.
CREATE FUNCTION erlang_term_encode(text) RETURNS bytea AS '$libdir/pg_erlang_term' LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION erlang_term_decode(bytea) RETURNS text AS '$libdir/pg_erlang_term' LANGUAGE C IMMUTABLE STRICT;


CREATE TYPE erlang_term;


CREATE FUNCTION erlang_term_input(cstring) RETURNS erlang_term AS '$libdir/pg_erlang_term' LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION erlang_term_output(erlang_term) RETURNS cstring AS '$libdir/pg_erlang_term' LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION erlang_term_receive(internal) RETURNS erlang_term AS '$libdir/pg_erlang_term' LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION erlang_term_send(erlang_term) RETURNS bytea AS '$libdir/pg_erlang_term' LANGUAGE C IMMUTABLE STRICT;


CREATE TYPE erlang_term (INPUT = erlang_term_input, OUTPUT = erlang_term_output, RECEIVE = erlang_term_receive, SEND = erlang_term_send);

