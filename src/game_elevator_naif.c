#ifdef __GAME_ELEVATOR__NAIF__

/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020-2021 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * RESIDENT MODULE                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

// Needed for system call.
#include <stdlib.h>
#include <time.h>

// Needed for MIDRES library call.
#include "midres.h"

// Include generated tiles reference.
#include "game_elevator_tiles.h"

/****************************************************************************
 ** DEFINES AND VARIABLES SECTION
 ****************************************************************************/

// This type can be used to denote which floor number is currently used.
// It is normally 0 based, so 0 = first floor, 1 = second floor, and so on.
typedef unsigned char floor;

// This type can be used to denote bitmap or bitmasks.
typedef unsigned char bitmap;

// This type can be used to denote a counter, i.e. a number that starts from
// a value and it will decrease by one up to zero value or increased up to
// a (given) final value.
typedef signed char counter;

// This type can be used to denote at which position of sliding doors
// the elevator is currently: 0 = totally opened, 4 = totally closed.
typedef signed char	door;

// This type can be used to represent the current direction of the elevator,
// i.e. if the elevator must go up (+1) or down (-1).
typedef signed char	direction;

///////////////////////////// GAME //////////////////////////////////////////

// Total number of floors in the simulation.
#define		FLOOR_COUNT				5

// Height of a floor, in tiles. We reserve 2 tiles for status lines (up&down).
#define		FLOOR_HEIGHT			( ( MR_SCREEN_HEIGHT - 2 ) / FLOOR_COUNT )

// Delay of simulation: lesser value = faster speed.
#define		DELAY_FRAME_COUNT		4

// This is the current value for delay. When it reaches DELAY_FRAME_COUNT,
// a new frame and I/O will be processed.
counter delayFrame = 0;

// Duration of alarm signal.
#define		ALARM_ACTIVE_COUNT		60

// This mode is a pure simulation (w/internal panel).
#define		GAME_MODE_SIMULATION	0

// This mode is a one player game.
#define		GAME_MODE_SINGLE		1

// This mode is a two player game, one against the other.
#define		GAME_MODE_COMPETITIVE	2

// This mode is a two player game together against the system.
#define		GAME_MODE_COOPERATIVE	3

///////////////////////////// ELEVATOR //////////////////////////////////////

// Center position for elevators:
#define		ELEVATOR_X				( (MR_SCREEN_WIDTH - TILE_BASEX_WIDTH ) >> 1 )

// Elevator's height.
#define		ELEVATOR_HEIGHT			( FLOOR_HEIGHT )

// Starting position of CONTAINER and size
#define		CONTAINER_WIDTH				( MR_SCREEN_WIDTH )
#define		CONTAINER_HEIGHT			( FLOOR_HEIGHT )
#define		CONTAINER_X					( 0 )
#define		CONTAINER_Y					( MR_SCREEN_HEIGHT - CONTAINER_HEIGHT )

// Starting position of PANEL and size
#define		PANEL_WIDTH					( FLOOR_COUNT + 8  )
#define		PANEL_HEIGHT				( FLOOR_HEIGHT )
#define		PANEL_X						( ELEVATOR_X - (PANEL_WIDTH>>1) )
#define		PANEL_Y						( MR_SCREEN_HEIGHT - PANEL_HEIGHT )

// Current floor (where the elevator is located).
floor actualFloor = 0;

// Current sliding door position (where the elevator is located).
door actualDoor = 0;

// The future floor where the elevator will go.
floor targetFloor = 0;

// 
mr_boolean doorIsOpening = mr_false;

// Which floor has been reserved on "up" direction?
// (bit 0 = first floor, bit 1 = second floor, and so on)
bitmap targetFloorsUp = 0;

// Which floor has been reserved on "down" direction?
// (bit 0 = first floor, bit 1 = second floor, and so on)
bitmap targetFloorsDown = 0;

// The request button has been pressed?
mr_boolean targetRequested = mr_false;

// This flag can be used to force update of runningFloors.
mr_boolean updateRunningFloors = mr_false;

// This counter is used to know if the elevator has passed an entire
// floor, and so its actual floor has to be updated, in order to reflect
// the fact and (moreover) the interface can return responsive.
counter runningFloor = FLOOR_HEIGHT;

// This counter is used to know if the elevator has finished its
// journey to the target floor.
counter runningFloors = 0;

// This value is used to denote what is the actual direction of the
// elevator. By default, the starting direction is "up" since the
// elevator starts on the first floor.
direction currentDirection = 1;

// The elevator has been halted by HALT key press?
mr_boolean elevatorHalted = mr_false;

