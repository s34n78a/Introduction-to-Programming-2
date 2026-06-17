#include <utility>
#include <algorithm>
#include "state.hpp"
#include "114006263_alphabeta.hpp"

int AlphaBetaSearch::alpha_beta(
    State *state,
    int depth,
    int alpha,
    int beta,
    GameHistory& history,
    int ply,
    SearchContext& ctx,
    const MMParams& p
) {
    ctx.nodes++;
    if (ply > ctx.seldepth) {
        ctx.seldepth = ply;
    }
    if (ctx.stop) {
        return 0;
    }

    if (state->legal_actions.empty() && state->game_state == UNKNOWN) {
        state->get_legal_actions();
    }

    if (state->game_state == WIN) {
        return P_MAX - ply; // Prefer faster checkmates
    }
    if (state->game_state == DRAW) {
        return 0;
    }

    int rep_score;
    if (state->check_repetition(history, rep_score)) {
        return rep_score;
    }

    if (depth <= 0) {
        return state->evaluate(p.use_kp_eval, p.use_eval_mobility, &history);
    }

    history.push(state->hash());

    int best_score = M_MAX;
    auto actions = state->legal_actions;

    // Simple Move Ordering: captures first
    std::sort(actions.begin(), actions.end(), [](const Move& /*m1*/, const Move& /*m2*/) { //So no warning for unused parameters
        // Fallback or custom sorting can go here if state handles pieces
        return false; 
    });

    for (auto& action : actions) {
        State* next = state->next_state(action);
        int score = -alpha_beta(next, depth - 1, -beta, -alpha, history, ply + 1, ctx, p);
        delete next;

        if (score > best_score) {
            best_score = score;
        }
        alpha = std::max(alpha, score);
        if (alpha >= beta) {
            break; // Beta Cutoff
        }
    }

    history.pop(state->hash());
    return best_score;
}

SearchResult AlphaBetaSearch::search(
    State *state,
    int depth,
    GameHistory& history,
    SearchContext& ctx
) {
    ctx.reset();
    MMParams p = MMParams::from_map(ctx.params);
    SearchResult result;
    
    if (state->legal_actions.empty() && state->game_state == UNKNOWN) {
        state->get_legal_actions();
    }

    auto actions = state->legal_actions;
    if (actions.empty()) {
        result.best_move = Move();
        result.score = M_MAX;
        return result;
    }

    int best_score = M_MAX;
    int alpha = M_MAX;
    int beta = P_MAX;
    int total_moves = actions.size();
    int move_index = 0;

    for (auto& action : actions) {
        State* next = state->next_state(action);
        int score = -alpha_beta(next, depth - 1, -beta, -alpha, history, 1, ctx, p);
        delete next;

        if (score > best_score) {
            best_score = score;
            result.best_move = action;
            result.score = score;

            if (p.report_partial && ctx.on_root_update) {
                ctx.on_root_update({result.best_move, best_score, depth, move_index + 1, total_moves});
            }
        }
        alpha = std::max(alpha, score);
    }

    result.score = best_score;
    result.depth = depth;
    result.nodes = ctx.nodes;
    return result;
}

ParamMap AlphaBetaSearch::default_params() {
    return {{"UseKPEval", "true"}, {"UseEvalMobility", "true"}, {"ReportPartial", "true"}};
}

std::vector<ParamDef> AlphaBetaSearch::param_defs() {
    return {
        {"UseKPEval", ParamDef::CHECK, "true"},
        {"UseEvalMobility", ParamDef::CHECK, "true"},
        {"ReportPartial", ParamDef::CHECK, "true"}
    };
}