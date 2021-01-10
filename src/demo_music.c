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

#ifdef __DEMO_MUSIC__

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
		case FILE_TOCCATINA_IMF:
			return "toccatina.imf";
		case FILE_ALICE_IMF:
			return "alice.imf";
		case FILE_ISLAND_IMF:
			return "island.imf";
	}

	return 0;

}

// Main program
void demo_music() {

	mr_musicplayer_protothread mpPt;

	mr_init();

	mpPt.done = mr_false;
	mpPt.buffer = mr_map_file(FILE_TOCCATINA_IMF, FILE_TOCCATINA_IMF_SIZE);
	mpPt.eof = mpPt.buffer + FILE_TOCCATINA_IMF_SIZE;

	while (!mpPt.done) {
		mr_musicplayer(&mpPt);
	}

	while(1) { }

}

#endif
