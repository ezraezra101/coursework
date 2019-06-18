`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:07:12 12/12/2013 
// Design Name: 
// Module Name:    mod10cnt 
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
module mod10cnt(
    input CLK,
    input R,
    input CE,
    output [3:0] Q,
    output R_out
    );

	assign R_out = (R | ((Q=='b1001) & CE)) ? 1'b1:1'b0;
	
	bin_cnt4 U1(CLK,R_out,CE,Q);

endmodule