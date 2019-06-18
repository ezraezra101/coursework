-- Miya Gaskell (mgaskell)
-- Ezra Davis (edavis)
DROP TABLE BookedTour PURGE;
DROP TABLE TravelingWith PURGE;
DROP TABLE Customer PURGE;
DROP TABLE Traveling PURGE;
DROP TABLE Location PURGE;
DROP TABLE Tour PURGE;
DROP TABLE Vehicle PURGE;
DROP TABLE Guide PURGE;

CREATE TABLE Guide (
	DriverLicense VARCHAR2(20),
	FirstName VARCHAR2(20),
	LastName VARCHAR2(20),
	Phone CHAR(10),
	VehicleType CHAR(10),
	Title VARCHAR2(30),
	Salary NUMBER,
	HireDate DATE NOT NULL,
	CONSTRAINT Guide_PK Primary Key (DriverLicense),
	CONSTRAINT Guide_VehicleVal check (VehicleType in ('car', 'amphibious', 'bus')),
	CONSTRAINT TitleVal check (Title in ('senior guide', 'junior guide', 'guide'))
);

CREATE TABLE Vehicle (
	LicensePlate CHAR(8),
	VehicleType CHAR(10),
	Make VARCHAR2(20),
	Model VARCHAR2(20),
	Year NUMBER(4),
	MaxPassenger NUMBER NOT NULL,
	CONSTRAINT Vehicle_PK Primary Key (LicensePlate),
	CONSTRAINT Vehicle_VehicleVal check (VehicleType in ('car', 'amphibious', 'bus')),
	CONSTRAINT Vehicle_PassVal check (MaxPassenger > 0)
);

CREATE TABLE Tour (
	TourID VARCHAR2(20),
	TourName VARCHAR2(20) NOT NULL,
	Description VARCHAR2(150),
	City VARCHAR2(20),
	State CHAR(2),
	Duration INTERVAL DAY TO SECOND,
	VehicleType CHAR(10),
	AdultCost NUMBER,
	ChildCost NUMBER,
	CONSTRAINT Tour_PK Primary Key (TourID),
	CONSTRAINT Tour_VehicleVal check (VehicleType in ('car', 'amphibious', 'bus'))
);

CREATE TABLE Location (
	LocationID VARCHAR2(20),
	LocationName VARCHAR2(30) NOT NULL,
	Type CHAR(10),
	Address VARCHAR2(150),
	Longitude NUMBER,
	Latitude NUMBER,
	CONSTRAINT Location_PK Primary Key (LocationID),
	CONSTRAINT TypeVal check (Type in ('historic', 'museum', 'restaurant'))
);

CREATE TABLE Traveling (
	TourID VARCHAR2(20),
	LocationID VARCHAR2(20),
	CONSTRAINT Traveling_PK Primary Key (TourID, LocationID),
	CONSTRAINT Traveling_TourID
		FOREIGN KEY (TourID)
		REFERENCES Tour(TourID),
	CONSTRAINT Traveling_LocationID
		FOREIGN KEY (LocationID)
		REFERENCES Location(LocationID)
);

CREATE TABLE Customer (
	CustomerID VARCHAR2(20),
	FirstName VARCHAR2(20),
	LastName VARCHAR2(20),
	Address VARCHAR2(150),
	Phone CHAR(10),
	Age NUMBER(3),
	CONSTRAINT Customer_PK Primary Key (CustomerID)
);

CREATE TABLE TravelingWith (
	TravelingWithID VARCHAR2(20),
	CustomerID VARCHAR2(20),
	FirstName VARCHAR2(20),
	LastName VARCHAR2(20),
	Age NUMBER(3),
	CONSTRAINT TravelingWith_PK Primary Key (TravelingWithID),
	CONSTRAINT TravelingWith_CustomerID
		FOREIGN KEY (CustomerID)
		REFERENCES Customer(CustomerID)
);

