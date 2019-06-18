`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:07:01 11/07/2013
// Design Name:   lab1_verilog
// Module Name:   C:/Users/edavis/Desktop/ECE2029/lab1_verilog/lab1_verilog_test.v
// Project Name:  lab1_verilog
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: lab1_verilog
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module lab1_verilog_test;

	// Inputs
	reg a;
	reg b;

	// Outputs
	wire c;
	wire d;
	wire e;
	wire f;

	// Instantiate the Unit Under Test (UUT)
	lab1_verilog uut (
		.a(a), 
		.b(b), 
		.c(c), 
		.d(d), 
		.e(e), 
		.f(f)
	);

	initial begin
		#100; //Wait added to match the schematic implementation's test
		// Initialize Inputs
		a = 0;
		b = 0;

		// Wait 100 ns for global reset to finish
		#100;
      a = 1;
		#100;
		a = 0;
		b = 1;
		#100;
		a = 1;
		#1000;

	end
      
endmodule

