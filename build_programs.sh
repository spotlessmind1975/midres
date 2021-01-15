#/*****************************************************************************
#* MIDRES Library - an isomorphic gamelib for retrocomputers                 *
#*****************************************************************************
#* Copyright 2020 Marco Spedaletti (asimov@mclink.it)
#* 
#* Licensed under the Apache License, Version 2.0 (the "License");
#* you may not use this file except in compliance with the License.
#* You may obtain a copy of the License at
#* 
#* http://www.apache.org/licenses/LICENSE-2.0
#* 
#* Unless required by applicable law or agreed to in writing, software
#* distributed under the License is distributed on an "AS IS" BASIS,
#* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#* See the License for the specific language governing permissions and
#* limitations under the License.
#*----------------------------------------------------------------------------
#* Concesso in licenza secondo i termini della Licenza Apache, versione 2.0 
#* (la "Licenza"); è proibito usare questo file se non in conformità alla 
#* Licenza. Una copia della Licenza è disponibile all'indirizzo:
#* 
#* http://www.apache.org/licenses/LICENSE-2.0
#* 
#* Se non richiesto dalla legislazione vigente o concordato per iscritto, 
#* il software distribuito nei termini della Licenza è distribuito 
#* "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o 
#* implicite. Consultare la Licenza per il testo specifico che regola le 
#* autorizzazioni e le limitazioni previste dalla medesima.
#****************************************************************************/
#!/bin/bash
results=docs/status.md
tmp=exe/build_programs.errors
tmp2=exe/buld_programs.colors
tmp3=exe/buld_programs.output
targetIndex=1
programIndex=2

rm $results 2>/dev/null
rm $tmp 2>/dev/null
rm $tmp2 2>/dev/null
rm $tmp3 2>/dev/null

function make_program() {
	make program=$1 target=$2 all >/dev/null 2>$tmp
	if [ $? -ne 0 ]; then
		errors=`cat $tmp`
		if [[ $errors == *"exceeds ROM address range"* ]]; then
  			printf "%-15s |" "NO[1]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: pink; }\n" $targetIndex $programIndex >>$tmp2
		elif [[ $errors == *"Main output binary exceeds"* ]]; then
  			printf "%-15s |" "NO[1]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: pink; }\n" $targetIndex $programIndex >>$tmp2
		elif [[ $errors == *"Cannot generate most of the files due to memory area overflow"* ]]; then
  			printf "%-15s |" "NO[1]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: pink; }\n" $targetIndex $programIndex >>$tmp2
		elif [[ $errors == *"No rule to make target"* ]]; then
			printf "%-15s |" "NO[4]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: yellow; }\n" $targetIndex $programIndex >>$tmp2
		elif [[ $errors == *"MR_TILESET_1"* ]]; then
			printf "%-15s |" "NO[2]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: cyan; }\n" $targetIndex $programIndex >>$tmp2
		elif [[ $errors == *"Error 1" ]]; then
			printf "%-15s |" "NO[3]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: orange; }\n" $targetIndex $programIndex >>$tmp2
		else 
			printf "%-15s |" "NO[3]" >>$tmp3;
			printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: orange; }\n" $targetIndex $programIndex >>$tmp2
		fi
	else
		printf "%-15s |" "YES" >>$tmp3;
		printf ".statusTable tr:nth-child(%d) > td:nth-child(%d) { background-color: lightgreen; }\n" $targetIndex $programIndex >>$tmp2
	fi;
}

targets=`cat docs/targets.md | egrep '\- .[a-z]+.' | awk -- 'BEGIN { FS=" "; } { print $2 }' | awk -- 'BEGIN { FS="\`"; } { print $2; }'`;
programs=`cat docs/programs.md | egrep '\- .[a-z]+.' | awk -- 'BEGIN { FS=" "; } { print $2 }' | awk -- 'BEGIN { FS="\`"; } { print $2; }'`;

make clean >/dev/null 2>/dev/null

printf "\n" >>$tmp3
printf "%-15s |" "target name" >>$tmp3
for program in $programs; do
	printf "%-15s |" $program >>$tmp3
done;
printf "\n" >> $tmp3

echo -n "----------------|" >>$tmp3
for program in $programs; do
	echo -n "----------------|" >>$tmp3
done;
printf "\n" >> $tmp3

for target in $targets; do
	printf "%-15s |" "**"$target"**" >>$tmp3
	programIndex=2
	for program in $programs; do
		make_program $program $target
		programIndex=$((programIndex+1))
	done;
	targetIndex=$((targetIndex+1))
	printf "\n" >> $tmp3
done;

# cat data/status_prologue1.md >>$results
# cat $tmp2 >>$results
cat data/status_prologue2.md >>$results
cat $tmp3 >>$results
cat data/status_epilogue.md >>$results
