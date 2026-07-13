#include <iostream>
#include <cstdlib>
#include <ctime>

#include "plants.h"
#include "sunflower.h"
#include "peashooter.h"
#include "chomper.h"
#include "zombies.h"

using std::cout;
using std::cin;
using std::endl;

//CONSTANTS
const int ROWS = 5;
const int COLS = 10;
const int SUN_START = 50;
const int MAX_ROUNDS = 10;  //Change depending on how many round you wish to play

// plant costs
const int COST_SUNFLOWER = 50;
const int COST_PEASHOOTER = 100;
const int COST_CHOMPER = 150;


void initGrids(plants* pGrid[ROWS][COLS], zombies* zGrid[ROWS][COLS]) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            //stores the pointers to plants at each tile
            pGrid[r][c] = nullptr;
            //stores the pointers to zombies at each tile
            zGrid[r][c] = nullptr;
        }
    }
}

int sunflowerProduction(plants* pGrid[ROWS][COLS]) {
    int gained = 0;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            //checks if plant is a sunflower
            sunflower* s = dynamic_cast<sunflower*>(pGrid[r][c]);
            if (s != nullptr) { // if yes calls produce_sun and adds it
                gained += s->produce_sun();
            }
        }
    }
    return gained;
}

void displayGrid(plants* pGrid[ROWS][COLS], zombies* zGrid[ROWS][COLS],
    int round, int sun, int gainedSun) {
    cout << "================================" << endl;
    cout << "           Round " << round << endl;
    cout << "================================" << endl;
    cout << "Total sun: " << sun;
    if (gainedSun > 0) cout << "  (+" << gainedSun << " from sunflowers)";
    cout << endl << endl;

    // column header
    cout << "    ";
    for (int c = 0; c < COLS; ++c) {
        cout << c << " ";
    }
    cout << endl;

    for (int r = 0; r < ROWS; ++r) {
        cout << r << " | ";
        for (int c = 0; c < COLS; ++c) {
            char cell = '.';

            if (zGrid[r][c] != nullptr) {
                cell = 'Z';
            }
            else if (pGrid[r][c] != nullptr) {
                std::string nm = pGrid[r][c]->show_name();
                cell = nm.empty() ? 'P' : nm[0]; // S / P / C etc.
            }
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// spawn one zombie at right edge in a random row that has space there
void spawnZombie(zombies* zGrid[ROWS][COLS]) {
    int candidates[ROWS];
    int count = 0;

    for (int r = 0; r < ROWS; ++r) {
        // looks at the rightmost column
        if (zGrid[r][COLS - 1] == nullptr) {
            //creates a list of rows that dont have zombies "candidates"
            //then picks a random row from candidates and creates a new zombie in that tile
            candidates[count++] = r;
        }
    }

    if (count == 0) {
        // rightmost column full in all rows, skips spawn
        return;
    }

    int idx = std::rand() % count;
    int row = candidates[idx];
    int col = COLS - 1;

    zGrid[row][col] = new zombies();
}

void plantsAttack(plants* pGrid[ROWS][COLS], zombies* zGrid[ROWS][COLS]) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (pGrid[r][c] == nullptr) continue;

            // check if plant is peashooter or chomper
            peashooter* pea = dynamic_cast<peashooter*>(pGrid[r][c]);
            chomper* ch = dynamic_cast<chomper*>(pGrid[r][c]);

            plants* plantPtr = nullptr;
            if (pea != nullptr) {   //peashooter gets range and damaga\e
                plantPtr = pea;
            }
            else if (ch != nullptr) {   //chomper gets range a damage
                plantPtr = ch;
            }
            else {
                // sunflower no attack
                continue;
            }

            int range = plantPtr->show_range();
            int dmg = plantPtr->show_damage();

            // scan to the right within range
            //peashooter and chomper attacks closest enemy within range
            for (int k = c + 1; k <= c + range && k < COLS; ++k) {
                if (zGrid[r][k] != nullptr) {
                    zGrid[r][k]->take_damage(dmg);
                    if (zGrid[r][k]->show_health() <= 0) {
                        delete zGrid[r][k];
                        zGrid[r][k] = nullptr;
                    }
                    break;
                }
            }
        }
    }
}

// zombies attack plants in front, then move left
bool zombiesAttackAndMove(plants* pGrid[ROWS][COLS], zombies* zGrid[ROWS][COLS]) {
    // ATTACK PHASE
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            zombies* z = zGrid[r][c];
            if (z == nullptr) continue;

            int leftCol = c - 1;    //looks one tile to the left
            if (leftCol >= 0 && pGrid[r][leftCol] != nullptr) {
                int dmg = z->show_damage();
                pGrid[r][leftCol]->take_damage(dmg);    //Does damage

                if (pGrid[r][leftCol]->show_health() <= 0) {
                    delete pGrid[r][leftCol];
                    pGrid[r][leftCol] = nullptr;    //if plant health=0 remove plant
                }
            }
        }
    }

    // MOVE PHASE
    bool baseBreached = false;

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            zombies* z = zGrid[r][c];
            if (z == nullptr) continue;

            int leftCol = c - 1;

            // plant in front, cannot move
            if (leftCol >= 0 && pGrid[r][leftCol] != nullptr) {
                continue;
            }

            zGrid[r][c] = nullptr;

            if (leftCol < 0) {
                baseBreached = true;    //zombie reached the house (all the way to the left)
                delete z;
            }
            else {
                if (zGrid[r][leftCol] == nullptr) {
                    zGrid[r][leftCol] = z;  // moves left if nothing there
                }
                else {
                    // another zombie there already, keep original position
                    zGrid[r][c] = z;
                }
            }
        }
    }

    return baseBreached;
}


