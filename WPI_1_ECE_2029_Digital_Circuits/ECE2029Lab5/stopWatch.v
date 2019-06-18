`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:49:27 12/12/2013 
// Design Name: 
// Module Name:    stopWatch 
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
module stopWatch(
    input CLK,
    input R,
    input CE,
    output [6:0] segs,
    output DP,
    output [3:0] anodes
    );

	wire clk_10K, clk_10Hz, R_out1, R_out2, R_out3, R_out4;
	wire [3:0] Qth, Qsec, Qsec10, Qmin;
	
	//Divide the system clock to 10KHz
	clkDiv10K U1(CLK,clk_10K);
	
	//Divide the 10KHz clock to 10Hz
	clkDiv1K U2(clk_10K,clk_10Hz);
	
	//Modulo 10 counter - for 10ths digit
	mod10cnt U3(clk_10Hz, R, CE, Qth, R_out1);
	
	// Modulo 10 counter - for seconds digit
	mod10cnt U4(clk_10Hz, R, R_out1, Qsec, R_out2);
	
	// Modulo 6 counter - for 10s of seconds digit
	mod6cnt U5(clk_10Hz, R, R_out2, Qsec10, R_out3);
	
	//Add modulo 10 counter for minutes (0-9)
	mod10cnt U6(clk_10Hz, R, R_out3, Qmin, R_out4);
	
	//Display output of 2 digit  counter with decimal point
	//Use the system CLK as the input clock and turn on the
	// 	decimal point on the seconds digit
	fourDigitDisplay U7(Qmin, Qsec10, Qsec, Qth,4'b0010, 4'b0000, CLK, segs, DP, anodes);
	//Argument order:
	//(inputs) W X Y Z decPts signs CLK (outputs) segs DP anodes


endmodule
