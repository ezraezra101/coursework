`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:58:58 12/05/2013 
// Design Name: 
// Module Name:    mux1bit4to1 
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
module mux1bit4to1(
    input [3:0] D,
    input [1:0] S,
    output out
    );

	assign Out = (S==2'b00) ? D[3] : (
					 (S==2'b01) ? D[2] : (
					 (S==2'b10) ? D[1] : D[0]
					 ));

endmodule
