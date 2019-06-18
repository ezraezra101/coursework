<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan6" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="C" />
        <signal name="D" />
        <signal name="E" />
        <signal name="F" />
        <signal name="B" />
        <signal name="A" />
        <port polarity="Output" name="C" />
        <port polarity="Output" name="D" />
        <port polarity="Output" name="E" />
        <port polarity="Output" name="F" />
        <port polarity="Input" name="B" />
        <port polarity="Input" name="A" />
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
        <blockdef name="xor2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="60" y1="-128" y2="-128" x1="0" />
            <line x2="208" y1="-96" y2="-96" x1="256" />
            <arc ex="44" ey="-144" sx="48" sy="-48" r="56" cx="16" cy="-96" />
            <arc ex="64" ey="-144" sx="64" sy="-48" r="56" cx="32" cy="-96" />
            <line x2="64" y1="-144" y2="-144" x1="128" />
            <line x2="64" y1="-48" y2="-48" x1="128" />
            <arc ex="128" ey="-144" sx="208" sy="-96" r="88" cx="132" cy="-56" />
            <arc ex="208" ey="-96" sx="128" sy="-48" r="88" cx="132" cy="-136" />
        </blockdef>
        <blockdef name="inv">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="160" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="-64" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="0" x1="128" />
            <line x2="64" y1="0" y2="-64" x1="64" />
            <circle r="16" cx="144" cy="-32" />
        </blockdef>
        <block symbolname="and2" name="XLXI_1">
            <blockpin signalname="B" name="I0" />
            <blockpin signalname="A" name="I1" />
            <blockpin signalname="C" name="O" />
        </block>
        <block symbolname="or2" name="XLXI_2">
            <blockpin signalname="B" name="I0" />
            <blockpin signalname="A" name="I1" />
            <blockpin signalname="D" name="O" />
        </block>
        <block symbolname="xor2" name="XLXI_3">
            <blockpin signalname="B" name="I0" />
            <blockpin signalname="A" name="I1" />
            <blockpin signalname="E" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_4">
            <blockpin signalname="A" name="I" />
            <blockpin signalname="F" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1440" y="1248" name="XLXI_1" orien="R0" />
        <instance x="1440" y="1408" name="XLXI_2" orien="R0" />
        <instance x="1440" y="1552" name="XLXI_3" orien="R0" />
        <instance x="1440" y="1616" name="XLXI_4" orien="R0" />
        <branch name="C">
            <wire x2="1744" y1="1152" y2="1152" x1="1696" />
        </branch>
        <branch name="D">
            <wire x2="1760" y1="1312" y2="1312" x1="1696" />
        </branch>
        <branch name="E">
            <wire x2="1760" y1="1456" y2="1456" x1="1696" />
        </branch>
        <branch name="F">
            <wire x2="1760" y1="1584" y2="1584" x1="1664" />
        </branch>
        <branch name="A">
            <wire x2="1440" y1="1120" y2="1120" x1="1376" />
        </branch>
        <branch name="B">
            <wire x2="1440" y1="1184" y2="1184" x1="1376" />
        </branch>
        <branch name="A">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1376" y="1280" type="branch" />
            <wire x2="1440" y1="1280" y2="1280" x1="1376" />
        </branch>
        <branch name="B">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1376" y="1344" type="branch" />
            <wire x2="1440" y1="1344" y2="1344" x1="1376" />
        </branch>
        <branch name="A">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1376" y="1424" type="branch" />
            <wire x2="1440" y1="1424" y2="1424" x1="1376" />
        </branch>
        <branch name="B">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1376" y="1488" type="branch" />
            <wire x2="1440" y1="1488" y2="1488" x1="1376" />
        </branch>
        <branch name="A">
            <attrtext style="alignment:SOFT-RIGHT;fontsize:28;fontname:Arial" attrname="Name" x="1376" y="1584" type="branch" />
            <wire x2="1440" y1="1584" y2="1584" x1="1376" />
        </branch>
        <iomarker fontsize="28" x="1376" y="1120" name="A" orien="R180" />
        <iomarker fontsize="28" x="1376" y="1184" name="B" orien="R180" />
        <iomarker fontsize="28" x="1760" y="1584" name="F" orien="R0" />
        <iomarker fontsize="28" x="1760" y="1456" name="E" orien="R0" />
        <iomarker fontsize="28" x="1760" y="1312" name="D" orien="R0" />
        <iomarker fontsize="28" x="1744" y="1152" name="C" orien="R0" />
    </sheet>
</drawing>