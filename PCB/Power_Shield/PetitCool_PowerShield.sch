<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.3.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="22" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="6" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="headers(.1in)">
<packages>
<package name="1X1">
<rectangle x1="-0.3175" y1="-0.3175" x2="0.3175" y2="0.3175" layer="21"/>
<pad name="1" x="0" y="0" drill="1.016" diameter="1.778"/>
</package>
</packages>
<symbols>
<symbol name="1X1_HEADER">
<wire x1="-2.54" y1="2.54" x2="2.54" y2="2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-2.54" x2="2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="2.54" y2="0" width="0.254" layer="94"/>
<text x="0" y="0" size="1.016" layer="94" font="vector" rot="MR0" align="center">01</text>
<text x="-2.54" y="3.175" size="1.27" layer="95" font="vector">&gt;NAME</text>
<text x="-2.54" y="-4.445" size="1.27" layer="96" font="vector">&gt;VALUE</text>
<pin name="1" x="2.54" y="0" visible="off" length="point" rot="R180"/>
<wire x1="2.54" y1="0" x2="2.54" y2="2.54" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="1X1" prefix="H">
<gates>
<gate name="G$1" symbol="1X1_HEADER" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X1">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="+V1" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="GND_V1" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="+V2" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="GND_V2" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="SLEEVE_V1_(GND)" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="PIN_V1_(+V1)" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="SLEEVE_SHUNT_V1" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="SLEEVE_SHUNT_V2" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="SLEEVE_V2_(GND)" library="headers(.1in)" deviceset="1X1" device="" value=""/>
<part name="PIN_V2_(+V2)" library="headers(.1in)" deviceset="1X1" device="" value=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="+V1" gate="G$1" x="-12.7" y="73.66"/>
<instance part="GND_V1" gate="G$1" x="-12.7" y="68.58"/>
<instance part="+V2" gate="G$1" x="-12.7" y="50.8"/>
<instance part="GND_V2" gate="G$1" x="-12.7" y="45.72"/>
<instance part="SLEEVE_V1_(GND)" gate="G$1" x="22.86" y="68.58"/>
<instance part="PIN_V1_(+V1)" gate="G$1" x="22.86" y="63.5"/>
<instance part="SLEEVE_SHUNT_V1" gate="G$1" x="38.1" y="66.04"/>
<instance part="SLEEVE_SHUNT_V2" gate="G$1" x="38.1" y="43.18"/>
<instance part="SLEEVE_V2_(GND)" gate="G$1" x="22.86" y="45.72"/>
<instance part="PIN_V2_(+V2)" gate="G$1" x="22.86" y="40.64"/>
</instances>
<busses>
</busses>
<nets>
<net name="+V1" class="0">
<segment>
<wire x1="2.54" y1="73.66" x2="-10.16" y2="73.66" width="0.1524" layer="91"/>
<wire x1="2.54" y1="73.66" x2="2.54" y2="63.5" width="0.1524" layer="91"/>
<wire x1="2.54" y1="63.5" x2="25.4" y2="63.5" width="0.1524" layer="91"/>
<pinref part="+V1" gate="G$1" pin="1"/>
<pinref part="PIN_V1_(+V1)" gate="G$1" pin="1"/>
</segment>
</net>
<net name="GND_V1" class="0">
<segment>
<wire x1="-10.16" y1="68.58" x2="25.4" y2="68.58" width="0.1524" layer="91"/>
<pinref part="GND_V1" gate="G$1" pin="1"/>
<pinref part="SLEEVE_V1_(GND)" gate="G$1" pin="1"/>
</segment>
</net>
<net name="+V2" class="0">
<segment>
<wire x1="-10.16" y1="50.8" x2="2.54" y2="50.8" width="0.1524" layer="91"/>
<wire x1="2.54" y1="50.8" x2="2.54" y2="40.64" width="0.1524" layer="91"/>
<pinref part="+V2" gate="G$1" pin="1"/>
<pinref part="PIN_V2_(+V2)" gate="G$1" pin="1"/>
<wire x1="2.54" y1="40.64" x2="25.4" y2="40.64" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND_V2" class="0">
<segment>
<wire x1="-10.16" y1="45.72" x2="25.4" y2="45.72" width="0.1524" layer="91"/>
<pinref part="GND_V2" gate="G$1" pin="1"/>
<pinref part="SLEEVE_V2_(GND)" gate="G$1" pin="1"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
