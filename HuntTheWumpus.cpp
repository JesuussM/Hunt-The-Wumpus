/*
  Program Name: HuntTheWumpus.cpp
  Description: A game where the player must find and kill the Wumpus in a cave.
  Author: Jesus Miranda
  Sources: 
    1. The random number generator: https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
    2. The vector: https://www.geeksforgeeks.org/vector-in-cpp-stl/
    3. Removing element from vector: https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

using namespace std;

// Declare Variables
vector<string> Caves[20];
string Player = "Player";
string Bat = "Bat";
string Pit = "Pit";
string Wumpus = "Wumpus";
int currentCave = 0;
int previousCave = -1;
int randomNumber = 0;
int playingGame = 1;
int batMovedPlayer = 0;
int arrowCount = 5;

// Function to print Caves at start of game that I used for testing
// void printCave(const vector<string> Caves[], int size) {
//     for (int i = 0; i < size; i++) {
//         cout << "Cave " << i << ": ";
//         for (const auto& entity : Caves[i]) {
//             cout << entity << " ";
//         }
//         cout << endl;
//     }
// }

// Function to get connected caves
vector<int> calculateConnectedCaves(int currentCave, int previousCave) {
    vector<int> connectedCaves;
    // If player has moved then connected caves are the previous cave and the next two caves
    if (previousCave != -1) {
        connectedCaves.push_back(previousCave);
        connectedCaves.push_back((currentCave + 1) % 20);
        connectedCaves.push_back((currentCave + 2) % 20);
    // If bat moved the player then player cannot go back to previous cave
    } else if (batMovedPlayer == 1) {
        connectedCaves.push_back((currentCave - 1) % 20);
        connectedCaves.push_back((currentCave + 1) % 20);
        connectedCaves.push_back((currentCave + 2) % 20);
    // If player has not moved then connected caves are the first three caves
    } else {
        connectedCaves.push_back((currentCave + 1) % 20);
        connectedCaves.push_back((currentCave + 2) % 20);
        connectedCaves.push_back((currentCave + 3) % 20);
    }

    // Checks for nearby entities
    for (int cave : connectedCaves) {
        for (const string& entity : Caves[cave]) {
            if (entity == Wumpus) {
                cout << "You smell the Wumpus nearby." << endl;
                cout << endl;
            } else if (entity == Bat) {
                cout << "You hear a bat nearby." << endl;
                cout << endl;
            } else if (entity == Pit) {
                cout << "You feel a cold wind blowing from a pit." << endl;
                cout << endl;
            }
        }
    }

    return connectedCaves;
}

// Game over function
void gameOver() {
    cout << "Game Over!" << endl;
    cout << endl;
    exit(0);
}

// Function to move player to new cave
void movePlayer(int newCave) {
    // Erases player form current cave and adds player to new cave
    auto it = std::remove(Caves[currentCave].begin(), Caves[currentCave].end(), Player);
    Caves[currentCave].erase(it, Caves[currentCave].end());
    previousCave = currentCave;
    currentCave = newCave;
    Caves[currentCave].push_back("Player");

    // Checking for entities in the cave
    for (const auto& entity : Caves[currentCave]) {
        if (entity == "Bat") {
            cout << "There's a bat in this cave!" << endl;
            cout << "The bat picked you up and dropped you in a new cave!" << endl;
            cout << endl;
            batMovedPlayer = 1;

            // Move bat to a random cave
            randomNumber = rand() % 19 + 1;
            auto it = std::remove(Caves[currentCave].begin(), Caves[currentCave].end(), Bat);
    	    Caves[currentCave].erase(it, Caves[currentCave].end());
            Caves[randomNumber].push_back(Bat);

            // Move player to a random cave
            randomNumber = rand() % 19 + 1;
            movePlayer(randomNumber);
        } else if (entity == "Pit") {
            cout << "There is a pit in this cave!" << endl;
            cout << "You have fallen into a pit!" << endl;
            cout << endl;
            // Player falls into pit and game is over
            gameOver();
        } else if (entity == "Wumpus") {
            cout << "There is a Wumpus in this cave!" << endl;
            cout << endl;

            // 75% chance that Wumpus kills the player
            // 25% chance that Wumpus moves to another cave
            randomNumber = rand() % 100;
            if (randomNumber < 75) {
                cout << "You startled the Wumpus! The Wumpus ran to another cave!" << endl;
                cout << endl;

                // Moves the Wumpus to a random cave
                randomNumber = rand() % 19 + 1;
                auto it = std::remove(Caves[currentCave].begin(), Caves[currentCave].end(), Wumpus);
    		Caves[currentCave].erase(it, Caves[currentCave].end());
                Caves[randomNumber].push_back(Wumpus);
            } else {
                cout << "The Wumpus killed you!" << endl;
                gameOver();
            }
        }
    }
}

// Function to shoot arrow into a cave
void shoot(int chosenCave) {
    // If arrow hits the Wumpus
    if (find(Caves[chosenCave].begin(), Caves[chosenCave].end(), Wumpus) != Caves[chosenCave].end()) {
        cout << endl;
        cout << "You have killed the Wumpus!" << endl;
        cout << "You have won the game!" << endl;
        cout << endl;
        gameOver();
    } else {
        arrowCount--;
        cout << "The arrow did not it anything." << endl;
        cout << "You have " << arrowCount << " arrows left." << endl;
        cout << endl;

        // 75% chance that Wumpus moves to another cave
        // 25% chance that Wumpus stays in the same cave
        randomNumber = rand() % 100;
        if (randomNumber < 75) {
            cout << "The Wumpus did not move!" << endl;
            cout << endl;
        } else {
            cout << "The Wumpus has been startled and moved to another cave!" << endl;
            cout << endl;
            randomNumber = rand() % 19 + 1;
            auto it = std::remove(Caves[currentCave].begin(), Caves[currentCave].end(), Wumpus);
    	    Caves[currentCave].erase(it, Caves[currentCave].end());
            Caves[randomNumber].push_back(Wumpus);
        }
    }
}

int main() {

    // Makes sure random is really random
    srand(time(0));
    
    // Placeing entities in caves
    Caves[0].push_back(Player);

    randomNumber = rand() % 19 + 1;
    Caves[randomNumber].push_back(Bat);

    randomNumber = rand() % 19 + 1;
    Caves[randomNumber].push_back(Bat);

    randomNumber = rand() % 19 + 1;
    Caves[randomNumber].push_back(Pit);

    randomNumber = rand() % 19 + 1;
    Caves[randomNumber].push_back(Pit);

    randomNumber = rand() % 19 + 1;
    Caves[randomNumber].push_back(Wumpus);

    // printCave(Caves, 20);

    while (playingGame == 1) {
        // Check if player ran out of arrows
        if (arrowCount == 0) {
            cout << endl;
            cout << "You have run out of arrows!" << endl;
            cout << "You have lost the game!" << endl;
            cout << endl;
            exit(0);
        }

        batMovedPlayer = 0;
        cout << "=========  HUNT THE WUMPUS  =========" << endl;
        cout << endl;
        cout << "You are in cave " << currentCave << endl;
        cout << endl;

        vector<int> connectedCaves = calculateConnectedCaves(currentCave, previousCave);
        
        // Display options
        cout << "What would you like to do?" << endl;
        cout << "(M)ove to another cave" << endl;
        cout << "(S)hoot an arrow" << endl;
        cout << "(Q)uit" << endl;
        cout << endl;
        cout << "Enter the letter you want to select: ";
        string option = "";
        cin >> option;

        cout << endl;
        if (option == "m" || option == "M") {
            // Print connected caves
            cout << "Tunnels lead to caves ";
            cout << endl;
            for (const auto& cave : connectedCaves) {
                cout << cave << " ";
            }
            cout << endl;
            cout << endl;

            cout << "Enter the cave you would like to move to: ";
            int newCave = 0;
            cin >> newCave;
            cout << endl;
            // Check if new cave is connected
            if (find(connectedCaves.begin(), connectedCaves.end(), newCave) != connectedCaves.end()) {
                movePlayer(newCave);
            } else {
                cout << endl;
                cout << "Invalid cave selection!" << endl;
                cout << endl;
            }
        } else if (option == "s" || option == "S") {
            // Print connected caves
            cout << "Tunnels lead to caves ";
            cout << endl;
            for (const auto& cave : connectedCaves) {
                cout << cave << " ";
            }
            cout << endl;
            cout << endl;

            cout << "Enter the cave you would like to shoot the arrow to: ";
            int chosenCave = 0;
            cin >> chosenCave;
            // Check if new cave is connected
            if (find(connectedCaves.begin(), connectedCaves.end(), chosenCave) != connectedCaves.end()) {
                shoot(chosenCave);
            } else {
                cout << endl;
                cout << "Invalid cave selection!" << endl;
                cout << endl;
            }
        } else if (option == "q" || option == "Q") {
            cout << "You have quit the game!" << endl;
            playingGame = 0;
        } else {
            cout << "Invalid option!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    exit(0);
}
