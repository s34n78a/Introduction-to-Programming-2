#pragma once
/*============================================================
 * Algorithm Registry
 *
 * Each algorithm defines:
 *   - search() function
 *   - default_params() returning ParamMap
 *   - param_defs() for UCI option advertisement
 *============================================================*/

#include <string>
#include <functional>
#include <vector>
#include "search_types.hpp"
#include "game_history.hpp"
#include "minimax.hpp"
#include "random.hpp"

// TO DO: Add new algorithms and register them in get_algo_table()
#include "114006263_alphabeta.hpp"
#include "114006263_pvs.hpp"
#include "114006263_submission.hpp"

struct AlgoEntry {
    std::string name;
    ParamMap default_params;
    std::vector<ParamDef> param_defs;
    std::function<SearchResult(State*, int, GameHistory&, SearchContext&)> search;
};

inline const std::vector<AlgoEntry>& get_algo_table(){
    static const std::vector<AlgoEntry> table = {
        {
            "minimax",
            MiniMax::default_params(),
            MiniMax::param_defs(),
            [](State* s, int d, GameHistory& h, SearchContext& c){
                return MiniMax::search(s, d, h, c);
            }
        },
        {
            "random",
            Random::default_params(),
            Random::param_defs(),
            [](State* s, int d, GameHistory& h, SearchContext& c){
                return Random::search(s, d, h, c);
            }
        },

        // TO DO: Add new algorithms here
        {
            "alphabeta", // Checkpoint Baseline Entry
            AlphaBetaSearch::default_params(),
            AlphaBetaSearch::param_defs(),
            [](State* s, int d, GameHistory& h, SearchContext& c){
                return AlphaBetaSearch::search(s, d, h, c);
            }
        },
        {
            "pvs",       // Advanced Checkpoint Entry
            PVSSearch::default_params(),
            PVSSearch::param_defs(),
            [](State* s, int d, GameHistory& h, SearchContext& c){
                return PVSSearch::search(s, d, h, c);
            }
        },
        {
            "submission", // Final Grading Target Matcher
            SubmissionSearch::default_params(),
            SubmissionSearch::param_defs(),
            [](State* s, int d, GameHistory& h, SearchContext& c){
                return SubmissionSearch::search(s, d, h, c);
            }
        },
    };
    return table;
}

inline const AlgoEntry* find_algo(const std::string& name){
    for(auto& entry : get_algo_table()){
        if(entry.name == name){
            return &entry;
        }
    }
    return nullptr;
}

inline std::string default_algo_name(){ return "minimax"; }
