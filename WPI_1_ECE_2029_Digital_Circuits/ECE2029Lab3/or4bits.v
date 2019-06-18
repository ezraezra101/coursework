`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:23:59 11/21/2013 
// Design Name: 
// Module Name:    or4bits 
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
module or4bits(
    input [3:0] A,
    input [3:0] B,
    output [3:0] orAB
    );

	assign orAB = A | B;

endmodule
