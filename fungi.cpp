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
const uint8_t TICKS = 10; /*Time to wait between growths */

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
   /**
    * Creates a useless Fungus
    */
   Fungus() {
       hp = 0;
       growthPoints = 0;
   }

   /**
    * Creates a Fungus
    * @param l The location the fungus should live in
    * @param c The color the fungus should be
    */
   Fungus(const Location& l, const Color& c) {
       local.posx = l.posx;
       local.posy = l.posy;
       color.red = c.red;
       color.green = c.green;
       color.blue = c.blue;
       color.alpha = c.alpha;
       hp = 10;
       growthPoints = 0;
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

   void addGrowth(uint8_t val) { growthPoints += val; }
   
   uint8_t getGrowthPoints() const { return growthPoints; }

   uint8_t getHP() const { return hp; }
};

/*
 * Petri
 * Where the fungus live, grow, and die
 */
class Petri {
private:
    uint32_t maxSize;
    std::vector<Fungus*> population; // Holds only the living fungus
    std::vector< std::vector<Fungus*> > dish; // Keeps track of each position
					      // a fungus may live

    /**
     * Grows a fungus
     * @param f The fungus to grow
     * @return Either a new fungus or nullptr if no children spawn
     */
    Fungus* growFungus(Fungus& f) {
	uint8_t val = random();
	while(val > 100) {
	    val = random();
	}
	if(val < GROWTH_RATE) {
	    f.addGrowth(1);
	}
	Fungus* child = nullptr;
	if(f.getGrowthPoints() >= 10) {
	    f.addGrowth(255 - f.getGrowthPoints());
	    Location home = f.getLocation();
	    home.posx += 1;
	    home.posy += 1;
	    child = new Fungus(home, f.getColor());
	}
	return child;
    }

public:
    /**
     * Creates a useless Petri dish
     */
    Petri() {
	maxSize = 0;
	//population = nullptr;
	//dish = nullptr;
    }

    /**
     * Creates a Petri dish for the fungus to live in
     * @param maxSize Maximum number of fungi that we will have
     * @param ws The size of the window the petri dish should fill 
     */
    Petri(uint32_t maxSize, const Window_Size& ws) {
	this->maxSize = maxSize;
	population.reserve(maxSize);
	for(int i = 0; i < maxSize; i++) {
	    population.push_back(nullptr);
	}
	dish.reserve(ws.width);
	std::vector<Fungus*> temp(ws.height);
	for(int i = 0; i < ws.height; i++) {
	    temp.push_back(nullptr);
	}
	for(int i = 0; i < ws.width; i++) {
	    dish.push_back(temp);
	}
	
    }

    /**
     * Copy construction
     * Doesn't work right apparently
     */
    /*
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
    */

    /**
     * Destroyer of dishes
     */
    ~Petri() {
	for(int i = 0; i < population.size(); i++) {
	    if(population[i] != nullptr) {
		Fungus* temp = population[i];
		population[i] = nullptr;
		delete temp;
	    }
	}
    }

    /**
     * Gets a Fungus from the dish
     * @param i The position of the fungus from the dish we want
     */
    Fungus* operator[](const uint32_t& i) {
	return getFungus(i);
    }
    
    /**
     * Gets a Fungus from the dish
     * @param i The position of the fungus from the dish we want
     */
    Fungus* getFungus(const uint32_t& i) const {
	if(i >= population.size()) {
	    return nullptr;
	} else {
	    return population[i];
	}
    }

    /**
     * Adds a fungus to the dish, unless that spot is full, then the fungus is
     * deleted from existence.
     * @param f The fungus to add
     */
    void addFungus(Fungus* f) {
	if(dish[f->getLocation().posy][f->getLocation().posx] == nullptr) {
	    dish[f->getLocation().posy][f->getLocation().posx] = f;
	    for(int i = 0; i < population.size(); i++) {
		if(population[i] == nullptr) {
		    population[i] = f;
		    break;
		}
	    }
	} else {
	    delete f;
	}
    }

    /**
     * Simulates one round of fungus life
     *
     */
    void grow() {
	for(int i = 0; i < dish.size(); i++) {
	    for(int j = 0; j < dish[i].size(); j++) {
		if(dish[i][j] != nullptr) {
		    Fungus* newFungus = growFungus(*dish[i][j]);
		    if(newFungus != nullptr) {
			addFungus(newFungus);
		    }
		}
	    }
	}
    
    }

    /**
     * Get size of dish
     * @return Max number of fungus we can hold in dish
     */
    uint32_t getSize() const { return maxSize; }
    
    /**
     * Gives the fungus
     * @return The 2D vector of fungus
     */
    std::vector<std::vector<Fungus*> > & getDish() { return dish; }

    /**
     * Get list of living fungus
     * @return Vector of fungus in the dish
     */
    std::vector< Fungus* > & getPopulation() { return population; }
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
    /**
     * Creates a SDLWindow without anything to show
     */
    SDLWindow() {
	bad = false;
	init({100, 100}, {SCREEN_WIDTH, SCREEN_HEIGHT});
    }

    /**
     * Creates a SDLWindow with an attached Petri dish
     * @param p A petri dish
     */
    SDLWindow(const Petri& p) {
	petri = p;
	bad = false;
	init({100, 100}, {SCREEN_WIDTH, SCREEN_HEIGHT});
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
	    
	    SDL_Delay(TICKS);
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

    Window_Size ws = {SCREEN_WIDTH, SCREEN_HEIGHT};
    Petri* p = new Petri(SCREEN_HEIGHT * SCREEN_WIDTH, ws);

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
    
    SDLWindow win(*p);
    if(!win.isBad()) {
	win.doFungus();
    }

    //delete p;

    return 0;
}
