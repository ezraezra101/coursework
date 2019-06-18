`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   08:35:19 12/12/2013
// Design Name:   bin_cnt4
// Module Name:   M:/ECE2029Lab5/bin_cnt4Test.v
// Project Name:  ECE2029Lab5
// Target Device:  
// Tool versio:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: bin_cnt4
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module bin_cnt4Test;

	// Inputs
	reg clk;
	reg Reset;
	reg CE;

	// Outputs
	wire [3:0] Q;

	// Itantiate the Unit Under Test (UUT)
	bin_cnt4 uut (
		.clk(clk), 
		.Reset(Reset), 
		.CE(CE), 
		.Q(Q)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		Reset = 1;
		CE = 1;

		// Wait 10  for global reset to finish
		#10;
		clk = 1;
		#10;
		clk = 0;
		Reset = 0; //Reset = 0
		
		#10;
		clk = 1;
		#10;
		clk = 0;        
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		Reset = 1; //Reset = 1
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		Reset = 0; //Reset = 0
//Clock cycle 4
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		CE = 0; //CE = 0
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		CE = 1; //CE = 1
//Clock cycle 8
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;        
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
//Clock cycle 12
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		CE = 0; //CE = 0
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		CE = 1;//CE = 1
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		CE = 0; //CE = 0
//Clock cycle 16	
		#10;
		clk = 1;
		#10;
		clk = 0;
		CE = 1;//CE = 1
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		Reset = 1;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		Reset = 0;
//Clock cycle 20
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;        
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
//Clock cycle 24
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;        
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
//Clock cycle 28
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;        
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
//Clock cycle 32
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;        
		
		#10;
		clk = 1;
		#10;
		clk = 0;
		
		#10;
		clk = 1;
		#10;
		clk = 0;
	end
      
endmodule

