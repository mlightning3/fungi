/**
 *
 *
 */

#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "window.h"
#include <iostream>
#include <SDL2/SDL.h>

/*
 * SDLWindow
 * The display into our fungus world
 */
class SDLWindow : public Window {
private:
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;

	/**
	 * Log an SDL error with some error message to the output stream of our choice
	 * @param os The output stream to write the message to
	 * @param msg The error message to write, format will be msg error: SDL_GetError()
	 */
	void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

	/**
	 * Brings up all the SDL objects we will need to draw our window
	 * @param winpos The position we want for the window
	 * @param winsize The size we want for the window
	 */
	void init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
			logSDLError(std::cout, "SDL_Init");
			bad = true;
		}
		sdlWindow = SDL_CreateWindow("Fungi", winposition.posx, 
										winposition.posy,
										winsize.width, winsize.height,
										SDL_WINDOW_OPENGL);
		if (sdlWindow == nullptr){
			logSDLError(std::cout, "CreateWindow");
			SDL_Quit();
			bad = true;
		}
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1,
											SDL_RENDERER_SOFTWARE);
		if (sdlRenderer == nullptr){
			logSDLError(std::cout, "CreateRenderer");
			SDL_DestroyWindow(sdlWindow);
			sdlWindow = nullptr;
			SDL_Quit();
			bad = true;
		}
		SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderPresent(sdlRenderer);
	}

	/**
	 * Draws a fungus on the screen
	 * @param f The fungus we want to draw
	 */
	void drawFungus(const Fungus& f) {
		Color color = f.getColor();
		Location loc = f.getLocation();
		SDL_SetRenderDrawColor(sdlRenderer, color.red, color.green, color.blue,
								color.alpha);
		SDL_RenderDrawPoint(sdlRenderer, loc.posx, loc.posy);
	}

public:
	/**
	 * Creates a SDLWindow without anything to show
	 */
	SDLWindow() : Window() {
		sdlWindow = nullptr;
		sdlRenderer = nullptr;
	}

	/**
	 * Creates a SDLWindow with an attached Petri dish
	 * @param p A petri dish
	 */
	SDLWindow(const Window_Pos& pos, const Window_Size& size, const Petri& p) :
		Window(pos, size, p) {
		init();
	}

	/**
	 * Destroyer of the window
	 */
	~SDLWindow() {
		if(sdlWindow != nullptr) {
			SDL_DestroyWindow(sdlWindow);
		}
		if(sdlRenderer != nullptr) {
			SDL_DestroyRenderer(sdlRenderer);
		}
		SDL_Quit();
	}

	/**
	 * Runs the growth of the fungus till the end of time
	 */
	void doFungus() {

		SDL_Event event;
		bool end = false;
		while(!end) {
			while(SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) {
					end = true;
				}
			}

			for(int i = 0; i < petri.getSize(); i++) {
				if(petri[i] != nullptr) {
					drawFungus(*petri[i]);
				}
			}

			SDL_RenderPresent(sdlRenderer);

			petri.grow();
	    
			//SDL_Delay(TICKS);
		}
    
	}

	/**
	 * Tells if the windows is bad, aka an SDL error
	 * @return True if window is bad, False if not
	 */
	bool isBad() const { return bad; }
};

#endif
