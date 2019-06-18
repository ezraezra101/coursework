`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:13:03 11/14/2013 
// Design Name: 
// Module Name:    from2scomplement 
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
module from2scomplement(
    input [3:0] In,
    output [3:0] Out,
    output Sign
    );
	wire [3:0] complementPlus;

	fourAdd U1 ((~In),4'b0001,complementPlus,Overflow);//Overflow unneeded

	assign Sign = In[3];
	
	assign Out = (Sign==1)? complementPlus : In;

endmodule
