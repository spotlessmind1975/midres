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

#ifdef __ELEVATOR__

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

// Needed for system call.
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Needed for MIDRES library call.
#include "midres.h"

// Tiles used in the game.
#include "game_elevator_tiles.h"

#include "rawdata.h"

/****************************************************************************
 ** DEFINES, TYPEDEFS AND VARIABLES SECTION
 ****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
/// TYPEDEFS
/////////////////////////////////////////////////////////////////////////////

// This type can be used to denote a counter, i.e. a number that starts from
// a value and it will decrease by one up to zero value or increased up to
// a (given) final value.
typedef signed char counter;

// This type can be used to denote at which position of sliding doors
// the elevator is currently: 0 = totally opened, 4 = totally closed.
typedef signed char	door;

typedef unsigned char floor;

// This type can be used to denote bitmap or bitmasks.
typedef unsigned char bitmap;

// This structure is used to store protothreads informations, along with
// the number of the passengers this protothread belongs to.
typedef struct {
	unsigned int lc;
	unsigned char index;
} passenger_protothread;

// This structure is used to store protothreads informations, along with
// the number of the passengers this protothread belongs to.
typedef struct {
	unsigned int lc;
	unsigned char index;
} walk_protothread;

/////////////////////////////////////////////////////////////////////////////
/// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

// Total number of floors
#if(MR_SCREEN_HEIGHT>23)
#define	FLOOR_COUNT						5
#else
#define	FLOOR_COUNT						4
#endif

#ifdef __ATARI__
#define OFFSET 1
#else
#define OFFSET 0
#endif

// Height of a floor, in tiles. We reserve 2 tiles for status lines (up&down).
#define	FLOOR_HEIGHT					( ( MR_SCREEN_HEIGHT - 2 ) / 5 )

// Center position for elevators:
#define	ELEVATOR_X						( (MR_SCREEN_WIDTH - TILE_BASEX_WIDTH ) >> 1 )

// Elevator's height.
#define	ELEVATOR_HEIGHT					( FLOOR_HEIGHT )

// Starting position of CONTAINER and size
#define	CONTAINER_WIDTH					( MR_SCREEN_WIDTH )
#define	CONTAINER_HEIGHT				( FLOOR_HEIGHT - OFFSET )
#define	CONTAINER_X						( 0 )
#define	CONTAINER_Y						( MR_SCREEN_HEIGHT - CONTAINER_HEIGHT )

// Starting position of PANEL and size
#define	PANEL_WIDTH						( FLOOR_COUNT + 8  )
#define	PANEL_HEIGHT					( FLOOR_HEIGHT )
#define	PANEL_X							( ELEVATOR_X - (PANEL_WIDTH>>1) )
#define	PANEL_Y							( MR_SCREEN_HEIGHT - PANEL_HEIGHT )

// This mode is a pure simulation (w/internal panel).
#define	GAME_MODE_SIMULATION			0

// This mode is a one player game (w/out internal panel).
#define	GAME_MODE_SINGLE				1

// This mode is a cooperative game.
#define	GAME_MODE_COOPERATIVE			2

// Filename with the intro screen.
#define FILENAME_INTRO_MPIC				"zelintro.bin"
#define FILENAME_TILES_BIN				"zeltiles.bin"

// Maximum number of passengers allowed on a single screen.
#define PASSENGER_COUNT					10

// Maximum number of passengers in the elevator.
#define PASSENGER_IN_ELEVATOR_COUNT		4

// Number of frames for passenger's animation.
#define	WALK_FRAME_COUNT				6

// Duration of alarm signal.
#define	ALARM_ACTIVE_COUNT				60

#if(MR_SCREEN_WIDTH>25)
	// Maximum time to play the game.
	#define TIME_REMAIN_MAX					160

	// Duration of a single time remain's tick.
	#define TIME_REMAIN_COUNT				10

	// Duration of a single latency's tick.
	#define COOPERATIVE_LATENCY_COUNT		1

	// Duration of the latency, in cooperative game.
	#define COOPERATIVE_LATENCY_MAX			32

	#define COLOR_BACKGROUND				MR_COLOR_LIGHT_GREY

#else
	// Maximum time to play the game.
	#define TIME_REMAIN_MAX					32

	// Duration of a single time remain's tick.
	#define TIME_REMAIN_COUNT				60

	// Duration of a single latency's tick.
	#define COOPERATIVE_LATENCY_COUNT		4

	// Duration of the latency, in cooperative game.
	#define COOPERATIVE_LATENCY_MAX			8

	#define COLOR_BACKGROUND				MR_COLOR_BLACK

#endif

/////////////////////////////////////////////////////////////////////////////
/// GAME'S VARIABLES
/////////////////////////////////////////////////////////////////////////////

// Current game mode.
unsigned char gameMode = GAME_MODE_SIMULATION;

// The first counter is used to store the "ticks". When zero is reached,
// we decrease effectively the cooperative latency time, that is the
// second variable. In other words, the first variable is a "demultiplexer"
// for the second variable.
counter cooperativeLatencyCount = COOPERATIVE_LATENCY_COUNT;
counter cooperativeLatency = 0;

// This flag is used if the elevator's player (true) 
// or the passengers' player (false) is selected.
mr_boolean cooperativePlayer = mr_true;

// Those variables are needed to store contextual informations
// for each thread defined in the game.
mr_protothread inputThread;

// The first counter is used to store the "ticks". When zero is reached,
// we decrease effectively the remaining time, that is the
// second variable. In other words, the first variable is a "demultiplexer"
// for the second variable.
unsigned char timeRemainCount = TIME_REMAIN_COUNT;
unsigned char timeRemain = TIME_REMAIN_MAX;

// Temporary space to store an addendum.
unsigned char addendum[4] = { 0, 0, 0, 0 };

// Store the current score.
mr_position score[4] = { 0, 0, 0, 0 };

// Store the current key pressed.
unsigned char currentKey = 0;

/////////////////////////////////////////////////////////////////////////////
/// ELEVATOR'S VARIABLES
/////////////////////////////////////////////////////////////////////////////

// This semaphore is used to mutually exclude the access
// to the lift, i.e. when a passengers is entering into
// the elevator or the elevator is moving.
mr_semaphore liftSemaphore;

// This is the current elevator's floor.
floor actualFloor;

// The first variable is the elevator's target floor, as requested by
// the various thread; the second variable, instead, is the effective
// target floor, i.e. the floor where the elevator have to reach.
floor targetFloor;
floor effectiveTargetFloor;

// Flag to store if the elevator's doors are opening (false) or closing (true).
mr_boolean closeDoors = mr_false;

// Flag to store if the elevator is moving up (true) or down (false).
mr_boolean directionUp = mr_true;

// Flag to store if the elevator has been halted.
mr_boolean elevatorHalted = mr_false;

// The current number of passengers in the elevator.
mr_position passengersInElevator = 0;

// Which floor has been reserved on "up" direction?
// (bit 0 = first floor, bit 1 = second floor, and so on)
bitmap targetFloorsUp = 0;

// Which floor has been reserved on "down" direction?
// (bit 0 = first floor, bit 1 = second floor, and so on)
bitmap targetFloorsDown = 0;

// The current frame index for the doors' animation.
door doorFrame = 0;

// The current step on moving the elevator between floors.
mr_position motorStep = 0;

// Those variables are needed to store contextual informations
// for each thread defined in the game.
mr_protothread liftThread;
mr_protothread doormoveThread;
mr_protothread elevatormoveThread;

// Used to wait the arrive of passengers before closing the doors.
mr_twait liftWait;

// Counter used to signal if an ALARM is active.
counter alarmActiveCount = 0;

/////////////////////////////////////////////////////////////////////////////
/// PASSENGERS' VARIABLES
/////////////////////////////////////////////////////////////////////////////

// This variable is used to store how much time the passenger
// has waited for the elevator.
counter passengerWait[PASSENGER_COUNT];

// Those variables are needed to store contextual informations
// for each thread defined in the game.
passenger_protothread passengerThread[PASSENGER_COUNT];
walk_protothread walkThread[PASSENGER_COUNT];

// This array store the actual floor for each passenger.
floor passengerFloor[PASSENGER_COUNT];

// This array store the desired floor for each passenger.
floor desiredFloor[PASSENGER_COUNT];

// This array stores if the passenger has entered inside the elevator.
mr_boolean passengerInElevator[PASSENGER_COUNT];

// This array stores the current horizontal position for each passenger.
mr_position walkPos[PASSENGER_COUNT];

// This array stores the desired horizontal position, 
// if a "walk" action is running.
mr_position walkMoveTo[PASSENGER_COUNT];

// This array stores the current animation's frame for each passenger.
mr_position walkFrame[PASSENGER_COUNT];

/****************************************************************************
 ** FUNCTIONS
 ****************************************************************************/

