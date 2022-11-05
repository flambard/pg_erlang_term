CREATE TABLE slask (id serial, term erlang_term);


INSERT INTO slask (term)
VALUES ('[]'), ('erlang'), ('{tusan, ["vad", "fin" | "t"]}');


INSERT INTO slask (term)
VALUES ('[inte erlang}');


SELECT *
FROM slask;