// Counter used to signal if an ALARM is active.
counter alarmActiveCount = 0;

// Current game mode.
unsigned char gameMode = 0;

///////////////////////////// PASSENGERS ////////////////////////////////////

// Number of frames for each passenger.
#define		WALK_FRAME_COUNT	6

// Current position of each passenger of all floors.
mr_position walkPos[FLOOR_COUNT];

// Animation frame of each passenger of all floors.
counter walkFrame[FLOOR_COUNT];

// This bitmap represent if any passenger is present on each floor. If a bit
// of this variable is one it means that a passenger is currently walking
// on that floor or it is waiting for elevator on that floor or is going
// away on ANOTHER floor.
bitmap passengerPerFloor = 0;

// This variable take care of the number of person inside the elevator.
counter passengersInElevator = 0;

// This bitmap represent if any passenger has requested the elevator 
// in order to go where it has to go.
bitmap passengersActualRequested = 0;

// This bitmap represent if any passenger, inside the elevator, has 
// requested a floor in order to go.
// This bitmap represent if any passenger, inside the elevator, has 
// requested a floor in order to go.
bitmap passengersTargetRequested = 0;

// This array will take note of each passenger destination.
floor targetFloorPerPeople[FLOOR_COUNT];

/****************************************************************************
 ** FUNCTIONS
 ****************************************************************************/

// This routine prepares the graphics needed for the game. It loads the 
// resources and it copy them from the mass memory (or from the embedded 
// memory) to the designated zones.
void prepare_graphics() {

	// Change the border to a dark grey and the background to a light gray.
	mr_set_border_color(MR_COLOR_DARK_GREY);
	mr_set_background_color(MR_COLOR_LIGHT_GREY);

	// Load tiles from mass memory.
	mr_tileset_load("zeltiles.bin", MR_TILESET_0, TILE_START, TILE_COUNT);

	// This little "trick" allows us to quickly clean the screen 
	// using the prepared empty tile.
	MR_RENDERED_MIXELS[0] = TILE_EMPTY;

	// Enable the custom charset.
	mr_tileset_visible(MR_TILESET_0);

}

