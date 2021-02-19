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

#ifdef __DEMO_MSC1__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "main.h"
#include "midres.h"
#include "rawdata.h"

/****************************************************************************
** RESIDENT VARIABLES SECTION
****************************************************************************/

unsigned char* mr_translate_file_user(mr_file _file) {

	switch (_file) {
	case FILE_ZZINTRO_MS1:
		return "zzintro.ms1";
	}

	return 0;

}

void demo_msc1() {

	// Initialize the graphical subsystem.
	mr_init();

	// Clear all involved screens.
	mr_clearv();

#ifdef __ATARI__

	mr_start_frame();

	mr_msc1_uncompress_memory(MR_SM(MR_SCREEN_DEFAULT), mr_map_file(FILE_ZZINTRO_MS1, FILE_ZZINTRO_MS1_SIZE));

	mr_end_frame(0);

	mr_wait(2);

#else

	mr_msc1_uncompress_memory(MR_SM(MR_SCREEN_1)-2, mr_map_file(FILE_ZZINTRO_MS1, FILE_ZZINTRO_MS1_SIZE));
	// Load compressed screen on the auxiliary space

	// Show titles.
	mr_uncompressv(MR_SCREEN_1);

	mr_end_frame(0);

#endif

	while (1) {

	}
}

#endif
