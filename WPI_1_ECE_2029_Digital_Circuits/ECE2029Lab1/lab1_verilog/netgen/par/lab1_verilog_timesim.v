////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: P.58f
//  \   \         Application: netgen
//  /   /         Filename: lab1_verilog_timesim.v
// /___/   /\     Timestamp: Thu Nov 07 09:22:26 2013
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -s 3 -pcf lab1_verilog.pcf -sdf_anno true -sdf_path netgen/par -insert_glbl true -insert_pp_buffers true -w -dir netgen/par -ofmt verilog -sim lab1_verilog.ncd lab1_verilog_timesim.v 
// Device	: 6slx16csg324-3 (PRODUCTION 1.23 2013-03-26)
// Input file	: lab1_verilog.ncd
// Output file	: C:\Users\edavis\Desktop\ECE2029\lab1_verilog\netgen\par\lab1_verilog_timesim.v
// # of Modules	: 1
// Design Name	: lab1_verilog
// Xilinx        : C:\Xilinx\14.5\ISE_DS\ISE\
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module lab1_verilog (
  a, b, c, d, e, f
);
  input a;
  input b;
  output c;
  output d;
  output e;
  output f;
  wire d_OBUF_29;
  wire f_OBUF_30;
  wire b_IBUF_0;
  wire e_OBUF_0;
  wire c_OBUF_33;
  wire \ProtoComp2.IINV.OUT ;
  wire f_OBUF_non_inverted;
  wire b_IBUF_9;
  wire e_OBUF_19;
  wire \NlwBufferSignal_e_OBUF/I ;
  wire \NlwBufferSignal_f_OBUF/I ;
  wire \NlwBufferSignal_c_OBUF/I ;
  wire \NlwBufferSignal_d_OBUF/I ;
  initial $sdf_annotate("netgen/par/lab1_verilog_timesim.sdf");
  X_OPAD #(
    .LOC ( "PAD126" ))
  e_3 (
    .PAD(e)
  );
  X_OBUF #(
    .LOC ( "PAD126" ))
  e_OBUF (
    .I(\NlwBufferSignal_e_OBUF/I ),
    .O(e)
  );
  X_OPAD #(
    .LOC ( "PAD125" ))
  f_6 (
    .PAD(f)
  );
  X_OBUF #(
    .LOC ( "PAD125" ))
  f_OBUF (
    .I(\NlwBufferSignal_f_OBUF/I ),
    .O(f)
  );
  X_IPAD #(
    .LOC ( "PAD154" ))
  a_11 (
    .PAD(a)
  );
  X_BUF #(
    .LOC ( "PAD154" ))
  a_IBUF (
    .O(f_OBUF_non_inverted),
    .I(a)
  );
  X_BUF #(
    .LOC ( "PAD154" ))
  \ProtoComp2.IMUX  (
    .I(\ProtoComp2.IINV.OUT ),
    .O(f_OBUF_30)
  );
  X_INV #(
    .LOC ( "PAD154" ))
  \ProtoComp2.IINV  (
    .I(f_OBUF_non_inverted),
    .O(\ProtoComp2.IINV.OUT )
  );
  X_IPAD #(
    .LOC ( "PAD159" ))
  b_15 (
    .PAD(b)
  );
  X_BUF #(
    .LOC ( "PAD159" ))
  b_IBUF (
    .O(b_IBUF_9),
    .I(b)
  );
  X_BUF #(
    .LOC ( "PAD159" ))
  \ProtoComp3.IMUX  (
    .I(b_IBUF_9),
    .O(b_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD130" ))
  c_18 (
    .PAD(c)
  );
  X_OBUF #(
    .LOC ( "PAD130" ))
  c_OBUF (
    .I(\NlwBufferSignal_c_OBUF/I ),
    .O(c)
  );
  X_OPAD #(
    .LOC ( "PAD129" ))
  d_21 (
    .PAD(d)
  );
  X_OBUF #(
    .LOC ( "PAD129" ))
  d_OBUF (
    .I(\NlwBufferSignal_d_OBUF/I ),
    .O(d)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X22Y1" ),
    .INIT ( 64'h0F0F00000F0F0000 ))
  c1 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR5(1'b1),
    .ADR3(1'b1),
    .ADR2(f_OBUF_30),
    .ADR4(b_IBUF_0),
    .O(c_OBUF_33)
  );
  X_BUF   \d_OBUF/d_OBUF_AMUX_Delay  (
    .I(e_OBUF_19),
    .O(e_OBUF_0)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X34Y2" ),
    .INIT ( 64'hFF55FF55FF55FF55 ))
  d1 (
    .ADR4(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR0(f_OBUF_30),
    .ADR3(b_IBUF_0),
    .ADR5(1'b1),
    .O(d_OBUF_29)
  );
  X_LUT5 #(
    .LOC ( "SLICE_X34Y2" ),
    .INIT ( 32'hAA55AA55 ))
  \Mxor_e_xo<0>1  (
    .ADR4(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR0(f_OBUF_30),
    .ADR3(b_IBUF_0),
    .O(e_OBUF_19)
  );
  X_BUF   \NlwBufferBlock_e_OBUF/I  (
    .I(e_OBUF_0),
    .O(\NlwBufferSignal_e_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_f_OBUF/I  (
    .I(f_OBUF_30),
    .O(\NlwBufferSignal_f_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_c_OBUF/I  (
    .I(c_OBUF_33),
    .O(\NlwBufferSignal_c_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_d_OBUF/I  (
    .I(d_OBUF_29),
    .O(\NlwBufferSignal_d_OBUF/I )
  );
endmodule


`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

