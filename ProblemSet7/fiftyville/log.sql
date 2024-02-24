-- Keep a log of any SQL queries you execute as you solve the mystery.
.schema -- to get an overview of what tables are available and what columns they do contain
SELECT * 
FROM crime_scene_reports 
WHERE year=2023 
AND month=7 
AND day=28; 
-- check what crimes were reported in the day of the theft
-- Theft took place at 10:15am at Humphrey street bakery.
-- here is a hint that all Interview transcripts mention the bakery. So we can make a query for this next
-- query for all interviews of that they containing the word bakery.
SELECT * 
FROM Interviews 
WHERE transcript 
LIKE '%bakery%' 
AND year = 2023 
AND month=7 
AND day=28;
-- Statement Ruth -> within 10 thief got in to car at parking lot of the bakery. -> check security footage for cars that leave the parking lot
-- Statement Eugene -> recognized the person and saw him withdraw money at the ATM at Leggett Street
-- Statement Raymond -> heard the phone call with the accomplice that they will take  the earliest flight tommor (29.7.2023)
--                      accomplice should by the flight tickets
/*
----------------------------------------------------------------------------------------------------------------------------------------+
| id  |  name   | year | month | day |                                                                                                                                                     transcript                                                                                                                                                      |
+-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 161 | Ruth    | 2023 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | 2023 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | 2023 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
+-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/
-- Lets check the leads in order:

-- LEAD1:

-- check bakery security logs of day of theft around the incident e.g.: 9-12 where cars exited the parking lot
SELECT * 
FROM bakery_security_logs 
WHERE year=2023 
AND month=7 
AND day=28 
AND hour > 9 
AND hour < 12 
AND activity='exit';
-- got some license plate for cars that left ~10 minutes after the incident 
/*
+-----+------+-------+-----+------+--------+----------+---------------+
| id  | year | month | day | hour | minute | activity | license_plate |
+-----+------+-------+-----+------+--------+----------+---------------+
| 260 | 2023 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
| 261 | 2023 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 262 | 2023 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
| 263 | 2023 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
| 264 | 2023 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
| 265 | 2023 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
| 266 | 2023 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
| 267 | 2023 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
| 268 | 2023 | 7     | 28  | 10   | 35     | exit     | 1106N58       |
+-----+------+-------+-----+------+--------+----------+---------------+
*/
-- license plates are stored in the people tables -> licence plate of thief probably in here

-- LEAD2

--check the ATM at leggett Street for transactions early in the morning
SELECT * 
FROM atm_transactions 
WHERE year=2023 
AND month=7 
AND day=28 
AND atm_location='Leggett Street' 
AND transaction_type='withdraw';
/*
+-----+----------------+------+-------+-----+----------------+------------------+--------+
| id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
+-----+----------------+------+-------+-----+----------------+------------------+--------+
| 246 | 28500762       | 2023 | 7     | 28  | Leggett Street | withdraw         | 48     |
| 264 | 28296815       | 2023 | 7     | 28  | Leggett Street | withdraw         | 20     |
| 266 | 76054385       | 2023 | 7     | 28  | Leggett Street | withdraw         | 60     |
| 267 | 49610011       | 2023 | 7     | 28  | Leggett Street | withdraw         | 50     |
| 269 | 16153065       | 2023 | 7     | 28  | Leggett Street | withdraw         | 80     |
| 288 | 25506511       | 2023 | 7     | 28  | Leggett Street | withdraw         | 20     |
| 313 | 81061156       | 2023 | 7     | 28  | Leggett Street | withdraw         | 30     |
| 336 | 26013199       | 2023 | 7     | 28  | Leggett Street | withdraw         | 35     |
+-----+----------------+------+-------+-----+----------------+------------------+--------+
*/
-- note: this is the account_number of the thief probably in here

-- LEAD 3:

