--Lists the names of all people who have directed a movie with a rating of at least 9.0
SELECT name FROM people
WHERE people.id IN (
SELECT directors.person_id from directors
JOIN ratings ON directors.movie_id = ratings.movie_id
where ratings.rating >= 9.0);