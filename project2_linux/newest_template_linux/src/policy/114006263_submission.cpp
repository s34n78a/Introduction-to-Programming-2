#include "state.hpp"
#include "114006263_submission.hpp"
#include "114006263_pvs.hpp" // Root execution through the high-performance pipeline

SearchResult SubmissionSearch::search(
    State *state,
    int depth,
    GameHistory& history,
    SearchContext& ctx
) {
    // Routes evaluation into the advanced PVS branch
    return PVSSearch::search(state, depth, history, ctx);
}

ParamMap SubmissionSearch::default_params() {
    return PVSSearch::default_params();
}

std::vector<ParamDef> SubmissionSearch::param_defs() {
    return PVSSearch::param_defs();
}