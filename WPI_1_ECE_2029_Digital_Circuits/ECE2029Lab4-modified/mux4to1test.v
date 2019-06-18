`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   08:52:24 12/05/2013
// Design Name:   mux4to1
// Module Name:   M:/ECE 2029/ECE2029Lab4/mux4to1test.v
// Project Name:  ECE2029Lab4
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: mux4to1
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module mux4to1test;

	// Inputs
	reg [3:0] D0;
	reg [3:0] D1;
	reg [3:0] D2;
	reg [3:0] D3;
	reg [1:0] S;

	// Outputs
	wire [3:0] Out;

	// Instantiate the Unit Under Test (UUT)
	mux4to1 uut (
		.D0(D0), 
		.D1(D1), 
		.D2(D2), 
		.D3(D3), 
		.S(S), 
		.Out(Out)
	);

	initial begin
		// Initialize Inputs
		D0 = 4'b0001;
		D1 = 4'b0010;
		D2 = 4'b0100;
		D3 = 4'b1000;
		S = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		S = 2'b01;
		#100;
		S = 2'b10;
		#100;
		S = 2'b11;
		#100;

	end
      
endmodule

