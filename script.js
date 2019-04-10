// Main game variables
var board, game = new Chess();

var bestMoveSecondAgent, bestHeuristicSecondAgent, bestMove;

/******************** IA Code ********************/


// IA Mode (Random, One Ahead, Minimax, Minimax AB Pruning)
ia_mode = "minimax_pruning"

//Analysis of movement by the second agent
var analyze_move = function (moveFirstAgent) {
    
    var note;
    
    bestMoveSecondAgent = getBestMove(game);
    
    game.ugly_move(moveFirstAgent);
    var boardValue = -heuristic(game.board());
    
    if(boardValue < bestHeuristicSecondAgent) {
        note = Math.random(); 
    }
    else {
        note = Math.floor(Math.random() * 10) + 1.1;
    }
    
    return note;
    
}

// Make an IA move
var makeBestMove = function () {
    
    // Get the current best move and make it
    //var bestMove = getBestMove(game);
    
    
    game.ugly_move(bestMove);
    board.position(game.fen());
    renderMoveHistory(game.history({ verbose: true }));
    
    // Game over after move
    if (game.game_over()) {
        alert('Game over');
    }

};

// Get the best move
var getBestMove = function (game) {
    
    // Game over before move
    if (game.game_over()) {
        alert('Game over');
    }

    // Get selected mode
    ia_mode = $('#algorithm').find(':selected').text();

    // Use the current mode
    if (ia_mode == "Random") {
        return randomMove(game);
    } else if (ia_mode == "One Ahead") {
        return oneAheadMove(game);
    } else if (ia_mode == "Minimax") {
        var depth = parseInt($('#tree_depth').find(':selected').text());
        return calcMinimax(depth, game, true, false);
    } else if (ia_mode == "Minimax AB Pruning") {
        var depth = parseInt($('#tree_depth').find(':selected').text());
        return calcMinimax(depth, game, true, true);
    }

};

// Generate all the possible moves
// Return a random one
var randomMove = function(game) {
    var validMoves = game.ugly_moves();
    return validMoves[Math.floor(Math.random() * validMoves.length)];
}

// Loops for all the possible moves, apply them, 
// and, then, evaluate the heuristic function
// Chooses the one that returns the best heuristic 
// value after applied
var oneAheadMove = function(game) {
    
    var validMoves = game.ugly_moves();
    var bestMove = null;
    var bestValue = -1000;

    for (var i = 0; i < validMoves.length; i++) {
        var validMove = validMoves[i];
        game.ugly_move(validMove);

        // Take the negative as AI plays as black
        var boardValue = -heuristic(game.board())
        game.undo();
        
        // Update best move
        if (boardValue > bestValue) {
            bestValue = boardValue;
            bestMove = validMove
        }
    }
    
    //linha adicionada
    bestHeuristicSecondAgent = bestValue;

    return bestMove;

}

// Calc Minimax
var calcMinimax = function(depth, game, isMax, pruning) {

    var validMoves = game.ugly_moves();    
    var bestMove = null;
    var bestValue = -1000;

    for(var i = 0; i < validMoves.length; i++) {
        var validMove = validMoves[i]
        game.ugly_move(validMove);
        
        // Call minimax for this branch
        if (pruning == false) {
            var value = minimax(depth - 1, game, !isMax);
        } else {
            var value = minimax_pruning(depth - 1, game, -1000, 1000, !isMax);
        }
        
        // Undo move
        game.undo();
        
        // Update best move
        if(value >= bestValue) {
            bestValue = value;
            bestMove = validMove;
        }
    }
    
    //linha adicionada
    bestHeuristicSecondAgent = bestValue;
    
    return bestMove;

}

// Minimax algorithm
var minimax = function(depth, game, isMax) {

    // Take the negative as AI plays as black
    if (depth === 0) {
        return -heuristic(game.board());
    }

    // Get the new valid moves
    var validMoves = game.ugly_moves();

    if (isMax) {
        var bestMove = -1000;
        
        // Maximizer
        for (var i = 0; i < validMoves.length; i++) {
            game.ugly_move(validMoves[i]);
            bestMove = Math.max(bestMove, minimax(depth - 1, game, !isMax));
            game.undo();
        }
        
        return bestMove;
    } else {
        var bestMove = 1000;
        
        // Minimizer
        for (var i = 0; i < validMoves.length; i++) {
            game.ugly_move(validMoves[i]);
            bestMove = Math.min(bestMove, minimax(depth - 1, game, !isMax));
            game.undo();
        }
        
        return bestMove;
    }

}

