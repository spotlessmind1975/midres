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
#include <string.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_MEMORY)

void mr_msc1_uncompress_memory(unsigned char* _destination, unsigned char* _source) {

	unsigned char token, count, tmp;
	unsigned int offset;
	unsigned char* start = _destination;
	unsigned char* source = _source;

	do {
		token = *source;
		++source;
		if (token == 0) {

		}
		else if ((token & 0x80) == 0x00) {
			count = token & 0x7f;
			memcpy(_destination, source, count);
			_destination += count;
			source += count;
		}
		else if ((token & 0x80) == 0x80) {
			count = (token & 0x7f) >> 2;
			tmp = *source;
			++source;
			offset = tmp | ((token & 0x03) << 8);
			if (count == 0) count = 32;
			mr_memfill4(_destination, start + offset, count);
			_destination += (count << 2);
		}
	} while (token);

}

#endif
