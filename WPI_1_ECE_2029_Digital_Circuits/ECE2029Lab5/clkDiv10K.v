`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:29:26 11/27/2011 
// Design Name: 
// Module Name:    clkDiv10K 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module clkDiv10K(
    input clk_in,
    output div_clk
    );

// This module divides down the input clock by 10000. That means 
// if the input clock is the 100MHz Nexys 3 system clock then the 
// output clock is a 10KHz clock. 

// If the input clock to this module is 10KHz then the output 
// clock is a 1Hz clock

	reg clk_out;
	reg [13:0] div_count;

	always @ (posedge clk_in)
		if (div_count < 5000) 
			begin
				div_count <= div_count + 14'b1;
				clk_out <= 1'b0;
			end
		else if (div_count < 10000) 
			begin
				div_count <= div_count + 14'b1;
				clk_out <= 1'b1;
			end
		else begin
			div_count <= 1'b0;
		end
		
	assign div_clk = clk_out;	

endmodule
