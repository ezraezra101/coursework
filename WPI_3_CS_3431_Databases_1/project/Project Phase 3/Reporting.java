// Written by Ezra Davis (edavis) and Miya Gaskell (mgaskell)

import java.sql.*;
import java.util.Scanner;
public class Reporting {

	// Prints information about a customer given by the user
	static void customerInfo(Connection connection) throws SQLException {
		Scanner in = new Scanner(System.in);
		PreparedStatement stmt = connection.prepareStatement("SELECT * FROM CUSTOMER WHERE CustomerID = ?");
		ResultSet rset = null;

		try {
			System.out.print("Enter Customer ID: ");
			String input = in.next();
			stmt.setString(1, input);

			rset = stmt.executeQuery();

			if(!rset.next()) {
				System.out.println("Customer not found :-(");
			} else {
				System.out.println("Customer Information");
				System.out.println("ID: " + rset.getString("CustomerID"));
				System.out.println("First Name: " +rset.getString("FirstName"));
				System.out.println("Last Name: " + rset.getString("LastName"));
				System.out.println("Address: " + rset.getString("Address"));
				System.out.println("Phone: " + rset.getString("Phone"));
				System.out.println("Age: " + rset.getString("Age"));
			}

		} finally {
			if(rset != null)
				rset.close();
			stmt.close();
		}
	}

	// Prints information about a guide given by the user
	static void guideInfo(Connection connection) throws SQLException {
		Scanner in = new Scanner(System.in);
		PreparedStatement stmt = connection.prepareStatement("SELECT * FROM GUIDE WHERE DriverLicense = ?");
		ResultSet rset = null;

		try {

			System.out.print("Enter Tour Guide Driver License: ");
			String input = in.next();
			stmt.setString(1, input);

			rset = stmt.executeQuery();

			if(!rset.next()) {
				System.out.println("Tour Guide not found :-(");
			} else {
				System.out.println("Tour Guide Information");
				System.out.println("Driver License: " + rset.getString("DriverLicense"));
				System.out.println("First Name: " +rset.getString("FirstName"));
				System.out.println("Last Name: " + rset.getString("LastName"));
				System.out.println("Title: " + rset.getString("Title"));
				System.out.println("Vehicle Type: " + rset.getString("VehicleType"));
			}

		} finally {
			if(rset != null)
				rset.close();
			stmt.close();
		}
	}

	// Prints information about a booked tour given by the user
	static void bookedTourInfo(Connection connection) throws SQLException {
		Scanner in = new Scanner(System.in);

		String bookedTourQuery = // Horribly inefficient query
		"SELECT BookedTourID," + 
		"		Customer.CustomerID AS CustomerID, " +
		"		Customer.FirstName || ' '|| Customer.LastName AS CustomerName, " + 
		"		Customer.Age AS Age, " +
		"		TourName, PurchaseDate, TravelDate, " +
		"		Guide.FirstName || ' ' || Guide.LastName AS GuideName, " +
		"		LicensePlate, " +
		"		TotalPrice " +
		"FROM BookedTour, Customer, Tour, Guide " +
		"WHERE BookedTourID = ? " +
		"	AND Customer.CustomerID = BookedTour.CustomerID " +
		"	AND Tour.TourID = BookedTour.TourID " +
		"	AND Guide.DriverLicense = BookedTour.DriverLicense";

		String travelingWithQuery =
		"SELECT TravelingWithID, " +
		"		FirstName || ' ' || LastName AS Name, " +
		"		Age " + 
		"FROM TravelingWith " +
		"WHERE CustomerID = ?";

		PreparedStatement stmt = connection.prepareStatement(bookedTourQuery);
		ResultSet rset = null;

		PreparedStatement travelingWithStmt = connection.prepareStatement(travelingWithQuery);
		ResultSet travelingWithRset = null;
		try {
			System.out.print("Enter Booked Tour ID: ");
			String input = in.next();
			stmt.setString(1, input);

			rset = stmt.executeQuery();

			if(!rset.next()) {
				System.out.println("Booked Tour not found :-(");
			} else {
				System.out.println("Booked Tour Information");
				System.out.println("Booked Tour ID: " + rset.getString("BookedTourID"));
				System.out.println("Customer Name: " + rset.getString("CustomerName"));
				System.out.println("Customer Age: " + rset.getString("Age"));
				System.out.println("People Traveling With");

				travelingWithStmt.setString(1, rset.getString("CustomerID"));
				travelingWithRset = travelingWithStmt.executeQuery();
				if(!travelingWithRset.next()) {
					System.out.println("\tNo people traveling with the customer");
				} else {
					do {
						System.out.println("\t" +
							travelingWithRset.getString("TravelingWithID") + " " +
							travelingWithRset.getString("Name") + " " +
							travelingWithRset.getString("Age"));
					} while(travelingWithRset.next());
				}
				System.out.println("Tour Name: " + rset.getString("TourName"));
				System.out.println("Purchase Date: " + rset.getString("PurchaseDate"));
				System.out.println("Travel Date: " + rset.getString("TravelDate"));
				System.out.println("Tour Guide Name: " + rset.getString("GuideName"));
				System.out.println("License Plate: " + rset.getString("LicensePlate"));
				System.out.println("Total Price: " + rset.getString("TotalPrice"));
			}

		} finally {
			if(rset != null)
				rset.close();
			if(travelingWithRset != null)
				travelingWithRset.close();
			stmt.close();
			travelingWithStmt.close();
		}
	}