CREATE TABLE BookedTour (
	BookedTourID VARCHAR2(20),
	PurchaseDate DATE NOT NULL,
	TravelDate DATE,
	TotalPrice NUMBER,
	TourID VARCHAR2(20) NOT NULL,
	DriverLicense VARCHAR2(20),
	LicensePlate CHAR(8),
	CustomerID VARCHAR2(20),
	CONSTRAINT BookedTour_PK Primary Key (BookedTourID),
	CONSTRAINT BookedTour_TourID
		FOREIGN KEY (TourID)
		REFERENCES Tour(TourID),
	CONSTRAINT BookedTour_DriverLicense
		FOREIGN KEY (DriverLicense)
		REFERENCES Guide(DriverLicense),
	CONSTRAINT BookedTour_LicensePlate
		FOREIGN KEY (LicensePlate)
		REFERENCES Vehicle(LicensePlate),
	CONSTRAINT BookedTour_CustomerID
		FOREIGN KEY (CustomerID)
		REFERENCES Customer(CustomerID)
);

-- Guides:
INSERT INTO Guide VALUES ('KLKJLKJ90', 'John', 'Smith', '5555555555', 'car', 'junior guide', 9000, TO_DATE('2012/12/12', 'yyyy/mm/dd'));
INSERT INTO Guide VALUES ('AASKJDLFIOE10', 'Jane', 'Doe', '5555550000', 'amphibious', 'senior guide', 30000, TO_DATE('2008-01-25', 'yyyy/mm/dd'));
INSERT INTO Guide VALUES ('D43543KKD', 'Jorge', 'Martinez', '5555551111', 'bus', 'guide', 15000, TO_DATE('2003-08-09', 'yyyy/mm/dd'));
INSERT INTO Guide VALUES ('MKLDSK54WP', 'James', 'Bond', '5555552340', 'amphibious', 'guide', 15000, TO_DATE('2001-07-12', 'yyyy/mm/dd'));
INSERT INTO Guide VALUES ('NCN666MCK', 'Josephine', 'Martinez', '5555559036', 'bus', 'senior guide', 15000, TO_DATE('2010-11-11', 'yyyy/mm/dd'));

-- Vehicles:
INSERT INTO Vehicle VALUES ('ABC123', null, 'Metacar', 'Frogger', 2020, 5);
INSERT INTO Vehicle VALUES ('DUCKY4', 'amphibious', 'Cars inc', 'Frogger', 1983, 7);
INSERT INTO Vehicle VALUES ('BIGBUS1', 'bus', 'Cars inc', 'minibus', 2010, 20);
INSERT INTO Vehicle VALUES ('A-CAR', 'car', 'Toyota', 'Prius', 2005, 4);
INSERT INTO Vehicle VALUES ('RACECAR', 'car', 'Jonson', 'Palindrome', null, 2);

-- Tours:
INSERT INTO Tour VALUES ('1', 'IT', 'Balloon tour with clowns', 'Chicago', 'IL', INTERVAL '2' HOUR, 'car', 500, 10);
INSERT INTO Tour VALUES ('2', 'Pac Man', 'Experience great cuisine on the go! (you may or may not be the cuisine)', 'Honolulu', 'HI', INTERVAL '3' DAY, 'amphibious', 10, 2);
INSERT INTO Tour VALUES ('3', 'DK', 'Observe the jungle with barrels!', 'Chicago', 'IL', INTERVAL '24' HOUR, 'bus', 90, 20);
INSERT INTO Tour VALUES ('4', 'Ralph', 'Learn the sights of the city by wrecking it!', 'New York City', 'NY', INTERVAL '9' HOUR, 'car', 20, 1);

