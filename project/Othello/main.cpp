#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <string>

#define WHITE 0
#define BLACK 1
#define DOT 2
#define NOTHING 3
using namespace std;

class OthelloBoard{
public:
    int board[8][8],whitePieces{0},blackPieces{0};
    vector<int> availableMovesX;
    vector<int> availableMovesY;
    vector<int>::iterator it;
    OthelloBoard();
    void printBoardor();
    void moveCursor(int ,int );
    void displayPieces();
    void displayDots();
    void clearDots();
    void findValidMoves(bool );
    void flipPieces(int , int);
    void countPieces();
    int min(int b, int a) { return b < a ? b : a; }
    int max(int b, int a) { return b < a ? a : b; }

};
OthelloBoard::OthelloBoard(){
    // Initialize board with starting pieces
    for(int row{0};row<8;row++)
    {
        for(int col{0};col<8;col++)
        {
            board[row][col]=NOTHING;
        }
    }
    board[3][3] =WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

void OthelloBoard::printBoardor()
{
    //  1  2  3  4  5  6  7  8
    for(int i{0};i<8;i++)
    {
       cout<<"    "<<char(49+i);
    }
    cout<<endl;

    // ╔══╤══╤══╤══╤══╤══╤══╤══╗
    cout<<' '<<char(713);
    for(int k{0}; k < 7; k++)
    {
        cout<<char(717)<<char(717)<<char(717)<<char(717)<<char(721);

    }
    cout<<char(717)<<char(717)<<char(717)<<char(717)<<char(699)<<endl;


    for(int j{0}; j < 8;j++)
    {

        cout<<char('A'+j);
        for(int k{0}; k < 2 ;k++)
        {
            if(k==1)cout<<' ';
            cout<<char(698);
            for(int i{0} ; i < 7 ; i++)
            {
                cout<<"    "<<char(691);
            }
            cout<<"    "<<char(698)<<endl;
        }


        if(j==7) break;
        cout<<' '<<char(711);
        for(int i{0}; i < 7; i++)
        {
            cout<<char(708)<<char(708)<<char(708)<<char(708)<<char(709);
        }
        cout<<char(708)<<char(708)<<char(708)<<char(708)<<char(694)<<endl;
    }

    //  ╚══╧══╧══╧══╧══╧══╧══╧══╝
    cout<<' '<<char(712);
    for(int k{0}; k < 7; k++)
    {
        cout<<char(717)<<char(717)<<char(717)<<char(717)<<char(719);

    }
    cout<<char(717)<<char(717)<<char(717)<<char(717)<<char(700);

    displayPieces();
}

void OthelloBoard::moveCursor(int x,int y)
{
    COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void OthelloBoard::displayPieces()
{
    int x,y;
    for(int row{0};row < 8; row++)
    {

        for(int col{0};col < 8;col++)
        {
            if(board[row][col] == WHITE)//
            {
                y = row * 3 + 2;
                x = col * 5 + 2;
                moveCursor(x,y);
                cout<<' '<<char(220)<<char(220);
                moveCursor(x,y+1);
                cout<<' '<<char(223)<<char(223);
            }
            else if(board[row][col] == BLACK)
            {

                y = row * 3 + 2;
                x = col * 5 + 2;
                moveCursor(x,y);
                cout<<char(218)<<char(196)<<char(191);
                moveCursor(x,y+1);
                cout<<char(192)<<char(196)<<char(217);
            }
        }
    }
}

void OthelloBoard::displayDots()
{
    int x, y;
    int row ,col ;


    // Iterate over the availableMovesX and availableMovesY vectors
    for (int i = 0; i < availableMovesX.size(); i++)
    {
        row = availableMovesY[i];
        col = availableMovesX[i];

        // Convert board coordinates to display coordinates
        y = row * 3 + 3;
        x = col * 5 + 3;

        moveCursor(x, y);
        cout << char(248); // Display the dot
    }
}

void OthelloBoard::findValidMoves(bool color) {
    // Clear previously stored valid moves
    availableMovesX.clear();
    availableMovesY.clear();

    // Define direction vectors
    vector<vector<int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},   // Vertical and horizontal
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // Diagonals
    };

    // Iterate over the board to find all valid moves
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Check if the current cell contains the player's piece
            if (board[row][col] == color) {
                // Check each direction
                for (const auto& dir : directions) {
                    int dr = dir[0];
                    int dc = dir[1];
                    int r = row + dr;
                    int c = col + dc;
                    bool foundOpponent = false;

                    // Traverse in the direction to find valid moves
                    while (r >= 0 && r < 8 && c >= 0 && c < 8) {
                        if (board[r][c] == !color){//!color = (color == WHITE ? BLACK : WHITE)
                            foundOpponent = true;
                        } else if (board[r][c] == NOTHING && foundOpponent) {
                            // Valid move found
                            availableMovesX.push_back(c);
                            availableMovesY.push_back(r);
                            board[r][c]=DOT;
                            break;
                        } else {
                            break;
                        }
                        r += dr;
                        c += dc;
                    }
                }
            }
        }
    }
}

