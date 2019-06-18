<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan6" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_1" />
        <signal name="XLXN_2" />
        <signal name="XLXN_3" />
        <signal name="A2" />
        <signal name="B2" />
        <signal name="A1" />
        <signal name="B1" />
        <signal name="B0" />
        <signal name="A0" />
        <signal name="A3" />
        <signal name="B3" />
        <signal name="S0" />
        <signal name="S1" />
        <signal name="S2" />
        <signal name="S3" />
        <signal name="XLXN_29" />
        <signal name="XLXN_30" />
        <signal name="XLXN_31" />
        <signal name="OverFlow" />
        <port polarity="Input" name="A2" />
        <port polarity="Input" name="B2" />
        <port polarity="Input" name="A1" />
        <port polarity="Input" name="B1" />
        <port polarity="Input" name="B0" />
        <port polarity="Input" name="A0" />
        <port polarity="Input" name="A3" />
        <port polarity="Input" name="B3" />
        <port polarity="Output" name="S0" />
        <port polarity="Output" name="S1" />
        <port polarity="Output" name="S2" />
        <port polarity="Output" name="S3" />
        <port polarity="Output" name="OverFlow" />
        <blockdef name="myFullAdder">
            <timestamp>2013-11-14T14:6:2</timestamp>
            <rect width="256" x="64" y="-192" height="192" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
        </blockdef>
        <blockdef name="gnd">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-96" x1="64" />
            <line x2="52" y1="-48" y2="-48" x1="76" />
            <line x2="60" y1="-32" y2="-32" x1="68" />
            <line x2="40" y1="-64" y2="-64" x1="88" />
            <line x2="64" y1="-64" y2="-80" x1="64" />
            <line x2="64" y1="-128" y2="-96" x1="64" />
        </blockdef>
        <blockdef name="and3b1">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
        </blockdef>
        <blockdef name="and3b2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="40" y1="-64" y2="-64" x1="0" />
            <circle r="12" cx="52" cy="-64" />
            <line x2="40" y1="-128" y2="-128" x1="0" />
            <circle r="12" cx="52" cy="-128" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
        </blockdef>
        <blockdef name="or2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <arc ex="192" ey="-96" sx="112" sy="-48" r="88" cx="116" cy="-136" />
            <arc ex="48" ey="-144" sx="48" sy="-48" r="56" cx="16" cy="-96" />
            <line x2="48" y1="-144" y2="-144" x1="112" />
            <arc ex="112" ey="-144" sx="192" sy="-96" r="88" cx="116" cy="-56" />
            <line x2="48" y1="-48" y2="-48" x1="112" />
        </blockdef>
        <block symbolname="myFullAdder" name="XLXI_1">
            <blockpin signalname="A0" name="A" />
            <blockpin signalname="B0" name="B" />
            <blockpin signalname="XLXN_29" name="Cin" />
            <blockpin signalname="XLXN_1" name="Cout" />
            <blockpin signalname="S0" name="Sum" />
        </block>
        <block symbolname="myFullAdder" name="XLXI_2">
            <blockpin signalname="A1" name="A" />
            <blockpin signalname="B1" name="B" />
            <blockpin signalname="XLXN_1" name="Cin" />
            <blockpin signalname="XLXN_2" name="Cout" />
            <blockpin signalname="S1" name="Sum" />
        </block>
        <block symbolname="myFullAdder" name="XLXI_3">
            <blockpin signalname="A2" name="A" />
            <blockpin signalname="B2" name="B" />
            <blockpin signalname="XLXN_2" name="Cin" />
            <blockpin signalname="XLXN_3" name="Cout" />
            <blockpin signalname="S2" name="Sum" />
        </block>
        <block symbolname="myFullAdder" name="XLXI_4">
            <blockpin signalname="A3" name="A" />
            <blockpin signalname="B3" name="B" />
            <blockpin signalname="XLXN_3" name="Cin" />
            <blockpin name="Cout" />
            <blockpin signalname="S3" name="Sum" />
        </block>
        <block symbolname="gnd" name="XLXI_5">
            <blockpin signalname="XLXN_29" name="G" />
        </block>
        <block symbolname="and3b1" name="XLXI_6">
            <blockpin signalname="S3" name="I0" />
            <blockpin signalname="B3" name="I1" />
            <blockpin signalname="A3" name="I2" />
            <blockpin signalname="XLXN_30" name="O" />
        </block>
        <block symbolname="and3b2" name="XLXI_7">
            <blockpin signalname="B3" name="I0" />
            <blockpin signalname="A3" name="I1" />
            <blockpin signalname="S3" name="I2" />
            <blockpin signalname="XLXN_31" name="O" />
        </block>
        <block symbolname="or2" name="XLXI_8">
            <blockpin signalname="XLXN_31" name="I0" />
            <blockpin signalname="XLXN_30" name="I1" />
            <blockpin signalname="OverFlow" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1200" y="1232" name="XLXI_1" orien="R0">
        </instance>
        <instance x="1200" y="1552" name="XLXI_2" orien="R0">
        </instance>
        <branch name="XLXN_1">
            <wire x2="1136" y1="1296" y2="1520" x1="1136" />
            <wire x2="1200" y1="1520" y2="1520" x1="1136" />
            <wire x2="1664" y1="1296" y2="1296" x1="1136" />
            <wire x2="1664" y1="1136" y2="1136" x1="1584" />
            <wire x2="1664" y1="1136" y2="1296" x1="1664" />
        </branch>
        <branch name="A1">
            <wire x2="1200" y1="1392" y2="1392" x1="1088" />
        </branch>
        <branch name="B1">
            <wire x2="1200" y1="1456" y2="1456" x1="1088" />
        </branch>
        <branch name="B0">
            <wire x2="1184" y1="1136" y2="1136" x1="1088" />
            <wire x2="1200" y1="1136" y2="1136" x1="1184" />
        </branch>
        <branch name="A0">
            <wire x2="1200" y1="1072" y2="1072" x1="1088" />
        </branch>
        <branch name="S0">
            <wire x2="1744" y1="1072" y2="1072" x1="1584" />
        </branch>
        <branch name="S1">
            <wire x2="1744" y1="1392" y2="1392" x1="1584" />
        </branch>
        <instance x="1200" y="1872" name="XLXI_3" orien="R0">
        </instance>
        <branch name="A2">
            <wire x2="1200" y1="1712" y2="1712" x1="1088" />
        </branch>
        <branch name="B2">
            <wire x2="1200" y1="1776" y2="1776" x1="1088" />
        </branch>
        <branch name="XLXN_2">
            <wire x2="1136" y1="1616" y2="1840" x1="1136" />
            <wire x2="1200" y1="1840" y2="1840" x1="1136" />
            <wire x2="1600" y1="1616" y2="1616" x1="1136" />
            <wire x2="1600" y1="1456" y2="1456" x1="1584" />
            <wire x2="1600" y1="1456" y2="1616" x1="1600" />
        </branch>
        <branch name="S2">
            <wire x2="1744" y1="1712" y2="1712" x1="1584" />
        </branch>
        <instance x="1200" y="2192" name="XLXI_4" orien="R0">
        </instance>
        <branch name="A3">
            <wire x2="1200" y1="2032" y2="2032" x1="1088" />
        </branch>
        <branch name="B3">
            <wire x2="1200" y1="2096" y2="2096" x1="1088" />
        </branch>
        <branch name="XLXN_3">
            <wire x2="1120" y1="1936" y2="2160" x1="1120" />
            <wire x2="1200" y1="2160" y2="2160" x1="1120" />
            <wire x2="1600" y1="1936" y2="1936" x1="1120" />
            <wire x2="1600" y1="1776" y2="1776" x1="1584" />
            <wire x2="1600" y1="1776" y2="1936" x1="1600" />
        </branch>
        <branch name="S3">
            <wire x2="1744" y1="2032" y2="2032" x1="1584" />
        </branch>
        <instance x="912" y="1344" name="XLXI_5" orien="R0" />
        <branch name="XLXN_29">
            <wire x2="1200" y1="1200" y2="1200" x1="976" />
            <wire x2="976" y1="1200" y2="1216" x1="976" />
        </branch>
        <iomarker fontsize="28" x="1088" y="1072" name="A0" orien="R180" />
        <iomarker fontsize="28" x="1088" y="1136" name="B0" orien="R180" />
        <iomarker fontsize="28" x="1088" y="1392" name="A1" orien="R180" />
        <iomarker fontsize="28" x="1088" y="1456" name="B1" orien="R180" />
        <iomarker fontsize="28" x="1088" y="1712" name="A2" orien="R180" />
        <iomarker fontsize="28" x="1088" y="1776" name="B2" orien="R180" />
        <iomarker fontsize="28" x="1088" y="2032" name="A3" orien="R180" />
        <iomarker fontsize="28" x="1088" y="2096" name="B3" orien="R180" />
        <iomarker fontsize="28" x="1744" y="2032" name="S3" orien="R0" />
        <iomarker fontsize="28" x="1744" y="1712" name="S2" orien="R0" />
        <iomarker fontsize="28" x="1744" y="1392" name="S1" orien="R0" />
        <iomarker fontsize="28" x="1744" y="1072" name="S0" orien="R0" />
        <instance x="1120" y="2512" name="XLXI_6" orien="R0" />
        <instance x="1120" y="2720" name="XLXI_7" orien="R0" />
        <instance x="1424" y="2576" name="XLXI_8" orien="R0" />
        <branch name="XLXN_30">
            <wire x2="1392" y1="2384" y2="2384" x1="1376" />
            <wire x2="1392" y1="2384" y2="2448" x1="1392" />
            <wire x2="1424" y1="2448" y2="2448" x1="1392" />
        </branch>
        <branch name="XLXN_31">
            <wire x2="1392" y1="2592" y2="2592" x1="1376" />
            <wire x2="1392" y1="2512" y2="2592" x1="1392" />
            <wire x2="1424" y1="2512" y2="2512" x1="1392" />
        </branch>
        <branch name="OverFlow">
            <wire x2="1712" y1="2480" y2="2480" x1="1680" />
        </branch>
        <branch name="A3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1072" y="2320" type="branch" />
            <wire x2="1120" y1="2320" y2="2320" x1="1072" />
        </branch>
        <branch name="B3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1072" y="2384" type="branch" />
            <wire x2="1120" y1="2384" y2="2384" x1="1072" />
        </branch>
        <branch name="S3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1072" y="2448" type="branch" />
            <wire x2="1120" y1="2448" y2="2448" x1="1072" />
        </branch>
        <branch name="B3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1072" y="2656" type="branch" />
            <wire x2="1120" y1="2656" y2="2656" x1="1072" />
        </branch>
        <iomarker fontsize="28" x="1712" y="2480" name="OverFlow" orien="R0" />
        <branch name="S3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1072" y="2528" type="branch" />
            <wire x2="1120" y1="2528" y2="2528" x1="1072" />
        </branch>
        <branch name="A3">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1072" y="2592" type="branch" />
            <wire x2="1120" y1="2592" y2="2592" x1="1072" />
        </branch>
    </sheet>
</drawing>