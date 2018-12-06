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
    uint8_t alpha;
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

   Fungus(const Location& l, const Color& c) {
       local.posx = l.posx;
       local.posy = l.posy;
       color.red = c.red;
       color.green = c.green;
       color.blue = c.blue;
       color.alpha = c.alpha;
   }

   ~Fungus() {}

   /**
    * Get the location of the fungus
    * @return The location of the fungus
    */
   Location getLocation() const { return local; }

   /**
    * Get the color of the fungus
    * @return The color of the fungus
    */
   Color getColor() const { return color; }
};

/*
 * Petri
 * Where the fungus live, grow, and die
 */
class Petri {
private:
    std::vector<Fungus*> dish;

public:
    Petri() {}

    /**
     * @param maxSize Maximum number of fungi that we will have
     */
    Petri(int maxSize) {
	dish.reserve(maxSize);
	for(int i = 0; i < maxSize; i++) {
	    dish.push_back(nullptr);
	}
    }

    /**
     * Copy construction
     * Doesn't work right apparently
     */
    Petri(const Petri& p) {
	dish.reserve(p.getSize());
	for(int i = 0; i < p.getSize(); i++) {
	    dish.push_back(nullptr);
	}
	for(int i = 0; i < dish.size(); i++) {
	    Fungus* temp = p.getFungus(i);
	    if(temp != nullptr) {
		addFungus(new Fungus(temp->getLocation(), temp->getColor()));
	    }
	}
    }

    ~Petri() {
	for(int i = 0; i < dish.size(); i++) {
	    if(dish[i] != nullptr) {
		Fungus* temp = dish[i];
		dish[i] = nullptr;
		delete temp;
	    }
	}
    }

    /**
     * Gets a Fungus from the dish
     * @param i The position of the fungus from the dish we want
     */
    Fungus* operator[](const int& i) {
	if(i >= dish.size()) {
	    return nullptr;
	} else {
	    return dish[i];
	}
    }

    Fungus* getFungus(const int& i) const {
	if(i >= dish.size()) {
	    return nullptr;
	} else {
	    return dish[i];
	}
    }

    /**
     * Adds a fungus to the dish, unless the dish is full
     * @param f The fungus to add
     */
    void addFungus(Fungus* f) {
	for(int i = 0; i < dish.size(); i++) {
	    if(dish[i] == nullptr) {
		dish[i] = f;
		break;
	    }
	}
    }

    /**
     * Simulates one round of fungus life
     *
     */
    void grow() {}

    /**
     * Get size of dish
     * @return Max number of fungus we can hold in dish
     */
    int getSize() const { return dish.size(); }
    
    /**
     * Gives the fungus
     * @return The vector of fungus
     */
    std::vector<Fungus*> & getDish() { return dish; }
};

/*
 * SDLWindow
 * The display into our fungus world
 */
class SDLWindow {
private:
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    Petri petri;
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
     * @param winpos The position we want for the window
     * @param winsize The size we want for the window
     */
    void init(const Window_Pos& winpos, const Window_Size& winsize) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	    logSDLError(std::cout, "SDL_Init");
	    bad = true;
	}
	sdlWindow = SDL_CreateWindow("Fungi", winpos.posx, winpos.posy,
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
    SDLWindow() {
	bad = false;
	init({100, 100}, {SCREEN_WIDTH, SCREEN_HEIGHT});
    }

    SDLWindow(const Petri& p) {
	petri = p;
	bad = false;
	init({100, 100}, {SCREEN_WIDTH, SCREEN_HEIGHT});
    }

    ~SDLWindow() {
	if(sdlWindow != nullptr) {
	    SDL_DestroyWindow(sdlWindow);
	}
	if(sdlRenderer != nullptr) {
	    SDL_DestroyRenderer(sdlRenderer);
	}
	SDL_Quit();
    }

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
	    /*
	    std::vector<Fungus*> dish = petri.getDish();
	    for(int i = 0; i < dish.size(); i++) {
		if(dish[i] != nullptr) {
		    drawFungus(*dish[i]);
		}
	    }
	    */
	    SDL_RenderPresent(sdlRenderer);
	
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

    Petri* p = new Petri(SCREEN_HEIGHT * SCREEN_WIDTH);

    for(int i = 0; i < 1; i++) {
	p->addFungus(new Fungus({0, i}, {i,255 - i,255,255}));
    }

    
    SDLWindow win(*p);
    if(!win.isBad()) {
	win.doFungus();
    }

    return 0;
}
