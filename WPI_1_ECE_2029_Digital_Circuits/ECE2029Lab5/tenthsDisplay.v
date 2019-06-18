`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:11:50 12/12/2013 
// Design Name: 
// Module Name:    tenthsDisplay 
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
module tenthsDisplay(
    input CLK,
    input R,
    input CE,
    output [6:0] segs,
    output DP,
    output [3:0] anodes
    );

	wire clk_10K, clk_10Hz, R_out;
	wire [3:0] Q;
	
	//Divide the system clock to 10KHz
	clkDiv10K U1(CLK, clk_10K);
	
	// Dividwe the 10KHz clkock to 10Hz
	clkDiv1K U2(clk_10K,clk_10Hz);
	
	// Modulo 10 counter
	mod10cnt U3(clk_10Hz, R, CE, Q, R_out);
	
	//Display output of counter without decimal point
	Hex2Seg U4(Q,0,0,segs,DP);
	
	assign anodes = 4'b1110;

endmodule