void OthelloBoard::clearDots()
{
    int x,y;
    int row , col;

    for (int i = 0; i < availableMovesX.size(); i++)
    {
        col= availableMovesX[i];
        row = availableMovesY[i];
        board[row][col] = NOTHING;
        x = col * 5 + 3;
        y = row * 3 + 3;
        moveCursor(x, y);
        cout << ' ';
    }
}


void OthelloBoard::flipPieces(int row, int col)
{
    bool color = board[row][col]; // The color of the piece to flip

    // Define direction vectors for all possible directions
    vector<vector<int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}, // Vertical and horizontal
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // Diagonals
    };

    // Iterate over each direction
    for (const auto& dir : directions)
    {
        int dr = dir[0]; // Direction row increment
        int dc = dir[1]; // Direction column increment
        int r = row + dr; // Start row in the direction
        int c = col + dc; // Start column in the direction
        bool foundOpponent = false; // Flag to track if we found an opponent piece

        // Traverse in the current direction
        while (r >= 0 && r < 8 && c >= 0 && c < 8)
        {
            if (board[r][c] == (color == WHITE ? BLACK : WHITE))
            {
                // Found an opponent piece
                foundOpponent = true;
            }
            else if (board[r][c] == color && foundOpponent)
            {
                // Found a piece of the same color and we have encountered opponent pieces
                // Flip all pieces between the start and current position
                int flipRow = row;
                int flipCol = col;
                while (flipRow != r || flipCol != c)
                {
                    board[flipRow][flipCol] = color;
                    flipRow += dr;
                    flipCol += dc;
                }
                break;
            }
            else
            {
                // No valid pieces to flip in this direction
                break;
            }

            // Move to the next position in the direction
            r += dr;
            c += dc;
        }
    }
}

void OthelloBoard::countPieces()
{
    whitePieces = 0;
    blackPieces = 0;

    // Iterate over each row
    for(int row = 0; row < 8; ++row)
    {
        // Iterate over each column in the row
        for(int col = 0; col < 8; ++col)
        {
            if(board[row][col] == WHITE)
            {
                whitePieces++;
            }
            else if(board[row][col] == BLACK)
            {
                blackPieces++;
            }
        }
    }

    // Move the cursor and display the count of pieces
    moveCursor(15, 26);
    cout << " w = " << whitePieces+0 << " b = " << blackPieces +0<< ' ';
}

class OthelloGame:public OthelloBoard{
private:
    char x,y;
    int row,col;
    int passFlag;
public:
    OthelloGame();
    void getMove(bool );
    void playerTurn(const bool );
    void playGameHumanVsHuman();
    void displayGameResult();
    void playGameHumanVscomputer(bool,int);
    void computerTurn(bool , int );
    void chooseBestMove(bool , int );
    ~OthelloGame();
};


OthelloGame::OthelloGame()
{
    int menu;
    int level;
    cout << "human{0},computer{1}:";
    cin >> menu;
    system("cls");

    if (menu == 0)
    {
        playGameHumanVsHuman(); // Human vs Human
    }
    else
    {
        cout << "level(0to2):";
        cin >> level;
        level = max(0, min(level, 2)); // Clamp level between 0 and 2
        bool color;
        cout << "color w{0},b{1}:";
        cin >> color;
        system("cls");

        if (color)
        {
            playGameHumanVscomputer(color,level);// Computer vs Human
        }
        else
        {
            playGameHumanVscomputer(color,level);// Human vs Computer
        }
    }

    displayGameResult(); // Call the function to display the result
}

void OthelloGame::getMove(bool color)
{
    bool validMove = false;
    moveCursor(0, 27);
    cout << "                               \n              ";
    moveCursor(0, 27);
    cout << "Enter x (1-8) and y (A-H): ";
    cin >> x >> y;


    // Convert input coordinates to 2D indices
    if (x >= '1' && x <= '8') {
        row = x - '1';  // Convert char '1'-'8' to int 0-7
    } else {
        moveCursor(0, 27);
        cout << "Invalid column. Try again.     ";
        Sleep(1200);
        getMove(color);
        return;
    }

    y = toupper(y);

    if (y >= 'A' && y <= 'H') {
           col = y - 'A';  // Convert char 'A'-'B' to int 0-7
    }else {
        moveCursor(0, 27);
        cout << "Invalid row. Try again.         ";
        Sleep(1200);
        getMove(color);
        return;
    }

    // Check if the move is in the list of available moves


    for (int i = 0; i < availableMovesX.size(); i++) {
        if (availableMovesX[i] == row && availableMovesY[i] == col) {
            validMove = true;
            break;
        }
    }

    if (validMove) {
        board[col][row] = color;
    } else {
        moveCursor(0, 27);
        cout << "Not a valid move. Try again.    ";
        Sleep(1200);
        getMove(color);
    }
}


