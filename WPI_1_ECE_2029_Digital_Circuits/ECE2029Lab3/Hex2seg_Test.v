`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   08:19:34 11/21/2013
// Design Name:   Hex2Seg
// Module Name:   M:/ECE 2029/Lab3/Hex2seg_Test.v
// Project Name:  Lab3
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Hex2Seg
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module Hex2seg_Test;

	// Inputs
	reg [3:0] D;

	// Outputs
	wire [6:0] segs;
	wire [3:0] anodes;

	// Instantiate the Unit Under Test (UUT)
	Hex2Seg uut (
		.D(D), 
		.segs(segs), 
		.anodes(anodes)
	);

	initial begin
		// Initialize Inputs
		D = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		D = 4'b0000;
		#50;
		D = 4'b0001;
		#50;
		D = 4'b0010;
		#50;
		D = 4'b0011;
		#50;
		D = 4'b0100;
		#50;
		D = 4'b0101;
		#50;
		D = 4'b0110;
		#50;
		D = 4'b0111;
		#50;
		D = 4'b1000;
		#50;
		D = 4'b1001;
		#50;
		D = 4'b1010;
		#50;
		D = 4'b1011;
		#50;
		D = 4'b1100;
		#50;
		D = 4'b1101;
		#50;
		D = 4'b1110;
		#50;
		D = 4'b1111;
		#50;
	end
      
endmodule

