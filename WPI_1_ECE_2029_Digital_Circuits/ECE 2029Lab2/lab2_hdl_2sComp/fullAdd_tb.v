`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:45:21 11/14/2013
// Design Name:   fullAdd
// Module Name:   M:/ECE 2029/lab2_hdl/fullAdd_tb.v
// Project Name:  lab2_hdl
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: fullAdd
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module fullAdd_tb;

	// Inputs
	reg A;
	reg B;
	reg Cin;

	// Outputs
	wire Sum;
	wire Cout;

	// Instantiate the Unit Under Test (UUT)
	fullAdd uut (
		.A(A), 
		.B(B), 
		.Cin(Cin), 
		.Sum(Sum), 
		.Cout(Cout)
	);

	initial begin
		// Initialize Inputs
		A = 0;
		B = 0;
		Cin = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		Cin = 1;
		#100;//Wait
		B = 1;
		Cin = 0;
		#100;//Wait
		Cin = 1;
		#100;//Wait
		A = 1;
		B = 0;
		Cin = 0;
		#100//Wait
		Cin = 1;
		#100;//Wait
		B = 1;
		Cin = 0;
		#100;//Wait
		Cin = 1;
		#500;//Wait
	end
      
endmodule

