<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="aspartan6" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_1" />
        <signal name="XLXN_2" />
        <signal name="XLXN_4" />
        <signal name="Cin" />
        <signal name="B" />
        <signal name="A" />
        <signal name="Cout" />
        <signal name="Sum" />
        <port polarity="Input" name="Cin" />
        <port polarity="Input" name="B" />
        <port polarity="Input" name="A" />
        <port polarity="Output" name="Cout" />
        <port polarity="Output" name="Sum" />
        <blockdef name="and2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="192" y1="-96" y2="-96" x1="256" />
            <arc ex="144" ey="-144" sx="144" sy="-48" r="48" cx="144" cy="-96" />
            <line x2="64" y1="-48" y2="-48" x1="144" />
            <line x2="144" y1="-144" y2="-144" x1="64" />
            <line x2="64" y1="-48" y2="-144" x1="64" />
        </blockdef>
        <blockdef name="or3">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="48" y1="-64" y2="-64" x1="0" />
            <line x2="72" y1="-128" y2="-128" x1="0" />
            <line x2="48" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <arc ex="192" ey="-128" sx="112" sy="-80" r="88" cx="116" cy="-168" />
            <arc ex="48" ey="-176" sx="48" sy="-80" r="56" cx="16" cy="-128" />
            <line x2="48" y1="-64" y2="-80" x1="48" />
            <line x2="48" y1="-192" y2="-176" x1="48" />
            <line x2="48" y1="-80" y2="-80" x1="112" />
            <arc ex="112" ey="-176" sx="192" sy="-128" r="88" cx="116" cy="-88" />
            <line x2="48" y1="-176" y2="-176" x1="112" />
        </blockdef>
        <blockdef name="xor3">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="48" y1="-64" y2="-64" x1="0" />
            <line x2="72" y1="-128" y2="-128" x1="0" />
            <line x2="48" y1="-192" y2="-192" x1="0" />
            <line x2="208" y1="-128" y2="-128" x1="256" />
            <arc ex="48" ey="-176" sx="48" sy="-80" r="56" cx="16" cy="-128" />
            <arc ex="64" ey="-176" sx="64" sy="-80" r="56" cx="32" cy="-128" />
            <arc ex="128" ey="-176" sx="208" sy="-128" r="88" cx="132" cy="-88" />
            <line x2="48" y1="-64" y2="-80" x1="48" />
            <line x2="48" y1="-192" y2="-176" x1="48" />
            <line x2="64" y1="-80" y2="-80" x1="128" />
            <line x2="64" y1="-176" y2="-176" x1="128" />
            <arc ex="208" ey="-128" sx="128" sy="-80" r="88" cx="132" cy="-168" />
        </blockdef>
        <block symbolname="and2" name="XLXI_1">
            <blockpin signalname="B" name="I0" />
            <blockpin signalname="Cin" name="I1" />
            <blockpin signalname="XLXN_1" name="O" />
        </block>
        <block symbolname="and2" name="XLXI_2">
            <blockpin signalname="A" name="I0" />
            <blockpin signalname="Cin" name="I1" />
            <blockpin signalname="XLXN_2" name="O" />
        </block>
        <block symbolname="and2" name="XLXI_3">
            <blockpin signalname="B" name="I0" />
            <blockpin signalname="A" name="I1" />
            <blockpin signalname="XLXN_4" name="O" />
        </block>
        <block symbolname="or3" name="XLXI_4">
            <blockpin signalname="XLXN_4" name="I0" />
            <blockpin signalname="XLXN_2" name="I1" />
            <blockpin signalname="XLXN_1" name="I2" />
            <blockpin signalname="Cout" name="O" />
        </block>
        <block symbolname="xor3" name="XLXI_5">
            <blockpin signalname="Cin" name="I0" />
            <blockpin signalname="B" name="I1" />
            <blockpin signalname="A" name="I2" />
            <blockpin signalname="Sum" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1600" y="1248" name="XLXI_1" orien="R0" />
        <instance x="1600" y="1392" name="XLXI_2" orien="R0" />
        <instance x="1600" y="1536" name="XLXI_3" orien="R0" />
        <instance x="1968" y="1424" name="XLXI_4" orien="R0" />
        <branch name="XLXN_1">
            <wire x2="1968" y1="1152" y2="1152" x1="1856" />
            <wire x2="1968" y1="1152" y2="1232" x1="1968" />
        </branch>
        <branch name="XLXN_2">
            <wire x2="1872" y1="1296" y2="1296" x1="1856" />
            <wire x2="1968" y1="1296" y2="1296" x1="1872" />
        </branch>
        <branch name="XLXN_4">
            <wire x2="1968" y1="1440" y2="1440" x1="1856" />
            <wire x2="1968" y1="1360" y2="1440" x1="1968" />
        </branch>
        <branch name="Cin">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1440" y="1120" type="branch" />
            <wire x2="1600" y1="1120" y2="1120" x1="1440" />
        </branch>
        <branch name="B">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1440" y="1184" type="branch" />
            <wire x2="1584" y1="1184" y2="1184" x1="1440" />
            <wire x2="1600" y1="1184" y2="1184" x1="1584" />
        </branch>
        <branch name="Cin">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1440" y="1264" type="branch" />
            <wire x2="1600" y1="1264" y2="1264" x1="1440" />
        </branch>
        <branch name="A">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1440" y="1328" type="branch" />
            <wire x2="1600" y1="1328" y2="1328" x1="1440" />
        </branch>
        <branch name="A">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1440" y="1408" type="branch" />
            <wire x2="1600" y1="1408" y2="1408" x1="1440" />
        </branch>
        <branch name="B">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1440" y="1472" type="branch" />
            <wire x2="1600" y1="1472" y2="1472" x1="1440" />
        </branch>
        <branch name="Cout">
            <wire x2="2304" y1="1296" y2="1296" x1="2224" />
        </branch>
        <instance x="1504" y="912" name="XLXI_5" orien="R0" />
        <branch name="Sum">
            <wire x2="1808" y1="784" y2="784" x1="1760" />
        </branch>
        <branch name="A">
            <wire x2="1504" y1="720" y2="720" x1="1472" />
        </branch>
        <branch name="B">
            <wire x2="1504" y1="784" y2="784" x1="1472" />
        </branch>
        <branch name="Cin">
            <wire x2="1504" y1="848" y2="848" x1="1472" />
        </branch>
        <iomarker fontsize="28" x="1472" y="720" name="A" orien="R180" />
        <iomarker fontsize="28" x="1472" y="784" name="B" orien="R180" />
        <iomarker fontsize="28" x="1472" y="848" name="Cin" orien="R180" />
        <iomarker fontsize="28" x="1808" y="784" name="Sum" orien="R0" />
        <iomarker fontsize="28" x="2304" y="1296" name="Cout" orien="R0" />
    </sheet>
</drawing>