// This function will draw the elevator's panel, with a synthetic representation
// of its internal variables.
void draw_elevator_panel() {
	unsigned char i;

	// mr_puttilev(CONTAINER_X , PANEL_Y + 1, TILE_DIGIT0 + (runningFloors / 10), MR_COLOR_RED);
	// mr_puttilev(CONTAINER_X + 1, PANEL_Y + 1, TILE_DIGIT0 + (runningFloors-((runningFloors / 10)*10)), MR_COLOR_RED);

	// First of all, let's draw the current direction.
	mr_puttilev(PANEL_X, PANEL_Y + 1, TILE_UP, currentDirection > 0 ? MR_COLOR_GREEN : MR_COLOR_BLACK);
	mr_puttilev(PANEL_X, PANEL_Y + 2, TILE_DOWN, currentDirection < 0 ? MR_COLOR_GREEN : MR_COLOR_BLACK);

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
		} else {
			// Remove its number from the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 1, TILE_EMPTY, MR_COLOR_BLACK);
		}

		// The floor has been requested on "down" direction?
		if ((targetFloorsDown & bitmask) == bitmask) {
			// Draw its number on the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 2, TILE_DIGIT1 + i, MR_COLOR_BLACK);
		} else {
			// Remove its number from the panel.
			mr_puttilev(PANEL_X + 2 + i, PANEL_Y + 2, TILE_EMPTY, MR_COLOR_BLACK);
		}
	}

	// Draw the current floor where the elevator is, currently.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 1, PANEL_Y + 1, TILE_DIGIT1 + actualFloor, MR_COLOR_BLACK);

	// Draw a red light if the elevator has been halted by pressing
	// the HALT button, else it is grey.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 3, PANEL_Y + 1, TILE_LED, elevatorHalted ? MR_COLOR_RED : MR_COLOR_GREY);

	// Draw the actual position of the sliding door.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 5, PANEL_Y + 1, TILE_DIGIT1 + actualDoor, MR_COLOR_GREY);

	// Draw the number of passengers currently inside the elevator.
	if (passengersInElevator >= 3) {
		mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 7, PANEL_Y + 1, TILE_DIGIT0 + passengersInElevator, MR_COLOR_RED);
	} else if (passengersInElevator >= 0) {
		mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 7, PANEL_Y + 1, TILE_DIGIT0 + passengersInElevator, MR_COLOR_GREEN);
	} else {
		mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 7, PANEL_Y + 1, TILE_DIGIT0 + passengersInElevator, MR_COLOR_GREY);
	}

	// Draw the floor where the elevator will go.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 1, PANEL_Y + 2, TILE_DIGIT1 + targetFloor, (targetFloor == actualFloor) ? MR_COLOR_GREY : MR_COLOR_GREEN);

	// Draw a green light if the elevator has been requested.
	mr_puttilev(PANEL_X + 2 + FLOOR_COUNT + 3, PANEL_Y + 2, TILE_LED, targetRequested ? MR_COLOR_GREEN : MR_COLOR_GREY);

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

	// Now we draw the interior part of the elevato, starting from the base..
	mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT, tile_base, TILE_BASE_WIDTH, TILE_BASE_HEIGHT, MR_COLOR_BLACK);
	// ... and going up to the final part of the elevator, that is the line
	// BEFORE the top, that is not drawn anymore.
	for (i = 0; i < (ELEVATOR_HEIGHT-2); ++i) {
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
	mr_vtilesv(ELEVATOR_X, (realFloor * FLOOR_HEIGHT) - (ELEVATOR_HEIGHT - 2), ( realFloor * FLOOR_HEIGHT ) - 1, TILE_LJAMB, MR_COLOR_BLACK);
	// Let's draw the right jamb.
	mr_vtilesv(ELEVATOR_X + (TILE_BASEX_WIDTH - 1), (realFloor * FLOOR_HEIGHT) - (ELEVATOR_HEIGHT - 2), ( realFloor * FLOOR_HEIGHT ) - 1, TILE_RJAMB, MR_COLOR_BLACK);

	// Let's draw the top jamb.
	mr_putetilesv(ELEVATOR_X, (realFloor * FLOOR_HEIGHT) - (ELEVATOR_HEIGHT - 1), TILE_TOP, TILE_TOP_WIDTH, TILE_TOP_HEIGHT, MR_COLOR_BLACK);

	// Finally, draw the floor number.
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT, TILE_FLOOR, MR_COLOR_BLACK);
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT - 1, TILE_EMPTY, MR_COLOR_BLACK);
	mr_puttilev(ELEVATOR_X - 1, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 2), TILE_DIGIT1 + _floor, MR_COLOR_BLACK);

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
	mr_htilesv(ELEVATOR_X + 1, ELEVATOR_X + TILE_BASE_WIDTH, realFloor * FLOOR_HEIGHT + _step + 1, TILE_EMPTY, MR_COLOR_BLACK);
	mr_htilesv(ELEVATOR_X + 1, ELEVATOR_X + TILE_BASE_WIDTH, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT) + _step, TILE_EMPTY, MR_COLOR_BLACK);

	// If any person is present inside the elevator...
	if (passengersInElevator) {
		// ... we have to draw him/her/them.
		mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT + _step - 2, TILE_WALK3, TILE_WALK3_WIDTH, TILE_WALK3_HEIGHT, MR_COLOR_BLACK);
	} else {
		// ... otherwise, we have to draw an empty (internal) elevator.
		for (i = 1; i < (FLOOR_HEIGHT - 1); ++i) {
			mr_putetilesv(ELEVATOR_X + 1, (realFloor * FLOOR_HEIGHT) - i + _step, TILE_DOOR, TILE_DOOR_WIDTH, TILE_DOOR_HEIGHT, MR_COLOR_BLACK);
		}
		mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT + _step, TILE_BASE, TILE_BASE_WIDTH, TILE_BASE_HEIGHT, MR_COLOR_BLACK);
	}

	// Finally, we draw the top of the elevator.
	mr_putetilesv(ELEVATOR_X + 1, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT) + 1 + _step, TILE_BASE, TILE_BASE_WIDTH, TILE_BASE_HEIGHT, MR_COLOR_BLACK);
}

