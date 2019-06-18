`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:16:33 12/05/2013 
// Design Name: 
// Module Name:    anodeDriver 
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
module anodeDriver(
    input CLK,
    input Reset,
    output [6:0] segs,
    output DP,
    output [3:0] anodes
    );

	wire clk_10K, clk_1Hz;
	wire [1:0] Q;
	
	// Divide the system clock to 10KHz
	clkDiv10K U1(CLK, clk_10K);
	
	// Divide the 10KHz clock to 1Hz
	clkDiv10K U2(clk_10K, clk_1Hz);
	
	// Generate the 7-seg for hex A (i.e. 4'b1010)
	// without sign or decimal
	Hex2Seg U3(4'b1010, 0, 0, segs, DP);
	
	// --- Actual anode driving part ---
	// 2-bit binary counter driving the 2-to-4 anode decoder
	bin_cnt2 U4(clk_1Hz, Reset, Q);
	
	// For now set En = 1
	decodeAnode U5(1,Q[0],Q[1],anodes);

endmodule
