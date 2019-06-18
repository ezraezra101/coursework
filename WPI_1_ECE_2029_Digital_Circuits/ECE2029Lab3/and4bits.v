`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:23:09 11/21/2013 
// Design Name: 
// Module Name:    and4bits 
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
module and4bits(
    input [3:0] A,
    input [3:0] B,
    output [3:0] andAB
    );

	assign andAB = A & B ;

endmodule
