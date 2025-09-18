#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getDigit.h"

#define BOARD_SIZE 8


void initBoard(char board[BOARD_SIZE][BOARD_SIZE][3]);
void displayBoard(char board[BOARD_SIZE][BOARD_SIZE][3]);
int makeMove(char board[BOARD_SIZE][BOARD_SIZE][3], int srcX, int srcY, int destX, int destY, int playerTurn);
int isValidMove(char board[BOARD_SIZE][BOARD_SIZE][3], int srcX, int srcY, int destX, int destY, int playerTurn);

int main() {
    char board[BOARD_SIZE][BOARD_SIZE][3];
    int srcX, srcY, destX, destY;
    int playerTurn = 1; 

    initBoard(board);
	int i=0;
    while (1) {
		
        displayBoard(board);
        printf("\nPlayer %d (%s), enter your move:\n", playerTurn, playerTurn == 1 ? "White" : "Black");
		printf("\nPress any Key to start recording Source Coordinate\n");
		getch();
        printf("\nSpeak source Row coordinate (1-8): ");
		srcX=liveTest();
        printf("\nEnter source Column coordinate (1-8): ");
		srcY=liveTest();
		printf("\nSource Coordinate : %d , %d \n",srcX,srcY);

		printf("\nPress any Key to start recording Destination Coordinate\n");
		getch();
        printf("\nEnter destination Row coordinate (1-8): ");
		
		destX=liveTest();
        printf("\nEnter destination Column coordinate (1-8): ");
		destY=liveTest();
		printf("\nDestination Coordinate : %d , %d \n",destX,destY);
        // Convert input coordinates to 0-indexed for internal array use
        srcX = 8 - srcX;
        srcY = srcY - 1;
        destX = 8 - destX;
        destY = destY - 1;

        if (srcX < 0 || srcX >= BOARD_SIZE || srcY < 0 || srcY >= BOARD_SIZE ||
            destX < 0 || destX >= BOARD_SIZE || destY < 0 || destY >= BOARD_SIZE) {
			printf("\n-------------------------------------------------------------\n");
            printf("Coordinates out of bounds. Please enter values between 1 and 8.\n");
			printf("---------------------------------------------------------------\n");
            continue;
        }

        if (!makeMove(board, srcX, srcY, destX, destY, playerTurn)) {
			printf("\n----------------------------------------------\n");
            printf("\nInvalid move. Try again.\n\n");
			printf("\n----------------------------------------------\n");
        } else {
            playerTurn = 3 - playerTurn; // Switch between player 1 (white) and player 2 (black)
        }
		i=i+4;
    }

    printf("Game over.\n");
    return 0;
}

void initBoard(char board[BOARD_SIZE][BOARD_SIZE][3]) {
    // Set up the initial chessboard using abbreviations
    const char *initialBoard[BOARD_SIZE][BOARD_SIZE] = {
        {"BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR"},
        {"BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"},
        {"..", "..", "..", "..", "..", "..", "..", ".."},
        {"..", "..", "..", "..", "..", "..", "..", ".."},
        {"..", "..", "..", "..", "..", "..", "..", ".."},
        {"..", "..", "..", "..", "..", "..", "..", ".."},
        {"WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"},
        {"WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"}
    };
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            strncpy(board[i][j], initialBoard[i][j], 3);
        }
    }
}

void displayBoard(char board[BOARD_SIZE][BOARD_SIZE][3]) {
    printf("\n                      CHESS BOARD\n\n");
    printf("       1     2     3     4     5     6     7     8\n");
    printf("     +-----+-----+-----+-----+-----+-----+-----+-----+\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("  %d  |", 8 - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %-3s |", board[i][j]); // Left-align within cell to ensure uniform spacing
        }
        printf("  \n");
        printf("     +-----+-----+-----+-----+-----+-----+-----+-----+\n");
    }
}

int makeMove(char board[BOARD_SIZE][BOARD_SIZE][3], int srcX, int srcY, int destX, int destY, int playerTurn) {
    char piece[3];
    strncpy(piece, board[srcX][srcY], 3);

    // Check if the piece belongs to the correct player
    if ((playerTurn == 1 && piece[0] != 'W') || (playerTurn == 2 && piece[0] != 'B')) {
        return 0; // Not the player's piece
    }

    // Simple move validation
    if (!isValidMove(board, srcX, srcY, destX, destY, playerTurn)) {
        return 0; // Invalid move
    }

    // Make the move
    strncpy(board[destX][destY], board[srcX][srcY], 3);
    strcpy(board[srcX][srcY], "..");

    return 1;
}

int isValidMove(char board[BOARD_SIZE][BOARD_SIZE][3], int srcX, int srcY, int destX, int destY, int playerTurn) {
    char destPiece[3];
    strncpy(destPiece, board[destX][destY], 3);

    // Check if the destination is occupied by the same player's piece
    if ((playerTurn == 1 && destPiece[0] == 'W') || (playerTurn == 2 && destPiece[0] == 'B')) {
        return 0; // Destination occupied by own piece
    }

    // Simple validation (expand as needed for real chess rules)
    return 1;
}

