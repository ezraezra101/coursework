`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:51:13 11/14/2013 
// Design Name: 
// Module Name:    adder4hex 
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
module adder4hex(
    input [3:0] A,
    input [3:0] B,
    output [3:0] Sum,
    output OverFlow,
    output [6:0] segs7,
    output [3:0] Anodes,
	 output period
    );
wire [3:0] afterConvert;
wire useless;

fourAdd U1(A,B,Sum,OverFlow);

from2scomplement U0 (Sum,afterConvert,period);

hex2seg U2(afterConvert,segs7,Anodes,useless);

endmodule
