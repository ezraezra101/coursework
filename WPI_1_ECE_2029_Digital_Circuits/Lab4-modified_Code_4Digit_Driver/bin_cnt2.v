`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:09:38 12/05/2013 
// Design Name: 
// Module Name:    bin_cnt2 
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
module bin_cnt2(
    input clk,
    input Reset,
    output [1:0] Q
    );
	 
	 reg [1:0] cnt;
	 
	 // This module implements a 2-bit binary counter with a
	 // synchronous reset
	 
		always @ (posedge clk)
			if (Reset ==1)
				begin
					cnt = 2'b00;
				end
			else begin
				if (cnt == 2'b00)
					begin
						cnt = 2'b01;
					end
				else if (cnt == 2'b01)
					begin
						cnt = 2'b10;
					end
				else if (cnt == 2'b10)
					begin
						cnt = 2'b11;
					end
				else if (cnt == 2'b11)
					begin
						cnt = 2'b00;
					end
			end
		
		assign Q = cnt;
		
endmodule