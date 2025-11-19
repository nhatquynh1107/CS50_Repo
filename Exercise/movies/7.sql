SELECT title, rating FROM movies
JOIN ratings On movies.id = ratings.movie_id
WHERE movies.year = 2010 
ORDER BY rating DESC, title ASC