-- Lists all movie titles in which both Johnny Depp and Helena Bonham Carter starred.
SELECT title from movies
WHERE id IN (
SELECT movie_id from stars
WHERE person_id = (
SELECT id from people
WHERE name = "Johnny Depp"))
INTERSECT
SELECT title from movies
WHERE id IN(
SELECT movie_id from stars
WHERE person_id = (
SELECT id from people
WHERE name = "Helena Bonham Carter"));