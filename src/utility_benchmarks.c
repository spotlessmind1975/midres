/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *----------------------------------------------------------------------------
 * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
 * (la "Licenza"); è proibito usare questo file se non in conformità alla
 * Licenza. Una copia della Licenza è disponibile all'indirizzo:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Se non richiesto dalla legislazione vigente o concordato per iscritto,
 * il software distribuito nei termini della Licenza è distribuito
 * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
 * implicite. Consultare la Licenza per il testo specifico che regola le
 * autorizzazioni e le limitazioni previste dalla medesima.
 ****************************************************************************/

#define __BENCHMARKS__ 1
#if defined(__BENCHMARKS__)

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

// Needed for MIDRES library call.
#include "midres.h"

#include "rawdata.h"

/****************************************************************************
 ** DEFINES AND VARIABLES SECTION
 ****************************************************************************/

unsigned char* mr_translate_file_user(mr_file _file) {

	switch (_file) {
#ifdef FILE_DATA_DAT
	case FILE_DATA_DAT:
		return "zdata.dat";
#endif
	}

	return 0;

}

#include <stdio.h>

char buffer[4096];
int startJiffies;
int endJiffies;
int size;

// Main program
void utility_benchmarks() {
	
	int times = 0;
	FILE* fh;

	int ratio = 0;
	startJiffies = mr_get_jiffies_int();
	mr_wait(1);
	endJiffies = mr_get_jiffies_int() - startJiffies;

	printf("\n\nRATIO=%d\n\n", endJiffies*16);

	printf("\n\nBENCHMARKS\n");
	printf("==========\n");
	printf("\n\n");
#if MR_SCREEN_WIDTH > 39
	printf("Sequential read from disk drive\n");
	printf("+-----------+---------+-----------+\n");
	printf("|BLKSIZE (B)|TIME (ms)|SPEED (B/s)|\n");
	printf("+-----------+---------+-----------+\n");
#else
	printf("Sequential read\n");
	printf("| (B)  | (ms) |(B/s) |\n");
#endif

	size = 256;
	while (size < 4096) {
		fh = fopen("zdata.dat", "rb");
		times = 0;
		startJiffies = mr_get_jiffies_int();
		while (!feof(fh)) {
			++times;
			startJiffies = mr_get_jiffies_int();
			fread(buffer, size, 1, fh);
			endJiffies = mr_get_jiffies_int() - startJiffies;
			{
				unsigned long effectiveMilliSize = ((unsigned long)1000 * (unsigned long)size);
				unsigned long effectiveMilliTime = (unsigned long)(endJiffies * 16);
				unsigned long ratio = effectiveMilliSize / effectiveMilliTime;
				unsigned int ratioAsInteger = (unsigned int)ratio;
				char sizeBuffer[12]; 
				char timeBuffer[12];
				char speedBuffer[12];

				sprintf(sizeBuffer, "%d", size);
				sprintf(timeBuffer, "%d", endJiffies * 16);
				sprintf(speedBuffer, "%d", ratioAsInteger);

#if MR_SCREEN_WIDTH > 39
				printf("|%11s|%9s|%11s|\n", sizeBuffer, timeBuffer, speedBuffer);
#elif MR_SCREEN_WIDTH > 30
				printf("|%6s|%6s|%6s|\n", sizeBuffer, timeBuffer, speedBuffer);
#else
				printf("|%6s|%6s|%6s|", sizeBuffer, timeBuffer, speedBuffer);
#endif
			}
			if (times == 3) break;
		}
		fclose(fh);
		size <<= 1;
#if MR_SCREEN_WIDTH > 39
		printf("+-----------+---------+-----------+\n");
#else
#endif
	}

	while(1) { }

}

#endif