-- check the telephone call between on the 2023-07-28 less than a minute
SELECT * 
FROM phone_calls 
WHERE year=2023 
AND month=7 
AND day=28 
AND duration<=60;
/*
+-----+----------------+----------------+------+-------+-----+----------+
| id  |     caller     |    receiver    | year | month | day | duration |
+-----+----------------+----------------+------+-------+-----+----------+
| 221 | (130) 555-0289 | (996) 555-8899 | 2023 | 7     | 28  | 51       |
| 224 | (499) 555-9472 | (892) 555-8872 | 2023 | 7     | 28  | 36       |
| 233 | (367) 555-5533 | (375) 555-8161 | 2023 | 7     | 28  | 45       |
| 234 | (609) 555-5876 | (389) 555-5198 | 2023 | 7     | 28  | 60       |
| 251 | (499) 555-9472 | (717) 555-1342 | 2023 | 7     | 28  | 50       |
| 254 | (286) 555-6063 | (676) 555-6554 | 2023 | 7     | 28  | 43       |
| 255 | (770) 555-1861 | (725) 555-3243 | 2023 | 7     | 28  | 49       |
| 261 | (031) 555-6622 | (910) 555-3251 | 2023 | 7     | 28  | 38       |
| 279 | (826) 555-1652 | (066) 555-9701 | 2023 | 7     | 28  | 55       |
| 281 | (338) 555-6650 | (704) 555-2131 | 2023 | 7     | 28  | 54       |
+-----+----------------+----------------+------+-------+-----+----------+
*/
-- caller is thief aand receiver should be the accomplies

-- LEAD 4:

-- check the flight list of the next morning of all flight from Fiftyville
SELECT * 
FROM flights 
WHERE origin_airport_id = (
    SELECT id 
    FROM airports 
    WHERE city='Fiftyville'
) 
AND year=2023 
AND month=7 
AND day=29 and 
hour <10;
/*
+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
| 43 | 8                 | 1                      | 2023 | 7     | 29  | 9    | 30     |
+----+-------------------+------------------------+------+-------+-----+------+--------+
*/
-- flight id to check passenger lists

-- extract person id from the atm list
SELECT * 
FROM bank_accounts  
WHERE account_number IN(
    SELECT account_number 
    FROM atm_transactions 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND atm_location='Leggett Street' 
    AND transaction_type='withdraw'
);

-- next stage cross referencing informations to find possible suspects for the thief and the accomplice
-- first lets check the license plate and the phone numbers in the people table and the atm info

SELECT * 
FROM people 
WHERE license_plate IN(
    SELECT license_plate 
    FROM bakery_security_logs 
    WHERE year=2023 
    AND month=7              
    AND day=28 
    AND hour > 9 
    AND hour < 12 
    AND activity='exit'
) 
AND phone_number IN(
    SELECT caller 
    FROM phone_calls 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
)
AND id IN(
    SELECT person_id 
    FROM bank_accounts  
    WHERE account_number IN(
        SELECT account_number 
        FROM atm_transactions 
        WHERE year=2023 
        AND month=7 
        AND day=28 
        AND atm_location='Leggett Street' 
        AND transaction_type='withdraw'
    )
);
-- list of current thief suspects:
-- hint the airport information is still missing this is the next part we will add to the query
/*
+--------+--------+----------------+-----------------+---------------+
|   id   |  name  |  phone_number  | passport_number | license_plate |
+--------+--------+----------------+-----------------+---------------+
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
| 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+--------+----------------+-----------------+---------------+
*/
-- First get list of all the passport number on the flights in the morning

SELECT passport_number 
FROM passengers
WHERE flight_id IN(
    SELECT id 
    FROM flights 
    WHERE origin_airport_id = (
        SELECT id 
        FROM airports 
        WHERE city='Fiftyville'
    ) 
    AND year=2023 
    AND month=7 
    AND day=29 and 
    hour <10
);