// This function will draw the lights next to each elevator.
void draw_elevator_lights(floor _floor, mr_boolean _up_light, mr_boolean _down_light) {

	// We use a reverse notation for floors: the _floor 0 is, really, the last
	// so we need to invert the meaning of the floor in order to draw them
	// correctly.
	unsigned char realFloor = (FLOOR_COUNT - _floor);

	// Draw a green light if the elevator is moving up, grey otherwise.
	mr_puttilev(ELEVATOR_X + TILE_BASEX_WIDTH, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT - 2), TILE_UP, _up_light ? MR_COLOR_GREEN : MR_COLOR_GREY);

	// Draw a red light if the elevator is moving down, grey otherwise.
	mr_puttilev(ELEVATOR_X + TILE_BASEX_WIDTH, realFloor * FLOOR_HEIGHT - (ELEVATOR_HEIGHT - 3), TILE_DOWN, _down_light ? MR_COLOR_RED : MR_COLOR_GREY);
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
	mr_vtilesv(ELEVATOR_X, (realFloor * FLOOR_HEIGHT) - (FLOOR_HEIGHT - 1), (realFloor * FLOOR_HEIGHT)-1, TILE_LJAMB, MR_COLOR_BLACK);

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
void draw_passenger_walking(floor _floor, mr_position _pos, counter _frame, floor _target_floor) {

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
		mr_vtilesv(_pos -1, realFloor * FLOOR_HEIGHT - TILE_WALK1_HEIGHT, realFloor * FLOOR_HEIGHT - 1, TILE_EMPTY, MR_COLOR_BLACK);

		// Restore the floor.
		mr_puttilev(_pos - 1, realFloor * FLOOR_HEIGHT, TILE_FLOOR, MR_COLOR_BLACK);

		// Clear the space where, previously, 
		// the needed floor will be put.
		mr_puttilev(_pos + 1, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 1), TILE_EMPTY, MR_COLOR_BLACK);
	}

	// If we are drawing a passenger OVER the elevator, 
	// the entire (static) floor must be redrawn.
	if ((_pos >= (ELEVATOR_X-3)) && (_pos < (ELEVATOR_X + (TILE_BASE_WIDTH)+TILE_WALK1_WIDTH+1))) {
		draw_floor(_floor, actualFloor==_floor?0:4);
	}

	// Finally, we can draw, in order:
	// - the target floor
	mr_puttilev(_pos + 2, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 1), TILE_DIGIT1 + _target_floor, MR_COLOR_BLUE);

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
	mr_puttilev(_pos + 1, realFloor * FLOOR_HEIGHT - (FLOOR_HEIGHT - 1), TILE_EMPTY, MR_COLOR_BLACK);

	// Clear the animation frame.
	mr_putftilesv(_pos - 1, realFloor * FLOOR_HEIGHT - 2, TILE_EMPTY, TILE_WALK1_WIDTH, TILE_WALK1_HEIGHT, MR_COLOR_BLACK);

	// Restore the floor.
	mr_putftilesv(_pos - 1, realFloor * FLOOR_HEIGHT, TILE_FLOOR, TILE_WALK1_WIDTH, TILE_FLOOR_HEIGHT, MR_COLOR_BLACK);
}

// This function will draw the starting screen of the game.
void draw_settings() {

	unsigned char i;

	switch (gameMode) {
		case GAME_MODE_SIMULATION:
			// Draw word: "SIMULATION"
			mr_putetilesv(0, 0, TILE_SIMULATION, TILE_SIMULATION_WIDTH, TILE_SIMULATION_HEIGHT, MR_COLOR_BLACK);
			break;
		case GAME_MODE_SINGLE:
			// Draw word: "SINGLE"
			mr_putetilesv(0, 0, TILE_SINGLE, TILE_SINGLE_WIDTH, TILE_SINGLE_HEIGHT, MR_COLOR_BLACK);
			break;
		case GAME_MODE_COMPETITIVE:
			// Draw word: "COMPETITIVE"
			mr_putetilesv(0, 0, TILE_COMPETITIVE, TILE_COMPETITIVE_WIDTH, TILE_COMPETITIVE_HEIGHT, MR_COLOR_BLACK);
			break;
		case GAME_MODE_COOPERATIVE:
			// Draw word: "COOPERATIVE"
			mr_putetilesv(0, 0, TILE_COOPERATIVE, TILE_COOPERATIVE_WIDTH, TILE_COOPERATIVE_HEIGHT, MR_COLOR_BLACK);
			break;
	}

	// Draw background of PANEL (the CONTAINER)
	mr_putftilesv(CONTAINER_X, CONTAINER_Y, TILE_PANEL, CONTAINER_WIDTH, CONTAINER_HEIGHT, MR_COLOR_BLACK);

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
		draw_elevator_lights(i, 0, 0);
	}

}

