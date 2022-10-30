CREATE EXTENSION IF NOT EXISTS pg_erlang_term;

select erlang_term_encode('[]');
select erlang_term_encode('erlang');
