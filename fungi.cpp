/*
 * Fungi
 *
 * Art generator, inspiration spore from Culture by Charlotte Koch
 *
 * 2018 Matt Kohls
 * GPL v3
 */

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>


/** Important Constants **/

const uint8_t GROWTH_RATE = 4; /* A percent out of 100 */

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

/** Important Objects **/

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

/*
 * Location
 * Keeps track of where we are
 */
struct Location {
    uint16_t posx;
    uint16_t posy;
};

/*
 * Color
 * Keeps track of the appearance of the fungus
 */
struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

/*
 * Fungus
 * More or less just a pixel
 */
class Fungus {
private:
   struct Location local;
   struct Color color;
   uint8_t growthPoints;
   uint8_t hp;

public:
   Fungus() {}

   ~Fungus() {}
};

/*
 * Window
 * The display into our fungus world
 */
class Window {
private:
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    
    bool bad;

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
     *
     */
    void init(const Window_Pos& winpos, const Window_Size& winsize) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	    logSDLError(std::cout, "SDL_Init");
	    bad = true;
	}
	sdlWindow = SDL_CreateWindow("Fungi", winpos.posx, winpos.posy,
		                     winsize.width, winsize.height, true);
	if (sdlWindow == nullptr){
	    logSDLError(std::cout, "CreateWindow");
	    SDL_Quit();
	    bad = true;
	}
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1,
	SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRenderer == nullptr){
	    logSDLError(std::cout, "CreateRenderer");
	    SDL_DestroyWindow(sdlWindow);
	    sdlWindow = nullptr;
	    SDL_Quit();
	    bad = true;
	}
    }

public:
    Window() {
	bad = false;
	init({100, 100}, {SCREEN_WIDTH, SCREEN_HEIGHT});
    }

    ~Window() {
	if(sdlWindow != nullptr) {
	    SDL_DestroyWindow(sdlWindow);
	}
	if(sdlRenderer != nullptr) {
	    SDL_DestroyRenderer(sdlRenderer);
	}
	SDL_Quit();
    }

    void do_fugus() {

	SDL_Event event;
	bool end = false;
	while(!end) {
	    while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) {
		    end = true;
		}
	    }
	
	}
    
    }

    /**
     * Tells if the windows is bad, aka an SDL error
     * @return True if window is bad, False if not
     */
    bool isBad() const { return bad; }
};

/** Important Functions **/


int main(int argv, char* argc[]) {
    
    srandom(11);


    return 0;
}
