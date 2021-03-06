-- Keep a log of any SQL queries you execute as you solve the mystery.
-- the evidence that has been gathered is : 10:15am at the Humphrey Street bakery, id is 295.

 SELECT * FROM crime_scene_reports WHERE YEAR = 2021 and month = 7 AND day = '28';
--| 295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.


--To get more evidence, it is needed to check on the interviews table to get the tree witnesses testimonies
 SELECT * FROM interviews WHERE year = 2021 AND month = 7 and day = 28 AND transcript LIKE '%bakery%';
--The transcripts are in records.txt
--evidence gathered:
--phone call for about half an hour /ATM on Leggett Street and saw the thief there withdrawing some money/
--cars that left the parking lot within 10 mintutes of the robbery 10:15am.
--Evidence retained : earliest flight out of Fiftyville tomorrow.

--to get a record of the atm trasactions that happened near to the location on Humphrey Lane with the account number also listed.
SELECt account_number FROM atm_transactions WHERE Month= 7 and day = 28  AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw' ;
"""
+----------------+
| account_number |
+----------------+
| 28500762       |
| 28296815       |
| 76054385       |
| 49610011       |
| 16153065       |
| 25506511       |
| 81061156       |
| 26013199       |
+----------------+
"""
--gather more info about the people who made withdrwals,cross with the people table and bank accounts table.

SELECT * from bank_accounts
JOIN people ON people.id = bank_accounts.person_id
--
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE Month= 7 and day = 28  AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw';

"""

+----------------+-----------+---------------+--------+---------+----------------+-----------------+---------------++----------------+-----------+---------------+--------+---------+----------------+-----------------+---------------+-----+----------------+------+-------+-----+----------------+------------------+--------+
| account_number | person_id | creation_year |   id   |  name   |  phone_number  | passport_number | license_plate | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
+----------------+-----------+---------------+--------+---------+----------------+-----------------+---------------+-----+----------------+------+-------+-----+----------------+------------------+--------+
| 49610011       | 686048    | 2010          | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
| 26013199       | 514354    | 2012          | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
| 16153065       | 458378    | 2012          | 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       | 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
| 28296815       | 395717    | 2014          | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       | 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
| 25506511       | 396669    | 2014          | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
| 28500762       | 467400    | 2014          | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
| 76054385       | 449774    | 2015          | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
| 81061156       | 438727    | 2018          | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       | 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
+----------------+-----------+---------------+--------+---------+----------------+-----------------+---------------+-----+----------------+------+-------+-----+----------------+------------------+--------+

"""

--gather more info about the people who made withdrwals, try to cross the data with the security logs from the bakery.and the'people' table
-- will need some refining :

SELECT * from bank_accounts
JOIN people ON people.id = bank_accounts.person_id
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN bakery_security_logs ON  bakery_security_logs.license_plate = people.license_plate
WHERE phone_calls.day = 28
AND atm_transactions.Month= 7 and atm_transactions.day = 28 AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw'
AND bakery_security_logs.day = 28 AND activity = 'exit' AND bakery_security_logs.minute < 26;


+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
| account_number | person_id | creation_year |   id   | name  |  phone_number  | passport_number | license_plate | id  |     caller     |    receiver    | year | month | day | duration | id  | account_number | year | month | day |  atm_location  | transaction_type | amount | id  | year | month | day | hour | minute | activity | license_plate |
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 245 | (367) 555-5533 | (022) 555-4052 | 2021 | 7     | 28  | 241      | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 236 | (367) 555-5533 | (344) 555-9601 | 2021 | 7     | 28  | 120      | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 285 | (367) 555-5533 | (704) 555-5790 | 2021 | 7     | 28  | 75       | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 26013199       | 514354    | 2012          | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+

--After filtering on duration :
SELECT * from bank_accounts
JOIN people ON people.id = bank_accounts.person_id
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN bakery_security_logs ON  bakery_security_logs.license_plate = people.license_plate
WHERE phone_calls.day = 28 AND duration < 50
AND atm_transactions.Month= 7 and atm_transactions.day = 28 AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw'
AND bakery_security_logs.day = 28 AND activity = 'exit' AND bakery_security_logs.minute < 26;
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
| account_number | person_id | creation_year |   id   | name  |  phone_number  | passport_number | license_plate | id  |     caller     |    receiver    | year | month | day | duration | id  | account_number | year | month | day |  atm_location  | transaction_type | amount | id  | year | month | day | hour | minute | activity | license_plate |
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 26013199       | 514354    | 2012          | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+

