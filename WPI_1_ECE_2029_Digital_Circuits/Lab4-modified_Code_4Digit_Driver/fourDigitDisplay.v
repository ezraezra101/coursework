`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:35:14 12/05/2013 
// Design Name: 
// Module Name:    fourDigitDisplay 
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
module fourDigitDisplay(
    input [3:0] W,
    input [3:0] X,
    input [3:0] Y,
    input [3:0] Z,
    input [3:0] decPts,
    input [3:0] signs,
    input CLK,
    output [6:0] segs,
    output DP,
    output [3:0] anodes
    );
	 
	wire clk_10K;
	wire [1:0] Q;
	wire [3:0] inputCurrent;
	wire decPt, sgn;// The currently displayed value of decPts and signs
	
	// Divide the system clock to 10KHz
	clkDiv10K U1(CLK, clk_10K);
	
		//Chooses which digit to display
	digitChooser U2(W,X, Y, Z, decPts, signs, Q, inputCurrent, decPt, sgn, anodes);
	

	Hex2Seg U3(inputCurrent, sgn, decPt, segs, DP);
	
	// --- State memory ---
	// 2-bit binary counter driving the 2-to-4 anode decoder
	//Reset is 0
	bin_cnt2 U4(clk_10K, 1'b0, Q);


endmodule