	// Updates booked tour's license plate (and thus its associated vehicle)
	// Based on user input.
	static void updateBookedVehicle(Connection connection) throws SQLException {
		Scanner in = new Scanner(System.in);
		String sql = 
		"UPDATE BookedTour " +
		"SET LicensePlate = ? " +
		"WHERE BookedTour.BookedTourID = ?";
		PreparedStatement stmt = connection.prepareStatement(sql);

		try {
			System.out.print("Enter Booked Tour ID: ");
			String input = in.next();
			stmt.setString(2, input);

			System.out.print("Enter the new vehicle License Plate: ");
			input = in.next();
			stmt.setString(1, input);
			stmt.executeUpdate();

		} catch (SQLIntegrityConstraintViolationException e) {
			System.out.println("Vehicle with that license plate does not exist :-(");
		} finally {
			stmt.close();
		}
	}

	// Simple command line interface for a few simple database queries
	public static void main(String[] args) {
		if(args.length == 2) {
			System.out.println("1 - Report Customer Information");
			System.out.println("2 - Report Tour Guide Information");
			System.out.println("3 - Report Booked Tour Information");
			System.out.println("4 - Update Booked Tour Vehicle");
			return;
		} else if(args.length != 3) {
			System.out.println("Expected: java Reporting <username> <password> [1, 2, 3, 4, or blank]");
			return;
		}

		String userID = args[0];
		String password = args[1];
		int task = -1;
		try {
			task = Integer.parseInt(args[2]);
		} catch (java.lang.NumberFormatException e) {
			// Don't do anything
		} finally {
			if(task > 4  || task < 1) {
				System.out.println("Illegal task! Expected: java Reporting <username> <password> [1, 2, 3, 4, or blank]");
				return;
			}
		}

		try {
			Class.forName("oracle.jdbc.driver.OracleDriver");
			
		} catch (ClassNotFoundException e){
			System.out.println("Where is your Oracle JDBC Driver?");
			e.printStackTrace();
			return;
		}

		Connection connection = null;
		try {
			 connection = DriverManager.getConnection(
			 		"jdbc:oracle:thin:@oracle.wpi.edu:1521:WPI11grxx", userID, password);
		} catch (SQLException e) {
			System.out.println("Connection Failed! Check output console");
			e.printStackTrace();
			return;
		}
		
		// Performing the query
		try {

			switch(task) {
				case 1: customerInfo(connection);
						break;
				case 2: guideInfo(connection);
						break;
				case 3: bookedTourInfo(connection);
						break;
				case 4: updateBookedVehicle(connection);
						break;
				default: System.out.println("Error! Illegal task value");
						return;
			}

		} catch (SQLException e) {
			System.out.println("Get Data Failed! Check output console");
			e.printStackTrace();	
		} finally {
			try {
				connection.close();
			} catch (SQLException e) {
				System.out.println("Unable to close database connection");
			}
		}

	}
}