// This function is needed to process a request for reservation 
// of a floor. It manages the internal bitmap representation and,
// moreover, it restart the elevator if has been halted previously,
void process_request_floor_reservation( floor _floor ) {

	// If the floor required is greater than the first...
	if (_floor > 0) {
		if (_floor < FLOOR_COUNT) {
			// If the actual floor if smaller that the one requested,
			// we must reserve the floor on the "up" queue, by setting
			// the specific bit on the bitmap.
			if (actualFloor < _floor) {
				targetFloorsUp |= (1 << _floor);
			}
			// If the actual floor if greater that the one requested,
			// we must reserve the floor on the "down" queue, by setting
			// the specific bit on the bitmap.
			else if (actualFloor > _floor) {
				targetFloorsDown |= (1 << _floor);
			}
		} else {
			targetFloorsUp |= (1 << _floor);
		}
	} else {
		targetFloorsDown |= (1 << _floor);
	}

	// If the floor required is smaller than the last...
	if ( _floor < FLOOR_COUNT ) {
		// If the actual floor if greater that the one requested,
		// we must reserve the floor on the "down" queue, by setting
		// the specific bit on the bitmap.
		if (actualFloor > _floor) {
			targetFloorsDown |= (1 << _floor);
		}
	} 

	// Whatever is the floor reserved, we must
	// signal that a target has been requested.
	targetRequested = mr_true;

	// Restart the movement of the elevator.
	elevatorHalted = mr_false;

}

// This function is needed to process a request for reservation 
// of a floor by external buttons. It manages the internal 
// bitmap representation for the specific direction queue. It 
// restart the elevator if has been halted previously,
void process_request_floor_up(floor _current, floor _target) {

	// We must reserve the floor on the "up" queue, by setting
	// the specific bit on the bitmap.
	targetFloorsUp |= (1 << _target);

	if (actualFloor < _current) {
		targetFloorsUp |= (1 << _current);
	}
	else if (actualFloor > _current) {
		targetFloorsDown |= (1 << _current);
	}


	// Whatever is the floor reserved, we must
	// signal that a target has been requested.
	targetRequested = mr_true;

	// Finally, we must update the light 
	// (to give to the user a feedback).
	draw_elevator_lights(_current, mr_true, mr_false);

}

// This function is needed to process a request for reservation 
// of a floor by external buttons. It manages the internal 
// bitmap representation for the specific direction queue. It 
// restart the elevator if has been halted previously,
void process_request_floor_down(floor _current, floor _target) {

	// We must reserve the floor on the "down" queue, by setting
	// the specific bit on the bitmap.
	targetFloorsDown |= (1 << _target);

	if (actualFloor < _current) {
		targetFloorsUp |= (1 << _current);
	} else if (actualFloor > _current) {
		targetFloorsDown |= (1 << _current);
	}

	// Whatever is the floor reserved, we must
	// signal that a target has been requested.
	targetRequested = mr_true;

	// Finally, we must update the light 
	// (to give to the user a feedback).
	draw_elevator_lights(_current, mr_false, mr_true);

}

// This function will generate a "passenger" for a specific floor.
// This will be done only if a passenger has not been already
// defined on that floor.
void add_passenger_on_floor(floor _floor) {

	// If the passenger is not already present...
	if ((passengerPerFloor & (1<<_floor)) == 0) {

		unsigned char randomFloor = 0;
		// Set the specific bit for that floor.
		passengerPerFloor |= (1<<_floor);

		// Generate a random target floor for the passenger.
		// The target floor cannot be the same as the
		// one where passenger appears.
		randomFloor = rand() & 0x0f;
		while (randomFloor >= (FLOOR_COUNT)) {
			randomFloor -= FLOOR_COUNT;
		}
		if (randomFloor == _floor) {
			++randomFloor;
		}
		if (randomFloor >= (FLOOR_COUNT)) {
			randomFloor -= FLOOR_COUNT;
		}
		targetFloorPerPeople[_floor] = randomFloor;
	}

}

// This function will process the input from the user, if any.
void process_input() {

	// This is the last key pressed.
	unsigned char key = mr_get_key_pressed();

	switch (key) {

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
		// EXTERNAL BUTTONS
		//-------------------------------------------------------

		// BUTTON "UP" ON 5th FLOOR
		case MR_KEY_Q:
			process_request_floor_up(actualFloor, 4);
			break;
		// BUTTON "DOWN" ON 5th FLOOR
		case MR_KEY_W:
			process_request_floor_down(actualFloor, 4);
			break;
		// BUTTON "UP" ON 4th FLOOR
		case MR_KEY_E:
			process_request_floor_up(actualFloor, 3);
			break;
		// BUTTON "DOWN" ON 4th FLOOR
		case MR_KEY_R:
			process_request_floor_down(actualFloor, 3);
			break;
		// BUTTON "UP" ON 3rd FLOOR
		case MR_KEY_T:
			process_request_floor_up(actualFloor, 2);
			break;
		// BUTTON "DOWN" ON 3rd FLOOR
		case MR_KEY_Y:
			process_request_floor_down(actualFloor, 2);
			break;
		// BUTTON "UP" ON 2nd FLOOR
		case MR_KEY_U:
			process_request_floor_up(actualFloor, 1);
			break;
		// BUTTON "DOWN" ON 2nd FLOOR
		case MR_KEY_I:
			process_request_floor_down(actualFloor, 1);
			break;
		// BUTTON "UP" ON 1st FLOOR
		case MR_KEY_O:
			process_request_floor_up(actualFloor, 0);
			break;
		// BUTTON "UP" ON 1st FLOOR
		case MR_KEY_P:
			process_request_floor_down(actualFloor, 0);
			break;

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
	} else if (alarmActiveCount == 1) {

		mr_sound_stop_channel(0);
		--alarmActiveCount;

	// On all other cases, we are going to change
	// rapidly the sound, in order to emulate
	// an "alarm".
	} else if (alarmActiveCount > 0) {

		mr_sound_change_channel(0, (alarmActiveCount & 1) ? 2000 : 1500);
		--alarmActiveCount;

	}

}

