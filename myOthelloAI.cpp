#include<ics46/factory/DynamicFactory.hpp>
#include"myOthelloAI.hpp"
#include"OthelloCell.hpp"
#include"OthelloBoard.hpp"
#include<algorithm>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI,
    thomasqv::myOthelloAI, "VanT OthelloAI (Required)");

std::pair<int, int> thomasqv::myOthelloAI::chooseMove (const 
    OthelloGameState& state) {
    
    const OthelloBoard& board = state.board();
    int score = -1;
    std::pair<int , int> move;
    
    if(!state.isGameOver()) {
        
        //If a corner is available and valid, take it.
        if(state.isValidMove(board.width() - 1, board.height() - 1)) {
        
            return {board.width() - 1, board.height() - 1};
        }
        if(state.isValidMove(0, board.height() - 1)) {
        
            return {0, board.height() - 1};
        }
        if(state.isValidMove(board.width() - 1, 0)) {
        
            return {board.width() - 1, 0};
        }
        if(state.isValidMove(0, 0)) {
        
            return {0, 0};
        }
        
        //Check the effectiveness of all possible and valid moves.
        for(unsigned int i = 0; i < board.width(); ++i) {
            for(unsigned int j = 0; j < board.height(); ++j) {
            
                if(state.isValidMove(i, j)) {
                    
                    //Search on cloned state.
                    std::unique_ptr<OthelloGameState> newState = state.clone();
                    newState->makeMove(i, j);

                    int current = search_(*newState, 3, false); //Depth 3.
                    score = current;
                    move = {i, j};
                }
            }
        }
    }

    return { move.first, move.second };
}

int thomasqv::myOthelloAI::evaluation(const OthelloGameState& state) {
   
    const OthelloBoard& board = state.board();
    
    int moves = 0; //Takes into account the number of moves 
                   //available per player.
    if(myMoves > theirMoves) {
        moves = (myMoves - theirMoves);
    }
    else if(myMoves < theirMoves) {
        moves = -(theirMoves - myMoves);
    }
    
    //Takes into account which player manages to grab a corner in this state.
    int black = 0, white = 0;
    if(board.cellAt(board.width() - 1, board.height() - 1) == 
        OthelloCell::black) { black++;}
    else if(board.cellAt(board.width() - 1, board.height() - 1) ==
        OthelloCell::white) { white++; }
    if(board.cellAt(0, board.height() - 1) == OthelloCell::
        black) { black++; }
    else if(board.cellAt(0, board.height() - 1) == OthelloCell::
        white) { white++; }
    if(board.cellAt(board.width() - 1, 0) == OthelloCell::
        black) { black++; }
    else if(board.cellAt(board.width() - 1, 0) == OthelloCell::
        white) { white++; }
    if(board.cellAt(0, 0) == OthelloCell::black) { black++; }
    else if(board.cellAt(0, 0) == OthelloCell::white) { white++; }

    //Takes into account the cells neighboring the corners.
    //These are important as they dictate who will most likely
    //obtain said corner(s).
    int black_ = 0, white_ = 0;
    if(board.cellAt(board.width() - 1, board.height() - 1) == 
        OthelloCell::empty) {

        if(board.cellAt(board.width() - 1, board.height() - 2) ==
            OthelloCell::black) { black_++; }
        else if(board.cellAt(board.width() - 1, board.height() - 2) ==
            OthelloCell::white) { white_++; }
        if(board.cellAt(board.width() - 2, board.height() - 1) ==
            OthelloCell::black) { black_++; }
        else if(board.cellAt(board.width() - 2, board.height() - 1) ==
            OthelloCell::white) { white_++; }
        if(board.cellAt(board.width() - 2, board.height() - 2) ==
            OthelloCell::black) { black_++; }
        else if(board.cellAt(board.width() - 2, board.height() - 2) ==
            OthelloCell::white) { white_++; }
    }
    if(board.cellAt(0, board.height() - 1) == OthelloCell::empty) { 
        
        if(board.cellAt(0, board.height() - 2) == OthelloCell::
            black) { black_++; }
        else if(board.cellAt(0, board.height() - 2) == OthelloCell::
            white) { white_++; }
        if(board.cellAt(1, board.height() - 1) == OthelloCell::
            black) { black_++; }
        else if(board.cellAt(1, board.height() - 1) == OthelloCell::
            white) { white_++; }
        if(board.cellAt(1, board.height() - 2) == OthelloCell::
            black) { black_++; }
        else if(board.cellAt(1, board.height() - 2) == OthelloCell::
            white) { white_++; }
    }
    if(board.cellAt(board.width() - 1, 0) == OthelloCell::empty) {

        if(board.cellAt(board.width() - 1, 1) == OthelloCell::
            black) { black_++; }
        else if(board.cellAt(board.width() - 1, 1) == OthelloCell::
            white) { white_++; }
        if(board.cellAt(board.width() - 2, 0) == OthelloCell::
            black) { black_++; }
        else if(board.cellAt(board.width() - 2, 0) == OthelloCell::
            white) { white_++; }
        if(board.cellAt(board.width() - 2, 1) == OthelloCell::
            black) { black_++; }
        else if(board.cellAt(board.width() - 2, 1) == OthelloCell::
            white) { white_++; }
    }
    if(board.cellAt(0, 0) == OthelloCell::empty) {

        if(board.cellAt(0, 1) == OthelloCell::black) { black_++; }
        else if(board.cellAt(0, 1) == OthelloCell::white) { white_++; }
        if(board.cellAt(1, 0) == OthelloCell::black) { black_++; }
        else if(board.cellAt(1, 0) == OthelloCell::white) { white_++; }
        if(board.cellAt(1, 1) == OthelloCell::black) { black_++; }
        else if(board.cellAt(1, 1) == OthelloCell::white) { white_++; }
    }

    if(state.isBlackTurn()) { //Evaluation considers player's turn.
    
    //Values are weights based off of what I believe is more
    //important; worth more. (e.g. corners over generic cells)
        return ((state.blackScore() - state.whiteScore()) + (25 * (
            black - white)) + (-5 * (black_ - white_)) + moves);
    }
    else {
        return ((state.whiteScore() - state.blackScore()) + (25 * (
            white - black)) + (-5 * (white_ - black_)) + moves);
    }
}

int thomasqv::myOthelloAI::search_(const OthelloGameState& state, 
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
                    int current = search_(*newState, depth - 1, false);
                    
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
                    int current = search_(*newState, depth - 1, true);
                    
                    //Returns the minimum value returned from recursive calls.
                    score = std::min(current, score);
                    theirMoves++;
                }
            }
        }

        return score; 
    }
}
