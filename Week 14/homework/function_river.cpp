#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include "function_river.h"

using namespace std;

using std::vector;
using std::set;
using std::list;
using std::cout;
using std::endl;

// A state contains seven components:
// The first three components denote the current numbers of
// wolves, goats and cabbages at the left bank of the river.
// The fourth to sixth components denote the current numbers
// of wolves, goats and cabbages at the right bank.
// The seventh component denotes the location of the boat:
// 1 means "left bank" and -1 means "right bank".
using State = vector<int>;

vector<int> _items;        // wolves, goats and cabbages
                           // we use vector<int> as a tuple (int, int, int)
                           // the first integer indicates the number of wolves
                           // the second integer indicates the number of goats
                           // the third integer indicates the number of cabbages.
set<list<State>> _paths;   // trial paths in progress
set<State> _explored;      // explored states
set<list<State>> _solutions;

// specify the numbers of wolves, goats and cabbages
Crossing::Crossing(vector<int> np): _items {np} { 
    
}

// the starting point of your implementation
void Crossing::solve() {

}

// extend to other possible states from a certain state
set<State> Crossing::extend(State s) {

}

// may use s[6] to indicate the direction of move
State Crossing::Go(State s, int wolf, int goat, int cabbage) {

}

// check the validity of a state
bool Crossing::valid(State s) {

}

// check if all people are at the right bank
bool Crossing::found(State s) {

}
