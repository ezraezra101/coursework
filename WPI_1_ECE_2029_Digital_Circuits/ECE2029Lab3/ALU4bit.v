`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:30:04 11/21/2013 
// Design Name: 
// Module Name:    ALU4bit 
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
module ALU4bit(
    input [3:0] A,
    input [3:0] B,
    input [1:0] op,
    output [3:0] Result,
    output negSign,
    output [6:0] segs7,
    output [3:0] anodes,
	 output OverFlow
    );
	 
	 wire [3:0] summ, diff, my4and, my4or, Bneg, Barg, magg;
	 wire ovf;
	 
	 comp2 U1(B,Bneg);
	 
	 assign Barg = (op=='b01) ? Bneg : B;
	 
	 fourAdd U2(A,Barg,summ,ovf);
	 
	 and4bits U3(A,B,my4and);
	 
	 or4bits U4(A,B,my4or);
	 
	 comp2 U5(summ, magg);
	 
	 assign diff = (negSign == 1'b1) ? magg : summ;

	assign Result = (op=='b00) ? summ : (
							(op=='b01) ? diff : (
							(op=='b10) ? my4and : my4or //if op=='b11
							));
	
	Hex2Seg U6(Result, segs7, anodes);

	assign negSign = (op==2'b01 && summ[3]==1'b1) ? 1'b1 : 1'b0;
	
	assign OverFlow = //Addition: (unsigned)
							(op==2'b00) ? (  ( ((~summ[3])& B[3]) |
													 ((~summ[3])& A[3]) )  |
													 (A[3] & B[3])  ) : (
							//Subtraction: (signed)
							(op==2'b01) ? ovf :
							1'b0); //Logical operations don't have Overflow
endmodule