unsigned char* mr_translate_file_user(mr_file _file) {

	switch (_file) {
		case FILE_ZELINTRO_BIN:
			return "zelintro.bin";
		case FILE_ZELTILES_BIN:
			return "zeltile.bin";
	}

	return 0;

}

/////////////////////////////////////////////////////////////////////////////
/// AUXILIARY FUNCTIONS
/////////////////////////////////////////////////////////////////////////////

// This function can be used to generate a random floor. If the extracted
// floor matches the given one, the next random floor will be picked.
floor calculate_random_floor(floor _exclude) {

	// We took a random number between 0 and 15.
	floor randomFloor = rand() & 0x0f;

	// While the number is greater than the 
	// maximum number of floors...
	while (randomFloor >= (FLOOR_COUNT)) {
		// ... decrease it.
		randomFloor -= FLOOR_COUNT;
	}

	// If the extracted number has to be excluded
	// then take the next one.
	if (randomFloor == _exclude) {
		++randomFloor;
	}

	// We must assure that the floor is valid.
	if (randomFloor >= (FLOOR_COUNT)) {
		randomFloor -= FLOOR_COUNT;
	}

	return randomFloor;

}

// This function will add an addendum to a variable.
void add(unsigned char* _points, unsigned char* _value) {
	unsigned char i;
	for (i = 0; i < 4; ++i) {
		unsigned char s = _points[i] + _value[i];
		if (s > 9) {
			s -= 10;
			_points[i] = s;
			if (i < 3) {
				++_points[i + 1];
			}
		}
		else {
			_points[i] = s;
		}
	}
}

// This function will manage the alarm.
void manage_alarm() {

	// If the alarm has been fired, we initialize 
	// the sound subsystem.
	if (alarmActiveCount == ALARM_ACTIVE_COUNT) {

		mr_sound_start_channel(0, 1000);
		mr_sound_change_channel(0, 1000);
		--alarmActiveCount;

		// If the alarm is going to end, we stop
		// the sound subsystem.
	}
	else if (alarmActiveCount == 1) {

		mr_sound_stop_channel(0);
		--alarmActiveCount;

		// On all other cases, we are going to change
		// rapidly the sound, in order to emulate
		// an "alarm".
	}
	else if (alarmActiveCount > 0) {

		mr_sound_change_channel(0, (alarmActiveCount & 1) ? 2000 : 1500);
		--alarmActiveCount;

	}

}

// This function is needed to process a request for reservation 
// of a floor by external buttons. It manages the internal 
// bitmap representation for the specific direction queue. It 
// restart the elevator if has been halted previously,
void process_request_floor_up(floor _current) {

	targetFloorsUp |= (1 << _current);
	if (gameMode == GAME_MODE_COOPERATIVE) {
		if (cooperativePlayer) {
			cooperativePlayer = mr_false;
			cooperativeLatency = COOPERATIVE_LATENCY_MAX;
		}
	}

}

// This function is needed to process a request for reservation 
// of a floor by external buttons. It manages the internal 
// bitmap representation for the specific direction queue. It 
// restart the elevator if has been halted previously,
void process_request_floor_down(floor _current) {

	targetFloorsDown |= (1 << _current);
	if (gameMode == GAME_MODE_COOPERATIVE) {
		if (cooperativePlayer) {
			cooperativePlayer = mr_false;
			cooperativeLatency = COOPERATIVE_LATENCY_MAX;
		}
	}

}

// This function is needed to process a request for reservation 
// of a floor by external buttons. It manages the internal 
// bitmap representation for the specific direction queue. It 
// restart the elevator if has been halted previously,
void process_request_floor(floor _target) {

	if (actualFloor > _target) {
		process_request_floor_down(_target);
	}
	else if (actualFloor < _target) {
		process_request_floor_up(_target);
	}

}

// This function is needed to process a request for reservation 
// of a floor. It manages the internal bitmap representation and,
// moreover, it restart the elevator if has been halted previously,
void process_request_floor_reservation(floor _floor) {

	if (_floor < FLOOR_COUNT) {
		process_request_floor(_floor);

		// Restart the movement of the elevator.
		elevatorHalted = mr_false;
	}

}

mr_boolean passenger_can_walk(int _number) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - passengerFloor[_number]);

	return (mr_gettilev(walkPos[_number] + TILE_WALK1_WIDTH, (realFloor * FLOOR_HEIGHT) - 1) < TILE_WALK1);

}