-- now add this addional information to the thief suspect query from before:
SELECT * 
FROM people 
WHERE license_plate IN(
    SELECT license_plate 
    FROM bakery_security_logs 
    WHERE year=2023 
    AND month=7              
    AND day=28 
    AND hour > 9 
    AND hour < 12 
    AND activity='exit'
) 
AND phone_number IN(
    SELECT caller 
    FROM phone_calls 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
)
AND id IN(
    SELECT person_id 
    FROM bank_accounts  
    WHERE account_number IN(
        SELECT account_number 
        FROM atm_transactions 
        WHERE year=2023 
        AND month=7 
        AND day=28 
        AND atm_location='Leggett Street' 
        AND transaction_type='withdraw'
    )
)
AND passport_number IN(
    SELECT passport_number 
    FROM passengers
    WHERE flight_id IN(
        SELECT id 
        FROM flights 
        WHERE origin_airport_id = (
            SELECT id 
            FROM airports 
            WHERE city='Fiftyville'
        ) 
        AND year=2023 
        AND month=7 
        AND day=29 and 
        hour <10
    )
);
/*
-- new suuspect list:
+--------+--------+----------------+-----------------+---------------+
|   id   |  name  |  phone_number  | passport_number | license_plate |
+--------+--------+----------------+-----------------+---------------+
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+--------+----------------+-----------------+---------------+
*/
-- now limit the parking lot time exit time frame to 10 minutes 10:15 to 10:25 since Taylor left almost 20 minutes 
-- after the incident and can likely be exluded, since the time frame mentioned in the crime report states a 10
-- minute time window after the incident. I wanted to be more thorough with the time window in the beginning.
SELECT * 
FROM people 
WHERE license_plate IN(
    SELECT license_plate 
    FROM bakery_security_logs 
    WHERE year=2023 
    AND month=7              
    AND day=28 
    AND hour=10 
    AND minute >= 0
    AND minute <=25 
    AND activity='exit'
) 
AND phone_number IN(
    SELECT caller 
    FROM phone_calls 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
)
AND id IN(
    SELECT person_id 
    FROM bank_accounts  
    WHERE account_number IN(
        SELECT account_number 
        FROM atm_transactions 
        WHERE year=2023 
        AND month=7 
        AND day=28 
        AND atm_location='Leggett Street' 
        AND transaction_type='withdraw'
    )
)
AND passport_number IN(
    SELECT passport_number 
    FROM passengers
    WHERE flight_id IN(
        SELECT id 
        FROM flights 
        WHERE origin_airport_id = (
            SELECT id 
            FROM airports 
            WHERE city='Fiftyville'
        ) 
        AND year=2023 
        AND month=7 
        AND day=29 and 
        hour <10
    )
);
/*
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+-------+----------------+-----------------+---------------+
*/

-- get the flight destinaction of the thief:
SELECT *
FROM airports
WHERE id IN(
    SELECT destination_airport_id
    FROM flights
    WHERE id IN(
        SELECT flight_id 
        FROM passengers
        WHERE passport_number IN(
            SELECT passport_number
            FROM people 
            WHERE license_plate IN(
                SELECT license_plate 
                FROM bakery_security_logs 
                WHERE year=2023 
                AND month=7              
                AND day=28 
                AND hour=10 
                AND minute >= 0
                AND minute <=25 
                AND activity='exit'
            ) 
            AND phone_number IN(
                SELECT caller 
                FROM phone_calls 
                WHERE year=2023 
                AND month=7 
                AND day=28 
                AND duration<=60
            )
            AND id IN(
                SELECT person_id 
                FROM bank_accounts  
                WHERE account_number IN(
                    SELECT account_number 
                    FROM atm_transactions 
                    WHERE year=2023 
                    AND month=7 
                    AND day=28 
                    AND atm_location='Leggett Street' 
                    AND transaction_type='withdraw'
                )
            )
            AND passport_number IN(
                SELECT passport_number 
                FROM passengers
                WHERE flight_id IN(
                    SELECT id 
                    FROM flights 
                    WHERE origin_airport_id = (
                        SELECT id 
                        FROM airports 
                        WHERE city='Fiftyville'
                    ) 
                    AND year=2023 
                    AND month=7 
                    AND day=29 and 
                    hour <10
                )
            )
        )

    )        
);
/*
+----+--------------+-------------------+---------------+
| id | abbreviation |     full_name     |     city      |
+----+--------------+-------------------+---------------+
| 4  | LGA          | LaGuardia Airport | New York City |
+----+--------------+-------------------+---------------+
*/


