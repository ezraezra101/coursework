// Verilog test fixture created from schematic M:\ECE 2029\Lab2Sch\myFullAdder.sch - Thu Nov 14 08:23:27 2013

`timescale 1ns / 1ps

module myFullAdder_myFullAdder_sch_tb();

// Inputs
   reg Cin;
   reg B;
   reg A;

// Output
   wire Cout;
   wire Sum;

// Bidirs

// Instantiate the UUT
   myFullAdder UUT (
		.Cin(Cin), 
		.B(B), 
		.A(A), 
		.Cout(Cout), 
		.Sum(Sum)
   );
// Initialize Inputs
 //  `ifdef auto_init
       initial begin
		 #50 //Wait before initialization
		Cin = 0;
		B = 0;
		A = 0;
		#100; //Wait
		Cin = 1;
		#100;//Wait
		B = 1;
		Cin = 0;
		#100;//Wait
		Cin = 1;
		#100;//Wait
		A = 1;
		B = 0;
		Cin = 0;
		#100//Wait
		Cin = 1;
		#100;//Wait
		B = 1;
		Cin = 0;
		#100;//Wait
		Cin = 1;
		#500;//Wait
		end
 //  `endif
endmodule
