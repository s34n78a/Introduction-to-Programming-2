#include <utility>
#include <algorithm>
#include "state.hpp"
#include "114006263_pvs.hpp"

#include <map>

// Simple MVV-LVA scoring matrix based on piece types (1=P, 2=R, 3=N, 4=B, 5=Q, 6=K)
static const int mvv_lva_values[7] = {0, 10, 50, 30, 30, 90, 900};

int score_move_mvv_lva(State* state, const Move& m) {
    int from_r = m.first.first;
    int from_c = m.first.second;
    int to_r = m.second.first;
    int to_c = m.second.second;

    int attacker = 0;
    int victim = 0;
    int current_player = state->player;
    int opponent = 1 - current_player;

    // Identify the attacking piece
    if (state->board.board[current_player][from_r][from_c] > 0) {
        attacker = state->board.board[current_player][from_r][from_c];
    }

    // Identify if a piece is being captured (victim)
    if (state->board.board[opponent][to_r][to_c] > 0) {
        victim = state->board.board[opponent][to_r][to_c];
    }

    // If it's a capture, prioritize higher victim value and lower attacker value
    if (victim > 0) {
        return 10000 + (mvv_lva_values[victim] * 10) - mvv_lva_values[attacker];
    }

    // Secondary priority: Promote pawns early (moving closer to promotion rank)
    if (attacker == 1) {
        // White pawns move up (row decreases), Black pawns move down (row increases)
        return (current_player == 0) ? (BOARD_H - to_r) : to_r;
    }

    return 0; // Standard quiet move
}

int PVSSearch::quiescence(
    State *state,
    int alpha,
    int beta,
    int ply,
    SearchContext& ctx,
    const MMParams& p
) {
    ctx.nodes++;
    int stand_pat = state->evaluate(p.use_kp_eval, p.use_eval_mobility, nullptr);
    
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    if (state->legal_actions.empty() && state->game_state == UNKNOWN) {
        state->get_legal_actions();
    }

    for (auto& action : state->legal_actions) {
        // MVV-LVA capturing logic approximation: verify if destination contains an enemy piece
        int target_p1 = state->board.board[0][action.second.first][action.second.second];
        int target_p2 = state->board.board[1][action.second.first][action.second.second];
        
        if (target_p1 == 0 && target_p2 == 0) {
            continue; // Skip non-capturing moves during volatile horizon extension
        }

        State* next = state->next_state(action);
        int score = -quiescence(next, -beta, -alpha, ply + 1, ctx, p);
        delete next;

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

int PVSSearch::pvs(
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
    if (ply > ctx.seldepth) ctx.seldepth = ply;
    if (ctx.stop) return 0;

    if (state->legal_actions.empty() && state->game_state == UNKNOWN) {
        state->get_legal_actions();
    }
    if (state->game_state == WIN) return P_MAX - ply;
    if (state->game_state == DRAW) return 0;

    int rep_score;
    if (state->check_repetition(history, rep_score)) return rep_score;

    if (depth <= 0) {
        return quiescence(state, alpha, beta, ply, ctx, p);
    }

    history.push(state->hash());

    auto actions = state->legal_actions;

    // Sort moves from highest score to lowest score
    std::sort(actions.begin(), actions.end(), [state](const Move& a, const Move& b) {
        return score_move_mvv_lva(state, a) > score_move_mvv_lva(state, b);
    });

    bool b_search_pv = true;

    for (size_t i = 0; i < actions.size(); ++i) {
        State* next = state->next_state(actions[i]);
        int score;

        if (b_search_pv) {
            score = -pvs(next, depth - 1, -beta, -alpha, history, ply + 1, ctx, p);
        } else {
            // Null Window Search
            score = -pvs(next, depth - 1, -alpha - 1, -alpha, history, ply + 1, ctx, p);
            if (score > alpha && score < beta) {
                // Re-search window fallback
                score = -pvs(next, depth - 1, -beta, -alpha, history, ply + 1, ctx, p);
            }
        }
        delete next;

        if (score >= beta) {
            history.pop(state->hash());
            return beta; // Prune
        }
        if (score > alpha) {
            alpha = score;
            b_search_pv = false;
        }
    }

    history.pop(state->hash());
    return alpha;
}

SearchResult PVSSearch::search(
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

    std::sort(actions.begin(), actions.end(), [state](const Move& a, const Move& b) {
        return score_move_mvv_lva(state, a) > score_move_mvv_lva(state, b);
    });

    int best_score = M_MAX;
    int alpha = M_MAX;
    int beta = P_MAX;

    for (size_t i = 0; i < actions.size(); ++i) {
        State* next = state->next_state(actions[i]);
        int score;
        if (i == 0) {
            score = -pvs(next, depth - 1, -beta, -alpha, history, 1, ctx, p);
        } else {
            score = -pvs(next, depth - 1, -alpha - 1, -alpha, history, 1, ctx, p);
            if (score > alpha) {
                score = -pvs(next, depth - 1, -beta, -alpha, history, 1, ctx, p);
            }
        }
        delete next;

        if (score > best_score) {
            best_score = score;
            result.best_move = actions[i];
            result.score = score;
            if (p.report_partial && ctx.on_root_update) {
                ctx.on_root_update({result.best_move, best_score, depth, (int)i + 1, (int)actions.size()});
            }
        }
        alpha = std::max(alpha, score);
    }

    result.score = best_score;
    result.depth = depth;
    result.nodes = ctx.nodes;
    return result;
}

ParamMap PVSSearch::default_params() {
    return {{"UseKPEval", "true"}, {"UseEvalMobility", "true"}, {"ReportPartial", "true"}};
}

std::vector<ParamDef> PVSSearch::param_defs() {
    return {
        {"UseKPEval", ParamDef::CHECK, "true"},
        {"UseEvalMobility", ParamDef::CHECK, "true"},
        {"ReportPartial", ParamDef::CHECK, "true"}
    };
}