// This function will update the screen with the state of the
// elevator.
void update_elevator() {

	unsigned char i;

	// Note that we are going to process the elevator command only
	// if the elevator is at the right place, i.e. on an actual
	// floor. This limit has been introduced in order to update
	// the various variables on an "integer" size.
	// if the elevator is at the right place, i.e. on an actual
	// floor. This limit has been introduced in order to update
	// the various variables on an "integer" size.
	if (runningFloor == FLOOR_HEIGHT) {

		// If a target floor has been requested...
		if (targetRequested) {

			// Let's define the bitmask for the (current) target floor.
			// This will be used in order to memorize the previous
			// destination.
			bitmap bitmaskFuture = (1 << targetFloor);

			// Turn off the various flags that has fired this
			// procedure, in order to avoid repetitions.
			targetRequested = mr_false;
			updateRunningFloors = mr_false;

			// If a target floor has been requested when the elevator
			// was going up...
			if (currentDirection > 0) {
				// We must check if the current journey must
				// be updated, in order to take account of the 
				// new target.
				for (i = actualFloor; i < FLOOR_COUNT; ++i) {
					// Is the new floor requested
					// placed BEFORE the actual destination?
					unsigned char bitmask = (1 << i);
					if ((targetFloorsUp & bitmask) == bitmask) {
						// So the new floor requested is the new
						// destination.
						targetFloor = i;
						targetFloorsUp &= ~bitmask;
						// The old target floor
						// will be put again on the queue, as the
						// next step, if the elevator was moving.
						if (runningFloors) {
							targetFloorsUp |= bitmaskFuture;
						}
						updateRunningFloors = mr_true;
						break;
					}
				}
				// If the journey was not modified AND there are
				// request for the other direction...
				if (!updateRunningFloors && targetFloorsDown && (actualFloor >= targetFloor ) ) {
					// We should invert the running direction 
					// *and* reprocess the request.
					currentDirection = -1;
					targetRequested = mr_true;
				}
			}
			else {
				// We must check if the current journey must
				// be updated, in order to take account of the 
				// new target.
				for (i = actualFloor; i != 255; --i) {
					// Is the new floor requested
					// placed BEFORE the actual destination?
					unsigned char bitmask = (1 << i);
					if ((targetFloorsDown & bitmask) == bitmask) {
						// So the new floor requested is the new
						// destination.
						targetFloor = i;
						targetFloorsDown &= ~bitmask;
						// The old target floor
						// will be put again on the queue, as the
						// next step, if the elevator was moving.
						if (runningFloors) {
							targetFloorsDown |= bitmaskFuture;
						}
						updateRunningFloors = mr_true;
						break;
					}
				}
				// If the journey was not modified AND there are
				// request for the other direction...
				if (!updateRunningFloors && targetFloorsUp && (actualFloor <= targetFloor)) {
					// We should invert the running direction 
					// *and* reprocess the request.
					currentDirection = 1;
					targetRequested = mr_true;
				}
			}

			// If the journey has been modified...
			if (updateRunningFloors) {
				// We must recalculate, as an absolute value,
				// the number of steps between the actual position
				// and the new position.
				if (actualFloor > targetFloor) {
					runningFloors = ((actualFloor - targetFloor) * FLOOR_HEIGHT);
				}
				else {
					runningFloors = ((targetFloor - actualFloor) * FLOOR_HEIGHT);
				}
				runningFloor = FLOOR_HEIGHT;
			}

		} // nothing must be done if a target floor has not been requested

	} // nothing is done if the elevator is in the middle of a floor!

	// Next, if elevator is halted nothing must be done.
	if (!elevatorHalted) {

		// If elevator is currently on a floor different from the one
		// requested, we must move the elevator up to that position.
		if (actualFloor != targetFloor) {

			// If the actual floor is smaller than the target floor,
			// we must update the path lights giving the "up" direction
			// (because this should be the direction of the elevator).
			if (actualFloor < targetFloor) {
				for (i = actualFloor; i <= targetFloor; ++i) {
					draw_elevator_lights(i, mr_true, mr_false);
				}
			}
			// Else, we must update the path lights giving the "down" direction.
			// (again, because this should be the direction of the elevator).
			else if (actualFloor > targetFloor) {
				for (i = targetFloor; i <= actualFloor; ++i) {
					draw_elevator_lights(i, mr_false, mr_true);
				}
			}

			// While the door is open...
			if (actualDoor < 4) {
				// ... we must close it.
				++actualDoor;
				// Update the graphical representation of the elevator.
				draw_elevator_complete(actualFloor, actualDoor);
			}

			// If the door is closed and an elevator movement is registered...
			else if ((actualDoor == 4) && (updateRunningFloors)) {
				// Update the entire screen, in order to give to the user
				// the feedback about the movement of the elevator.
				for (i = 0; i < FLOOR_COUNT; ++i) {
					draw_moving_floor(i);
				}
				// De-register elevator movement.
				updateRunningFloors = 0;
			}

			// In all other cases...
			else {

				// If the elevator is still moving...
				if (runningFloors > 0) {
					// Decrease the needed steps.
					--runningFloors;
					// Decrease the local position of the elevator.
					--runningFloor;
					// If arrived to zero...
					if (runningFloor == 0) {
						// ... move by one floor.
						actualFloor += currentDirection;
						// Reset the counter.
						runningFloor = FLOOR_HEIGHT;
					}
					// If the entire movement arrived to zero...
					if (runningFloors == 0) {
						// Update the entire screen, in order to give to the user
						// the feedback about the finished movement of the elevator.
						for (i = 0; i < FLOOR_COUNT; ++i) {
							draw_elevator_complete(i, 4);
						}
						// We arrived to the target floor.
						actualFloor = targetFloor;
					}

					// Else, we are still moving...
					else {
						// If we are moving from low to high floor...
						if (actualFloor < targetFloor) {
							// ... running floor is a negative offset.
							draw_moving_elevator(targetFloor, runningFloors);
						}
						// ... else...
						else if (actualFloor > targetFloor) {
							// ... it's a positive offset.
							draw_moving_elevator(targetFloor, -runningFloors);
						}
					}
				} else {
					runningFloor = FLOOR_HEIGHT;
				}
			}

		// Else, we are arrived (or we are) on the target floor.
		} else {
			// If doors are closed...
			if (actualDoor == 4) {
				// Update the state of the elevators, in order to give to the user
				// the feedback about the finished movement of the elevator.
				for (i = 0; i < FLOOR_COUNT; ++i) {
					if (i == actualFloor) {
						draw_elevator_complete(i, actualDoor);
					} else {
						draw_elevator_complete(i, 4);
					}
				}
			}
			// If doors are still opening...
			if (actualDoor > 0) {
				--actualDoor;
			}
			// ... otherwise, if they are already opened...
			else if (actualDoor == 0) {
				// Request a new target floor if any floor is reserved.
				if (targetFloorsUp || targetFloorsDown) {
					targetRequested = mr_true;
				}
			}
			// Update the state of the lights.
			for (i = 0; i < FLOOR_COUNT; ++i) {
				draw_elevator_lights(i, mr_false, mr_false);
			}
			// Update the elevator.
			draw_elevator_complete(actualFloor, actualDoor);
		}
	} // Nothing must be done if elevator is halted!

}

