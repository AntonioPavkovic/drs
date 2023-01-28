#include <iostream>
#include <mpi.h>

using namespace std;

enum Type {EMPTY, PAWN, ROOK, BISHOP};
struct Piece {Type type; int x, y;};

const int BOARD_SIZE = 4;
Piece board[BOARD_SIZE][BOARD_SIZE];

void initBoard() 
{
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            board[i][j].type = EMPTY;
            board[i][j].x = i;
            board[i][j].y = j;
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        board[1][i].type = PAWN;
        board[6][i].type = PAWN;
    }

    board[0][0].type = ROOK;
    board[0][7].type = ROOK;
    board[7][0].type = ROOK;
    board[7][7].type = ROOK;

    board[0][2].type = BISHOP;
    board[0][5].type = BISHOP;
    board[7][2].type = BISHOP;
    board[7][5].type = BISHOP;
}

void movePiece(Piece &piece, int newX, int newY) 
{
    piece.x = newX;
    piece.y = newY;
}

void printBoard(Piece board[][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            cout << board[i][j].type << " ";
        }
        cout << endl;
    }
}

bool gameOver(Piece board[][BOARD_SIZE])
{
    Type remainingType = EMPTY;
    int remainingCount = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j].type != EMPTY)
            {
                if (remainingCount == 0)
                {
                    remainingType = board[i][j].type;
                }
                else if (remainingType != board[i][j].type)
                {
                    return false;
                }
                remainingCount++;
            }
        }
    }

    if (remainingCount == 1)
    {
        cout << "Igra zavrsena! Figure koje su ostale: " << remainingType << endl;
        return true;
    }

    return false;
}

bool validateMove(Piece board[][BOARD_SIZE], int x, int y, int newX, int newY)
{
    if (newX < 0 || newX >= BOARD_SIZE || newY < 0 || newY >= BOARD_SIZE)
    {
        cout << "Nevazeci potez" << endl;
        return false;
    }

    if(board[x][y].type == EMPTY)
    {
        cout << "U odabranom polju nema figure" << endl;
        return false;
    }

    if (board[x][y].type == ROOK)
    {
        if (x != newX && y != newY)
        {
            cout << "Nevazeci potez topa" << endl;
            return false;
        }
    }
    else if (board[x][y].type == PAWN)
    {
        if (abs(x - newX) > 1 || abs(y - newY) > 1)
        {
            cout << "Nevazeci potez pijuna" << endl;
            return false;
        }
    }

    if (board[newX][newY].type != EMPTY)
    {
        if(board[newX][newY].type == board[x][y].type) 
        {
            board[newX][newY].type = EMPTY;
        }
        else if (board[newX][newY].type == PAWN)
        {
            if(board[x][y].type == ROOK)
            {
                board[x][y].type = EMPTY;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    MPI::Init(argc, argv);

    initBoard();

    int rank = MPI::COMM_WORLD.Get_rank();

    MPI::COMM_WORLD.Bcast(board, BOARD_SIZE*BOARD_SIZE, MPI::INT, 0);

    while (true) {
        if (gameOver(board)) {
            break;
        }

        if (rank == 0) {
            int x, y, newX, newY;
            x = rand() % BOARD_SIZE;
            y = rand() % BOARD_SIZE;
            newX = rand() % BOARD_SIZE;
            newY = rand() % BOARD_SIZE;
            if (validateMove(board, x, y, newX, newY)) {
                movePiece(board[x][y], newX, newY);
            }
            
            printBoard(board);

            MPI::COMM_WORLD.Bcast(board, BOARD_SIZE*BOARD_SIZE, MPI::INT, 0);
        } else {
            MPI::COMM_WORLD.Bcast(board, BOARD_SIZE*BOARD_SIZE, MPI::INT, 0);
        }
    }

    MPI::Finalize();
    return 0;
}