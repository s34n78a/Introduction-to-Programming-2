#include <stdio.h>

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Piece {
    char type;
    Position pos;
} Piece;

typedef struct Move {
    Position start;
    Position end;
} Move;

typedef struct GameState {
    int player_count;
    Piece player[16];
    int opponent_count;
    Piece opponent[16];
    Move moves[2];
} GameState;

typedef struct SearchResult {
    int min_steps_to_checkmate;
    Move moves[2];
} SearchResult;

// If you would like to solve with BFS
// typedef struct QueueNode {
//     GameState state;
//     int step;
// } QueueNode;

int QUEEN_DIRS[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
int ROOK_DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
int KNIGHT_DIRS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
int BISHOP_DIRS[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

int is_movable_piece(char type) {
    return type == 'Q' || type == 'R' || type == 'B' || type == 'N';
}

void get_piece_dirs(char type, int (**dirs)[2], int *len_dirs) {
    if (type == 'Q') {
        *dirs = QUEEN_DIRS;
        *len_dirs = 8;
    }
    else if (type == 'R') {
        *dirs = ROOK_DIRS;
        *len_dirs = 4;
    }
    else if (type == 'B') {
        *dirs = BISHOP_DIRS;
        *len_dirs = 4;
    }
    else {
        *dirs = KNIGHT_DIRS;
        *len_dirs = 8;
    }
}

int check_new_position(Position target_pos, const GameState *state) {
    // check player's pieces
    for (int piece_id = 0; piece_id < state->player_count; piece_id++)
    {
        if (target_pos.x == state->player[piece_id].pos.x && target_pos.y == state->player[piece_id].pos.y)
        {
            //printf("(%d, %d) is blocked by player's %c piece at position (%d, %d)\n", target_pos.x, target_pos.y, state.player[piece_id].type, state.player[piece_id].pos.x, state.player[piece_id].pos.y); // debug print
            return 1;
        }
    }

    //printf("check opponent's pieces for position (%d, %d)\n", target_pos.x, target_pos.y); // debug print
    // check opponent's pieces
    for (int piece_id = 0; piece_id < state->opponent_count; piece_id++)
    {
        if (target_pos.x == state->opponent[piece_id].pos.x && target_pos.y == state->opponent[piece_id].pos.y)
        {
            //printf("Blocked by opponent's %c piece at position (%d, %d)\n", state.opponent[piece_id].type, state.opponent[piece_id].pos.x, state.opponent[piece_id].pos.y); // debug print
            if (state->opponent[piece_id].type == 'K')
            {
                return 2;
            }
            
            return 1;
        }
    }
    
    return 0;
}

int check_piece_can_capture(Piece current_piece, const GameState *state) {
    // Implement the logic to check if the piece can capture the opponent's king
    // based on its type and the current game state.
    // This function should return 1 if it can capture, and 0 otherwise.
    
    int (*dirs)[2]; // pointer to the direction array for the current piece type
    int len_dirs = 0; // length of the direction array for the current piece type

    // type Q, R, B (similar logic with unlimitd range, just different directions):
    if (current_piece.type == 'Q' || current_piece.type == 'R' || current_piece.type == 'B')
    {
        get_piece_dirs(current_piece.type, &dirs, &len_dirs);

        //printf("(%d,%d)", dirs[4][0], dirs[4][1]); // debug print
        for (int dir_id = 0; dir_id < len_dirs; dir_id++)
        {
            //printf("Checking direction (%d, %d) for piece type %c\n", dirs[dir_id][0], dirs[dir_id][1], current_piece.type); // debug print
            for (int dist = 1; dist < 8; dist++)
            {
                Position new_pos;

                new_pos.x = current_piece.pos.x + dirs[dir_id][0]*dist;
                new_pos.y = current_piece.pos.y + dirs[dir_id][1]*dist;

                //boundary check
                if (new_pos.x < 0 || new_pos.x > 7 || new_pos.y < 0 || new_pos.y > 7)
                {
                    break;
                }

                int check = check_new_position(new_pos, state);

                //printf("Checking new position (%d, %d) for piece type %c: check result = %d\n", new_pos.x, new_pos.y, current_piece.type, check); // debug print
                
                if (check == 1)
                {
                    break; // blocked by other piece, can't move further in this direction
                }
                else if (check == 2)
                {
                    return 1; // can capture king
                }
            }
        }
    }

    // type N:
    else if (current_piece.type == 'N')
    {
        get_piece_dirs(current_piece.type, &dirs, &len_dirs);

        for (int dir_id = 0; dir_id < len_dirs; dir_id++)
        {
            Position new_pos;

            new_pos.x = current_piece.pos.x + dirs[dir_id][0];
            new_pos.y = current_piece.pos.y + dirs[dir_id][1];

            //boundary check
            if (new_pos.x < 0 || new_pos.x > 7 || new_pos.y < 0 || new_pos.y > 7)
            {
                continue;
            }

            int check = check_new_position(new_pos, state);
            
            if (check == 2)
            {
                return 1; // can capture king
            }
        }
    }

    return 0; // can't capture king
}

int find_one_move_capture(const GameState *state, Position enemy_king, Move *capture_move) {
    for (int piece_id = 0; piece_id < state->player_count; piece_id++)
    {
        if (!is_movable_piece(state->player[piece_id].type))
        {
            continue;
        }

        if (check_piece_can_capture(state->player[piece_id], state))
        {
            capture_move->start = state->player[piece_id].pos;
            capture_move->end = enemy_king;
            return 1;
        }
    }

    return 0;
}

int try_first_move_and_capture(GameState *state, int piece_id, Position enemy_king, SearchResult *result) {
    Piece current_piece = state->player[piece_id];
    Position original_pos = current_piece.pos;
    int (*dirs)[2];
    int len_dirs = 0;

    if (!is_movable_piece(current_piece.type))
    {
        return 0;
    }

    get_piece_dirs(current_piece.type, &dirs, &len_dirs);
    int is_knight = (current_piece.type == 'N');
    int max_dist = is_knight ? 1 : 7;

    for (int dir_id = 0; dir_id < len_dirs; dir_id++)
    {
        for (int dist = 1; dist <= max_dist; dist++)
        {
            Position new_pos;
            new_pos.x = original_pos.x + dirs[dir_id][0] * dist;
            new_pos.y = original_pos.y + dirs[dir_id][1] * dist;

            if (new_pos.x < 0 || new_pos.x > 7 || new_pos.y < 0 || new_pos.y > 7)
            {
                if (is_knight)
                {
                    continue;
                }
                break;
            }

            int check = check_new_position(new_pos, state);

            if (check == 0)
            {
                state->player[piece_id].pos = new_pos;

                if (find_one_move_capture(state, enemy_king, &result->moves[1]))
                {
                    result->min_steps_to_checkmate = 2;
                    result->moves[0].start = original_pos;
                    result->moves[0].end = new_pos;
                    state->player[piece_id].pos = original_pos;
                    return 1;
                }

                state->player[piece_id].pos = original_pos;
            }
            else if (check == 1)
            {
                if (is_knight)
                {
                    continue;
                }
                break;
            }
            else if (check == 2)
            {
                // 1-step capture is handled before calling this helper.
            }
        }
    }

    return 0;
}

void Check_Checkmate(GameState state, SearchResult *result) {
    Piece opponent_king;

    for (int piece_id = 0; piece_id < state.opponent_count; piece_id++)
    {
        if (state.opponent[piece_id].type == 'K')
        {
            opponent_king = state.opponent[piece_id];
        }
    }
    
    // check 1 move capture for all pieces
    if (find_one_move_capture(&state, opponent_king.pos, &result->moves[0]))
    {
        result->min_steps_to_checkmate = 1;
        return;
    }
    
    //check 2 move capture
    for (int piece_id = 0; piece_id < state.player_count; piece_id++)
    {
        if (try_first_move_and_capture(&state, piece_id, opponent_king.pos, result))
        {
            return;
        }
    }
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        char type, pos_x, pos_y;
        GameState state;
        SearchResult result;
        result.min_steps_to_checkmate = 3;  // 3 indicates it is impossible to capture the King within two steps

        scanf("%d", &state.player_count);
        for (int i = 0; i < state.player_count; i++) {
            scanf(" %c %c%c", &type, &pos_x, &pos_y);
            state.player[i].type = type;
            state.player[i].pos.x = pos_x - 'A';
            state.player[i].pos.y = pos_y - '1';
        }

        scanf("%d", &state.opponent_count);
        for (int i = 0; i < state.opponent_count; i++) {
            scanf(" %c %c%c", &type, &pos_x, &pos_y);
            state.opponent[i].type = type;
            state.opponent[i].pos.x = pos_x - 'A';
            state.opponent[i].pos.y = pos_y - '1';
        }

        Check_Checkmate(state, &result);

        if (result.min_steps_to_checkmate == 3) {
            printf("None\n");
        }
        else {
            printf("%d\n", result.min_steps_to_checkmate);
            for (int i = 0; i < result.min_steps_to_checkmate; i++) {
                printf("%c%c %c%c\n",
                       result.moves[i].start.x + 'A', result.moves[i].start.y + '1',
                       result.moves[i].end.x + 'A', result.moves[i].end.y + '1');
            }
        }
    }
}