`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:00:18 11/14/2013 
// Design Name: 
// Module Name:    fourAdd 
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
module fourAdd(
    input [3:0] A,
    input [3:0] B,
    output [3:0] Sum,
    output OverFlow
    );
	wire co[3:0];
	
	fullAdd U1 (A[0],B[0],1'b0,Sum[0],co[0]);
	fullAdd U2 (A[1],B[1],co[0],Sum[1],co[1]);
	fullAdd U3 (A[2],B[2],co[1],Sum[2],co[2]);
	fullAdd U4 (A[3],B[3],co[2],Sum[3],co[3]);
	
	assign OverFlow = (((~A[3]) & (~B[3])) & Sum[3]) | ((A[3] & B[3]) & (~Sum[3]));

endmodule
