CREATE TABLE test(real x, text t);

INSERT INTO test VALUES (3.14, 'askljdfhnaskdfj');
INSERT INTO test VALUES('string', 2.728);

SELECT * FROM test;

SELECT * FROM test order by y desc;

SELECT x FROM test
WHERE x < 5
LIMIT 4;

