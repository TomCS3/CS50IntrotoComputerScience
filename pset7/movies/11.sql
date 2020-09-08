-- Lists the titles of the five highest rated movies that Chadwick Boseman satted in from highest to lowest
SELECT title FROM movies
JOIN ratings on ratings.movie_id = movies.id
WHERE id IN (
SELECT movie_id FROM stars
WHERE person_id = (
SELECT id FROM people
WHERE name = "Chadwick Boseman"
))
ORDER BY ratings.rating DESC
LIMIT 5;