-- Locations:
INSERT INTO Location VALUES ('A', 'the city', 'historic', '9 Main Street', 90, 80);
INSERT INTO Location VALUES ('B', 'ocean', 'restaurant', '1 The Equator Line', 1, 1);
INSERT INTO Location VALUES ('C', 'volcano', 'restaurant', '10 Ring of Fire Boulevard', 2, 2);
INSERT INTO Location VALUES ('D', 'jungle japes', 'museum', '126 Barrelabarrel Street', 30, 5);
INSERT INTO Location VALUES ('E', 'the empire state building', 'historic', '578 Fix-it Avenue', 90, 80);

-- Travels:
INSERT INTO Traveling VALUES ('1', 'A');
INSERT INTO Traveling VALUES ('1', 'B');
INSERT INTO Traveling VALUES ('2', 'A');
INSERT INTO Traveling VALUES ('2', 'B');
INSERT INTO Traveling VALUES ('2', 'C');
INSERT INTO Traveling VALUES ('2', 'D');
INSERT INTO Traveling VALUES ('2', 'E');
INSERT INTO Traveling VALUES ('3', 'A');
INSERT INTO Traveling VALUES ('3', 'C');
INSERT INTO Traveling VALUES ('3', 'E');
INSERT INTO Traveling VALUES ('4', 'B');
INSERT INTO Traveling VALUES ('4', 'C');

-- Customers:
INSERT INTO Customer VALUES ('11', 'Mary', 'Sue', '25 Perfect Place', null, 35);
INSERT INTO Customer VALUES ('12', 'Gary', 'Stu', '25 Perfect Place', null, 40);
INSERT INTO Customer VALUES ('13', 'Harry', 'Potter', '4 Privet Drive', '5551234567', 11);
INSERT INTO Customer VALUES ('14', 'Larry', 'Butz', '987 Roseart Way', '555789098', 26);
INSERT INTO Customer VALUES ('15', 'Barry', 'Grump', '1 Wide Wide Internet', '555789098', 26);

-- TravelingWith:
INSERT INTO TravelingWith VALUES ('111', '11', 'Eragon', 'Blandface', 19);
INSERT INTO TravelingWith VALUES ('112', '13', 'Ron', 'Weasley', 12);
INSERT INTO TravelingWith VALUES ('113', '13', 'Hermione', 'Granger', 12);
INSERT INTO TravelingWith VALUES ('114', '13', 'Hagrid', null, 65);
INSERT INTO TravelingWith VALUES ('115', '13', 'Neville', 'Longbottom', 11);
INSERT INTO TravelingWith VALUES ('116', '13', 'Luna', 'Lovegood', 10);
INSERT INTO TravelingWith VALUES ('117', '11', 'Mew Mew', 'Beri', 16);
INSERT INTO TravelingWith VALUES ('118', '12', 'Ebony', 'RavenWay', 17);
INSERT INTO TravelingWith VALUES ('119', '12', 'Commander', 'Shepard', 27);
INSERT INTO TravelingWith VALUES ('120', '14', 'Phoenix', 'Wright', 26);
INSERT INTO TravelingWith VALUES ('121', '14', 'Miles', 'Edgeworth', 27);
INSERT INTO TravelingWith VALUES ('122', '15', 'Arin', 'Hanson', 29);
INSERT INTO TravelingWith VALUES ('123', '15', 'Dan', 'Avidan', 25);

INSERT INTO TravelingWith VALUES ('123', '11', 'Linda', 'Harris', 20);
INSERT INTO TravelingWith VALUES ('124', '11', 'William', 'Parker', 21);
INSERT INTO TravelingWith VALUES ('125', '11', 'David', 'Murphy', 17);
INSERT INTO TravelingWith VALUES ('126', '11', 'David', 'Morgan', 5);
INSERT INTO TravelingWith VALUES ('127', '11', 'Patricia', 'Miller', 5);
INSERT INTO TravelingWith VALUES ('128', '11', 'Robert', 'Allen', 7);
INSERT INTO TravelingWith VALUES ('129', '11', 'Richard', 'Hernandez', 7);
INSERT INTO TravelingWith VALUES ('130', '11', 'Maria', 'White', 24);
INSERT INTO TravelingWith VALUES ('131', '11', 'Carol', 'Bailey', 15);
INSERT INTO TravelingWith VALUES ('132', '11', 'David', 'Lewis', 17);
INSERT INTO TravelingWith VALUES ('133', '11', 'John', 'Johnson', 19);

