/* get list of all movies where Kevin Bacon plays
SELECT id,title FROM movies WHERE id IN( SELECT movie_id FROM stars WHERE person_id=(SELECT id FROM people WHERE name='Kevin Bacon' and birth=1958));
query were I get the list containing my person
SELECT COUNT(name) FROM people WHERE id IN(SELECT person_id FROM stars WHERE movie_id IN (SELECT id FROM movies WHERE id IN( SELECT movie_id FROM stars WHERE person_id=(SELECT id FROM people WHERE name='Kevin Bacon' and birth=1958))))
now remove hom from that list */

SELECT COUNT(name) 
FROM people 
WHERE id IN(
    SELECT person_id 
    FROM (
        SELECT * 
        FROM stars 
        WHERE NOT person_id IN(
            SELECT id 
            FROM people 
            WHERE name='Kevin Bacon' AND birth=1958
        )
    )
    WHERE movie_id IN(
        SELECT id 
        FROM movies 
        WHERE id IN(
            SELECT movie_id 
            FROM stars 
            WHERE person_id=(
                SELECT id 
                FROM people 
                WHERE name='Kevin Bacon' and birth=1958
            )
        )
    )
);
