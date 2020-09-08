-- Lists the names of all people who starred in a movie released in 2004, ordered by birth year
SELECT name from people
WHERE people.id IN (
SELECT stars.person_id from stars
JOIN movies ON stars.movie_id = movies.id
where movies.year = 2004)
ORDER BY people.birth;