-- now the search for the accomplice begins.
-- list what information we have so far:
-- phone call receiver 
-- bought the airplane tickets look for transactions -> no online transaction info available...
-- maybe check for adjacent seat on the plane with the thief?
-- must be on the passenger list of the flights

-- start with phone call receiver and passenger flight list matches:

SELECT *
FROM people
WHERE phone_number IN(
    SELECT receiver
    FROM phone_calls 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
)
AND passport_number IN(
    SELECT passport_number
    FROM passengers 
    WHERE flight_id IN(
        SELECT id
        FROM flights
        WHERE origin_airport_id IN(
            SELECT id
            FROM airports
            WHERE city='Fiftyville'
        )
    )
);
/*
+--------+---------+----------------+-----------------+---------------+
|   id   |  name   |  phone_number  | passport_number | license_plate |
+--------+---------+----------------+-----------------+---------------+
| 250277 | James   | (676) 555-6554 | 2438825627      | Q13SVG6       |
| 251693 | Larry   | (892) 555-8872 | 2312901747      | O268ZZ0       |
| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| 626361 | Melissa | (717) 555-1342 | 7834357192      |               |
| 847116 | Philip  | (725) 555-3243 | 3391710505      | GW362R6       |
| 953679 | Doris   | (066) 555-9701 | 7214083635      | M51FA04       |
+--------+---------+----------------+-----------------+---------------+
*/
-- get passenger information of thief first:
SELECT *
FROM passengers
WHERE passport_number IN(
    SELECT passport_number 
    FROM people 
    WHERE license_plate IN(
        SELECT license_plate 
        FROM bakery_security_logs 
        WHERE year=2023 
        AND month=7              
        AND day=28 
        AND hour=10 
        AND minute >= 0
        AND minute <=25 
        AND activity='exit'
    ) 
    AND phone_number IN(
        SELECT caller 
        FROM phone_calls 
        WHERE year=2023 
        AND month=7 
        AND day=28 
        AND duration<=60
    )
    AND id IN(
        SELECT person_id 
        FROM bank_accounts  
        WHERE account_number IN(
            SELECT account_number 
            FROM atm_transactions 
            WHERE year=2023 
            AND month=7 
            AND day=28 
            AND atm_location='Leggett Street' 
            AND transaction_type='withdraw'
        )
    )
    AND passport_number IN(
        SELECT passport_number 
        FROM passengers
        WHERE flight_id IN(
            SELECT id 
            FROM flights 
            WHERE origin_airport_id = (
                SELECT id 
                FROM airports 
                WHERE city='Fiftyville'
            ) 
            AND year=2023 
            AND month=7 
            AND day=29 and 
            hour <10
        )
    )
);
/*
+-----------+-----------------+------+
| flight_id | passport_number | seat |
+-----------+-----------------+------+
| 36        | 5773159633      | 4A   |
+-----------+-----------------+------+
*/
-- I just realized that we can make the query for the flight tighter since we now know the excat flight they took.
-- So lets try the query with only the phone number and the reduced list of passengers:

SELECT *
FROM people
WHERE phone_number IN(
    SELECT receiver
    FROM phone_calls 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
)
AND passport_number IN(
    SELECT passport_number
    FROM passengers 
    WHERE flight_id IN(
        SELECT flight_id
        FROM passengers
        WHERE passport_number IN(
            SELECT passport_number 
            FROM people 
            WHERE license_plate IN(
                SELECT license_plate 
                FROM bakery_security_logs 
                WHERE year=2023 
                AND month=7              
                AND day=28 
                AND hour=10 
                AND minute >= 0
                AND minute <=25 
                AND activity='exit'
            ) 
            AND phone_number IN(
                SELECT caller 
                FROM phone_calls 
                WHERE year=2023 
                AND month=7 
                AND day=28 
                AND duration<=60
            )
            AND id IN(
                SELECT person_id 
                FROM bank_accounts  
                WHERE account_number IN(
                    SELECT account_number 
                    FROM atm_transactions 
                    WHERE year=2023 
                    AND month=7 
                    AND day=28 
                    AND atm_location='Leggett Street' 
                    AND transaction_type='withdraw'
                )
            )
            AND passport_number IN(
                SELECT passport_number 
                FROM passengers
                WHERE flight_id IN(
                    SELECT id 
                    FROM flights 
                    WHERE origin_airport_id = (
                        SELECT id 
                        FROM airports 
                        WHERE city='Fiftyville'
                    ) 
                    AND year=2023 
                    AND month=7 
                    AND day=29 and 
                    hour <10
                )
            )
        )
    )
);
/*
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 467400 | Luca  | (389) 555-5198 | 8496433585      | 4328GD8       |
| 953679 | Doris | (066) 555-9701 | 7214083635      | M51FA04       |
+--------+-------+----------------+-----------------+---------------+
*/
-- now lets check all seats of the flight if there are seats next to our thief:

SELECT * 
FROM passengers
WHERE flight_id IN(
    SELECT id 
    FROM flights 
    WHERE origin_airport_id = (
        SELECT id 
        FROM airports 
        WHERE city='Fiftyville'
    ) 
    AND year=2023 
    AND month=7 
    AND day=29 and 
    hour <10
);
/*
+-----------+-----------------+------+
| flight_id | passport_number | seat |
+-----------+-----------------+------+
| 36        | 7214083635      | 2A   |
| 36        | 1695452385      | 3B   |
| 36        | 5773159633      | 4A   |
| 36        | 1540955065      | 5C   |
| 36        | 8294398571      | 6C   |
| 36        | 1988161715      | 6D   |
| 36        | 9878712108      | 7A   |
| 36        | 8496433585      | 7B   |
| 43        | 7597790505      | 7B   |
| 43        | 6128131458      | 8A   |
| 43        | 6264773605      | 9A   |
| 43        | 3642612721      | 2C   |
| 43        | 4356447308      | 3B   |
| 43        | 7441135547      | 4A   |
+-----------+-----------------+------+
*/
-- There are no adjacent seat to Bruce seat 4A booked...
-- maybe check if the accomplice did make an transaction on the 28 to buy the tickets and refernce that with the list form above

SELECT *
FROM people
WHERE phone_number IN(
    SELECT receiver
    FROM phone_calls 
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
)
AND passport_number IN(
    SELECT passport_number
    FROM passengers 
    WHERE flight_id IN(
        SELECT flight_id
        FROM passengers
        WHERE passport_number IN(
            SELECT passport_number 
            FROM people 
            WHERE license_plate IN(
                SELECT license_plate 
                FROM bakery_security_logs 
                WHERE year=2023 
                AND month=7              
                AND day=28 
                AND hour=10 
                AND minute >= 0
                AND minute <=25 
                AND activity='exit'
            ) 
            AND phone_number IN(
                SELECT caller 
                FROM phone_calls 
                WHERE year=2023 
                AND month=7 
                AND day=28 
                AND duration<=60
            )
            AND id IN(
                SELECT person_id 
                FROM bank_accounts  
                WHERE account_number IN(
                    SELECT account_number 
                    FROM atm_transactions 
                    WHERE year=2023 
                    AND month=7 
                    AND day=28 
                    AND atm_location='Leggett Street' 
                    AND transaction_type='withdraw'
                )
            )
            AND passport_number IN(
                SELECT passport_number 
                FROM passengers
                WHERE flight_id IN(
                    SELECT id 
                    FROM flights 
                    WHERE origin_airport_id = (
                        SELECT id 
                        FROM airports 
                        WHERE city='Fiftyville'
                    ) 
                    AND year=2023 
                    AND month=7 
                    AND day=29 and 
                    hour <10
                )
            )
        )
    )
)
AND id IN(
    SELECT id 
    FROM bank_accounts
    WHERE account_number IN(
        SELECT account_number
        FROM atm_transactions
        WHERE year=2023
        AND month=7
        AND day=28
        AND transaction_type = 'deposit'
    )
);
-- yields no more information....
-- Think what connections the thief and the accomplice have
-- phone call -> I also can narrow the receiver phone call list with the information of the thief:
-- so now combine the following informations into one query:
-- flight id -> passenger list -> person id list of all possible accomplices
-- get phone number of receiver with info from thief phone number