void OthelloGame::playGameHumanVsHuman()
{
    printBoardor();
    countPieces();

    while ((whitePieces + blackPieces) != 64 && passFlag != 2)
    {
        passFlag = 0;

        if ((whitePieces + blackPieces) != 64)
        {
            playerTurn(BLACK); // Human's turn (black)
        }

        if ((whitePieces + blackPieces) != 64)
        {
                playerTurn(WHITE); // Human's turn (white)
        }
    }
}

void OthelloGame::playGameHumanVscomputer(bool color,int level)
{
    printBoardor();
    countPieces();
    while ((whitePieces + blackPieces) != 64 && passFlag != 2)
    {
        passFlag = 0;

        if ((whitePieces + blackPieces) != 64)
        {
            if(color) playerTurn(BLACK); // Human's turn (black)
            else computerTurn(BLACK,level);
        }

        if ((whitePieces + blackPieces) != 64)
        {
            if(!color) playerTurn(WHITE); // Human's turn (white)
            else computerTurn(WHITE,level);
        }
    }

}

void OthelloGame::playerTurn(const bool color)
{
    findValidMoves(color);
    displayDots();

    // Determine the player color for display
    string playerColor = (color == WHITE) ? "White" : "Black";
    moveCursor(0, 26);
    cout << playerColor;

    if (availableMovesX.empty() || availableMovesY.empty()) // Check if there are no valid moves
    {
        passFlag++;
        moveCursor(6, 26);
        cout << "Pass";
        Sleep(1200);
        moveCursor(0, 26);
        cout << "          "; // Clear previous message
        return;
    }


    getMove(color); // Handle input from the player
    clearDots();    // Clear previous dots shown for available moves
    board[col][row]=color;
    flipPieces(col,row); // Flip pieces based on the move
    displayPieces();      // Update the board display
    countPieces();        // Count and display the pieces
}

void OthelloGame::displayGameResult()
{
    moveCursor(0, 28); // Move the cursor to the position where the result will be displayed

    if (whitePieces > blackPieces)
    {
        cout << "White win";
    }
    else if (whitePieces < blackPieces)
    {
        cout << "Black win";
    }
    else
    {
        cout << "The game is a tie";
    }
}

void OthelloGame::computerTurn(bool color, int level)
{
    findValidMoves(color);
    displayDots();
    Sleep(1000);
    moveCursor(0, 26);
    cout << "Compu";

    if (availableMovesX.empty()) // Check if there are no valid moves
    {
        passFlag++;
        moveCursor(6, 26);
        cout << "Pass";
        Sleep(1200);
        moveCursor(0, 26);
        cout << "                "; // Clear previous message
        return;
    }

    //chooseBestMove(color, level); // Select and execute the best move

    chooseBestMove(color,level);
    clearDots();  // Clear dots indicating valid moves
    board[col][row]= color;
    flipPieces(col,row);
    displayPieces(); // Update board display
    countPieces();   // Count and display pieces
}

void OthelloGame::chooseBestMove(bool color, int level)
{
    vector<int> results(availableMovesX.size());
    int MAX,indexMAX,MIN,indexMIN,bestMoveIndex;
    int board_copy[8][8];

    for (int row {0}; row < 8; row++)
    {
        for (int col {0}; col < 8; col++)
        {
            board_copy[row][col] = board[row][col];
        }
    }
    // Evaluate each possible move
    for (int i {0}; i < availableMovesX.size(); i++)
    {
        for (int row{0}; row < 8; row++)
        {
            for (int col {0}; col < 8; col++)
            {
                board[row][col] = board_copy[row][col];
            }
        }
        // Make the move
        board[availableMovesY.at(i)][availableMovesX.at(i)]= color;
        flipPieces(availableMovesY.at(i),availableMovesX.at(i));
        countPieces();

        // Store the result based on the color
        results.at(i) = (color == WHITE) ? blackPieces : whitePieces;
    }

    // Restore the board state
    // Copy the current board state
    for (int row {0}; row < 8;row++)
    {
        for (int col = 0; col < 8; col++)
        {
            board[row][col] = board_copy[row][col];
        }
    }
    // Find the best move based on the level
    MAX = results.at(0);
    MIN = results.at(0);
    indexMAX = 0;
    indexMIN = 0;
    for(int i{1};i<results.size();i++)
    {
        if(results.at(i)> MAX)
        {
            indexMAX = i;
            MAX = results.at(i);
        }
        if(results.at(i) < MAX)
        {
            indexMIN = i;
            MIN = results.at(i);
        }
    }

    // Calculate the index from availableMovesX and availableMovesY
    if(level == 0) bestMoveIndex = indexMIN;
    else if(level = 1)bestMoveIndex = 0;
    else bestMoveIndex = indexMAX;
    // Execute the best move
    col = availableMovesY[bestMoveIndex];
    row = availableMovesX[bestMoveIndex];
}


OthelloGame::~OthelloGame()
{
    moveCursor(0,29);
}

int main()
{
    OthelloGame ob;
    return 0;
}