-- BookedTours:
INSERT INTO BookedTour VALUES ('ichi', TO_DATE('2012/12/12', 'yyyy/mm/dd'), TO_DATE('2013/01/01', 'yyyy/mm/dd'), 900, '1', 'KLKJLKJ90', 'A-CAR', '11');
INSERT INTO BookedTour VALUES ('ni', TO_DATE('2012/12/12', 'yyyy/mm/dd'), TO_DATE('2011/01/01', 'yyyy/mm/dd'), 1000, '3', 'D43543KKD', 'BIGBUS1', '12');
INSERT INTO BookedTour VALUES ('san', TO_DATE('2011/10/10', 'yyyy/mm/dd'), TO_DATE('2014/01/01', 'yyyy/mm/dd'), 800, '2', 'AASKJDLFIOE10', 'DUCKY4', '13');
INSERT INTO BookedTour VALUES ('shi', TO_DATE('2015/10/12', 'yyyy/mm/dd'), TO_DATE('2015/01/01', 'yyyy/mm/dd'), 10000, '1', 'KLKJLKJ90', 'A-CAR', '14');
INSERT INTO BookedTour VALUES ('go', TO_DATE('2016/04/09', 'yyyy/mm/dd'), TO_DATE('2016/01/01', 'yyyy/mm/dd'), 7000, '1', 'KLKJLKJ90', 'A-CAR', '15');
INSERT INTO BookedTour VALUES ('roku', TO_DATE('2015/08/03', 'yyyy/mm/dd'), TO_DATE('2015/01/01', 'yyyy/mm/dd'), 100, '1', 'D43543KKD', 'A-CAR', '15');

-- Queries:

-- Q1: Report vehicles that can carry more than 10 passengers.
SELECT * 
FROM Vehicle 
WHERE MaxPassenger > 10;

-- Q2: For a given tour guide (say TourID = 10), report which of the 5 company vehicles that the tour guide can drive. Assume that the categories are distinct: car license permits driving only of cars, bus license only permits driving of buses, etc.
SELECT * 
FROM Vehicle 
WHERE VehicleType = (SELECT VehicleType 
					 FROM Guide 
					 WHERE FirstName = 'Jane');

-- Q3: Report the number of booked tours each customer has. Also include the FirstName, LastName, and Age
SELECT Customer.CustomerID, FirstName, LastName, Age, COUNT(BookedTourID) As ToursBooked 
FROM Customer, BookedTour 
WHERE Customer.CustomerID = BookedTour.CustomerID 
GROUP BY Customer.CustomerID, FirstName, LastName, Age;

-- Q4: Report the number of booked tours to which each tour guide has been assigned. Also include the FirstName, LastName, and Title of the tour guide.
SELECT FirstName,LastName, Title, COUNT (BookedTourID) As ToursBooked
FROM BookedTour, Guide
WHERE BookedTour.DriverLicense = Guide.DriverLicense
GROUP BY Guide.DriverLicense, FirstName, Lastname, Title;

-- Q5: Report the number of each location type (historic, museum, restaurant) along with the tour name that each tour contains.
SELECT TourName, Type, COUNT (Location.LocationID) As Locations
FROM Tour, Location, Traveling
WHERE Tour.TourID = Traveling.TourID AND Location.LocationID = Traveling.LocationID
GROUP BY TourName, Type, Tour.TourID
ORDER BY TourName, Type;