void add_passenger_on_floor(floor _floor) {

	unsigned char i;

	if (_floor < FLOOR_COUNT) {
		for (i = 0; i < PASSENGER_COUNT; ++i) {
			if (desiredFloor[i] == 0xff) {
				passengerFloor[i] = _floor;
				desiredFloor[i] = calculate_random_floor(_floor);
				if (!passenger_can_walk(i)) {
					passengerFloor[i] = 0;
					desiredFloor[i] = 0xff;
				}
				else {
					if (!cooperativePlayer) {
						cooperativePlayer = mr_true;
						cooperativeLatency = COOPERATIVE_LATENCY_MAX;
					}
				}
				break;
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
/// GRAPHIC FUNCTIONS
/////////////////////////////////////////////////////////////////////////////

// This routine prepares the graphics needed for the game. It loads the 
// resources and it copy them from the mass memory (or from the embedded 
// memory) to the designated zones.
void prepare_graphics() {

	// Load tiles from mass memory.
	mr_tileset_load_file(FILE_ZELTILES_BIN, MR_TILESET_0, TILE_START, TILE_COUNT);

	// This little "trick" allows us to quickly clean the screen 
	// using the prepared empty tile.
	MR_RENDERED_MIXELS[0] = TILE_EMPTY;

	mr_set_background_color(MR_COLOR_LIGHT_GREY);

	mr_clearv();

	// Enable the custom charset.
	mr_tileset_visible(MR_TILESET_0);

}

// This routine will fillup the background of the entire container,
// by using the given tile.
void draw_background_panel(mr_tile _filler) {
	mr_putftilesv(CONTAINER_X, CONTAINER_Y, _filler, CONTAINER_WIDTH, CONTAINER_HEIGHT, MR_COLOR_BLACK);
}

// This function will draw the elevator's panel, with a synthetic representation
// of its internal variables.
void draw_elevator_panel() {

	unsigned char i;

	// First of all, let's draw the current direction.
	mr_puttilev(PANEL_X, PANEL_Y + 1, TILE_UP, directionUp ? MR_COLOR_GREEN : MR_COLOR_BLACK);
	mr_puttilev(PANEL_X, PANEL_Y + 2, TILE_DOWN, !directionUp ? MR_COLOR_GREEN : MR_COLOR_BLACK);

	// Draw the current floor where the elevator is, currently.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 1, PANEL_Y + 1, TILE_DIGIT1 + actualFloor, MR_COLOR_BLACK);

	// Now we are going to update the directions on each floor.
	for (i = 0; i < FLOOR_COUNT; ++i) {
		// We must check if the floor has been request on "up" or "down" direction.
		// This is represented using a single bit on targetFloorsUp or targetFloorsDown, respectively.
		// So we calculate a single bitmask to check against.
		bitmap bitmask = (1 << i);

		// The floor has been requested on "up" direction?
		if ((targetFloorsUp & bitmask) == bitmask) {
			// Draw its number on the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 1, TILE_DIGIT1 + i, MR_COLOR_BLACK);
		}
		else {
			// Remove its number from the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 1, TILE_EMPTY, MR_COLOR_BLACK);
		}

		// The floor has been requested on "down" direction?
		if ((targetFloorsDown & bitmask) == bitmask) {
			// Draw its number on the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 2, TILE_DIGIT1 + i, MR_COLOR_BLACK);
		}
		else {
			// Remove its number from the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 2, TILE_EMPTY, MR_COLOR_BLACK);
		}
	}

	// Draw a red light if the elevator has been halted by pressing
	// the HALT button, else it is grey.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 3, PANEL_Y + 1, TILE_LED, elevatorHalted ? MR_COLOR_RED : MR_COLOR_GREY);

	// Draw the actual position of the sliding door.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 5, PANEL_Y + 1, TILE_DIGIT1 + doorFrame, MR_COLOR_GREY);

	// Draw the number of passengers currently inside the elevator.
	if (passengersInElevator >= 3) {
		mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 7, PANEL_Y + 1, TILE_DIGIT0 + passengersInElevator, MR_COLOR_RED);
	} else if (passengersInElevator > 0) {
		mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 7, PANEL_Y + 1, TILE_DIGIT0 + passengersInElevator, MR_COLOR_GREEN);
	}
	else {
		mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 7, PANEL_Y + 1, TILE_DIGIT0 + passengersInElevator, MR_COLOR_GREY);
	}

	// Draw the floor where the elevator will go.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 1, PANEL_Y + 2, TILE_DIGIT1 + targetFloor, (targetFloor == actualFloor) ? MR_COLOR_GREY : MR_COLOR_GREEN);

	// Draw a red ringbell if the ALARM has been activated.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 5, PANEL_Y + 2, TILE_ALARM, alarmActiveCount ? MR_COLOR_RED : MR_COLOR_GREY);

}

// This function will draw the internal part of the elevator, 
// in terms of sliding doors.
void draw_elevator(floor _floor, door _door) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// We have to choose the correct tile, in base to the sliding door
	// step (_door). Currently, _door = 0 means "opened door" (TILE_BASE for
	// the base and TILE_DOOR for the interior) while _door=4 means "closed door"
	// (TILE_BASE5 for the base and TILE_DOOR5 for the interior). The values
	// between are decoded as follows.
	unsigned char tile_door = TILE_DOOR, tile_base = TILE_BASE, i;

	switch (_door) {
	case 0:
		tile_base = TILE_BASE;
		tile_door = TILE_DOOR;
		break;
	case 1:
		tile_base = TILE_BASE2;
		tile_door = TILE_DOOR2;
		break;
	case 2:
		tile_base = TILE_BASE3;
		tile_door = TILE_DOOR3;
		break;
	case 3:
		tile_base = TILE_BASE4;
		tile_door = TILE_DOOR4;
		break;
	case 4:
		tile_base = TILE_BASE5;
		tile_door = TILE_DOOR5;
		break;
	}

	// Now we draw the interior part of the elevator, starting from the base..
	mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT, tile_base, TILE_BASE_WIDTH, TILE_BASE_HEIGHT, MR_COLOR_BLACK);
	// ... and going up to the final part of the elevator, that is the line
	// BEFORE the top, that is not drawn anymore.
	for (i = 0; i < (ELEVATOR_HEIGHT - 2); ++i) {
		mr_putetilesv(ELEVATOR_X + 1, (realFloor * FLOOR_HEIGHT) - i - 1, tile_door, TILE_DOOR_WIDTH, TILE_DOOR_HEIGHT, MR_COLOR_BLACK);
	}
}

// This function will draw the entire elevator, 
// in terms of sliding doors, borders and floor number.
void draw_elevator_complete(floor _floor, door _door) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// Let's draw the base (with angles).
	mr_putetilesv(ELEVATOR_X, realFloor * FLOOR_HEIGHT, TILE_BASEX, TILE_BASEX_WIDTH, TILE_BASEX_HEIGHT, MR_COLOR_BLACK);

	// Let's draw the left jamb.
	mr_vtilesv(ELEVATOR_X, (realFloor * FLOOR_HEIGHT) - (ELEVATOR_HEIGHT - 2), (realFloor * FLOOR_HEIGHT) - 1, TILE_LJAMB, MR_COLOR_BLACK);
	// Let's draw the right jamb.
	mr_vtilesv(ELEVATOR_X + (TILE_BASEX_WIDTH - 1), (realFloor * FLOOR_HEIGHT) - (ELEVATOR_HEIGHT - 2), (realFloor * FLOOR_HEIGHT) - 1, TILE_RJAMB, MR_COLOR_BLACK);

	// Let's draw the top jamb.
	mr_putetilesv(ELEVATOR_X, (realFloor * FLOOR_HEIGHT) - (ELEVATOR_HEIGHT - 1), TILE_TOP, TILE_TOP_WIDTH, TILE_TOP_HEIGHT, MR_COLOR_BLACK);

	// Finally, draw the floor number.
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT, TILE_FLOOR, MR_COLOR_BLACK);
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT - 1, TILE_EMPTY, MR_COLOR_BLACK);
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 2), TILE_DIGIT1 + _floor, MR_COLOR_GREY);

	// Draw the internal part of the elevator.
	draw_elevator(_floor, _door);
}

