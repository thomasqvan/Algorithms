// Minimax_Algorithm.cpp (for Othello AI)
#include<algorithm>

int thomasqv::myOthelloAI::minimax(const OthelloGameState& state, 
    int depth, bool yourTurn) {
    
    if(depth == 0) { 
    
        return evaluation(state); 
    }
    else if(yourTurn) { //If it's my turn to move.
        
        myMoves = 0;
        int score = -1; //Place holder for comparison.
        const OthelloBoard& board = state.board();
        
        //Checks the effectiveness of all possible moves within the minimax.
        for(unsigned int i = 0; i < board.width(); ++i) {
            for(unsigned int j = 0; j < board.height(); ++j) {
                
                if(state.isValidMove(i, j)) {
                    
                    //Creates a cloned game state for checking.
                    std::unique_ptr<OthelloGameState> newState = state.clone();

                    //Makes a move on the cloned game state and passes it.
                    newState->makeMove(i, j);
                    int current = minimax(*newState, depth - 1, false);
                    
                    //Returns the maximum value returned from recursive calls.
                    score = std::max(current, score);             
                    myMoves++;
                }
            }
        }
        
        return score;
    }
    else { //If it's my opponent's turn to move.

        theirMoves = 0;
        int score = 1000;
        const OthelloBoard& board = state.board();
        for(unsigned int i = 0; i < board.width(); ++i) {
            for(unsigned int j = 0; j < board.height(); ++j) {
                
                if(state.isValidMove(i, j)) {
                    
                    std::unique_ptr<OthelloGameState> newState = state.clone();
                    newState->makeMove(i, j);
                    int current = minimax(*newState, depth - 1, true);
                    
                    //Returns the minimum value returned from recursive calls.
                    score = std::min(current, score);
                    theirMoves++;
                }
            }
        }

        return score; 
    }
}

// Minimax_Algorithm.cpp