-- Q6: Report the number of people taking each of the different tours. Include the TourName, City, State and VehicleType. Make sure to count the primary customer as well as those who are traveling along with him or her.
-- Assumption: Do not double count individuals who go on a tour twice
SELECT TourName, City, State, VehicleType, COUNT (DISTINCT TravelingWith.CustomerID) + COUNT (DISTINCT TravelingWithID) As PeoplePerTour
FROM Tour, BookedTour, TravelingWith
WHERE Tour.TourID = BookedTour.TourID AND TravelingWith.CustomerID = BookedTour.CustomerID
GROUP BY Tour.TourID, TourName, City, State, VehicleType;

-- Q7: Report primary customers who have booked tours in 2015 and 2016.
-- Assumption: Only report customers who have booked tours in BOTH 2015 and 2016
SELECT CustomerID
FROM BookedTour
WHERE PurchaseDate >= TO_DATE('2015/01/01', 'yyyy/mm/dd') AND PurchaseDate <= TO_DATE('2015/12/31', 'yyyy/mm/dd')
INTERSECT (
SELECT CustomerID
FROM BookedTour
WHERE PurchaseDate >= TO_DATE('2016/01/01', 'yyyy/mm/dd') AND PurchaseDate <= TO_DATE('2016/12/31', 'yyyy/mm/dd')
);

-- Q8: Report which is the most popular tour, i.e. the tour that has been booked the most often (appears most often in BookedTours). Do not factor in traveling companions of the primary customer. 
SELECT TourID
FROM BookedTour
GROUP BY TourID
HAVING COUNT (*) =  (SELECT MAX (Bookings) FROM
											(SELECT COUNT (*) As Bookings
					 						 FROM BookedTour
					 						 GROUP BY TourID));

-- Q9: Report the total number of tours taken by people (primary customers and traveling companions). 
SELECT SUM (PeoplePerTour)
FROM (SELECT COUNT (TravelingWithID) + 1 As PeoplePerTour
	  FROM BookedTour, TravelingWith
	  WHERE BookedTour.CustomerID = TravelingWith.CustomerID
	  GROUP BY BookedTourID);

-- Q10: For a specific booked tour, insert the TotalPrice by calculating $100 per adult ( >= 18 yrs) and $50 per child (<18 yrs).
-- Assumption: All primary customers are over 18
CREATE OR REPLACE PROCEDURE CalculatePrice (BookedTourID IN VARCHAR2) IS
	AdultCount NUMBER;
	ChildCount NUMBER;
BEGIN
SELECT 1 + COUNT(DISTINCT TravelingWithID) As NumAdults
INTO AdultCount
FROM BookedTour, TravelingWith
WHERE TravelingWith.CustomerID = BookedTour.CustomerID AND BookedTour.BookedTourID = CalculatePrice.BookedTourID AND TravelingWith.Age >= 18;
dbms_output.put_line('adult count completed');

SELECT COUNT (DISTINCT TravelingWithID) As NumChildren
INTO ChildCount
FROM BookedTour, TravelingWith
WHERE TravelingWith.CustomerID = BookedTour.CustomerID AND BookedTour.BookedTourID = CalculatePrice.BookedTourID AND TravelingWith.Age < 18;
dbms_output.put_line('child count completed');
UPDATE BookedTour
SET TotalPrice = 100 * AdultCount + 50* ChildCount
WHERE BookedTour.BookedTourID = CalculatePrice.BookedTourID;
dbms_output.put_line('insertion completed with ' || AdultCount || ' adults and ' || ChildCount || ' children');
END CalculatePrice;
/

set serveroutput on format wrapped;
exec CalculatePrice ('ichi');








--- Begin Phase 3:


-- Part 1
BEGIN
dbms_output.put_line('PHASE 3 Code beginning');
END;
/

CREATE OR REPLACE VIEW LargeGroup AS
SELECT BookedTourID, TravelDate, CASE WHEN TotalPeople < 10 THEN 'Regular' ELSE 'Large' END AS GroupSize
FROM BookedTour, (
	SELECT Customer.CustomerID AS CustomerID, COUNT(TravelingWithID) + 1 AS TotalPeople
	FROM Customer, TravelingWith
	WHERE Customer.CustomerID = TravelingWith.CustomerID
	GROUP BY Customer.CustomerID) C
