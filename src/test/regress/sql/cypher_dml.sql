--
-- Cypher Query Language - DML
--

-- prepare

DROP TABLE IF EXISTS history;

CREATE TABLE history (year, event) AS VALUES
(1996, 'PostgreSQL'),
(2016, 'Graph');

DROP GRAPH agens CASCADE;
CREATE GRAPH agens;

--
-- RETURN
--

RETURN 3 + 4, 'hello' || ' agens';

RETURN 3 + 4 AS lucky, 'hello' || ' agens' AS greeting;

RETURN (SELECT event FROM history WHERE year = 2016);

SELECT * FROM (RETURN 3 + 4, 'hello' || ' agens') AS _(lucky, greeting);

--
-- CREATE
--

CREATE VLABEL repo;
CREATE ELABEL lib;
CREATE ELABEL doc;

CREATE (g:repo {'name': 'agens-graph',
                year: (SELECT year FROM history WHERE event = 'Graph')})
RETURN properties(g) AS g;

MATCH (g:repo)
CREATE (j:repo '{"name": "agens-graph-jdbc", "year": 2016}'),
       (d:repo =jsonb_build_object('name', 'agens-graph-docs', 'year', 2016))
CREATE (g)-[l:lib {lang: 'java'}]->(j),
       p=(g)
         -[:lib {lang: 'c'}]->
         (:repo {name: 'agens-graph-odbc', year: 2016}),
       (g)-[e:doc {lang: 'en'}]->(d)
RETURN properties(l) AS lj, properties(j) AS j,
       properties((edges(p))[1]) AS lc, properties((vertices(p))[2]) AS c,
       properties(e) AS e, properties(d) AS d;

CREATE ()-[a:lib]->(a);
CREATE a=(), (a);
CREATE (a), (a {});
CREATE (a), (a);
CREATE (=0);
CREATE ()-[]-();
CREATE ()-[]->();
CREATE ()-[:lib|doc]->();
CREATE (a)-[a:lib]->();
CREATE ()-[a:lib]->()-[a:doc]->();
CREATE a=(), ()-[a:doc]->();
CREATE ()-[:lib =0]->();
CREATE (a), a=();
CREATE ()-[a:lib]->(), a=();
CREATE a=(), a=();

--
-- MATCH
--

MATCH (a) RETURN a.name AS a;
MATCH (a), (a) RETURN a.name AS a;

CREATE ();
MATCH (a:repo) RETURN a.name AS name, a['year'] AS year;

MATCH p=(a)-[b]-(c)
RETURN a.name AS a, b.lang AS b, c.name AS c
       ORDER BY a, b, c;

MATCH (a)<-[b]-(c)-[d]->(e)
RETURN a.name AS a, b.lang AS b, c.name AS c,
       d.lang AS d, e.name AS e
       ORDER BY a, b, c, d, e;

MATCH (a)<-[b]-(c), (c)-[d]->(e)
RETURN a.name AS a, b.lang AS b, c.name AS c,
       d.lang AS d, e.name AS e
       ORDER BY a, b, c, d, e;

MATCH (a)<-[b]-(c) MATCH (c)-[d]->(e)
RETURN a.name AS a, b.lang AS b, c.name AS c,
       d.lang AS d, e.name AS e
       ORDER BY a, b, c, d, e;

MATCH (a)<-[b]-(c), (f)-[g]->(h), (c)-[d]->(e)
RETURN a.name AS a, b.lang AS b, c.name AS c,
       d.lang AS d, e.name AS e,
       f.name AS f, g.lang AS g, h.name AS h
       ORDER BY a, b, c, d, e, f, g, h;

MATCH (a {name: 'agens-graph'}), (a {year: 2016}) RETURN properties(a) AS a;
MATCH p=(a)-[]->({name: 'agens-graph-jdbc'}) RETURN a.name AS a;
MATCH p=()-[:lib]->(a) RETURN a.name AS a;
MATCH p=()-[{lang: 'en'}]->(a) RETURN a.name AS a;

MATCH (a {year: (SELECT year FROM history WHERE event = 'Graph')})
WHERE a.name = 'agens-graph'
RETURN a.name AS a;

MATCH (a), (a:repo) RETURN a.name AS a;

MATCH ();
MATCH ()-[a]-(), (a) RETURN *;
MATCH a=(), (a) RETURN *;
MATCH (a =0) RETURN *;
MATCH ()-[a]-(a) RETURN *;
MATCH ()-[a]-()-[a]-() RETURN *;
MATCH a=(), ()-[a]-() RETURN *;
MATCH p=()-[:lib|doc]->() RETURN *;
MATCH ()-[a =0]-() RETURN *;
MATCH (a), a=() RETURN *;
MATCH ()-[a]->(), a=() RETURN *;
MATCH a=(), a=() RETURN *;

MATCH (a {name: properties->'name'}) RETURN *;
MATCH (a {name: a.properties->'name'}) RETURN *;

-- OPTIONAL MATCH

CREATE GRAPH o;
SET graph_path = o;

