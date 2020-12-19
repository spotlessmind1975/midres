<style>
.statusTable th {
background: gray;
word-wrap: break-word;
text-align: center;
color: white;
}
.statusTable td { text-align: center; }
.statusTable tr:nth-child(odd) { background: lightgray; }
.statusTable tr:nth-child(even) { background: lightgray; }
.statusTable tr:nth-child(odd) td:nth-child(1) { background: gray; text-align: right; }
.statusTable tr:nth-child(even) td:nth-child(1) { text-align: right; }
.statusTable tr:nth-child(1) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(1) > td:nth-child(3) { background-color: pink; }
.statusTable tr:nth-child(1) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(1) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(1) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(2) > td:nth-child(2) { background-color: cyan; }
.statusTable tr:nth-child(2) > td:nth-child(3) { background-color: pink; }
.statusTable tr:nth-child(2) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(2) > td:nth-child(5) { background-color: cyan; }
.statusTable tr:nth-child(2) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(3) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(3) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(3) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(3) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(3) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(4) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(4) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(4) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(4) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(4) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(5) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(5) > td:nth-child(3) { background-color: pink; }
.statusTable tr:nth-child(5) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(5) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(5) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(6) > td:nth-child(2) { background-color: yellow; }
.statusTable tr:nth-child(6) > td:nth-child(3) { background-color: yellow; }
.statusTable tr:nth-child(6) > td:nth-child(4) { background-color: yellow; }
.statusTable tr:nth-child(6) > td:nth-child(5) { background-color: yellow; }
.statusTable tr:nth-child(6) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(7) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(7) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(7) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(7) > td:nth-child(5) { background-color: yellow; }
.statusTable tr:nth-child(7) > td:nth-child(6) { background-color: orange; }
.statusTable tr:nth-child(8) > td:nth-child(2) { background-color: cyan; }
.statusTable tr:nth-child(8) > td:nth-child(3) { background-color: pink; }
.statusTable tr:nth-child(8) > td:nth-child(4) { background-color: pink; }
.statusTable tr:nth-child(8) > td:nth-child(5) { background-color: cyan; }
.statusTable tr:nth-child(8) > td:nth-child(6) { background-color: pink; }
.statusTable tr:nth-child(9) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(9) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(9) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(9) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(9) > td:nth-child(6) { background-color: orange; }
.statusTable tr:nth-child(10) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(10) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(10) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(10) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(10) > td:nth-child(6) { background-color: orange; }
.statusTable tr:nth-child(11) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(11) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(11) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(11) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(11) > td:nth-child(6) { background-color: orange; }
.statusTable tr:nth-child(12) > td:nth-child(2) { background-color: pink; }
.statusTable tr:nth-child(12) > td:nth-child(3) { background-color: pink; }
.statusTable tr:nth-child(12) > td:nth-child(4) { background-color: pink; }
.statusTable tr:nth-child(12) > td:nth-child(5) { background-color: pink; }
.statusTable tr:nth-child(12) > td:nth-child(6) { background-color: yellow; }
.statusTable tr:nth-child(13) > td:nth-child(2) { background-color: lightgreen; }
.statusTable tr:nth-child(13) > td:nth-child(3) { background-color: lightgreen; }
.statusTable tr:nth-child(13) > td:nth-child(4) { background-color: lightgreen; }
.statusTable tr:nth-child(13) > td:nth-child(5) { background-color: lightgreen; }
.statusTable tr:nth-child(13) > td:nth-child(6) { background-color: orange; }
</style>
<div class="statusTable">

# STATUS

target name     |alienstorm      |elevator        |airattack       |totto           |joycheck        |
----------------|----------------|----------------|----------------|----------------|----------------|
**msx**         |YES             |NO[1]           |YES             |YES             |NO[4]           |
**msxc**        |NO[2]           |NO[1]           |YES             |NO[2]           |NO[4]           |
**svi**         |YES             |YES             |YES             |YES             |NO[4]           |
**mtx500**      |YES             |YES             |YES             |YES             |NO[4]           |
**coleco**      |YES             |NO[1]           |YES             |YES             |NO[4]           |
**atmos**       |NO[4]           |NO[4]           |NO[4]           |NO[4]           |NO[4]           |
**atari**       |YES             |YES             |YES             |NO[4]           |NO[3]           |
**vic20**       |NO[2]           |NO[1]           |NO[1]           |NO[2]           |NO[1]           |
**vic2024**     |YES             |YES             |YES             |YES             |NO[3]           |
**c64**         |YES             |YES             |YES             |YES             |NO[3]           |
**c128**        |YES             |YES             |YES             |YES             |NO[3]           |
**c16**         |NO[1]           |NO[1]           |NO[1]           |NO[1]           |NO[4]           |
**plus4**       |YES             |YES             |YES             |YES             |NO[3]           |

</div>

**Legend**
 
[1] the program cannot fit into the retrocomputer memory
[2] the program needs too much resources
[3] severe error during compilation
[4] the makefile rule is missing