WHERE C.CustomerID = BookedTour.CustomerID;

BEGIN dbms_output.put_line('Large groups traveling after March 1 2016'); END;
/

SELECT *
FROM LargeGroup
WHERE TravelDate > TO_DATE('2016/03/01', 'yyyy/mm/dd');


-- 2.1 Senior guides must bave a salary of >= $50,000.
CREATE TRIGGER GuideSeniorPay
BEFORE INSERT OR UPDATE ON Guide
FOR EACH ROW
BEGIN
	IF(:new.Salary < 50000 AND :new.Title = 'senior guide') THEN
		RAISE_APPLICATION_ERROR(-20001, 'Senior guides must be paid at least $50,000');
	END IF;
END;
/
-- INSERT INTO Guide VALUES ('QUIDLKSK4', 'John', 'Smith', '5555555555', 'car', 'senior guide', 9000, TO_DATE('2012/12/12', 'yyyy/mm/dd'));

-- 2.2 Buses must be made in 2010 or newer
CREATE TRIGGER VehicleTwentyTenBuses
BEFORE INSERT OR UPDATE ON Vehicle
FOR EACH ROW
BEGIN
	IF(:new.Year < 2010 AND :new.VehicleType = 'bus') THEN
		RAISE_APPLICATION_ERROR(-20002, 'Buses must be made in 2010 or newer');
	END IF;
END;
/
--INSERT INTO Vehicle VALUES ('BIGBUS2', 'bus', 'Cars inc', 'minibus', 2009, 20);

-- 2.3 All tours in boston must be amphibious
CREATE TRIGGER TourAmphibiousBoston
BEFORE INSERT OR UPDATE ON Tour
FOR EACH ROW
WHEN (new.VehicleType <> 'amphibious' AND new.City = 'Boston')
BEGIN
	RAISE_APPLICATION_ERROR(-20003, 'Boston only permits tours using amphibious vehicles.');
END;
/
-- INSERT INTO Tour VALUES ('42', 'IT', 'Balloon tour with clowns', 'Boston', 'MA', INTERVAL '2' HOUR, 'car', 500, 10);

-- 2.4 When created, booked tours have a purchase date three months from now.
CREATE TRIGGER BookedTourDefaultPurchaseDate
BEFORE INSERT ON BookedTour
FOR EACH ROW
BEGIN
	:new.PurchaseDate := ADD_MONTHS(SYSDATE, 3);
END;
/
-- INSERT INTO BookedTour VALUES ('shichi', TO_DATE('2015/08/03', 'yyyy/mm/dd'), TO_DATE('2015/01/01', 'yyyy/mm/dd'), 100, '1', 'KLKJLKJ90', 'A-CAR', '15');
-- SELECT PurchaseDate FROM BookedTour WHERE BookedTourID = 'shichi';

-- 2.5 Guides cannot have multiple tours in one day.
CREATE TRIGGER BookedTourGuideConflicts
AFTER UPDATE OR INSERT ON BookedTour
DECLARE
	conflicts number; -- Technically, this is conflicts * 2
BEGIN
	SELECT count(*) INTO conflicts
	FROM BookedTour One, BookedTour Two
	WHERE One.DriverLicense = Two.DriverLicense
		AND One.TravelDate = Two.TravelDate
		AND One.BookedTourID <> Two.BookedTourID;

	IF( 0 <> conflicts) THEN
		RAISE_APPLICATION_ERROR(-20004, 'Guide has more than one tour that day');
	END IF;
END;
/
-- INSERT INTO BookedTour VALUES ('shichi', TO_DATE('2015/08/03', 'yyyy/mm/dd'), TO_DATE('2015/01/01', 'yyyy/mm/dd'), 100, '1', 'KLKJLKJ90', 'A-CAR', '15');