// This function will update the animation of the passenger,
// and, if needed, his position.
void move_passenger_ahead(floor _floor) {
	++walkFrame[_floor];
	if ((walkFrame[_floor] & 1) == 0) {
		++walkPos[_floor];
	}
	if (walkFrame[_floor] == WALK_FRAME_COUNT) {
		walkFrame[_floor] = 0;
	}
}

// This function will update the screen with the state of the
// various passengers.
void update_passengers() {

	unsigned char i;

	// We are going to examine each floor, in order to detect 
	// if any passenger is going to move or interact with
	// the elevator.
	for (i = 0; i < FLOOR_COUNT; ++i) {

		bitmap bitmask = (1 << i);

		// If any passenger exists for this floor...
		if ((passengerPerFloor & bitmask) == bitmask) {

			if ((targetFloorPerPeople[i] == actualFloor)&&((passengersTargetRequested&bitmask)==bitmask)) {
				// If the passenger passed the elevator 
				// but he did not reach the end of the floor...
				if ((walkPos[i] >= (ELEVATOR_X + (TILE_BASE_WIDTH >> 1))) && (walkPos[i] < (MR_SCREEN_WIDTH - TILE_WALK1_WIDTH))) {
					// Move passenger ahead.
					draw_passenger_walking(targetFloorPerPeople[i], walkPos[i], walkFrame[i], targetFloorPerPeople[i]);
					move_passenger_ahead(i);
				}

				// If the passenger reached the end of the floor...
				else if (walkPos[i] >= (MR_SCREEN_WIDTH - TILE_WALK1_WIDTH)) {
					// ... delete the passenger and reset everything.
					clear_passenger_walking(targetFloorPerPeople[i], walkPos[i]);
					passengerPerFloor &= ~bitmask;
					walkFrame[i] = 0;
					walkPos[i] = 0;
					targetFloorPerPeople[i] = 0xff;
					passengersActualRequested &= ~bitmask;
					passengersTargetRequested &= ~bitmask;
				}

				else if (actualDoor == 0) {
					// Make the passenger appearing on the target floor.
					walkPos[i] = (ELEVATOR_X + (TILE_BASE_WIDTH >> 1));

					draw_passenger_walking(targetFloorPerPeople[i], walkPos[i], walkFrame[i], targetFloorPerPeople[i]);
					move_passenger_ahead(i);

					// Decrease the number of passengers inside the elevator.
					--passengersInElevator;
				}

			} 
			
			// If the passenger did not reach the elevator...
			else if (walkPos[i] < (ELEVATOR_X - 2)) {
				// Move passenger ahead.
				draw_passenger_walking(i, walkPos[i], walkFrame[i], targetFloorPerPeople[i]);
				move_passenger_ahead(i);
			}

			// In all the other cases: the passenger is waiting for the
			// elevator to arrive, and to enter.
			// The elevator is on the same floor than the passenger,
			// and its doors are open.
			else if (i == actualFloor && ((passengersActualRequested & bitmask) == bitmask) && (actualDoor == 0)) {

				// If the passenger is not entered inside the elevator...
				if (walkPos[i] < ( ELEVATOR_X + (TILE_BASE_WIDTH >> 1) + 1 ) ) {
					// Make the passenger enter into the elevator.
					draw_passenger_walking(i, walkPos[i], walkFrame[i], targetFloorPerPeople[i]);
					move_passenger_ahead(i);
				}

				// If there is space inside the elevator...
/*				else if (passengersInElevator < 4) {
				
					// ... delete the passenger.
					clear_passenger_walking(i, walkPos[i]);

					// Increase the number of passengers inside the elevator.
					++passengersInElevator;

					// Make the passenger press the internal button.
					process_request_floor_reservation(targetFloorPerPeople[i]);

					// Take note he pressed it.
					passengersTargetRequested |= bitmask;

					mr_set_background_color(MR_COLOR_YELLOW);

				}
				else {
					mr_puttilev(CONTAINER_X, PANEL_Y + 3, TILE_LED, MR_COLOR_GREEN);
					//@TODO: if there is no space, what I have to do?
				}*/
			
			}
			// The elevator is on another floor...
			else {
				if ((passengersActualRequested & bitmask) == 0) {
					// ... let's call it!
					if (targetFloorPerPeople[i] > i) {
						process_request_floor_up(i, targetFloorPerPeople[i]);
					}
					else {
						process_request_floor_down(i, targetFloorPerPeople[i]);
					}
					// Take note of that.
					passengersActualRequested |= bitmask;
					passengersTargetRequested |= bitmask;
				}
			}
		} // None has to be done if passenger is absent for this floor

	} // Loop throught all floors.

}

// This function will process a single "step" of the simulation.
// During a step the input / output will be processed, the graphical
// feedback will be done, and so on.
void game_step() {

	mr_start_frame();

	process_input();

	manage_alarm();

	draw_elevator_panel();

	++delayFrame;

	if (delayFrame >= DELAY_FRAME_COUNT) {
		delayFrame = 0;
		
		update_elevator();

		update_passengers();

	}

	mr_end_frame(2);
}

void gameloop() {

	draw_settings();

	while (1) {
	
		game_step();
	
	}

}

void game_elevator() {

	// Initialize random number generator
	srand(clock());

	// Let's initialize the graphics library.
	mr_init();

	// We prepare the graphics.
	prepare_graphics();

	mr_clearv();

	// Endless loop.
	while (1) {

		gameloop();

	}

}

#endif
