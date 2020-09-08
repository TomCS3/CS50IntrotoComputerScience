-- Lists the names of all the people who starred in a movie with Kevin Bacon
SELECT name FROM people
WHERE name is NOT "Kevin Bacon" and id IN (
SELECT person_id FROM stars
WHERE movie_id IN (
SELECT movie_id FROM stars
WHERE person_id IN (
SELECT id FROM people
WHERE name = "Kevin Bacon" and birth = 1958)));