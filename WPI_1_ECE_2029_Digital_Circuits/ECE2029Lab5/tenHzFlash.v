`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:05:29 12/12/2013 
// Design Name: 
// Module Name:    tenHzFlash 
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
module tenHzFlash(
    input CLK,
    output clk10Hz
    );
	wire tenKHz;
	
	clkDiv10K U1(CLK, tenKHz);
	
	clkDiv1K U2(tenKHz, clk10Hz);

endmodule
