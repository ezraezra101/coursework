////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: P.58f
//  \   \         Application: netgen
//  /   /         Filename: tut_timesim.v
// /___/   /\     Timestamp: Thu Nov 07 08:41:25 2013
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -s 3 -pcf tut.pcf -sdf_anno true -sdf_path netgen/par -insert_glbl true -insert_pp_buffers true -w -dir netgen/par -ofmt verilog -sim tut.ncd tut_timesim.v 
// Device	: 6slx16csg324-3 (PRODUCTION 1.23 2013-03-26)
// Input file	: tut.ncd
// Output file	: C:\Users\edavis\Desktop\ECE2029\lab1ECE2029\netgen\par\tut_timesim.v
// # of Modules	: 1
// Design Name	: tut
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

module tut (
  A, B, C, D, E, F
);
  input A;
  input B;
  output C;
  output D;
  output E;
  output F;
  wire F_OBUF_30;
  wire B_IBUF_0;
  wire C_OBUF_32;
  wire D_OBUF_33;
  wire E_OBUF_34;
  wire \ProtoComp0.IINV.OUT ;
  wire F_OBUF_non_inverted;
  wire B_IBUF_5;
  wire \NlwBufferSignal_C_OBUF/I ;
  wire \NlwBufferSignal_D_OBUF/I ;
  wire \NlwBufferSignal_E_OBUF/I ;
  wire \NlwBufferSignal_F_OBUF/I ;
  initial $sdf_annotate("netgen/par/tut_timesim.sdf");
  X_IPAD #(
    .LOC ( "PAD154" ))
  A_5 (
    .PAD(A)
  );
  X_BUF #(
    .LOC ( "PAD154" ))
  A_IBUF (
    .O(F_OBUF_non_inverted),
    .I(A)
  );
  X_BUF #(
    .LOC ( "PAD154" ))
  \ProtoComp0.IMUX  (
    .I(\ProtoComp0.IINV.OUT ),
    .O(F_OBUF_30)
  );
  X_INV #(
    .LOC ( "PAD154" ))
  \ProtoComp0.IINV  (
    .I(F_OBUF_non_inverted),
    .O(\ProtoComp0.IINV.OUT )
  );
  X_IPAD #(
    .LOC ( "PAD159" ))
  B_9 (
    .PAD(B)
  );
  X_BUF #(
    .LOC ( "PAD159" ))
  B_IBUF (
    .O(B_IBUF_5),
    .I(B)
  );
  X_BUF #(
    .LOC ( "PAD159" ))
  \ProtoComp1.IMUX  (
    .I(B_IBUF_5),
    .O(B_IBUF_0)
  );
  X_OPAD #(
    .LOC ( "PAD130" ))
  C_12 (
    .PAD(C)
  );
  X_OBUF #(
    .LOC ( "PAD130" ))
  C_OBUF (
    .I(\NlwBufferSignal_C_OBUF/I ),
    .O(C)
  );
  X_OPAD #(
    .LOC ( "PAD129" ))
  D_15 (
    .PAD(D)
  );
  X_OBUF #(
    .LOC ( "PAD129" ))
  D_OBUF (
    .I(\NlwBufferSignal_D_OBUF/I ),
    .O(D)
  );
  X_OPAD #(
    .LOC ( "PAD126" ))
  E_18 (
    .PAD(E)
  );
  X_OBUF #(
    .LOC ( "PAD126" ))
  E_OBUF (
    .I(\NlwBufferSignal_E_OBUF/I ),
    .O(E)
  );
  X_OPAD #(
    .LOC ( "PAD125" ))
  F_21 (
    .PAD(F)
  );
  X_OBUF #(
    .LOC ( "PAD125" ))
  F_OBUF (
    .I(\NlwBufferSignal_F_OBUF/I ),
    .O(F)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X23Y1" ),
    .INIT ( 64'h00000000FFFF0000 ))
  XLXI_1 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(1'b1),
    .ADR4(B_IBUF_0),
    .ADR5(F_OBUF_30),
    .O(C_OBUF_32)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X34Y2" ),
    .INIT ( 64'hFFFFFFFF55555555 ))
  XLXI_2 (
    .ADR4(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(1'b1),
    .ADR5(B_IBUF_0),
    .ADR0(F_OBUF_30),
    .O(D_OBUF_33)
  );
  X_LUT6 #(
    .LOC ( "SLICE_X37Y2" ),
    .INIT ( 64'hFFFF00000000FFFF ))
  XLXI_3 (
    .ADR0(1'b1),
    .ADR1(1'b1),
    .ADR2(1'b1),
    .ADR3(1'b1),
    .ADR4(B_IBUF_0),
    .ADR5(F_OBUF_30),
    .O(E_OBUF_34)
  );
  X_BUF   \NlwBufferBlock_C_OBUF/I  (
    .I(C_OBUF_32),
    .O(\NlwBufferSignal_C_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_D_OBUF/I  (
    .I(D_OBUF_33),
    .O(\NlwBufferSignal_D_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_E_OBUF/I  (
    .I(E_OBUF_34),
    .O(\NlwBufferSignal_E_OBUF/I )
  );
  X_BUF   \NlwBufferBlock_F_OBUF/I  (
    .I(F_OBUF_30),
    .O(\NlwBufferSignal_F_OBUF/I )
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