// Minimax algorithm with the alpha-beta pruning
// strategy to reduce the search space
var minimax_pruning = function(depth, game, alpha, beta, isMax) {

    // Take the negative as AI plays as black
    if (depth === 0) {
        return -heuristic(game.board());
    }

    // Get the new valid moves
    var validMoves = game.ugly_moves();

    if (isMax) {
        var bestMove = -1000;
        
        // Maximizer
        for (var i = 0; i < validMoves.length; i++) {
            game.ugly_move(validMoves[i]);
            bestMove = Math.max(bestMove, minimax_pruning(depth - 1, game, alpha, beta, !isMax));
            game.undo();

            // Pruning
            alpha = Math.max(alpha, bestMove);
            if (beta <= alpha) {
                return bestMove;
            }
        }
        
        return bestMove;
    } else {
        var bestMove = 1000;
        
        // Minimizer
        for (var i = 0; i < validMoves.length; i++) {
            game.ugly_move(validMoves[i]);
            bestMove = Math.min(bestMove, minimax_pruning(depth - 1, game, alpha, beta, !isMax));
            game.undo();

            // Pruning
            beta = Math.min(beta, bestMove);
            if (beta <= alpha) {
                return bestMove;
            }
        }
        
        return bestMove;
    }

}

// Heuristic function
var heuristic = function (board) {
    
    // Heuristic value
    var totalEvaluation = 0;
    
    // Get value of each piece and update 
    // the heuristic value
    for (var i = 0; i < 8; i++) {
        for (var j = 0; j < 8; j++) {
            totalEvaluation = totalEvaluation + getPieceValue(board[i][j], i ,j);
        }
    }
    
    return totalEvaluation;
};

// Rank pieces according to the Hans Berliner's system
// King value is 100 since it cannot be traded
// https://en.wikipedia.org/wiki/Chess_piece_relative_value
var getPieceValue = function(piece) {

    piece_value = 0

    if (piece == null) {
        return 0;
    }
    
    if (piece.type === 'p') {
        piece_value = 1;
    } else if (piece.type === 'r') {
        piece_value = 5.1;
    } else if (piece.type === 'n') {
        piece_value = 3.2;
    } else if (piece.type === 'b') {
        piece_value = 3.3;
    } else if (piece.type === 'q') {
        piece_value = 8.8;
    } else if (piece.type === 'k') {
        piece_value = 100;
    }

    return piece.color === 'w' ? piece_value : -piece_value;

}


/******************** Chess Code ********************/


// Render moves history
var renderMoveHistory = function (moves) {
    
    var color = 'White';
    var piece = "Pawn";
    
    // Get the div element
    var historyElement = $('#moves').empty();
    historyElement.empty();
    
    for (var i = 0; i < moves.length; i++) {

        // Get the piece color
        if (moves[i].color == 'w') {
            color = "White";
        } else if (moves[i].color == 'b') {
            color = "Black";
        }

        // Get the piece type
        if (moves[i].piece == "p") {
            piece = "Pawn";
        } else if (moves[i].piece == "b") {
            piece = "Bishop";
        } else if (moves[i].piece == "n") {
            piece = "Knight";
        } else if (moves[i].piece == "r") {
            piece = "Rook";
        } else if (moves[i].piece == "q") {
            piece = "Queen";
        } else if (moves[i].piece == "k") {
            piece = "King";
        }

        // Render history
        historyElement.append('<span>' + color + ": " + piece + " from " + moves[i].from + 
            " to " + moves[i].to + '</span><br>');
    }
    
    historyElement.scrollTop(historyElement[0].scrollHeight);

};

// Block move on checkmate
var onDragStart = function (source, piece, position, orientation) {
    if (game.in_checkmate() === true || game.in_draw() === true || piece.search(/^b/) !== -1) {
        return false;
    }
};

//Communication between agents
var communication = function(moveFirstAgent) {
    
    var note = analyze_move(moveFirstAgent);

    var takeAdvice = Math.random(); 
    
    if (takeAdvice <= 0.5) {
        return moveFirstAgent;
    } else {
        return bestMoveSecondAgent;
    }
    
}

// Move a piece from a source to a target
var onDrop = function (source, target) {

    var move = game.move({
        from: source,
        to: target,
        promotion: 'q'
    });

    if (move === null) {
        return 'snapback';
    }
    
    var moveFirstAgent = randomMove(game);
    
    bestMove = communication(moveFirstAgent);

    renderMoveHistory(game.history({ verbose: true }));
    window.setTimeout(makeBestMove, 250);

};

// Board config
var cfg = {
    draggable: true,
    position: 'start',
    onDragStart: onDragStart,
    onDrop: onDrop,
};


// Render the board with the given config
board = ChessBoard('board', cfg);
