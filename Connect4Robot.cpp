// Connect4Robot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


/*REFFERENCE OF BOARD
* 
* 
* 5
* 4
* 3
* 2
* 1 
* 0
*   0 1 2 3 4 5 6 

*/

class connect4
{
private:

    static const int ROWS = 6;
	static const int COLUMNS = 7;
    char board[ROWS][COLUMNS];

    char playerSymbol;
    char botSymbol;

    const int WIN_SCORE = 10000000;
    const int LOSS_SCORE = -10000000;

    int minimizingMaximizing(bool isMaximizing, char tempBoard[ROWS][COLUMNS], int depth ,int alpha, int beta)
    {
       // cout << "MinMax: depth=" << depth << ", isMaximizing=" << isMaximizing << endl; //DEBUG
        if (depth == 0 || checkWinOnBoard(tempBoard, botSymbol) || checkWinOnBoard(tempBoard, playerSymbol))
        {
            if (checkWinOnBoard(tempBoard, botSymbol))
            {
                return WIN_SCORE + depth;  // 
            }
            if (checkWinOnBoard(tempBoard, playerSymbol))
            {
                return LOSS_SCORE + depth; 
            }
            return evaluatePosition(tempBoard); 
        }

        if (isMaximizing)
        {
            int maxEval = -99999999;
            for (int i = 0; i < COLUMNS; i++)
            {
                if (tempBoard[ROWS - 1][i] == ' ')
                {
                    char newBoard[ROWS][COLUMNS];
                    copyBoardTo(tempBoard , newBoard);

                    placeToken(i,botSymbol,newBoard);
                    
                    int eval = minimizingMaximizing(!isMaximizing, newBoard, depth - 1, alpha, beta);

                    if (eval > maxEval)
                    {
                        maxEval = eval;
                    }
                    if (eval > alpha)
                    {
                        alpha = eval;
                    }
                    if (beta <= alpha)
                    {
                        i = COLUMNS;//break
                    }
                }
            }
            return maxEval;
        }
        else
        {
            int minEval = 99999999;
            for (int i = 0; i < COLUMNS; i++)
            {
                if (tempBoard[ROWS - 1][i] == ' ')
                {
                    char newBoard[ROWS][COLUMNS];
                    copyBoardTo(tempBoard,newBoard);

                    placeToken(i, playerSymbol, newBoard);

                    int eval = minimizingMaximizing(!isMaximizing, newBoard, depth - 1, alpha, beta);

                    /*Alpha beta pruning hehe*/
                    if (eval < minEval)
                    {
                        minEval = eval;
                    }
                    if (eval < beta)
                    {
                        beta = eval;
                    }
                    if (beta <= alpha)
                    {
                        i = COLUMNS;//break
                    }
                }
            }
            return minEval;
        }
    }

