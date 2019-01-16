/**
 *
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include "petri.h"

/*
 * Window Position
 * Keeps track of window position
 */
struct Window_Pos {
	uint16_t posx;
	uint16_t posy;
};

/*
 * Window Size
 * Keeps track of window size
 */
struct Window_Size {
	uint16_t width;
	uint16_t height;
};


class Window {
protected:
	Petri petri;
	bool bad;
	struct Window_Size winsize;
	struct Window_Pos winposition;

	/**
	 * Draws a fungus to the window display
	 * @param f The fungus to draw
	 */
	virtual void drawFungus(const Fungus& f);

public:
	Window() {
		winsize = {0, 0};
		winposition = {0, 0};
		bad = true; // Since there is nothing to be drawn
	}

	/**
	 * Creates a new window in a given position and a given size
	 * @param pos Position of window
	 * @param size Size of window
	 * @param p Petri dish to display
	 */
	Window(const Window_Pos& pos, const Window_Size& size, const Petri& p) {
		winsize.width = size.width;
		winsize.height = size.height;
		winposition.posx = pos.posx;
		winposition.posy = pos.posy;
		bad = false;
		petri = p;
	}

	/**
	 * Destroy the window
	 */
	virtual ~Window();

	/**
	 * Starts the simulation
	 */
	virtual void doFungus();

	/**
	 * Gives position of window
	 * @return Window_Pos struct holding window current position
	 */
	virtual Window_Pos getWindowPosition() const { return winposition; }

	/**
	 * Gives window size
	 * @return Window_Size struct holding size of current window
	 */
	virtual Window_Size getWindowSize() const { return winsize; }

	/**
	 * Tells if there has been some error setting up the window
	 * @return True if there is an error, False if not
	 */
	virtual bool isBad() const { return bad; }
};

#endif