// This function will draw the internal part of the elevator, 
// in terms of sliding doors, when the elevator is moving.
void draw_moving_elevator(floor _floor, counter _step) {

	// Used as index.
	unsigned char i;

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// Let's clear the top and the bottom part of the elevator.
	if ((realFloor * FLOOR_HEIGHT + _step + 1) < CONTAINER_Y) {
		mr_htilesv(ELEVATOR_X + 1, ELEVATOR_X + TILE_BASE_WIDTH, realFloor * FLOOR_HEIGHT + _step + 1, TILE_EMPTY, MR_COLOR_BLACK);
	}
	if ((realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT)+_step) > 0) {
		mr_htilesv(ELEVATOR_X + 1, ELEVATOR_X + TILE_BASE_WIDTH, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT)+_step, TILE_EMPTY, MR_COLOR_BLACK);
	}

	// If any person is present inside the elevator...
	if (passengersInElevator) {
		// ... we have to draw him/her/them.
		mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT + _step - 2, TILE_WALK3, TILE_WALK3_WIDTH, TILE_WALK3_HEIGHT, MR_COLOR_BLACK);
	}
	else {
		// ... otherwise, we have to draw an empty (internal) elevator.*/
		for (i = 1; i < (FLOOR_HEIGHT - 1); ++i) {
			mr_putetilesv(ELEVATOR_X + 1, (realFloor * FLOOR_HEIGHT) - i + _step, TILE_DOOR, TILE_DOOR_WIDTH, TILE_DOOR_HEIGHT, MR_COLOR_BLACK);
		}
		mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT + _step, TILE_BASE, TILE_BASE_WIDTH, TILE_BASE_HEIGHT, MR_COLOR_BLACK);
	}

	// Finally, we draw the top of the elevator.
	mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT)+1 + _step, TILE_BASE, TILE_BASE_WIDTH, TILE_BASE_HEIGHT, MR_COLOR_BLACK);
}

// This function will draw the lights next to given elevator.
void draw_elevator_light(floor _floor, mr_boolean _up_light, mr_boolean _down_light) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// Draw a green light if the elevator is moving up, grey otherwise.
	mr_puttilev(ELEVATOR_X + TILE_BASEX_WIDTH, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT - 2), TILE_UP, _up_light ? MR_COLOR_GREEN : MR_COLOR_GREY);

	// Draw a red light if the elevator is moving down, grey otherwise.
	mr_puttilev(ELEVATOR_X + TILE_BASEX_WIDTH, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT - 3), TILE_DOWN, _down_light ? MR_COLOR_RED : MR_COLOR_GREY);
}

// This function will draw the lights next to all elevators.
void draw_elevator_lights() {
	unsigned char i;

	// If the actual floor is smaller than the target floor,
	// we must update the path lights giving the "up" direction
	// (because this should be the direction of the elevator).
	if (actualFloor < targetFloor) {
		for (i = actualFloor; i <= targetFloor; ++i) {
			draw_elevator_light(i, mr_true, mr_false);
		}
	}
	// Else, we must update the path lights giving the "down" direction.
	// (again, because this should be the direction of the elevator).
	else if (actualFloor > targetFloor) {
		for (i = targetFloor; i <= actualFloor; ++i) {
			draw_elevator_light(i, mr_false, mr_true);
		}
	}
	else {
		for (i = 0; i < FLOOR_COUNT; ++i) {
			draw_elevator_light(i, mr_false, mr_false);
		}
	}
}

// This function will draw an entire (static) floor.
void draw_floor(floor _floor, door _door) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// Draw the base line of the floor.
	mr_htilesv(0, (MR_SCREEN_WIDTH - 1), realFloor * FLOOR_HEIGHT, TILE_FLOOR, MR_COLOR_BLACK);

	// Draw the elevator.
	draw_elevator_complete(_floor, _door);

}

// This function will draw an entire (moving elevator) floor.
void draw_moving_floor(floor _floor) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char i, realFloor = (FLOOR_COUNT - _floor);

	// Draw the specific base for moving, that is different from static one.
	mr_putetilesv(ELEVATOR_X, realFloor * FLOOR_HEIGHT, TILE_MBASEX, TILE_MBASEX_WIDTH, TILE_MBASEX_HEIGHT, MR_COLOR_BLACK);

	// Redraw fully the left jamb.
	mr_vtilesv(ELEVATOR_X, (realFloor * FLOOR_HEIGHT) - (FLOOR_HEIGHT - 1), (realFloor * FLOOR_HEIGHT) - 1, TILE_LJAMB, MR_COLOR_BLACK);

	// Redraw fully the right jamb.
	mr_vtilesv(ELEVATOR_X + (TILE_BASEX_WIDTH - 1), (realFloor * FLOOR_HEIGHT) - (FLOOR_HEIGHT - 1), (realFloor * FLOOR_HEIGHT) - 1, TILE_RJAMB, MR_COLOR_BLACK);

	// Let's fill the internal elevator.
	for (i = 0; i < FLOOR_HEIGHT; ++i) {
		mr_putetilesv(ELEVATOR_X + 1, (realFloor * FLOOR_HEIGHT) - i, TILE_DOOR, TILE_DOOR_WIDTH, TILE_DOOR_HEIGHT, MR_COLOR_BLACK);
	}

	// Finally, write the floor's number,
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 2), TILE_DIGIT1 + _floor, MR_COLOR_BLACK);

}

// This function will draw a single frame for a passenger that is walking at a given position.
void draw_passenger_walking(floor _floor, mr_position _pos, counter _frame, floor _target_floor, mr_color _color) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// We have to choose the correct tile, in base to the current _frame.
	unsigned char tile = TILE_WALK1;
	switch (_frame) {
	case 0:
		tile = TILE_WALK1;
		break;
	case 1:
		tile = TILE_WALK2;
		break;
	case 2:
		tile = TILE_WALK3;
		break;
	case 3:
		tile = TILE_WALK4;
		break;
	case 4:
		tile = TILE_WALK5;
		break;
	case 5:
		tile = TILE_WALK6;
		break;
	}

	// If we are drawing a passenger on a position that is not the
	// leftmost column, we must clear the previous position,
	// to avoid the ghosting.
	if (_pos > 0) {
		// Clear the leftmost column.
		mr_vtilesv(_pos - 1, realFloor * FLOOR_HEIGHT - TILE_WALK1_HEIGHT, realFloor * FLOOR_HEIGHT - 1, TILE_EMPTY, MR_COLOR_BLACK);

		// Restore the floor.
		mr_puttilev(_pos - 1, realFloor * FLOOR_HEIGHT, TILE_FLOOR, MR_COLOR_BLACK);

		// Clear the space where, previously, 
		// the needed floor will be put.
		mr_puttilev(_pos + 1, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 1), TILE_EMPTY, MR_COLOR_BLACK);
	}

	// If we are drawing a passenger OVER the elevator, 
	// the entire (static) floor must be redrawn.
	if ((_pos >= (ELEVATOR_X - 3)) && (_pos < (ELEVATOR_X + (TILE_BASE_WIDTH)+TILE_WALK1_WIDTH + 1))) {
		draw_elevator_complete(_floor, actualFloor == _floor ? 0 : 4);
	}

	// Finally, we can draw, in order:
	// - the target floor
	mr_puttilev(_pos + 2, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 1), TILE_DIGIT1 + _target_floor, _color);

	// - the animation frame.
	mr_putetilesv(_pos, realFloor * FLOOR_HEIGHT - 2, tile, TILE_WALK1_WIDTH, TILE_WALK1_HEIGHT, MR_COLOR_BLACK);
}

// This function will clear the space occupied by a passenger that is walking at a given position.
void clear_passenger_walking(floor _floor, mr_position _pos) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// Clear the requested target floor.
	mr_puttilev(_pos + 2, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 1), TILE_EMPTY, MR_COLOR_BLACK);

	// Clear the animation frame.
	mr_putftilesv(_pos, realFloor * FLOOR_HEIGHT - 2, TILE_EMPTY, TILE_WALK1_WIDTH, TILE_WALK1_HEIGHT, MR_COLOR_BLACK);

	// Restore the floor.
	mr_putftilesv(_pos, realFloor * FLOOR_HEIGHT, TILE_FLOOR, TILE_WALK1_WIDTH, TILE_FLOOR_HEIGHT, MR_COLOR_BLACK);
}