CREATE VLABEL person;
CREATE ELABEL knows;

CREATE (:person {name: 'someone'})-[:knows]->(:person {name: 'somebody'}),
       (:person {name: 'anybody'})-[:knows]->(:person {name: 'nobody'});

OPTIONAL MATCH (n)-[r]->(p), (m)-[s]->(q)
RETURN n.name AS n, type(r) AS r, p.name AS p,
       m.name AS m, type(s) AS s, q.name AS q
ORDER BY n, p, m, q;

MATCH (n:person), (m:person) WHERE id(n) <> id(m)
OPTIONAL MATCH (n)-[r]->(p), (m)-[s]->(q)
RETURN n.name AS n, type(r) AS r, p.name AS p,
       m.name AS m, type(s) AS s, q.name AS q
ORDER BY n, p, m, q;

MATCH (n:person), (m:person) WHERE id(n) <> id(m)
OPTIONAL MATCH (n)-[r]->(p), (m)-[s]->(q) WHERE m.name = 'someone'
RETURN n.name AS n, type(r) AS r, p.name AS p,
       m.name AS m, type(s) AS s, q.name AS q
ORDER BY n, p, m, q;

-- Variable Length Relationship

CREATE GRAPH t;
SET graph_path = t;

CREATE VLABEL time;
CREATE ELABEL goes;

CREATE (:time {sec: 1})-[:goes]->
       (:time {sec: 2})-[:goes]->
       (:time {sec: 3})-[:goes]->
       (:time {sec: 4})-[:goes]->
       (:time {sec: 5})-[:goes]->
       (:time {sec: 6})-[:goes]->
       (:time {sec: 7})-[:goes]->
       (:time {sec: 8})-[:goes]->
       (:time {sec: 9});

CREATE (:time {sec: 9})-[:goes*1..2]->(:time {sec: 10});

MATCH (a:time)-[x:goes*3]->(b:time)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

MATCH (a:time)-[x:goes*0]->(b:time)
RETURN a.sec AS a, x, b.sec AS b;

MATCH (a:time)-[x:goes*0..1]->(b:time)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

MATCH (a:time)-[x:goes*..1]->(b:time)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

MATCH (a:time)-[x:goes*0..]->(b:time)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

MATCH (a:time)-[x:goes*3..6]->(b:time)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

MATCH (a:time)-[x:goes*2]->(b:time)-[y:goes]->(c:time)-[z:goes*2]->(d:time)
RETURN a.sec AS a, array_length(x, 1) AS x,
       b.sec AS b, type(y) AS y,
       c.sec AS c, array_length(z, 1) AS z, d.sec AS d;

MATCH (a:time)-[x:goes*2]->(b:time)
MATCH (b)-[y:goes]->(c:time)
MATCH (c)-[z:goes*2]->(d:time)
RETURN a.sec AS a, array_length(x, 1) AS x,
       b.sec AS b, type(y) AS y,
       c.sec AS c, array_length(z, 1) AS z, d.sec AS d;

MATCH (d:time)<-[z:goes*2]-(c:time)<-[y:goes]-(b:time)<-[x:goes*2]-(a:time)
RETURN d.sec AS d, array_length(z, 1) AS z,
       c.sec AS c, type(y) AS y,
       b.sec AS b, array_length(x, 1) AS x, a.sec AS a;

MATCH (d:time)<-[z:goes*2]-(c:time)
MATCH (c)<-[y:goes]-(b:time)
MATCH (b)<-[x:goes*2]-(a:time)
RETURN d.sec AS d, array_length(z, 1) AS z,
       c.sec AS c, type(y) AS y,
       b.sec AS b, array_length(x, 1) AS x, a.sec AS a;

MATCH (a:time)-[x*0..2]-(b)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

CREATE (:time {sec: 11})-[:goes {int: 1}]->
       (:time {sec: 12})-[:goes {int: 1}]->
       (:time {sec: 13})-[:goes {int: 2}]->
       (:time {sec: 15})-[:goes {int: 1}]->
       (:time {sec: 16})-[:goes {int: 1}]->
       (:time {sec: 17});

MATCH (a:time)-[x:goes*1..2 {int: 1}]->(b:time)
RETURN a.sec AS a, array_length(x, 1) AS x, b.sec AS b;

SET graph_path = agens;

--
-- DISTINCT
--

MATCH (a:repo)-[]-() RETURN DISTINCT a.name AS a ORDER BY a;

MATCH (a:repo)-[b]-(c)
RETURN DISTINCT ON (a) a.name AS a, b.lang AS b, c.name AS c;

--
-- ORDER BY
--

MATCH (a:repo) RETURN a.name AS a ORDER BY a;
MATCH (a:repo) RETURN a.name AS a ORDER BY a ASC;
MATCH (a:repo) RETURN a.name AS a ORDER BY a DESC;

--
-- SKIP and LIMIT
--

MATCH (a:repo) RETURN a.name AS a ORDER BY a SKIP 1 LIMIT 1;

--
-- WITH
--

