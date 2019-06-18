`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:19:30 11/21/2013 
// Design Name: 
// Module Name:    comp2 
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
module comp2(
    input [3:0] B,
    output [3:0] negB
    );

	fourAdd U1 ((~B),4'b0001,negB ,Overflow);//Overflow unneeded
	
endmodule