// Draw the current game mode.
void draw_mode() {
	mr_putftilesv(0, 0, TILE_EMPTY, TILE_COOPERATIVE_WIDTH, TILE_COOPERATIVE_HEIGHT, MR_COLOR_BLACK);
	switch (gameMode) {
		case GAME_MODE_SIMULATION:
			// Draw word: "SIMULATION"*/
			mr_putetilesv(0, 0, TILE_SIMULATION, TILE_SIMULATION_WIDTH, TILE_SIMULATION_HEIGHT, MR_COLOR_BLACK);
			break;
		case GAME_MODE_SINGLE:
			// Draw word: "SINGLE"
			mr_putetilesv(0, 0, TILE_SINGLE, TILE_SINGLE_WIDTH, TILE_SINGLE_HEIGHT, MR_COLOR_BLACK);
			break;
		case GAME_MODE_COOPERATIVE:
			// Draw word: "COOPERATIVE"
			mr_putetilesv(0, 0, TILE_COOPERATIVE, TILE_COOPERATIVE_WIDTH, TILE_COOPERATIVE_HEIGHT, MR_COLOR_BLACK);
			break;
	}
}

// Draw the remaining time on the screen.
void draw_remain_time() {

	unsigned char i = (timeRemain >> 3);
	unsigned char j = timeRemain - (i << 3);

	// Draw the hourglass icon.
	mr_puttilev(TILE_SIMULATION_WIDTH + 1, 0, TILE_TIME, MR_COLOR_BLACK);

	// Draw the time bar.
	mr_putftilesv(TILE_SIMULATION_WIDTH + 2, 0, TILE_TIME8, i, 1, MR_COLOR_BLACK);
	mr_puttilev(TILE_SIMULATION_WIDTH + i + 2, 0, TILE_EMPTY + j, MR_COLOR_BLACK);

}

// Draw the latency time on the screen.
void draw_latency_time() {

	unsigned char i = (cooperativeLatency >> 3);
	unsigned char j = cooperativeLatency - (i << 3);

	// Draw the hourglass icon.
	mr_puttilev(TILE_SIMULATION_WIDTH + 3 + (TIME_REMAIN_MAX >> 3) , 0, TILE_TIME, MR_COLOR_BLUE);

	// Draw the time bar.
	mr_putftilesv(TILE_SIMULATION_WIDTH + 4 + (TIME_REMAIN_MAX >> 3), 0, TILE_TIME8, i, 1, MR_COLOR_BLUE);
	mr_puttilev(TILE_SIMULATION_WIDTH + 4 + (TIME_REMAIN_MAX >> 3) + i, 0, TILE_EMPTY + j, MR_COLOR_BLUE);

}

// This function will draw both the remaining time (if needed)
// and the latency time (if needed).
void draw_time() {

	if (gameMode == GAME_MODE_SINGLE) {
		draw_remain_time();
	}
	if (gameMode == GAME_MODE_COOPERATIVE) {
		draw_latency_time();
	}

}

// Clear the space on the screen occupied by the times.
void clear_time() {
	mr_putftilesv(TILE_SIMULATION_WIDTH+1, 0, TILE_EMPTY, (TIME_REMAIN_MAX>>3)+4+(COOPERATIVE_LATENCY_MAX>>3), 1, MR_COLOR_BLACK);
}

// This function will draw the score to a specific position on the screen.
void draw_score_at(mr_position _x, mr_position _y) {

	unsigned char i;

	// Draw the passenger icon.
	mr_puttilev(_x, _y, TILE_PASSENGER, MR_COLOR_BLACK);

	// Draw the score.
	for (i = 0; i < 4; ++i) {
		mr_puttilev(_x + 2 + i, _y, TILE_DIGIT0 + score[3 - i], MR_COLOR_BLACK);
	}

}

// This function will draw the score to the default position.
void draw_score() {
	draw_score_at(MR_SCREEN_WIDTH - 6, 0);
}

// Clear the space on the screen occupied by the score on the default position.
void clear_score() {
	mr_putftilesv(MR_SCREEN_WIDTH - 6, 0, TILE_EMPTY, 5, 1, MR_COLOR_BLACK);
}

// Calculate the correct color for the passenger, based on the fact
// that the passenger is waiting for a specific time.
mr_color calculate_passenger_color(unsigned char _number) {

	mr_color color = MR_COLOR_BLACK;

	if (passengerWait[_number] > 20) {
		color = MR_COLOR_RED;
	}
	else if (passengerWait[_number] > 10) {
		color = MR_COLOR_YELLOW;
	}
	else if (passengerWait[_number] > 5) {
		color = MR_COLOR_BLUE;
	}

	return color;

}

// This function will draw the various passengers inside the elevator.
void draw_passengers_in_elevator() {

	unsigned char i,j=0;
	mr_color color;

	// Fill up the screen with the wall background.
	draw_background_panel(TILE_WALL);

	// For each allowed passenger...
	for (i = 0; i < PASSENGER_COUNT; ++i) {
		// ... if the passengers is inside the elevator...
		if (passengerInElevator[i]) {
			// Let's draw the passenger head, with the desired floor and
			// the correct color.
			color = calculate_passenger_color(i);
			mr_putetilesv(PANEL_X + (j * (TILE_HEAD_WIDTH + 2)), PANEL_Y + 1, TILE_HEAD, TILE_HEAD_WIDTH, TILE_HEAD_HEIGHT, color);
			mr_puttilev(PANEL_X + (j * (TILE_HEAD_WIDTH + 2)) + 2, PANEL_Y + 1, TILE_DIGIT1 + desiredFloor[i], color);
			++j;
		}
	}

}

// This function will draw the starting screen of the game.
void draw_settings() {

	unsigned char i;

	// Draw the game mode.
	draw_mode();

	// Draw the background for the panel.
	draw_background_panel(TILE_PANEL);

	// For each floor...
	for (i = 0; i < FLOOR_COUNT; ++i) {

		if (i == actualFloor) {
			// Draw the elevators on the floor (with open doors).
			draw_floor(i, 0);
		}
		else {
			// Draw the elevators on the floor.
			draw_floor(i, 4);
		}

		// Draw the relative lights.
		draw_elevator_light(i, 0, 0);
	}

}

/////////////////////////////////////////////////////////////////////////////
/// THREADS
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

// This thread deals with moving the elevator.
// See "ELEVATOR MOVE" thread diagram.
MR_PT_THREAD(elevatormove) {

	MR_PTI_BEGIN();

		// ARRIVED TO FLOOR?
		while (motorStep < FLOOR_HEIGHT) {

			// HALTED?
			MR_PTI_YIELD_UNTIL(!elevatorHalted);

			// UPDATE MOTOR STEP
			++motorStep;

			// (yeld)
			MR_PTI_YIELD();

			// DRAW ELEVATOR
			if (directionUp) {
				draw_moving_elevator(actualFloor, -motorStep);
			} else {
				draw_moving_elevator(actualFloor, motorStep);
			}

		}

		// UPDATE FLOOR
		if (directionUp) {
			// Defensive programming: avoid crashes.
			if (actualFloor < FLOOR_COUNT) {
				++actualFloor;
			}
		} else {
			// Defensive programming: avoid crashes.
			if (actualFloor > 0) {
				--actualFloor;
			}
		}

		// RESET MOTOR STEP
		motorStep = 0;

	MR_PTI_END();

}

