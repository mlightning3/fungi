/**
 *
 *
 */

#ifndef PETRI_H
#define PETRI_H

#include <vector>
#include <stdint.h>

/*
 * Location
 * Keeps track of where we are
 */
struct Location {
	uint16_t posx;
	uint16_t posy;
};

/*
 * Dish Size
 * Keeps track of the size of the dish
 */
struct Dish_Size {
	uint16_t width;
	uint16_t height;
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
		color = {0, 0, 0, 0};
		local = {0, 0};
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

	/**
	 * Adds growth to the fungus
	 * @param val Amount to grow the fungus
	 */
	void addGrowth(uint8_t val) { growthPoints += val; }
   
	/**
	 * Gets the current growth of the fungus
	 * @return Current growth points of fungus
	 */
	uint8_t getGrowthPoints() const { return growthPoints; }

	/**
	 * Gets the current health of the fungus
	 * @return Current health points of the fungus
	 */
	uint8_t getHP() const { return hp; }
};

/*
 * Petri
 * Where the fungus live, grow, and die
 */
class Petri {
private:
	uint32_t maxSize;
	uint8_t growthRate;
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
		if(val < growthRate) {
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
	 * @param gr The growth rate of fungus inside this dish
	 */
	Petri(const uint32_t& maxSize, const Dish_Size& ds, const uint8_t& gr) {
		this->maxSize = maxSize;
		this->growthRate = gr;
		population.reserve(maxSize);
		for(int i = 0; i < maxSize; i++) {
			population.push_back(nullptr);
		}
		dish.reserve(ds.width);
		std::vector<Fungus*> temp(ds.height);
		for(int i = 0; i < ds.height; i++) {
			temp.push_back(nullptr);
		}
		for(int i = 0; i < ds.width; i++) {
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
	Fungus* operator[](const uint32_t& i) { return getFungus(i); }
    
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
	std::vector<Fungus*> & getPopulation() { return population; }
};

#endif
