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

	wire clk_10K;//, clk_1Hz;
	wire [1:0] Q;
	wire [3:0] inputCurrent;
	wire decPt, sgn;// The currently displayed value of decPts and signs
	
	wire uselessWire;
	
	// Divide the system clock to 10KHz
	clkDiv10K U1(CLK, clk_10K);
	
	// Divide the 10KHz clock to 1Hz
	//clkDiv10K U2(clk_10K, clk_1Hz);
	
	// Choosing which digit to display
	mux4to1 U6(Z,4'b0000, X,4'b0000,Q,inputCurrent);
	
	//mux1bit4to1 U7(decPts, Q, decPt);
	//mux1bit4to1 U8(signs, Q, sgn);
	assign DP = (((Q==2'b01) && (decPts[2]==1)) || ((Q==2'b11) && (decPts[0]))) ? 1'b0 : 1'b1 ;
	
	assign sgn = (((Q==2'b01) && (signs[2]==1)) || ((Q==2'b11) && (signs[0]))) ? 1'b1 : 1'b0 ;
	
	
	// Generate the 7-seg output for the display
	Hex2Seg U3(inputCurrent, sgn, decPt, segs, uselessWire);
	
	// --- Actual anode driving part ---
	// 2-bit binary counter driving the 2-to-4 anode decoder
	//Reset is 0
	bin_cnt2 U4(clk_10K, 0, Q);
	
	// For now set En = 1
	decodeAnode U5(1,Q[0],Q[1],anodes);
	


endmodule
