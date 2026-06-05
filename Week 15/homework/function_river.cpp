#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <functional>
#include "function_river.h"

using std::vector;
using std::set;
using std::list;

using State = vector<int>;

// the starting point of your implementation
void Crossing::solve() {
    // initial state: all items on left bank, none on right, boat on left (1)
    State start(7,0);
    start[0] = this->_items[0];
    start[1] = this->_items[1];
    start[2] = this->_items[2];
    start[3] = 0;
    start[4] = 0;
    start[5] = 0;
    start[6] = 1; // boat on left

    std::list<State> path;
    path.push_back(start);

    // recursive DFS with per-path visited checking to avoid duplicate states in a solution
    std::function<void(const State&, std::list<State>&)> dfs = [&](const State &s, std::list<State> &curPath) {
        if (found(s)) {
            this->_solutions.insert(curPath);
            return;
        }
        // generate extensions
        auto nexts = extend(s);
        for (auto ns : nexts) {
            // check duplicates in current path
            bool seen = false;
            for (auto &ps : curPath) {
                if (ps == ns) { seen = true; break; }
            }
            if (seen) continue;
            curPath.push_back(ns);
            dfs(ns, curPath);
            curPath.pop_back();
        }
    };

    dfs(start, path);
}

// extend to other possible states from a certain state
set<State> Crossing::extend(State s) {
    set<State> result;
    // possible cargo: none, wolf, goat, cabbage (at most one)
    int dir = s[6];
    // (wolf,goat,cabbage)
    vector<vector<int>> choices = {{0,0,0},{1,0,0},{0,1,0},{0,0,1}};
    for (auto &c : choices) {
        int w = c[0], g = c[1], cb = c[2];
        // check availability on source bank
        if (dir == 1) {
            if (s[0] < w || s[1] < g || s[2] < cb) continue;
        } else {
            if (s[3] < w || s[4] < g || s[5] < cb) continue;
        }
        State ns = Go(s, w, g, cb);
        if (ns != s && valid(ns)) result.insert(ns);
    }
    return result;
}

// may use s[6] to indicate the direction of move
State Crossing::Go(State s, int wolf, int goat, int cabbage) {
    State ns = s;
    if (s[6] == 1) {
        // move from left to right
        ns[0] -= wolf;
        ns[1] -= goat;
        ns[2] -= cabbage;
        ns[3] += wolf;
        ns[4] += goat;
        ns[5] += cabbage;
        ns[6] = -1;
    } else {
        // move from right to left
        ns[3] -= wolf;
        ns[4] -= goat;
        ns[5] -= cabbage;
        ns[0] += wolf;
        ns[1] += goat;
        ns[2] += cabbage;
        ns[6] = 1;
    }
    return ns;
}

// check the validity of a state
bool Crossing::valid(State s) {
    // counts must be non-negative and not exceed totals
    for (int i = 0; i < 6; ++i) if (s[i] < 0) return false;
    if (s[0] + s[3] != this->_items[0]) return false;
    if (s[1] + s[4] != this->_items[1]) return false;
    if (s[2] + s[5] != this->_items[2]) return false;

    // check safety: if farmer not on left bank, left bank must be safe
    if (s[6] == -1) {
        if (s[1] > 0 && s[0] > s[1]) return false; // wolves eat goats
        if (s[2] > 0 && s[1] > s[2]) return false; // goats eat cabbages
    }
    // if farmer not on right bank, right bank must be safe
    if (s[6] == 1) {
        if (s[4] > 0 && s[3] > s[4]) return false;
        if (s[5] > 0 && s[4] > s[5]) return false;
    }
    return true;
}

// check if all people are at the right bank
bool Crossing::found(State s) {
    // all items moved to right bank and boat on right
    if (s[0] == 0 && s[1] == 0 && s[2] == 0 &&
        s[3] == this->_items[0] && s[4] == this->_items[1] && s[5] == this->_items[2] && s[6] == -1)
        return true;
    return false;
}
