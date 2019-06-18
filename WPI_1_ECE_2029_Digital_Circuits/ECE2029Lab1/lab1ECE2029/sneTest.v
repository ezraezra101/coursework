// Verilog test fixture created from schematic C:\Users\edavis\Desktop\ECE2029\lab1ECE2029\tut.sch - Thu Nov 07 08:15:25 2013

`timescale 1ns / 1ps

module tut_tut_sch_tb();

// Inputs
   reg B;
   reg A;

// Output
   wire C;
   wire D;
   wire E;
   wire F;

// Bidirs

// Instantiate the UUT
   tut UUT (
		.C(C), 
		.D(D), 
		.E(E), 
		.F(F), 
		.B(B), 
		.A(A)
   );
// Initialize Inputs
       initial begin
		 // initialize variables
		#100; //wait 100 ns
		B = 0;
		A = 0;
		#100;
		A = 1;
		#100;
		A = 0;
		B = 1;
		#100;
		A = 1;
		#1000;
		end
endmodule
