`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:04:42 12/06/2013 
// Design Name: 
// Module Name:    digitChooser 
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
module digitChooser(
    input [3:0] W,
    input [3:0] X,
    input [3:0] Y,
    input [3:0] Z,
    input [3:0] decPts,
    input [3:0] signs,
	 input [1:0] Q,
    output [3:0] D, //D = data
    output DP,
    output sign,
	 output [3:0]anodes
    );
	 //W is most significant digit
	//Q 00 corresponds to W also decPts[3] & signs[3] & anode == 4'b0111
	//  01 -> X
	//				anodes == 4'b1011
	//  10 -> Y
	//				anodes == 4'b1101
	//  11 -> Z
	//				anodes == 4'b1110

	mux4to1 U1(W, X, Y, Z, Q, D);

	mux1bit4to1 U2(decPts,Q,DP);
	
	mux1bit4to1 U3(signs, Q, sign);
	
	//decodeAnode U4(1'b1, ~Q[1], ~Q[0], anodes);

	assign anodes = (Q==2'b00) ? 4'b0111 : (
				  (Q==2'b01) ? 4'b1011 : (
				  (Q==2'b10) ? 4'b1101 : 4'b1110
				  ));



endmodule
