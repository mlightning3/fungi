/*
 * Fungi
 *
 * Art generator, inspiration spore from Culture by Charlotte Koch
 *
 * 2018 Matt Kohls
 * GPL v3
 */

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "petri.h"
#include "window.h"

#include "sdlwindow.h"

/** Important Constants **/

uint8_t GROWTH_RATE = 4; /* A percent out of 100 */
const uint8_t TICKS = 10; /*Time to wait between growths */

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

/** Important Objects **/


/** Important Functions **/


int main(int argv, char* argc[]) {
    
	srandom(11);

	Window_Pos wp = {100, 100};
	Window_Size ws = {SCREEN_WIDTH, SCREEN_HEIGHT};
	Dish_Size ds = {SCREEN_WIDTH, SCREEN_HEIGHT};
	uint32_t max = SCREEN_WIDTH * SCREEN_HEIGHT;
	Petri* p = new Petri(max, ds, GROWTH_RATE);

	p->addFungus(new Fungus({0, 0}, {255, 0, 0, 0}));
	p->addFungus(new Fungus({0, 1}, {0, 255, 0, 0}));
	p->addFungus(new Fungus({0, 2}, {0, 0, 255, 0}));

    /*
    for(int i = 0; i < 255; i++) {
	for(int j = 0; j < 255; j++) {
	    p->addFungus(new Fungus({j, i}, {i, 255 - i, j, 255 - j}));
	}
    }
    */
    
	SDLWindow win(wp ,ws, *p);
	if(!win.isBad()) {
		win.doFungus();
	}

    //delete p;

	return 0;
}
