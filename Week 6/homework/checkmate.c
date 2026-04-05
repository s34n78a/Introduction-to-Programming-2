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

const int QUEEN_DIRS[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const int ROOK_DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const int KNIGHT_DIRS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
const int BISHOP_DIRS[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

int check_new_position(Position target_pos, GameState state) {
    // check player's pieces
    for (int piece_id = 0; piece_id < state.player_count; piece_id++)
    {
        if (target_pos.x == state.player[piece_id].pos.x || target_pos.y == state.player[piece_id].pos.y)
        {
            return 1;
        }
    }

    // check opponent's pieces
    for (int piece_id = 0; piece_id < state.opponent_count; piece_id++)
    {
        if (target_pos.x == state.opponent[piece_id].pos.x || target_pos.y == state.opponent[piece_id].pos.y)
        {
            if (state.opponent[piece_id].type == 'K')
            {
                return 2;
            }
            
            return 1;
        }
    }
    
    return 0;
}

int check_piece_can_capture(Piece current_piece, Position enemy_king, GameState state) {
    // Implement the logic to check if the piece can capture the opponent's king
    // based on its type and the current game state.
    // This function should return 1 if it can capture, and 0 otherwise.
    
    int (*dirs)[2];

    // type Q, R, B (similar logic with unlimitd range, just different directions):
    if (current_piece.type == 'Q' || current_piece.type == 'R' || current_piece.type == 'B')
    {

        // type Q:
        if (current_piece.type == 'Q')
        {
            dirs = QUEEN_DIRS;
        }
        
        // type R:
        else if (current_piece.type == 'R')
        {
            dirs = ROOK_DIRS;
        }

        // type B:
        else if (current_piece.type == 'B')
        {
            dirs = BISHOP_DIRS;
        }

        for (int dir_id = 0; dir_id < sizeof(dirs)/sizeof(dirs[0]); dir_id++)
        {
            for (int dist = 0; dist < 7; dist++)
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
        dirs = KNIGHT_DIRS;

        for (int dir_id = 0; dir_id < 4; dir_id++)
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

    return 0; // can't capture king
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
    for (int piece_id = 0; piece_id < state.player_count; piece_id++)
    {
        int check = check_piece_can_capture(state.player[piece_id], opponent_king.pos, state);

        if (check == 1)
        {
            result->min_steps_to_checkmate = 1;
            result->moves[0].start = state.player[piece_id].pos;
            result->moves[0].end = opponent_king.pos;
            return;
        }
    }
    
    //check 2 move capture
    for (int piece_id = 0; piece_id < state.player_count; piece_id++)
    {
        Piece current_piece = state.player[piece_id];

        // generate all possible moves for the current piece
        // (similar logic with check_piece_can_capture, but we need to consider all possible moves instead of just checking capture)

        // For each possible move, we need to create a new game state and check if it can capture the king in the next move.
        // This involves updating the position of the current piece in the new game state and then calling check_piece_can_capture for all pieces again.

        // If we find a move that leads to a capture in the next step, we update the result and return.
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