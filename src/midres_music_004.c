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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_MUSIC)

MR_PT_THREAD_EXT(mr_musicplayer, mr_musicplayer_protothread) {

	MR_PTI_BEGIN();

	if (MR_PTI_CTX()->auto_restart_buffer == NULL) {
		MR_PTI_CTX()->auto_restart_buffer = MR_PTI_CTX()->buffer;
	}

	mr_sound_init();

	if (MR_PTI_CTX()->buffer != NULL && ( MR_PTI_CTX()->buffer < MR_PTI_CTX()->eof ) ) {

		while (MR_PTI_CTX()->buffer < MR_PTI_CTX()->eof) {

			MR_PTI_CTX()->last_opcode = *(MR_PTI_CTX()->buffer);

			++MR_PTI_CTX()->buffer;

			if (MR_PTI_CTX()->last_opcode == IMF_TOKEN_WAIT1) {
				MR_PTI_CTX()->jiffies = *(MR_PTI_CTX()->buffer);
				MR_PTI_CTX()->buffer += 1;
				//printf("W1 (%d) at %x", MR_PTI_CTX()->jiffies, MR_PTI_CTX()->buffer);
				MR_PTI_CTX()->old_jiffies = mr_get_jiffies_int();
				MR_PTI_WAIT_WHILE((mr_get_jiffies_int() - MR_PTI_CTX()->old_jiffies) < MR_PTI_CTX()->jiffies);
			}
			else if (MR_PTI_CTX()->last_opcode == IMF_TOKEN_WAIT2) {
				MR_PTI_CTX()->jiffies = *(MR_PTI_CTX()->buffer) | (*(MR_PTI_CTX()->buffer + 1) << 8);
				MR_PTI_CTX()->buffer += 2;
				//printf("W2 (%d)", MR_PTI_CTX()->jiffies);
				MR_PTI_CTX()->old_jiffies = mr_get_jiffies_int();
				MR_PTI_WAIT_WHILE((mr_get_jiffies_int() - MR_PTI_CTX()->old_jiffies) < MR_PTI_CTX()->jiffies);
			}
			else if ((MR_PTI_CTX()->last_opcode & IMF_TOKEN_CONTROL) == IMF_TOKEN_CONTROL) {
				// Controller
				mr_sound_control_channel((MR_PTI_CTX()->last_opcode & 0x0f), *(MR_PTI_CTX()->buffer), *(MR_PTI_CTX()->buffer + 1));
				MR_PTI_CTX()->buffer += 2;
				//printf("CC");
			}
			else if ((MR_PTI_CTX()->last_opcode & IMF_TOKEN_PROGRAM_CHANGE) == IMF_TOKEN_PROGRAM_CHANGE) {
				mr_sound_program_change_channel((MR_PTI_CTX()->last_opcode & 0x0f), *(MR_PTI_CTX()->buffer));
				++MR_PTI_CTX()->buffer;
				//printf("PP");
			}
			else if ((MR_PTI_CTX()->last_opcode & IMF_TOKEN_NOTE) == IMF_TOKEN_NOTE) {
				// Note On
				//printf("NN");
				mr_sound_note_on_channel(MR_PTI_CTX()->last_opcode & 0x0f, *(MR_PTI_CTX()->buffer), (MR_PTI_CTX()->last_opcode & 0x70) >> 4);
				++MR_PTI_CTX()->buffer;
			}
			else {
				mr_sound_stop_channel(MR_PTI_CTX()->last_opcode & 0x0f);
				//printf("[%x]",MR_PTI_CTX()->last_opcode );
			}

			MR_PTI_YIELD();

		}

		mr_sound_init();

		if (MR_PTI_CTX()->auto_restart) {
			MR_PTI_CTX()->done = mr_false;
			MR_PTI_CTX()->buffer = MR_PTI_CTX()->auto_restart_buffer;
		} else {
			MR_PTI_CTX()->done = mr_true;
			MR_PTI_CTX()->buffer = NULL;
			MR_PTI_CTX()->eof = NULL;
		}
		
	}

	MR_PTI_END();

}

#endif