-- a syntaxlically longer query :
--Diana will be excluded since she didn't take the first flight the day after.

--cross them with the phone calls.
-- after querying only the entries that have  duration less thant 50 minutes
SELECT * from phone_calls
JOIN people on people.phone_number = phone_calls.caller
WHERE day = 28 AND duration < 50 AND caller IN(
    SELECT DISTINCT phone_number from bank_accounts
    JOIN people ON people.id = bank_accounts.person_id
    JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
    WHERE bakery_security_logs.day = 28 AND activity = 'exit' AND hour = 10 AND minute >15 AND minute < 26 AND account_number IN (
        SELECT account_number FROM atm_transactions
        WHERE Month= 7 and day = 28  AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw'
    )
) ORDER BY name, duration;

+-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+
| id  |     caller     |    receiver    | year | month | day | duration |   id   | name  |  phone_number  | passport_number | license_plate |
+-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+
| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
| 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
+-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+

-- Crossing with the passegenrs, flight air airports table gives an outlook that bruce is the one who took the first flight out of Fiftyville

 SELECT * from bank_accounts
    JOIN people ON people.id = bank_accounts.person_id
    JOIN passengers ON people.passport_number = passengers.passport_number
    JOIN flights ON passengers.flight_id = flights.id
    JOIN airports ON airports.id = flights.origin_airport_id
    JOIN phone_calls ON phone_calls.caller = people.phone_number
    JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
    JOIN bakery_security_logs ON  bakery_security_logs.license_plate = people.license_plate
    WHERE phone_calls.day = 28 AND duration < 50
    AND atm_transactions.Month= 7 and atm_transactions.day = 28 AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw'
    AND bakery_security_logs.day = 28 AND activity = 'exit' AND bakery_security_logs.minute < 26
    AND flights.day = 29 AND airports.city = 'Fiftyville'
    ORDER BY hour ;

+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
| account_number | person_id | creation_year |   id   | name  |  phone_number  | passport_number | license_plate | flight_id | passport_number | seat | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | id | abbreviation |          full_name          |    city    | id  |     caller     |    receiver    | year | month | day | duration | id  | account_number | year | month | day |  atm_location  | transaction_type | amount | id  | year | month | day | hour | minute | activity | license_plate |
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
| 49610011       | 686048    | 2010          | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 36        | 5773159633      | 4A   | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 26013199       | 514354    | 2012          | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       | 18        | 3592750733      | 4C   | 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      | 8  | CSF          | Fiftyville Regional Airport | Fiftyville | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
+----------------+-----------+---------------+--------+-------+----------------+-----------------+---------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+-----+----------------+----------------+------+-------+-----+----------+-----+----------------+------+-------+-----+----------------+------------------+--------+-----+------+-------+-----+------+--------+----------+---------------+
--Bruce took the first flight out of town. So diana is the only suspect left matching all the evidence gathered.
--Bruce, according to the evidence is the only suspect left.

--The thief then asked the person on the other end of the phone to purchase the flight ticket.
--So its bruce, first flight to laguardia new york.
"
+----+--------------+-------------------+---------------+
| id | abbreviation |     full_name     |     city      |
+----+--------------+-------------------+---------------+
| 4  | LGA          | LaGuardia Airport | New York City |
+----+--------------+-------------------+---------------+
--------------------------------------------------------------------
"



--Cross them with the flights and passenger.
SELECT * from people
JOIN bank_accounts on bank_accounts.person_id = people.id
WHERE phone_number IN
(
SELECT receiver from phone_calls
JOIN people on people.phone_number = phone_calls.caller
WHERE day = 28  AND caller IN(
    SELECT DISTINCT phone_number from bank_accounts
    JOIN people ON people.id = bank_accounts.person_id
    JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
    WHERE bakery_security_logs.day = 28 AND activity = 'exit' AND hour = 10 AND minute >15 AND minute < 26 AND account_number IN (
        SELECT account_number FROM atm_transactions
        WHERE Month= 7 and day = 28  AND atm_location LIKE '%Legget%' and transaction_type= 'withdraw'
        )
)
--Philip is to be excluded since :
--Philip was called by Diana.
EXCEPT SELECT receiver FROM phone_calls WHERE caller = '(770) 555-1861'--Diana's phone number excluded since the robber is bruce.
);

"

Thief:
+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
"
"
Accomplice:
+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       | 94751264       | 864400    | 2019          |
+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
"