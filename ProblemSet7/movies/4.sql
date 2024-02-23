SELECT COUNT(title) FROM movies
 WHERE movies.id=(
    SELECT movie_id 
    FROM ratings 
    WHERE rating=10);