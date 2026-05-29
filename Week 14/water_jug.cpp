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

class Pouring
{
private:
    vector<int> _capacities; // {3, 5, 7}
    set<State> _explored;
    set<list<State>> _paths;
    set<list<State>> _solutions;

public:

    Pouring(vector<int> cp): _capacities{cp} { }
    State Empty(State s, int jug_no);
    State Fill(State s, int jug_no);
    State Pour(State s, int from, int to);
    set<State> extend(State s);//return the set of all possible next states of s
    void show_state(State s);
    bool found(State s, int target);//if found, return 1
    void solve(int target, int steps);
    void show_solutions();
};//end class Pouring

void Pouring::solve(int target, int steps)
{
    list<State> initialPath;
    initialPath.push_back(State(_capacities.size()));
    _paths.insert(initialPath);
    
    while (steps > 0) {
        set<list<State>> newPaths;
        set<list<State>> oldPaths;
        
        for (auto p : _paths) {
            _explored.insert(p.back());//last elem. of p
            auto nextStates = extend(p.back());

            for (auto s : nextStates) {
                if (found(s, target)) {
                    auto np = p;
                    np.push_back(s);
                    _solutions.insert(np);
                } else {
                    auto search = _explored.find(s);
                    if (search == _explored.cend()) {
                        auto np = p;
                        np.push_back(s);
                        newPaths.insert(np);
                    }
                }
            }
            oldPaths.insert(p);
        }
        for (auto p : oldPaths) {
            _paths.erase(p);
        }
        for (auto p : newPaths) {
            _paths.insert(p);
        }
        --steps;
    }
}

set<State> Pouring::extend(State s)
{
    set<State> SS;
    
    for (int i=0; i<_capacities.size(); ++i) {
        SS.insert(Empty(s, i));
        SS.insert(Fill(s, i));
        for (int j=0; j<_capacities.size(); ++j) {
            if (i!=j)
                SS.insert(Pour(s, i, j));
        }
    }

    return SS;
}

void Pouring::show_solutions()
{
    for (auto path : _solutions) {
        for (auto state : path) {
            show_state(state);
        }
        cout << "\n";
    }
}

void Pouring::show_state(State s)
{
    for (auto i : s)
        cout << i << ", " ;
    cout << "->";
}

int main()
{
    vector<int> jugs = {3, 5, 7};
    Pouring problem(jugs);
    problem.solve(4, 3);
    problem.show_solutions();
    return 0;

}

State Pouring::Empty(State s, int jug_no)
{
    State newState = s;
    newState[jug_no] = 0;
    return newState;
}

State Pouring::Fill(State s, int jug_no)
{
    State newState = s;
    newState[jug_no] = _capacities[jug_no];
    return newState;
}

State Pouring::Pour(State s, int from, int to)
{
    State newState = s;
    int amountToPour = min(s[from], _capacities[to] - s[to]);
    newState[from] -= amountToPour;
    newState[to] += amountToPour;
    return newState;
}

bool Pouring::found(State s, int target)
{
    for (auto i : s) {
        if (i == target)
            return true;
    }
    return false;
}