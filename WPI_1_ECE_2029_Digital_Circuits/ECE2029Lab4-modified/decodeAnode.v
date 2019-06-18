`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:56:56 12/05/2013 
// Design Name: 
// Module Name:    decodeAnode 
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
module decodeAnode(
    input En,
    input A0,
    input A1,
    output [3:0] Y
    );

	assign Y = (~En) ? 4'b1111 : (
				  ((~A1) & (~A0)) ? 4'b1110 : (
				  ((~A1) & A0   ) ? 4'b1101 : (
				  (A1    & (~A0)) ? 4'b1011 : 4'b0111
				  )));

endmodule