/////////////////////////////////////////////////////////////////////////////

// This thread deals with opening or closing the elevator doors.
// See "DOOR MOVE" thread diagram.
MR_PT_THREAD(doormove) {

	MR_PTI_BEGIN();

	// CLOSE?
	if (closeDoors) {

		// DOOR FRAME 4?
		while (doorFrame < 4) {

			// INCREASE DOOR FRAME
			++doorFrame;

			// (yeld)
			MR_PTI_YIELD();

			// DRAW DOOR
			draw_elevator(actualFloor, doorFrame);

		}

	} else {

		while (doorFrame > 0) {

			// UPDATE DOOR FRAME
			--doorFrame;

			// (yeld)
			MR_PTI_YIELD();

			// DRAW FLOOR
			draw_elevator(actualFloor, doorFrame);

		}

	}

	MR_PTI_END();
}

/////////////////////////////////////////////////////////////////////////////

// This thread takes care of moving a passenger along the plane, from the 
// position where it is located to the one indicated as the input parameter.
// See "WALK" thread diagram.
MR_PT_THREAD_EXT(walk,walk_protothread) {

	MR_PTI_BEGIN();

	// ARRIVED?
	while (walkPos[_mr_pt->index] < walkMoveTo[_mr_pt->index]) {

		// WAIT FREE SPACE
		MR_PTI_WAIT_UNTIL(passenger_can_walk(_mr_pt->index));

		// INCREASE PASSENGER FRAME
		++walkFrame[_mr_pt->index];

		// EVEN FRAME?
		if ((walkFrame[_mr_pt->index] & 1) == 0) {
			// INCREASE PASSENGER POSITION
			++walkPos[_mr_pt->index];
		}

		// FRAME MAX REACHED?
		if (walkFrame[_mr_pt->index] > WALK_FRAME_COUNT) {
			// RESET FRAME
			walkFrame[_mr_pt->index] = 0;
		}

		// (yeld)
		MR_PTI_YIELD();

		// DRAW PASSENGER
		draw_passenger_walking(passengerFloor[_mr_pt->index], walkPos[_mr_pt->index], walkFrame[_mr_pt->index], desiredFloor[_mr_pt->index], calculate_passenger_color(_mr_pt->index));
	}

	MR_PTI_END();
}

/////////////////////////////////////////////////////////////////////////////

// This thread takes care of moving the lift, where the conditions are right.
// See "LIFT" thread diagram.
MR_PT_THREAD(lift) {

	unsigned char i;

	MR_PTI_BEGIN();

	while (1) {

		// WAIT BUTTON
		MR_PTI_YIELD_UNTIL(!(targetFloorsUp == 0 && targetFloorsDown == 0));

		while (!(targetFloorsUp == 0 && targetFloorsDown == 0)) {

			MR_PTI_SEM_WAIT(&liftSemaphore);

			// [close] DOOR
			closeDoors = mr_true;
			MR_PTI_SPAWN(&doormoveThread, doormove(&doormoveThread));

			// DRAW ANIMATED FLOORS
			for (i = 0; i < FLOOR_COUNT; ++i) {
				draw_moving_floor(i);
			}

			// By default, current floor must be removed 
			// from potential targets.
			targetFloorsUp &= ~(1 << actualFloor);
			targetFloorsDown &= ~(1 << actualFloor);

			targetFloor = 0xff;
			if (directionUp) {
				if (actualFloor < FLOOR_COUNT) {
					for (i = actualFloor + 1; i < FLOOR_COUNT; ++i) {
						if ((targetFloorsUp & (1 << i)) != 0) {
							targetFloor = i;
							break;
						}
					}
				}
			} else {
				if (actualFloor > 0) {
					for (i = actualFloor - 1; i != 255; --i) {
						if ((targetFloorsDown & (1 << i)) != 0) {
							targetFloor = i;
							break;
						}
					}
				}
			}
			
			if (targetFloor == 0xff) {
				directionUp = !directionUp;
				if (directionUp) {
					for (i = actualFloor + 1; i < FLOOR_COUNT; ++i) {
						if ((targetFloorsUp & (1 << i)) != 0) {
							targetFloor = i;
							break;
						}
					}
				}
				else {
					for (i = actualFloor - 1; i != 255; --i) {
						if ((targetFloorsDown & (1 << i)) != 0) {
							targetFloor = i;
							break;
						}
					}
				}

			}

			if (targetFloor != 0xff) {

				draw_elevator_lights();

				effectiveTargetFloor = targetFloor;

				// UP?
				if (actualFloor < targetFloor) {

					// RUNNING UP

					// REACHED?
					while (actualFloor != effectiveTargetFloor) {

						// RUNNING UP

						// MOVE [1 floor]
						directionUp = mr_true;
						MR_PTI_SPAWN(&elevatormoveThread, elevatormove(&elevatormoveThread));

						// DRAW FLOOR LIGHTS
						draw_elevator_lights();

						// REACHED?
						if ((targetFloorsUp & (1 << actualFloor)) != 0) {
							effectiveTargetFloor = actualFloor;
						}

					}

				}
				else {

					// RUNNING DOWN

					while (actualFloor != effectiveTargetFloor) {

						// RUNNING DOWN

						// MOVE [1 floor]
						directionUp = mr_false;
						MR_PTI_SPAWN(&elevatormoveThread, elevatormove(&elevatormoveThread));

						// DRAW FLOOR LIGHTS
						draw_elevator_lights();

						// REACHED?
						if ((targetFloorsDown & (1 << actualFloor)) != 0) {
							effectiveTargetFloor = actualFloor;
						}

					}

				}

			}

			if (directionUp) {
				targetFloorsUp &= ~(1 << actualFloor);
			} else {
				targetFloorsDown &= ~(1 << actualFloor);
			}

			// DRAW STATIC FLOORS
			for (i = 0; i < FLOOR_COUNT; ++i) {
				draw_elevator_complete(i, 4);
			}

			draw_elevator_lights();

			// [open] DOOR
			closeDoors = mr_false;
			MR_PTI_SPAWN(&doormoveThread, doormove(&doormoveThread));

			MR_PTI_SEM_SIGNAL(&liftSemaphore);

			MR_PTI_WAIT( &liftWait, 2 );

		}

	}

	MR_PTI_END();
}

/////////////////////////////////////////////////////////////////////////////

