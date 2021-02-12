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

#ifdef __TUTORIAL_PROTOTHREAD1__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

  // Needed for system call.
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Needed for MIDRES library call.
#include "midres.h"

mr_protothread firstThread;
mr_protothread secondThread;

MR_PT_THREAD(first) {

	MR_PTI_BEGIN();

	while (1) {

		printf("Hello world 1\n");

		MR_PTI_YIELD();

	}

	MR_PTI_END();
}

MR_PT_THREAD(second) {

	MR_PTI_BEGIN();

	while (1) {

		printf("Hello world 2\n");

		MR_PTI_YIELD();

	}

	MR_PTI_END();
}

void tutorial_protothread1() {

	while (1) {
		first(&firstThread);
		second(&secondThread);
	}

}

#endif
