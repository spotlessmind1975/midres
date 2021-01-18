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

#if defined(__IMFPLAYER__) || defined(__AIRATTACK_INTRO__) || defined(__AIRATTACK_ENDING__) || defined(__TOTTO_INTRO__) || defined(__TOTTO_ENDING__) || defined(__ELEVATOR_ENDING__) 

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
#ifdef FILE_ZAMUSIC_IMF
	case FILE_ZAMUSIC_IMF:
		return "zamusic.imf";
#endif
#ifdef FILE_ZAMUSIC2_IMF
	case FILE_ZAMUSIC2_IMF:
		return "zamusic2.imf";
#endif
#ifdef FILE_TOTTO_IMF
	case FILE_TOTTO_IMF:
		return "totto.imf";
#endif
#ifdef FILE_TOTTO3_IMF
	case FILE_TOTTO3_IMF:
		return "totto3.imf";
#endif
#ifdef FILE_ELEVATOR1_IMF
	case FILE_ELEVATOR1_IMF:
		return "elevator1.imf";
#endif
	}

	return 0;

}

mr_musicplayer_protothread musicPlayer;

#include <stdio.h>

// Main program
void utility_imfplayer() {
	
	musicPlayer.done = mr_false;
	musicPlayer.auto_restart = mr_true;

#ifdef FILE_ZAMUSIC_IMF
	musicPlayer.buffer = mr_map_file(FILE_ZAMUSIC_IMF, FILE_ZAMUSIC_IMF_SIZE);
	musicPlayer.eof = musicPlayer.buffer + FILE_ZAMUSIC_IMF_SIZE;
#endif

#ifdef FILE_ZAMUSIC2_IMF
	musicPlayer.buffer = mr_map_file(FILE_ZAMUSIC2_IMF, FILE_ZAMUSIC2_IMF_SIZE);
	musicPlayer.eof = musicPlayer.buffer + FILE_ZAMUSIC2_IMF_SIZE;
#endif

#ifdef FILE_TOTTO_IMF
	musicPlayer.buffer = mr_map_file(FILE_TOTTO_IMF, FILE_TOTTO_IMF_SIZE);
	musicPlayer.eof = musicPlayer.buffer + FILE_TOTTO_IMF_SIZE;
#endif

#ifdef FILE_TOTTO3_IMF
	musicPlayer.buffer = mr_map_file(FILE_TOTTO3_IMF, FILE_TOTTO3_IMF_SIZE);
	musicPlayer.eof = musicPlayer.buffer + FILE_TOTTO3_IMF_SIZE;
#endif

	musicPlayer.auto_restart_buffer = musicPlayer.buffer;

	while (1) {
		mr_musicplayer(&musicPlayer);
	}

}

#endif
