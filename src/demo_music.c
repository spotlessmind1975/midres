/*****************************************************************************
 * MIDRES Library - an isomorphic gamelib for retrocomputers                 *
 *****************************************************************************
 * Copyright 2020-2021 Marco Spedaletti (asimov@mclink.it)
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

  // Needed for system call.
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

  // Needed for MIDRES library call.
#include "midres.h"

#include "rawdata.h"

/****************************************************************************
 ** DEFINES AND VARIABLES SECTION
 ****************************************************************************/

unsigned char* mr_translate_file_user(mr_file _file) {

	switch (_file) {
		case FILE_TETRIS_IMF:
			return "tetris.imf";
	}

	return 0;

}


// Main program
void demo_music() {

	unsigned char i;

	mr_boolean backgroundColor = mr_false;

	mr_musicplayer_protothread mpPt;

	mr_init();

	MR_RENDERED_MIXELS[0] = 32;

	mr_start_frame();
	mr_clearv();
	mr_end_frame(0);

	mr_set_border_color(MR_COLOR_WHITE);
	mr_set_background_color(MR_COLOR_WHITE);

	mpPt.done = mr_false;
	mpPt.buffer = mr_map_file(FILE_TETRIS_IMF, FILE_TETRIS_IMF_SIZE);
	mpPt.eof = mpPt.buffer + FILE_TETRIS_IMF_SIZE;

	while (!mpPt.done) {
		mr_musicplayer(&mpPt);
		if (backgroundColor) {
			mr_set_border_color(MR_COLOR_RED);
			mr_set_background_color(MR_COLOR_RED);
			backgroundColor = mr_false;
		} else {
			mr_set_border_color(MR_COLOR_YELLOW);
			mr_set_background_color(MR_COLOR_YELLOW);
			backgroundColor = mr_true;
		}
	}

	mr_set_border_color(MR_COLOR_BLACK);
	mr_set_background_color(MR_COLOR_BLACK);

	while(1) { }

}

#endif