    void copyBoardTo(char toDupe[ROWS][COLUMNS] , char dupe[ROWS][COLUMNS])
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0 ; j < COLUMNS ; j++)

                dupe[i][j] = toDupe[i][j];

        }
    }

    int evaluatePosition(char tempBoard[ROWS][COLUMNS])
    {
        //Check if bot has a win on next turn then player
        if (checkWinOnBoard(tempBoard, botSymbol))
        {
            //cout << "EVAL: BOT WINS! +10000000" << endl;
            return 10000000;
        }
        if (checkWinOnBoard(tempBoard, playerSymbol))
        {
            //cout << "EVAL: PLAYER WINS! -10000000" << endl;
            return -10000000;
        }
        /*10 mil score for win*/

        int score = 0;

        /*You would rather put coins in middle*/
        for (int i = 0; i < ROWS; i++)
        {

            /*Better to put coins in center*/
            if (tempBoard[i][3] == botSymbol)
            {
                score += 50;
            }
            else if (tempBoard[i][3] == playerSymbol)
            {
                score -= 20;
            }

            /*You would rather put coins near middle as well.*/
            if (tempBoard[i][2] == botSymbol)
            {
                score += 15;
            }
            else if (tempBoard[i][2] == playerSymbol)
            {
                score -= 10;
            }

            if (tempBoard[i][4] == botSymbol)
            {
                score += 15;
            }
            else if (tempBoard[i][4] == playerSymbol)
            {
                score -= 10;
            }
        }

        /*Find amount of potential 3 in a rows*/
        score += findPotential(tempBoard,3,botSymbol) * 10000;

        /*Find amount of potential 3 in a rows player has >:C*/
        score -= findPotential(tempBoard, 3, playerSymbol) * 4000;

        /*Find amount of potential 2 in a rows*/
        score += findPotential(tempBoard, 2, botSymbol) * 1000;

        /*Find amount of potential 2 in a rows player has >:C*/
        score -= findPotential(tempBoard, 2, playerSymbol) * 400;

        
        //cout << "EVAL: Normal score = " << score << endl;
        return score;
    }


    int findPotential(char tempBoard[ROWS][COLUMNS], int chipLength , char symbol)
    {
        int potentialsFound = 0;
      //  cout << "findPotential: looking for " << chipLength << " in a row for " << symbol << endl;
        //Horizontal
        int symbolCounter = 0;
        int emptyCounter = 0;
        bool hasPotential = true;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS - 3; j++)
            {
                symbolCounter = 0;
                emptyCounter = 0;
                hasPotential = true;
                for (int k = 0; k < 4; k++)
                {
                    if (tempBoard[i][j+k] == symbol)
                    {
                        symbolCounter++; 
                    }
                    else if (tempBoard[i][j+k] == ' ')
                    {
                        emptyCounter++;
                    }
                    else
                    {
                        k = 5; //Opponent blocked potential :(
                        hasPotential = false;
                    }
                }
                if (symbolCounter == chipLength && hasPotential)
                {
                    potentialsFound++;
                }
            }
        }
        symbolCounter = 0;
        emptyCounter = 0;
        hasPotential = true;

        //Vertical
        for (int i = 0; i < ROWS - 3; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                symbolCounter = 0;
                emptyCounter = 0;
                hasPotential = true;
                for (int k = 0; k < 4; k++)
                {
                    if (tempBoard[i+k][j] == symbol)
                    {
                        symbolCounter++;
                    }
                    else if (tempBoard[i + k][j] == ' ')
                    {
                        emptyCounter++;
                    }
                    else
                    {
                        k = 5; //Opponent blocked potential :(
                        hasPotential = false;
                    }
                }
                if (symbolCounter == chipLength && hasPotential)
                {
                    potentialsFound++;
                }
            }
        }
        symbolCounter = 0;
        emptyCounter = 0;
        hasPotential = true;

        //Diagonal
        for (int i = 0; i < ROWS - 3; i++)
        {
            for (int j = 0; j < COLUMNS - 3; j++)
            {
                symbolCounter = 0;
                emptyCounter = 0;
                hasPotential = true;
                for (int k = 0; k < 4; k++)
                {
                    if (tempBoard[i + k][j + k] == symbol)
                    {
                        symbolCounter++;
                    }
                    else if (tempBoard[i + k][j + k] == ' ')
                    {
                        emptyCounter++;
                    }
                    else
                    {
                        k = 5; //Opponent blocked potential :(
                        hasPotential = false;
                    }
                }
                if (symbolCounter == chipLength && hasPotential)
                {
                    potentialsFound++;
                }
            }
        } 
        symbolCounter = 0;
        emptyCounter = 0;
        hasPotential = true;

        //Backwards Diagonal
        for (int i = 0 + 3; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS - 3; j++)
            {
                 symbolCounter = 0;
                emptyCounter = 0;
                hasPotential = true;
                for (int k = 0; k < 4; k++)
                {
                    if (tempBoard[i - k][j + k] == symbol)
                    {
                        symbolCounter++;
                    }
                    else if (tempBoard[i - k][j + k] == ' ')
                    {
                        emptyCounter++;
                    }
                    else
                    {
                        k = 5; //Opponent blocked potential :(
                        hasPotential = false;
                    }
                }
                if (symbolCounter == chipLength && hasPotential)
                {
                    potentialsFound++;
                }
            }
        }
        //cout << "findPotential: found " << potentialsFound << " potentials" << endl;
        return potentialsFound;
    }

    /*Creates new board so i dont mess up real board*/
    void copyBoard(char newBoard[ROWS][COLUMNS])
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                newBoard[i][j] = board[i][j];
            }
        }
    }


    /* bot specific check win*/
    bool checkWinOnBoard(char tempBoard[ROWS][COLUMNS] , char symbol)
    {
        //Horizontal
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS - 3; j++)
            {
                if (tempBoard[i][j] == symbol
                    && tempBoard[i][j] == tempBoard[i][j + 1]
                    && tempBoard[i][j] == tempBoard[i][j + 2]
                    && tempBoard[i][j] == tempBoard[i][j + 3])
                {
                    return true;
                }
            }
        }

        //Vertical
        for (int i = 0; i < ROWS - 3; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                if (tempBoard[i][j] == symbol
                    && tempBoard[i][j] == tempBoard[i + 1][j]
                    && tempBoard[i][j] == tempBoard[i + 2][j]
                    && tempBoard[i][j] == tempBoard[i + 3][j])
                {
                    return true;
                }
            }
        }

        //Diagonal
        for (int i = 0; i < ROWS - 3; i++)
        {
            for (int j = 0; j < COLUMNS - 3; j++)
            {
                if (tempBoard[i][j] == symbol
                    && tempBoard[i][j] == tempBoard[i + 1][j + 1]
                    && tempBoard[i][j] == tempBoard[i + 2][j + 2]
                    && tempBoard[i][j] == tempBoard[i + 3][j + 3])
                {
                    return true;
                }
            }
        }

        //Backwards Diagonal
        for (int i = 0 + 3; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS - 3; j++)
            {
                if (tempBoard[i][j] == symbol
                    && tempBoard[i][j] == tempBoard[i - 1][j + 1]
                    && tempBoard[i][j] == tempBoard[i - 2][j + 2]
                    && tempBoard[i][j] == tempBoard[i - 3][j + 3])
                {
                    return true;
                }
            }
        }
        return false;
    }


    

