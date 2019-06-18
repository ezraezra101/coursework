`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:29:58 11/14/2013 
// Design Name: 
// Module Name:    hex2seg 
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
module hex2seg(
    input [3:0] D,
    output [6:0] segs,
    output [3:0] AN,
	 output period
    );
	
	assign segs =  (D==4'b0000)?7'b0000001 : ( //0
						(D==4'b0001)?7'b1001111 : (//1
						(D==4'b0010)?7'b0010010 : (//2
						(D==4'b0011)?7'b0000110 : ( //3
						(D==4'b0100)?7'b1001100 : ( //4
						(D==4'b0101)?7'b0100100 : ( //5
						(D==4'b0110)?7'b0100000 : ( //6
						(D==4'b0111)?7'b0001111 : ( //7
						(D==4'b1000)?7'b0000000 : ( //8
						(D==4'b1001)?7'b0001100 : ( //9
						(D==4'b1010)?7'b0001000 : ( //A
						(D==4'b1011)?7'b1100000 : ( //B
						(D==4'b1100)?7'b0110001 : ( //C
						(D==4'b1101)?7'b1000010 : ( //D
						(D==4'b1110)?7'b0110000 : 7'b0111000 //E F
						))))))))))))));
	assign AN = 4'b1110;
	
	assign period = 1'b1;

endmodule
