#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <iterator>
#include <string>
#include <sstream>

using namespace std;
using State = vector<int>;

class Crossing
{
private:
    vector<int> _npeople;
    set<list<State>> _paths; // trial paths in progress
    set<State> _explored; // explored states
    set<list<State>> _solutions;

public:
    Crossing(vector<int> np): _npeople {np} { }
    State Go(State s, int missionary, int cannibal);
    // check the validity of a state
    bool valid(State s);
    set<State> extend(State s);
    // check if all people are at the right bank
    bool found(State s);
    void solve();
    void show_solutions();
};//end class Crossing

int main()
{

    vector<int> people = {3, 3};
    Crossing p(people);
    p.solve();
    p.show_solutions();

}

State Crossing::Go(State s, int missionary, int cannibal)
{
    s[0] = s[0] + s[4]*missionary;
    s[1] = s[1] + s[4]*cannibal;
    s[2] = s[2] - s[4]*missionary;
    s[3] = s[3] - s[4]*cannibal;
    s[4] = -s[4];
    return s;
}

bool Crossing::valid(State s)
{
    for (int i=0; i<2; ++i) {
        if (s[i] < 0 || s[i] > _npeople[i])
            return false;
    }
    // Check left bank: if missionaries present, cannibals can't outnumber them
    if (s[0] > 0 && s[1] > s[0])
        return false;
    // Check right bank: if missionaries present, cannibals can't outnumber them
    if (s[2] > 0 && s[3] > s[2])
        return false;
    return true;
}

bool Crossing::found(State s)
{
    return s[0] == 0 && s[1] == 0;
}

set<State> Crossing::extend(State s)
{
    set<State> nextStates;
    
    for (int m = 0; m <= 2; ++m) {
        for (int c = 0; c <= 2; ++c) {
            // at least one person must be on the boat, and at most two people can be on the boat
            if (m + c > 0 && m + c <= 2) {
                // Check if we have enough people on the current bank to move
                if (s[4] == -1 && (m > s[0] || c > s[1])) continue;  // boat on left
                if (s[4] == 1 && (m > s[2] || c > s[3])) continue;   // boat on right
                
                State newState = Go(s, m, c);
                if (valid(newState))
                    nextStates.insert(newState);
            }
        }
    }
    return nextStates;
}

void Crossing::solve()
{
    list<State> path;
    path.push_back({3, 3, 0, 0, -1}); // initial state: 3 missionaries and 3 cannibals on the left bank, boat on the left bank
    _paths.insert(path);
    
    while (!_paths.empty()) {
        auto it = _paths.begin();
        path = *it;
        _paths.erase(it);
        State s = path.back();
        
        if (_explored.find(s) == _explored.end()) {
            _explored.insert(s);
            if (found(s)) {
                _solutions.insert(path);
            }
            set<State> nextStates = extend(s);
            for (auto p : nextStates) {
                list<State> newPath(path);
                newPath.push_back(p);
                _paths.insert(newPath);
            }
        }
    }
}

void Crossing::show_solutions()
{
    for (auto path : _solutions) {
        for (auto state : path) {
            cout << "(" << state[0] << "," << state[1] << "," << state[2] << "," << state[3] << "," << state[4] << ") -> ";
        }
        cout << "\n";
    }
}