-- First get phone call id:
SELECT receiver
FROM phone_calls
WHERE id IN(
    SELECT id
    FROM phone_calls
    WHERE year=2023 
    AND month=7 
    AND day=28 
    AND duration<=60
    AND caller IN(
        SELECT phone_number 
        FROM people 
        WHERE license_plate IN(
            SELECT license_plate 
            FROM bakery_security_logs 
            WHERE year=2023 
            AND month=7              
            AND day=28 
            AND hour=10 
            AND minute >= 0
            AND minute <=25 
            AND activity='exit'
        ) 
        AND phone_number IN(
            SELECT caller 
            FROM phone_calls 
            WHERE year=2023 
            AND month=7 
            AND day=28 
            AND duration<=60
        )
        AND id IN(
            SELECT person_id 
            FROM bank_accounts  
            WHERE account_number IN(
                SELECT account_number 
                FROM atm_transactions 
                WHERE year=2023 
                AND month=7 
                AND day=28 
                AND atm_location='Leggett Street' 
                AND transaction_type='withdraw'
            )
        )
        AND passport_number IN(
            SELECT passport_number 
            FROM passengers
            WHERE flight_id IN(
                SELECT id 
                FROM flights 
                WHERE origin_airport_id = (
                    SELECT id 
                    FROM airports 
                    WHERE city='Fiftyville'
                ) 
                AND year=2023 
                AND month=7 
                AND day=29 and 
                hour <10
            )
        )
    )
);
-- now get the person informatin with the phone number:
SELECT *
FROM people
WHERE phone_number IN(
    SELECT receiver
    FROM phone_calls
    WHERE id IN(
        SELECT id
        FROM phone_calls
        WHERE year=2023 
        AND month=7 
        AND day=28 
        AND duration<=60
        AND caller IN(
            SELECT phone_number 
            FROM people 
            WHERE license_plate IN(
                SELECT license_plate 
                FROM bakery_security_logs 
                WHERE year=2023 
                AND month=7              
                AND day=28 
                AND hour=10 
                AND minute >= 0
                AND minute <=25
                AND activity='exit'
            ) 
            AND phone_number IN(
                SELECT caller 
                FROM phone_calls 
                WHERE year=2023 
                AND month=7 
                AND day=28 
                AND duration<=60
            )
            AND id IN(
                SELECT person_id 
                FROM bank_accounts  
                WHERE account_number IN(
                    SELECT account_number 
                    FROM atm_transactions 
                    WHERE year=2023 
                    AND month=7 
                    AND day=28 
                    AND atm_location='Leggett Street' 
                    AND transaction_type='withdraw'
                )
            )
            AND passport_number IN(
                SELECT passport_number 
                FROM passengers
                WHERE flight_id IN(
                    SELECT id 
                    FROM flights 
                    WHERE origin_airport_id = (
                        SELECT id 
                        FROM airports 
                        WHERE city='Fiftyville'
                    ) 
                    AND year=2023 
                    AND month=7 
                    AND day=29 and 
                    hour <10
                )
            )
        )
    )
);
/*
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
+--------+-------+----------------+-----------------+---------------+
*/

-- I am unsure if the accomplice took the flight with the thief or not. If the accomplice also took the flight
-- Robin can not be the accomplice since the does not have a passport_number. Maybe I made the wrong assumption in
-- the beginning. In the third statement they only take about the ticket in singular, so I believe that my solution
-- is correct. 
-- Did some sanity check with the second thief suspect Taylor and who he called. But there the possible accomplice 
-- is also on no flight so. I strongly believe that my conclusion is right.