// This thread takes care of moving the passenger.
// See "PASSENGER" thread diagram.
MR_PT_THREAD_EXT(passenger, passenger_protothread) {

	MR_PTI_BEGIN();

		MR_PTI_YIELD_UNTIL((desiredFloor[_mr_pt->index] != 0xff));

		walkPos[_mr_pt->index] = 0;

		passengerWait[_mr_pt->index] = 0;

		// WALK (to elevator)
		walkMoveTo[_mr_pt->index] = (ELEVATOR_X - 3);
		MR_PTI_SPAWN(&walkThread[_mr_pt->index], walk(&walkThread[_mr_pt->index]));

		// PRESS DIRECTION BUTTON
		if (gameMode == GAME_MODE_SIMULATION) {
			process_request_floor(passengerFloor[_mr_pt->index]);
		}

		do {

			do {

				// WAIT (lift open door)
				MR_PTI_YIELD_UNTIL((actualFloor == passengerFloor[_mr_pt->index]) && (doorFrame == 0));

				// TOO MUCH PEOPLE?
				if (passengersInElevator >= PASSENGER_IN_ELEVATOR_COUNT) {

					// WAIT (close door)
					MR_PTI_YIELD_UNTIL(doorFrame == 4);

					if (gameMode == GAME_MODE_SIMULATION) {
						// PRESS DIRECTION BUTTON
						process_request_floor(passengerFloor[_mr_pt->index]);
					}

					// Avoid to increment over a reasonable quota.
					if (passengerWait[_mr_pt->index] < 50) {
						++passengerWait[_mr_pt->index];
					}

				}

			} while ((actualFloor != passengerFloor[_mr_pt->index]) || ( doorFrame != 0 ) || (passengersInElevator >= PASSENGER_IN_ELEVATOR_COUNT));

			MR_PTI_SEM_WAIT(&liftSemaphore);

			if (passengersInElevator < PASSENGER_IN_ELEVATOR_COUNT && !passengerInElevator[_mr_pt->index]) {

				// WALK (in elevator)
				walkMoveTo[_mr_pt->index] = (ELEVATOR_X + 1);
				MR_PTI_SPAWN(&walkThread[_mr_pt->index], walk(&walkThread[_mr_pt->index]));

				// INCREASE PASSENGERS COUNT
				++passengersInElevator;

				// PASSENGER ON ELEVATOR
				passengerInElevator[_mr_pt->index] = mr_true;

				clear_passenger_walking(passengerFloor[_mr_pt->index], walkPos[_mr_pt->index]);

				if (gameMode != GAME_MODE_SIMULATION) {
					draw_passengers_in_elevator();
				}

				MR_PTI_RESET(&liftWait, 6);

			}

			MR_PTI_SEM_SIGNAL(&liftSemaphore);

		} while (!passengerInElevator[_mr_pt->index] && ( passengersInElevator >= PASSENGER_IN_ELEVATOR_COUNT));

		// PRESS FLOOR BUTTON
		if (gameMode == GAME_MODE_SIMULATION) {
			process_request_floor_reservation(desiredFloor[_mr_pt->index]);
		}

		while ((actualFloor != desiredFloor[_mr_pt->index])) {
			// WAIT (to floor and open door)
			MR_PTI_YIELD_UNTIL((doorFrame == 0));
			// Avoid to increment over a reasonable quota.
			if (passengerWait[_mr_pt->index] < 50) {
				++passengerWait[_mr_pt->index];
			}
		}

		passengerFloor[_mr_pt->index] = actualFloor;

		MR_PTI_SEM_WAIT(&liftSemaphore);

		// WALK (out elevator)
		walkMoveTo[_mr_pt->index] = (ELEVATOR_X  + TILE_BASE2_WIDTH + 3);
		MR_PTI_SPAWN(&walkThread[_mr_pt->index], walk(&walkThread[_mr_pt->index]));

		// DECREASE PASSENGERS COUNT
		--passengersInElevator;

		// PASSENGER OUT ELEVATOR
		passengerInElevator[_mr_pt->index] = mr_false;

		if (gameMode != GAME_MODE_SIMULATION) {
			draw_passengers_in_elevator();
		}

		MR_PTI_SEM_SIGNAL(&liftSemaphore);

		// WALK (out of floor)
		walkMoveTo[_mr_pt->index] = (MR_SCREEN_WIDTH - (TILE_WALK1_WIDTH + 1));
		MR_PTI_SPAWN(&walkThread[_mr_pt->index], walk(&walkThread[_mr_pt->index]));

		clear_passenger_walking(passengerFloor[_mr_pt->index], walkPos[_mr_pt->index]);
		
		desiredFloor[_mr_pt->index] = 0xff;

		if (gameMode != GAME_MODE_SIMULATION) {
			addendum[3] = 0;
			addendum[2] = 0;
			if (passengerWait[_mr_pt->index] > 30) {
				addendum[1] = 0;
				addendum[0] = 5;
			} else if (passengerWait[_mr_pt->index] > 20) {
				addendum[1] = 1;
				addendum[0] = 0;
			} else if (passengerWait[_mr_pt->index] > 10) {
				addendum[1] = 1;
				addendum[0] = 5;
			} else {
				addendum[1] = 2;
				addendum[0] = 0;
			}
			add(score, addendum);
			draw_score();
		}

	MR_PTI_END();

}

/////////////////////////////////////////////////////////////////////////////

// This thread takes care of manage the input from the player.
MR_PT_THREAD(input) {

	MR_PTI_BEGIN();

		if (gameMode == GAME_MODE_COOPERATIVE) {
			MR_PTI_YIELD_UNTIL(cooperativeLatency == 0);
		}

		// This is the last key pressed.
		currentKey = mr_get_key_pressed();

		switch (currentKey) {

			//-------------------------------------------------------
			// INTERNAL BUTTONS
			//-------------------------------------------------------

				// BUTTON "ALT"
			case MR_KEY_PLUS: // ALT
				// Set the flag that halts the elevator.
				elevatorHalted = mr_true;
				break;
				// BUTTON "ALARM"
			case MR_KEY_MINUS: // ALARM
				// Reset the alarm counter.
				alarmActiveCount = ALARM_ACTIVE_COUNT;
				break;
				// BUTTON "1"
			case MR_KEY_1:
				process_request_floor_reservation(0);
				break;
				// BUTTON "2"
			case MR_KEY_2:
				process_request_floor_reservation(1);
				break;
				// BUTTON "3"
			case MR_KEY_3:
				process_request_floor_reservation(2);
				break;
				// BUTTON "4"
			case MR_KEY_4:
				process_request_floor_reservation(3);
				break;
				// BUTTON "5"
			case MR_KEY_5:
				process_request_floor_reservation(4);
				break;

			//-------------------------------------------------------
			// CHANGE MODE
			//-------------------------------------------------------

			case MR_KEY_S:
				gameMode = GAME_MODE_SIMULATION;
				draw_background_panel(TILE_PANEL);
				draw_elevator_panel();
				draw_mode();
				clear_time();
				clear_score();
				break;

			case MR_KEY_SPACE:
				gameMode = GAME_MODE_SINGLE;
				draw_background_panel(TILE_WALL);
				draw_mode();
				draw_time();
				draw_score();
				draw_passengers_in_elevator();
				break;

			case MR_KEY_G:
				gameMode = GAME_MODE_COOPERATIVE;
				draw_background_panel(TILE_WALL);
				draw_mode();
				draw_time();
				draw_score();
				draw_passengers_in_elevator();
				break;

		}

		if (gameMode == GAME_MODE_SIMULATION) {

			switch (currentKey) {
				//-------------------------------------------------------
				// EXTERNAL BUTTONS
				//-------------------------------------------------------

				// BUTTON "UP" ON 5th FLOOR
			case MR_KEY_Q:
				//process_request_floor_up(actualFloor, 4);
				break;
				// BUTTON "DOWN" ON 5th FLOOR
			case MR_KEY_W:
				process_request_floor_down(4);
				break;
				// BUTTON "UP" ON 4th FLOOR
			case MR_KEY_E:
				process_request_floor_up(3);
				break;
				// BUTTON "DOWN" ON 4th FLOOR
			case MR_KEY_R:
				process_request_floor_down(3);
				break;
				// BUTTON "UP" ON 3rd FLOOR
			case MR_KEY_T:
				process_request_floor_up(2);
				break;
				// BUTTON "DOWN" ON 3rd FLOOR
			case MR_KEY_Y:
				process_request_floor_down(2);
				break;
				// BUTTON "UP" ON 2nd FLOOR
			case MR_KEY_U:
				process_request_floor_up(1);
				break;
				// BUTTON "DOWN" ON 2nd FLOOR
			case MR_KEY_I:
				process_request_floor_down(1);
				break;
				// BUTTON "UP" ON 1st FLOOR
			case MR_KEY_O:
				process_request_floor_up(0);
				break;
				// BUTTON "UP" ON 1st FLOOR
			case MR_KEY_P:
				process_request_floor_down(0);
				break;
			}
		}

		if (gameMode == GAME_MODE_COOPERATIVE || gameMode == GAME_MODE_SIMULATION) {

			switch (currentKey) {
					//-------------------------------------------------------
					// PASSENGERS MANAGEMENT
					//-------------------------------------------------------

					// BUTTON "[5]" ON GAMEPAD
				case MR_KEY_Z:
					add_passenger_on_floor(4);
					break;
					// BUTTON "[4]" ON GAMEPAD
				case MR_KEY_X:
					add_passenger_on_floor(3);
					break;
					// BUTTON "[3]" ON GAMEPAD
				case MR_KEY_C:
					add_passenger_on_floor(2);
					break;
					// BUTTON "[2]" ON GAMEPAD
				case MR_KEY_V:
					add_passenger_on_floor(1);
					break;
					// BUTTON "[1]" ON GAMEPAD
				case MR_KEY_B:
					add_passenger_on_floor(0);
					break;
			}
		}

	MR_PTI_END();

}

