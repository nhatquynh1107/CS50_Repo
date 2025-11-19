-- Keep a log of any SQL queries you execute as you solve the mystery.
.table
.schema airports
.schema crime_scene_reports

SELECT * FROM crime_scene_reports 
WHERE year = 2024 AND month = 7 AND day = 28
AND street = 'Humphrey Street';

SELECT name, transcript 
FROM interviews 
WHERE year = 2024 AND month = 7 AND day = 28 
AND transcript LIKE '%bakery%';

SELECT * FROM bakery_security_logs
WHERE year = 2024 AND month = 7 AND day = 28
AND hour = 10
AND minute BETWEEN 15 AND 25;

SELECT name, license_plate FROM people
WHERE license_plate IN
(
'5P2BI95', '94KL13X', '6P58WS2', '4328GD8',
'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55'
) ;  

SELECT account_number FROM atm_transactions
WHERE year = 2024 AND month = 7 AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

SELECT name, account_number FROM people 
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN 
( 28500762, 28296815, 76054385, 49610011,
  16153065, 25506511, 81061156, 26013199
);

SELECT * FROM flights
WHERE year = 2024 AND month = 7 AND day = 29
ORDER BY hour, minute
LIMIT 1;

SELECT name FROM people 
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE passengers.flight_id = 36;

SELECT city FROM airports
WHERE id = 4; 

SELECT * FROM phone_calls
WHERE year = 2024 AND month = 7 AND day = 28
AND duration < 60
AND caller IN
(
    SELECT phone_number FROM people
    WHERE name IN ('Bruce', 'Luca')
)

SELECT name FROM people
WHERE phone_number = '(367) 555-5533';

SELECT name FROM people
WHERE phone_number = '(375) 555-8161';