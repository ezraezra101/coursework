`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:41:13 11/14/2013 
// Design Name: 
// Module Name:    fullAdd 
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
module fullAdd(
    input A,
    input B,
    input Cin,
    output Sum,
    output Cout
    );

assign Sum = (A^B)^Cin;
assign Cout = ((Cin & B) | (Cin & A)) | (A & B);

endmodule