public:

    //Reset board to empty (needed for play again)
    void resetBoard()
    {
        for (int i = 0; i < ROWS; i++)
        {
			for (int j = 0; j < COLUMNS; j++)
            {
                board[i][j] = ' ';
            }
        }
    }

    void printBoard()
    {
        for (int i = ROWS - 1; i >= 0; i--)
        {
            for (int j = 0 ; j < COLUMNS; j++)
            {
                cout << "|" << board[i][j];
            }
            cout << "|" << endl;
        }
    }

    bool placeToken(int column, char symbol, char placeBoard[ROWS][COLUMNS])
    {
        for (int i = 0; i < ROWS; i++)
        {
            if (placeBoard[i][column] == ' ')
            {
                placeBoard[i][column] = symbol;
                return true;
            }
        }
        return false;
    }

    bool checkWin()
    {
        //Horizontal
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0 ; j < COLUMNS - 3; j++)
            {
                if (board[i][j] != ' ' 
                && board[i][j] == board[i][j+1] 
                && board[i][j] == board[i][j+2] 
                && board[i][j] == board[i][j+3])
                {
                    return true;
                }
            }
        }

        //Vertical
        for (int i = 0; i < ROWS-3; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                if (board[i][j] != ' ' 
                && board[i][j] == board[i + 1][j] 
                && board[i][j] == board[i + 2][j] 
                && board[i][j] == board[i + 3][j])
                { 
                    return true;
                }
            }
        }

        //Diagonal
        for (int i = 0; i < ROWS - 3; i++)
        {
            for (int j = 0; j < COLUMNS-3; j++)
            {
                if (board[i][j] != ' ' 
                && board[i][j] == board[i + 1][j + 1] 
                && board[i][j] == board[i + 2][j + 2] 
                && board[i][j] == board[i + 3][j + 3])
                {
                    return true;
                }
            }
        }

        //Backwards Diagonal
        for (int i  = 0 + 3 ; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS-3; j++)
            {
                if (board[i][j] != ' ' 
                && board[i][j] == board[i - 1][j + 1] 
                && board[i][j] == board[i - 2][j + 2] 
                && board[i][j] == board[i - 3][j + 3])
                {
                    return true;
                }
            }
        }
        return false;
    }

    void chooseColumn()
    {
        int x;
        bool choosen = false;
        while (!choosen)
        {
            cout << "Input Column number:";
            cin >> x;
            if (x < 0 || x >= COLUMNS)
            { 
                cout << "Input a proper number please";
            }
            else
            {
            choosen  = placeToken(x, playerSymbol, board);  
            }
        }
    }  


    void bot()
    {
        int bestScore = -99999999999;
        int bestMove = 0;


        for (int i = 0; i < COLUMNS; i++)
        {
            if (board[ROWS - 1][i] == ' ')
            {
                char newBoard[ROWS][COLUMNS];

                copyBoardTo(board , newBoard);

                placeToken(i , botSymbol , newBoard);

                int score = minimizingMaximizing(false, newBoard, 3 , -10000000 , 10000000);

                cout << "Column " << i << " score: " << score << endl; // for debugging


                if (score > bestScore)
                {
                    bestScore = score;
                    bestMove = i;
                }
            }
        }

        placeToken(bestMove,botSymbol,board);
    }



    void setSymbols(int playerNum)
    {
        if (playerNum == 1)
        {
            playerSymbol = 'X';
            botSymbol = 'O';
        }
        else
        {
            playerSymbol = 'O';
            botSymbol = 'X';
        }
    }
};



int main()
{
    connect4 game;
    game.resetBoard();

    char currentPlayer = 'O';
    bool gameOver = false;
    int moveCount = 0;
    const int MAX_MOVES = 42;

    cout << "-----Connect 4-----" << endl;
    cout << "Players: O then X" << endl;
    cout << "Columns: 0-6" << endl;


    //Insert function here about the change player :)

    game.setSymbols(1);

    while (!gameOver && moveCount < MAX_MOVES)
    {
        game.printBoard();
        cout << endl;

        if (currentPlayer == 'X')
        {
            cout << "Bot " << currentPlayer << endl;
            game.bot();
        }
        else 
        {
            cout << "Player " << currentPlayer << endl;
            game.chooseColumn();
        }

        

        if (game.checkWin())
        {
            game.printBoard();
            cout << "Player " << currentPlayer << " Wins!" << endl;
            gameOver = true;
        }
        
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        moveCount++;
    }

    if (!gameOver)
    {
        game.printBoard();
        cout << "Draw";
    }

}

