-- Lists all movies produced in 2010 sorted by their ratings
SELECT title, rating from movies
INNER JOIN ratings
ON ratings.movie_id = movies.id
WHERE year = 2010
ORDER BY rating DESC, title;