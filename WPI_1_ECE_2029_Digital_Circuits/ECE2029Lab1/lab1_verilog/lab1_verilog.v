`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: WPI
// Engineer: Scott Sargent & Ezra Davis
// 
// Create Date:    09:04:00 11/07/2013 
// Design Name: Example gates
// Module Name:    lab1_verilog 
// Project Name: Lab1
// Target Devices: Nexys 3
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
module lab1_verilog(
    input a,
    input b,
    output c,
    output d,
    output e,
    output f
    );

assign c = a & b; //and
assign d = a | b; //or
assign e = a ^ b; //xor
assign f = ~a;

endmodule
