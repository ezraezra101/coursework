`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:01:14 12/05/2013
// Design Name:   decodeAnode
// Module Name:   M:/ECE 2029/ECE2029Lab4/decodeAnodetest.v
// Project Name:  ECE2029Lab4
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: decodeAnode
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module decodeAnodetest;

	// Inputs
	reg En;
	reg A0;
	reg A1;

	// Outputs
	wire [3:0] Y;

	// Instantiate the Unit Under Test (UUT)
	decodeAnode uut (
		.En(En), 
		.A0(A0), 
		.A1(A1), 
		.Y(Y)
	);

	initial begin
		// Initialize Inputs
		En = 0;
		A0 = 0;
		A1 = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

		A0 = 1;
		#100;
		A0 = 0;
		A1 = 1;
		#100;
		A0 = 1;
		#100;
		En = 1;
		A0 = 0;
		A1 = 0;
		#100;
		A0 = 1;
		#100;
		A0 = 0;
		A1 = 1;
		#100;
		A0 = 1;
		#100;

	end
      
endmodule

