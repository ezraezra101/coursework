`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:58:56 11/21/2013 
// Design Name: 
// Module Name:    Add4hex 
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
module Add4hex(
    input [3:0] A,
    input [3:0] B,
    output [6:0] segs7,
    output [3:0] anodes
    );

	wire Overflow;
	wire [3:0] Sum;
	
	fourAdd U1(A,B,Sum,Overflow);
	Hex2Seg U2(Sum,segs7,anodes);

endmodule
