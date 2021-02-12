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

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

#if defined(MIDRES_STANDALONE_MUSIC)

void mr_sound_control_channel(mr_channel _channel, mr_audio_parameter _parameter, mr_audio_value _value) {

	switch (_parameter) {
		/*
		Controller number zero (with 32 as the LSB) is defined as the bank select. 
		The bank select function is used in some synthesizers in conjunction with the 
		MIDI Program Change message to expand the number of different instrument 
		sounds which may be specified (the Program Change message alone allows 
		selection of one of 128 possible program numbers). The additional sounds 
		are selected by preceding the Program Change message with a Control 
		Change message which specifies a new value for Controller zero and 
		Controller 32, allowing 16,384 banks of 128 sound each.

		Since the MIDI specification does not describe the manner in which a synthesizer's 
		banks are to be mapped to Bank Select messages, there is no standard way for a Bank 
		Select message to select a specific synthesizer bank. Some manufacturers, such as 
		Roland (with "GS") and Yamaha (with "XG") , have adopted their own practices to 
		assure some standardization within their own product lines.
		*/
		case IMF_PARAMETER_BANK_SELECT_MSB_GS:
		case IMF_PARAMETER_MODULATION_WHEEL_MSB:
		case IMF_PARAMETER_BREATH:
		case IMF_PARAMETER_FOOT:
		case IMF_PARAMETER_PORTAMENTO_TIME:
		case IMF_PARAMETER_DATA_ENTRY_MSB:
		case IMF_PARAMETER_VOLUME:
		case IMF_PARAMETER_BALANCE:
		case IMF_PARAMETER_PAN:
		case IMF_PARAMETER_EXPRESSION:
		case IMF_PARAMETER_EFFECT_CONTROL1:
		case IMF_PARAMETER_EFFECT_CONTROL2:
		case IMF_PARAMETER_GENERAL_PURPOSE1:
		case IMF_PARAMETER_GENERAL_PURPOSE2:
		case IMF_PARAMETER_GENERAL_PURPOSE3:
		case IMF_PARAMETER_GENERAL_PURPOSE4:
		case IMF_PARAMETER_BANK_SELECT_LSB_GS:
		case IMF_PARAMETER_MODULATION_WHEEL_LSB:
		case IMF_PARAMETER_BREATH_GS:
		case IMF_PARAMETER_FOOT_LSB:
		case IMF_PARAMETER_PORTAMENTO_TIME_LSB:
		case IMF_PARAMETER_DATA_ENTRY_LSB:
		case IMF_PARAMETER_VOLUME_LSB:
		case IMF_PARAMETER_BALANCE_LSB:
		case IMF_PARAMETER_PAN_LSB:
		case IMF_PARAMETER_EXPRESSION_LSB:
		case IMF_PARAMETER_EFFECT_CONTROL1_GS:
		case IMF_PARAMETER_EFFECT_CONTROL2_GS:
		case IMF_PARAMETER_EFFECT_CONTROL1_LSB:
		case IMF_PARAMETER_EFFECT_CONTROL2_LSB:
		case IMF_PARAMETER_EFFECT_CONTROL3_LSB:
		case IMF_PARAMETER_EFFECT_CONTROL4_LSB:
		case IMF_PARAMETER_HOLD_PEDAL1:
		case IMF_PARAMETER_PORTAMENTO_GS:
		case IMF_PARAMETER_SOSTENUTO_GS:
		case IMF_PARAMETER_SOFT_PEDAL_GS:
		case IMF_PARAMETER_LEGATO_PEDAL:
		case IMF_PARAMETER_HOLD_PEDAL:
		case IMF_PARAMETER_SOUND_VARIATION:
		case IMF_PARAMETER_SOUND_TIMBRE:
		case IMF_PARAMETER_RELEASE_TIME:
		case IMF_PARAMETER_ATTACK_TIME:
		case IMF_PARAMETER_SOUND_BRIGHTNESS:
		case IMF_PARAMETER_SOUND_CONTROL6:
		case IMF_PARAMETER_SOUND_CONTROL7:
		case IMF_PARAMETER_SOUND_CONTROL8:
		case IMF_PARAMETER_SOUND_CONTROL9:
		case IMF_PARAMETER_SOUND_CONTROL10:
		case IMF_PARAMETER_GENERAL_PURPOSE5:
		case IMF_PARAMETER_GENERAL_PURPOSE6:
		case IMF_PARAMETER_GENERAL_PURPOSE7:
		case IMF_PARAMETER_GENERAL_PURPOSE8:
		case IMF_PARAMETER_PORTAMENTO_CONTROL_GS:
		case IMF_PARAMETER_REVERB_LEVEL_GS:
		case IMF_PARAMETER_TREMOLO_DEPTH:
		case IMF_PARAMETER_CHORUS_LEVEL_GS:
		case IMF_PARAMETER_CELESTE_DEPTH:
		case IMF_PARAMETER_PHASER_DEPTH:
		case IMF_PARAMETER_DATA_INCREMENT:
		case IMF_PARAMETER_DATA_DECREMENT:
		case IMF_PARAMETER_NRPN_PARAMETER_LSB:
		case IMF_PARAMETER_NRPN_PARAMETER_MSB:
		case IMF_PARAMETER_RPN_PARAMETER_LSB:
		case IMF_PARAMETER_RPN_PARAMETER_MSB:
		case IMF_PARAMETER_ALL_SOUND_OFF_GS:
		case IMF_PARAMETER_RESET_ALL_CONTROLLERS:
		case IMF_PARAMETER_LOCAL_ON_OFF:
		case IMF_PARAMETER_ALL_NOTES_OFF:
		case IMF_PARAMETER_OMNI_MODE_OFF:
		case IMF_PARAMETER_OMNI_MODE_ON:
		case IMF_PARAMETER_MONO_MODE_ON:
		case IMF_PARAMETER_POLY_MODE_ON:
			break;
		}

}

#endif
