`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:49:47 12/05/2013 
// Design Name: 
// Module Name:    mux4to1 
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
module mux4to1(
    input [3:0] D0,
    input [3:0] D1,
    input [3:0] D2,
    input [3:0] D3,
    input [1:0] S,
    output [3:0] Out
    );

	assign Out = (S==2'b00) ? D0 : (
					 (S==2'b01) ? D1 : (
					 (S==2'b10) ? D2 : D3
					 ));

endmodule