/////////////////////////////////////////////////////////////////////////////
/// SCREENS AND GAME LOOPS
/////////////////////////////////////////////////////////////////////////////

// Final screen, when the game over arrives.
void show_final_screen() {

	// Clear the screen.
	mr_clearv();

	// Word: "GAME OVER"
	mr_putetilesv(((MR_SCREEN_WIDTH - TILE_GAMEOVER_WIDTH) >> 1), (MR_SCREEN_HEIGHT >> 1) - 2, TILE_GAMEOVER, TILE_GAMEOVER_WIDTH, TILE_GAMEOVER_HEIGHT, MR_COLOR_BLACK);
	
	// Draw the reached score.
	draw_score_at((MR_SCREEN_WIDTH - 6) >> 1, (MR_SCREEN_HEIGHT >> 1));

	// Wait for a key press.
	while(!mr_key_pressed()) { }

}

// Title screen.
void show_title_screen() {

	// Change the border to a dark grey and the background to a light gray.
	mr_set_border_color(MR_COLOR_DARK_GREY);
	mr_set_background_color(COLOR_BACKGROUND);

	// Clear screen bitmap.
	mr_clearv();

#ifdef __ATARI__

	// Load compressed screen on the auxiliary space
	mr_load(FILENAME_INTRO_MPIC, MR_SCREEN_DEFAULT);

	mr_wait(2);

#elif MIDRES_EMBEDDED_FILES

	// Load compressed screen on the auxiliary space
	mr_load_screen(FILE_ZELINTRO_BIN, MR_SCREEN_DEFAULT);

	mr_wait(2);

#else

	// Load compressed screen on the auxiliary space
	mr_load(FILENAME_INTRO_MPIC, MR_AUX_DEFAULT);

	// Show titles.
	mr_uncompressv(MR_AUX_DEFAULT);

#endif

}

// Overal initialization of the game.
void game_init() {

	unsigned char i;

	MR_PTI_SEM_INIT(&liftSemaphore, 1);

	for (i = 0; i < PASSENGER_COUNT; ++i) {
		passengerThread[i].index = i;
		walkThread[i].index = i;
		if (i > 0) {
			passengerFloor[i] = calculate_random_floor(i);
		}
		else {
			passengerFloor[i] = calculate_random_floor(0xff);
		}
		desiredFloor[i] = 0xff;
		passengerInElevator[i] = mr_false;
		walkPos[i] = 0;
		walkMoveTo[i] = 0;
		walkFrame[i] = 0;
	}

	gameMode = GAME_MODE_SIMULATION;
	cooperativeLatencyCount = COOPERATIVE_LATENCY_COUNT;
	cooperativeLatency = 0;
	cooperativePlayer = mr_true;
	passengerWait[PASSENGER_COUNT];
	actualFloor = 0;
	targetFloor = 0;
	effectiveTargetFloor = 0;
	closeDoors = mr_false;
	directionUp = mr_true;
	elevatorHalted = mr_false;
	passengersInElevator = 0;
	targetFloorsUp = 0;
	targetFloorsDown = 0;
	doorFrame = 0;
	motorStep = 0;
	alarmActiveCount = 0;
	timeRemainCount = TIME_REMAIN_COUNT;
	timeRemain = TIME_REMAIN_MAX;
	score[0] = 0;
	score[1] = 0;
	score[2] = 0;
	score[3] = 0;
	currentKey = 0;

	MR_PT_INIT(&inputThread);
	for (i = 0; i < PASSENGER_COUNT; ++i) {
		MR_PT_INIT(&passengerThread[i]);
	}
	MR_PT_INIT(&liftThread);

	draw_settings();

}

// The game loop.
void gameloop() {

	unsigned char i;

	while (1) {

		//---------------------------------- START FRAME
		mr_start_frame();

		// Generate a (random) passenger if playing in SINGLE mode
		if (gameMode == GAME_MODE_SINGLE) {
			if (rand() < (RAND_MAX >> 4)) {
				add_passenger_on_floor(calculate_random_floor(0xff));
			}
		}

		//  ----vvvvvvvv COOPERATIVE LOOP FOR PROTOTHREADING vvvvvvvv----
		input(&inputThread);
		for (i = 0; i < PASSENGER_COUNT; ++i) {
			passenger(&passengerThread[i]);
		}
		lift(&liftThread);
		// ----^^^^^^^^ COOPERATIVE LOOP FOR PROTOTHREADING ^^^^^^^^----

		// Time tracking (if not in SIMULATION mode)
		if (gameMode != GAME_MODE_SIMULATION) {
			--timeRemainCount;
			if (timeRemainCount == 0) {
				timeRemainCount = TIME_REMAIN_COUNT;
				if (timeRemain > 0) {
					--timeRemain;
					draw_remain_time();
				}
			}
		}
		else {
			draw_elevator_panel();
		}

		// Cooperative latency tracking (if in COOPERATIVE mode)
		if (gameMode == GAME_MODE_COOPERATIVE) {
			--cooperativeLatencyCount;
			if (cooperativeLatencyCount == 0) {
				cooperativeLatencyCount = COOPERATIVE_LATENCY_COUNT;
				if (cooperativeLatency > 0) {
					--cooperativeLatency;
					draw_latency_time();
				}
			}
		}

		manage_alarm();

		//---------------------------------- END FRAME (SYNC)
		mr_end_frame(4);

		// If the player(s) are not in SIMULATION mode and the time has
		// gone... game over!
		if (gameMode != GAME_MODE_SIMULATION && timeRemain == 0) {
			break;
		}

	}

}

void game_elevator() {

	// Initialize random number generator
	srand(clock());

	// Let's initialize the graphics library.
	mr_init();

	show_title_screen();

	// We prepare the graphics.
	prepare_graphics();

	mr_clearv();

	// Endless loop.
	while (1) {

		game_init();

		gameloop();

		show_final_screen();

	}	

}

#endif