int handleMenu(plants* pGrid[ROWS][COLS], zombies* zGrid[ROWS][COLS], int sun) {
    cout << "Menu:" << endl;
    // displays the options and cost of plants
    cout << "1. Buy Sunflower  (cost " << COST_SUNFLOWER << ", +25 sun/round)" << endl;
    cout << "2. Buy Peashooter (cost " << COST_PEASHOOTER << ")" << endl;
    cout << "3. Buy Chomper    (cost " << COST_CHOMPER << ")" << endl;
    cout << "4. Remove plant" << endl;
    cout << "5. Next round" << endl;
    cout << "0. Quit game" << endl;
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cout << endl;

    if (!cin) return -2; // invalid input, display menu again

    if (choice == 0) {
        return -1;  // Quit
    }

    // remove plant
    if (choice == 4) {
        int row, col;
        cout << "Enter row of plant to remove (0-" << ROWS - 1 << "): ";
        cin >> row;
        cout << "Enter column of plant to remove (0-" << COLS - 1 << "): ";
        cin >> col;

        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            cout << "Invalid position. Try again." << endl << endl;
            return -2;
        }

        if (pGrid[row][col] == nullptr) {
            cout << "No plant at that position. Try again." << endl << endl;
            return -2;
        }

        delete pGrid[row][col];
        pGrid[row][col] = nullptr;
        cout << "Plant removed from (" << row << ", " << col << ")." << endl << endl;
        return sun; 
    }

    // next round
    if (choice == 5) {
        return sun;    // valid action, no sun change
    }

    // buy plants
    if (choice < 1 || choice > 3) {
        cout << "Invalid menu choice. Please select an option from 0 to 5." << endl << endl;
        return -2;
    }

    //sun costs
    int cost = 0;
    if (choice == 1) cost = COST_SUNFLOWER;
    if (choice == 2) cost = COST_PEASHOOTER;
    if (choice == 3) cost = COST_CHOMPER;

    if (sun < cost) {
        cout << "You do not have enough sun for that plant. "
            << "You have " << sun << ", need " << cost << "." << endl << endl;
        return -2;
    }

    int row, col;
    cout << "Choose row (0-" << ROWS - 1 << "): ";
    cin >> row;
    cout << "Choose column (0-" << COLS - 1 << "): ";
    cin >> col;

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        cout << "Invalid position. Try again." << endl << endl;
        return -2;
    }

    if (pGrid[row][col] != nullptr) {
        cout << "That tile already has a plant. Try again." << endl << endl;
        return -2;
    }
    if (zGrid[row][col] != nullptr) {
        cout << "A zombie is standing there! Choose another tile." << endl << endl;
        return -2;
    }

    if (choice == 1) {
        pGrid[row][col] = new sunflower();
    }
    else if (choice == 2) {
        pGrid[row][col] = new peashooter();
    }
    else if (choice == 3) {
        pGrid[row][col] = new chomper();
    }

    sun -= cost;
    cout << "Plant placed at (" << row << ", " << col
        << "). Sun left: " << sun << endl << endl;

    return sun;
}

void cleanup(plants* pGrid[ROWS][COLS], zombies* zGrid[ROWS][COLS]) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            delete pGrid[r][c]; //delete plant at certain location
            delete zGrid[r][c]; //delete zombie at certain location
            pGrid[r][c] = nullptr;  //replaces plants with null pointer
            zGrid[r][c] = nullptr;  //replaces zombies with null pointer
        }
    }
}




int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    plants* pGrid[ROWS][COLS];
    zombies* zGrid[ROWS][COLS];

    initGrids(pGrid, zGrid);

    int round = 1;
    int sun = SUN_START;
    bool running = true;

    while (running) {
        if (round > MAX_ROUNDS) {
            cout << "You survived all " << MAX_ROUNDS << " rounds! You WIN! " << endl;
            running = false;
            break;
            ++round;
        }
        // suns each round
        sun += 25;
        cout << "You gained 25 sun this round!" << endl;

        // Zombies attack and move
        if (zombiesAttackAndMove(pGrid, zGrid)) {
            cout << endl << "A zombie reached your house! Game over." << endl;
            break;
        }

        // Spawn a new zombie at right side
        spawnZombie(zGrid);

        //  Sun from sunflowers
        int gained = sunflowerProduction(pGrid);
        sun += gained;

        // Plants attack zombies
        plantsAttack(pGrid, zGrid);

        // Show grid
        displayGrid(pGrid, zGrid, round, sun, gained);

        // Menu loop: keep asking until valid action
        while (true) {
            int res = handleMenu(pGrid, zGrid, sun);

            if (res == -1) {        // quit
                running = false;
                break;
            }
            else if (res == -2) { // invalid / not enough sun / bad pos
                continue;
            }
            else {
                // valid action, update sun and go to next round
                sun = res;
                break;
            }
        }
        if (!running) {
            break;
        }

        ++round;
    }

   
    cleanup(pGrid, zGrid);
    //ending displays final amount of suns
    cout << "Thanks for playing! Final sun: " << sun << endl;
    return 0;
}
