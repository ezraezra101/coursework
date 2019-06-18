`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:27:54 12/12/2013 
// Design Name: 
// Module Name:    bin_cnt4 
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
module bin_cnt4(
	input clk,
	input Reset,
	input CE,
	output [3:0] Q
	);
	reg [3:0] cnt;
	// This module implements a 4-bit binary counter with a
	// count enable (CE) and synchronous reset
	always @ (posedge clk)
	if (Reset == 1)
	begin
	cnt = 4'b0000;
	end
	else begin
	if (CE) begin
		if (cnt == 4'b0000)
			begin
				cnt = 4'b0001;
			end
		else if (cnt == 4'b0001)
			begin
				cnt = 4'b0010;
			end
		else if (cnt == 4'b0010)
			begin
				cnt = 4'b0011;
			end
		else if (cnt == 4'b0011)
			begin
				cnt = 4'b0100;
			end
		else if (cnt == 4'b0100)
			begin
				cnt = 4'b0101;
			end
		else if (cnt == 4'b0101)
			begin
				cnt = 4'b0110;
			end
		else if (cnt == 4'b0110)
			begin
				cnt = 4'b0111;
			end
		else if (cnt == 4'b0111)
			begin
				cnt = 4'b1000;
			end
		else if (cnt == 4'b1000)
			begin
				cnt = 4'b1001;
			end
		else if (cnt == 4'b1001)
			begin
				cnt = 4'b1010;
			end
		else if (cnt == 4'b1010)
			begin
				cnt = 4'hB;
			end
		else if (cnt == 4'b1011)
			begin
				cnt = 4'b1100;
			end
		else if (cnt == 4'b1100)
			begin
				cnt = 4'b1101;
			end
		else if (cnt == 4'b1101)
			begin
				cnt = 4'b1110;
			end
		else if (cnt == 4'b1110)
			begin
				cnt = 4'b1111;
			end
		else if (cnt == 4'b1111)
			begin
				cnt = 4'b0000;
			end
		end // if (CE)
	end
	assign Q = cnt;
endmodule