MATCH (a:repo) WITH a.name AS name RETURN name;

MATCH (a)
WITH a WHERE label(a) = 'repo'
MATCH p=(a)-[]->(b)
RETURN b.name AS b ORDER BY b;

MATCH (a) WITH a RETURN b;
MATCH (a) WITH a.name RETURN *;
MATCH () WITH a AS z RETURN a;

--
-- UNION
--

MATCH (a:repo)
RETURN a.name AS a
UNION ALL
MATCH (b:lib)
RETURN b.lang AS b
UNION ALL
MATCH (c:doc)
RETURN c.lang AS c;

MATCH (a)
RETURN a
UNION
MATCH (b)
RETURN b.name;

--
-- aggregates
--

MATCH (a)-[]-(b) RETURN count(a) AS a, b.name AS b ORDER BY a, b;

--
-- EXISTS
--

MATCH (a:repo) WHERE exists((a)-[]->()) RETURN a.name AS a;

--
-- SIZE
--

MATCH (a:repo) RETURN a.name AS a, size((a)-[]->()) AS s;

--
-- LOAD
--

MATCH (a) LOAD FROM history AS a RETURN *;

CREATE VLABEL feature;
CREATE ELABEL supported;

MATCH (a:repo {name: 'agens-graph'})
LOAD FROM history AS h
CREATE (:feature {name: (h).event})-[:supported]->(a);

MATCH p=(a)-[:supported]->() RETURN properties(a) AS a ORDER BY a;

--
-- DELETE
--

MATCH (a) DELETE a;

MATCH p=()-[:lib]->() DETACH DELETE (vertices(p))[2];
MATCH (a:repo) RETURN a.name AS a;

MATCH ()-[a:doc]->() DETACH DELETE end_vertex(a);
MATCH (a:repo) RETURN a.name AS a;

MATCH (a) DETACH DELETE a;
MATCH (a) RETURN a;

SELECT count(*) FROM agens.ag_edge;

--
-- Uniqueness
--

CREATE GRAPH u;
SET graph_path = u;

CREATE ELABEL rel;

CREATE (s {id: 1})-[:rel {p: 'a'}]->({id: 2})-[:rel {p: 'b'}]->(s);

MATCH (s)-[r1]-(m)-[r2]-(x)
RETURN s.id AS s, r1.p AS r1, m.id AS m, r2.p AS r2, x.id AS x
       ORDER BY s, r1, m, r2, x;

--
-- SET/REMOVE
--

CREATE GRAPH p;
SET graph_path = p;

CREATE ELABEL rel;

CREATE ({name: 'someone'})-[:rel {k: 'v'}]->({name: 'somebody'});

MATCH (n)-[r]->(m) SET r.l = '"w"', n = m, r.k = NULL;
MATCH (n)-[r]->(m) REMOVE m.name;

MATCH (n)-[r]->(m)
RETURN properties(n) AS n, properties(r) AS r, properties(m) AS m;

MATCH (n) DETACH DELETE (n);

-- overwrite (Standard SQL)
CREATE ({age: 10});
MATCH (a) SET a.age = '11', a.age = (a.age::int + 1)::text::jsonb;
MATCH (a) RETURN properties(a);
MATCH (a) DETACH DELETE (a);

-- multiple SET's

CREATE ({age: 10});
MATCH (a) SET a.age = '11' SET a.age = (a.age::int + 1)::text::jsonb;
MATCH (a) RETURN properties(a);
MATCH (a) DETACH DELETE (a);

CREATE ()-[:rel {k: 'v'}]->();
MATCH ()-[r]->() SET r.l = '"x"' SET r.l = '"y"';
MATCH ()-[r]->() RETURN properties(r) AS r;
MATCH (a) DETACH DELETE (a);

-- += operator

CREATE ({age: 10});
MATCH (a) SET a += {name: 'bitnine', age: '3'};
MATCH (a) RETURN properties(a);

MATCH (a) SET a += NULL;
MATCH (a) SET a.name += NULL;
MATCH (a) SET a.name += '"someone"';

MATCH (a) DETACH DELETE (a);

-- remove

CREATE ({a: 'a', b: 'b', c: 'c'});
MATCH (a) SET a.a = NULL REMOVE a.b;
MATCH (a) RETURN properties(a);

MATCH (a) SET a = NULL;

MATCH (a) DETACH DELETE (a);

-- referring to undefined attributes
CREATE ({name: 'bitnine'});
CREATE ({age: 10});
MATCH (a) SET a.age = (a.age::int + 1)::text::jsonb;
MATCH (a) RETURN properties(a);

-- cleanup

DROP GRAPH p CASCADE;
DROP GRAPH u CASCADE;
DROP GRAPH t CASCADE;
DROP GRAPH o CASCADE;

SET graph_path = agens;

DROP VLABEL feature;
DROP ELABEL supported;

DROP VLABEL repo;
DROP ELABEL lib;
DROP ELABEL doc;

DROP GRAPH agens CASCADE;

DROP TABLE history;
