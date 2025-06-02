/*

  ミニマックス法を使って探索
  αβ法は使っていないので非効率的

*/
#include <iostream>
#include <array>
#include <vector>
#include <string>

enum PlayerType {
    NONE = 0,
    BLACK = 1,
    WHITE = 2
};

class Osero {
protected:
    // Game board representation
    std::vector<std::vector<int>> board; // 2D vector to represent the game board
    int currentPlayer; // Current player (BLACK or WHITE)
    // Game board representation


    const int dx[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int dy[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
    char transitoin_mark(int type){
        // Convert type to a character representation
        switch (type) {
            case NONE: return '_'; // NONE
            case BLACK: return '@'; // BLACK
            case WHITE: return 'O'; // WHITE
            default: return '?';
        }
    }
    void board_clear() {
        // Clear the board
        for (auto& row : board) {
            std::fill(row.begin(), row.end(), NONE);
        }
    }
    bool canFlip(int row, int col, int playerType) {
        // playerType: BLACK or WHITE

        int opponent = (playerType == BLACK) ? WHITE : BLACK;
    
        for (int dir = 0; dir < 8; ++dir) {
            int nx = row + dx[dir];
            int ny = col + dy[dir];
            bool foundOpponent = false;
    
            
            while (nx >= 0 && nx < board.size() && ny >= 0 && ny < board.size()) {
                if (board[nx][ny] == opponent) {
                    foundOpponent = true;
                    nx += dx[dir];
                    ny += dy[dir];
                } else if (board[nx][ny] == playerType && foundOpponent) {
                    return true; 
                } else {
                    break;
                }
            }
        }
        return false;
    }

    void flipDisks(int x, int y, int playerType) {

        int opponent = (playerType == BLACK) ? WHITE : BLACK;
    
        for (int dir = 0; dir < 8; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            std::vector<std::pair<int, int>> toFlip; 
            
            while (nx >= 0 && nx < board[0].size() && ny >= 0 && ny < board.size() && board[nx][ny] == opponent) {
                toFlip.push_back({nx, ny});
                nx += dx[dir];
                ny += dy[dir];
            }
            
            if (nx >= 0 && nx < board[0].size() && ny >= 0 && ny < board.size() && board[nx][ny] == playerType) {
                for (auto p : toFlip) {
                    board[p.first][p.second] = playerType;
                }
            }
        }
    }

public:
    Osero(int length,int player) : board(length, std::vector<int>(length, 0)),currentPlayer(player){}
    void changePlayer(int player) {
        // Change the current player
        if (player == BLACK || player == WHITE) {
            currentPlayer = player;
        } else {
            std::cout << "Invalid player type!" << std::endl;
        }
    }
    void startGame() {
        // Start the game logic
        // Initialize the board with starting positions
        int mid = board.size() / 2;
        board[mid - 1][mid - 1] = BLACK;
        board[mid][mid] = BLACK;
        board[mid - 1][mid] = WHITE;
        board[mid][mid - 1] = WHITE;
    }

    std::vector<std::vector<int> > makeMove(int row, int col) {
        // Logic for making a move
        if (row < 0 || row >= board.size() || col < 0 || col >= board.size()) {
            std::cout << "Invalid move!" << std::endl;
            return board; // Return the current board state
        }
        if (board[row][col] == 0 && canFlip(row, col, currentPlayer)) {
            board[row][col] = currentPlayer; // Place the disk
            this->flipDisks(row, col, currentPlayer);
            currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK; // Switch player
            return board; // Return the updated board state
        } else {
            std::cout << "error in osero.cpp class osero in void makeMove(int row,int col)!" << std::endl;
            return board; // Return the current board state
        }
        
    }

    void placeDisks(std::vector<std::pair<int, std::array<int, 2>>> positions) {
        // Logic to place disks on the board at specified positions
        board_clear(); // Clear the board before placing disks
        // Place disks on the board at specified positions
        
        for (const auto& pos : positions) {
            int row = pos.second[0];
            int col = pos.second[1];
            if (row >= 0 && row < board.size() && col >= 0 && col < board.size() && board[row][col] == NONE) {
                board[row][col] = pos.first; // pos.first is the player type (BLACK or WHITE)
            } else {
                //std::cout << "Invalid position (" << x << ", " << y << ") for player " << playerType << std::endl;
            }
        }
    }

    void displayBoard() {
        
        std::cout << "   ";
        for (int x = 0; x < board.size(); ++x) {
            std::cout << x << " ";
        }
        std::cout << std::endl;

        for (int y = 0; y < board.size(); ++y) {
            
            std::cout << y << "  ";
            for (int x = 0; x < board[y].size(); ++x) {
                std::cout << transitoin_mark(board[y][x]) << " ";
            }
            std::cout << std::endl;
        }
    }
    int countDisks(int playerType) {
        // Count the number of disks for a player
        int count = 0;
        for (const auto& row : board) {
            for (int cell : row) {
                if (cell == playerType) {
                    count++;
                }
            }
        }
        return count;
    }

    void endGame() {
        // Logic to end the game
        std::cout << "Game over!" << std::endl;
        int blackCount = countDisks(BLACK);
        int whiteCount = countDisks(WHITE);
        std::cout << "Final score - BLACK: " << blackCount << ", WHITE: " << whiteCount << std::endl;
    }

    std::string getCurrentPlayer() {
        return (currentPlayer == BLACK) ? "BLACK" : "WHITE";
    }

    int checkGameOver() {
        // Check if the game is over
        for (int row = 0; row < board.size(); ++row) {
            for (int col = 0; col < board[row].size(); ++col) {
                if (board[row][col] == NONE && (canFlip(row, col, currentPlayer))) {
                    return 1; // If there's at least one valid move, the game is not over
                }else if(board[row][col] == NONE && !canFlip(row, col, (currentPlayer == BLACK) ? WHITE : BLACK) && !canFlip(row, col, currentPlayer)){
                    continue; // If no valid moves for both players, end the game
                }else if(board[row][col] == NONE && !canFlip(row, col, (currentPlayer == BLACK) ? WHITE : BLACK)){
                    std::cout << "No valid moves for " << ((currentPlayer == BLACK) ? "BLACK" : "WHITE") << ". Switching player." << std::endl;
                    currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK; // Switch player
                    return 1; // Continue checking for the next player
                }   
            }
        }
        endGame(); // If no valid moves left, end the game
        return 0;// Game over
    }
    void go_to_game(){
        try {
            this->displayBoard();
            int row, col;
            while(this->checkGameOver()){
                std::cout << "Current player: " << this->getCurrentPlayer() << std::endl;
                std::cout << "Enter your move (row col): ";
                std::cin >> row >> col;
                if (std::cin.fail()) {
                    throw std::runtime_error("Invalid input. Please enter two integers for row and column.");
                }
                this->makeMove(row, col);
                this->displayBoard();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

class OseroAI : public Osero {

    private:
                // AI-specific attributes can be added here
        int depthLimit; // Depth limit for AI's search algorithm

        bool canFlip(std::vector<std::vector<int> > board,int row, int col, int playerType) {
        // playerType: BLACK or WHITE

        int opponent = (playerType == BLACK) ? WHITE : BLACK;
    
        for (int dir = 0; dir < 8; ++dir) {
            int nx = row + dx[dir];
            int ny = col + dy[dir];
            bool foundOpponent = false;
    
         
            while (nx >= 0 && nx < board.size() && ny >= 0 && ny < board.size()) {
                if (board[nx][ny] == opponent) {
                    foundOpponent = true;
                    nx += dx[dir];
                    ny += dy[dir];
                } else if (board[nx][ny] == playerType && foundOpponent) {
                    return true; 
                } else {
                    break;
                }
            }
        }
        return false;
    }

    std::vector<std::vector<int> > flipDisks(std::vector<std::vector<int> > board,int x, int y, int playerType) {
        int opponent = (playerType == BLACK) ? WHITE : BLACK;
    
        for (int dir = 0; dir < 8; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            std::vector<std::pair<int, int>> toFlip; 
          
            while (nx >= 0 && nx < board[0].size() && ny >= 0 && ny < board.size() && board[nx][ny] == opponent) {
                toFlip.push_back({nx, ny});
                nx += dx[dir];
                ny += dy[dir];
            }
            if (nx >= 0 && nx < board[0].size() && ny >= 0 && ny < board.size() && board[nx][ny] == playerType) {
                for (auto p : toFlip) {
                    board[p.first][p.second] = playerType;
                }
            }
        }
        return board; // Return the updated board state after flipping disks
    }

    std::vector<std::vector<int> > makeMove(std::vector<std::vector<int> > board, int row, int col,int currentPlayer) {
        // Logic for making a move with AI
        if (row < 0 || row >= board.size() || col < 0 || col >= board.size()) {
            std::cout << "Invalid move!" << std::endl;
            return {{}}; // Return the current board state
        }
        if (board[row][col] == NONE && canFlip(board,row, col, currentPlayer)) {
            board[row][col] = currentPlayer; // Place the disk
            board = flipDisks(board, row, col, currentPlayer);
            return board; // Return the updated board state
        } else {
            return {{}}; // Return the current board state
        }
    }



    void aiMakeMove() {
        int bestScore = -10000;
        int bestRow = -1, bestCol = -1;
        for (int row = 0; row < board.size(); ++row) {
            for (int col = 0; col < board[row].size(); ++col) {
                if (board[row][col] == NONE && Osero::canFlip(row, col, currentPlayer)) {
                    std::cout << "AI checking move at: (" << row << ", " << col << ")" << std::endl;
                    auto newBoard = board;
                    newBoard = makeMove(newBoard, row, col, currentPlayer); 
                    int score = minimax(newBoard, depthLimit - 1, true, (currentPlayer == BLACK ? WHITE : BLACK), currentPlayer); 
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = row;
                        bestCol = col;
                    }
                }
            }
        }
        if (bestRow != -1 && bestCol != -1) {
            std::cout << "currentPlayer: " << this->getCurrentPlayer() << std::endl;
            std::cout << "AI chooses move: (" << bestRow << ", " << bestCol << ") with score: " << bestScore << std::endl;
            Osero::makeMove(bestRow, bestCol); 
        } else {
            std::cout << "AI has no valid moves. Passing turn." << std::endl;
            currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
        }
    }
    void displayBoardOseroAI(std::vector<std::vector<int>> board) {
        // Display the board with player types
        std::cout << "   ";
        for (int x = 0; x < board.size(); ++x) {
            std::cout << x << " ";
        }
        std::cout << std::endl;

        for (int y = 0; y < board.size(); ++y) {
            std::cout << y << "  ";
            for (int x = 0; x < board[y].size(); ++x) {
                std::cout << transitoin_mark(board[y][x]) << " ";
            }
            std::cout << std::endl;
        }
    }
    int evaluateBoard(const std::vector<std::vector<int>>& board, int playerType) {
        int score = 0;
        int opponentType = (playerType == BLACK) ? WHITE : BLACK;
        for (const auto& row : board) {
            for (int cell : row) {
                if (cell == playerType) score++;
                else if (cell == opponentType) score--;
            }
        }   
            std::cout << "Evaluating board for player " << ((playerType == BLACK) ? "BLACK" : "WHITE") << ": " << score << std::endl;
            displayBoardOseroAI(board); // Display the board after evaluation
            return score; // Positive score for BLACK, negative for WHITE
    }

    int minimax(std::vector<std::vector<int>> board, int depth, bool maximizingPlayer, int turnPlayer, int evalPlayerType) {
        if (depth <= 0) {
            return evaluateBoard(board, evalPlayerType); 
        }
        int bestScore = maximizingPlayer ? -10000 : 10000;
        bool hasMove = false;
        for (int row = 0; row < board.size(); ++row) {
            for (int col = 0; col < board[row].size(); ++col) {
                if (board[row][col] == NONE && canFlip(board, row, col, turnPlayer)) {
                    hasMove = true;
                    auto newBoard = makeMove(board, row, col, turnPlayer);
                    int score = minimax(newBoard, depth - 1, !maximizingPlayer, (turnPlayer == BLACK ? WHITE : BLACK), evalPlayerType);
                    if (maximizingPlayer) {
                        bestScore = std::max(bestScore, score);
                    } else {
                        bestScore = std::min(bestScore, score);
                    }
                }
            }
        }
        if (!hasMove) {
            return evaluateBoard(board, evalPlayerType);
        }
        return bestScore;
    }

    public:
    void go_to_game() { 
        try {
            this->displayBoard();
            int row, col;
            while (this->checkGameOver()) {
                if (currentPlayer == BLACK) { 
                    std::cout << "Current player: " << this->getCurrentPlayer() << std::endl;
                    std::cout << "Enter your move (row col): ";
                    std::cin >> row >> col;
                    if (std::cin.fail()) {
                        throw std::runtime_error("Invalid input. Please enter two integers for row and column.");
                    }
                    Osero::makeMove(row, col); 
                } else { // AI
                    aiMakeMove(); 
                }
                this->displayBoard();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
        OseroAI(int length, int player,int depth) : Osero(length, player) {depthLimit = depth;}  
    // Additional game-specific methods can be added here
};

int main(int argc, char* argv[]) {
    // Initialize the Osero 
    // game with a 4x4 board
    int boardSize = 4;
    int startingPlayer = BLACK; // BLACK starts the game
    int depthLimit = 2; // Depth limit for AI's search algorithm
    if (argc > 1) {
        try {
            std::cout << "Command line arguments detected.<boardSize:int> <startingPlayer:int(1forBlack,2forWhite)> <depthLimit:int>" << std::endl;
            boardSize = std::stoi(argv[1]);
            startingPlayer = (std::stoi(argv[2]) == 1) ? BLACK : WHITE; // 1 for BLACK, 2 for WHITE
            depthLimit = std::stoi(argv[3]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid command line arguments. Using default values." << std::endl;
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    OseroAI game(boardSize, startingPlayer, depthLimit); 
    game.startGame(); // Start the game with initial setup
    game.go_to_game(); // Start the game loop
    return 0;
}
