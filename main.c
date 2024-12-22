/* __ShortDescription__
**
** @author IRYNA KORCHOVSKA  09/12/2024
** Tic Tac Toe Game is a classic game where players take turns to place their marks (X or O) on a grid.
** The game ends when one player aligns 3 or 4 marks horizontally, vertically, or diagonally.
** Players can either play against each other (Human vs Human) or against AI bots (with varying difficulty levels).
**The game features both 3x3 and 4x4 board sizes.
*************************************************************************/

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */

char filename[256];
char win3();
char win4();
char num3[3][3];
char num4[4][4];
int checkWinMove3(int row, int col, char player);
int checkWinMove4(int row, int col, char player);
int num_players;
int score_x[2] = {0};   // 0 is for losses, 1 is for wins for x user
int score_o[2] = {0};   // 0 is for losses, 1 is for wins for o user

typedef struct {        // creating the structure for the profile
    char name[50];
    int wins;
    int losses;
} Profile;

/* ==================================================================== */
/* ==================== function prototypes =========================== */
/* ==================================================================== */

void addProfile();
void viewProfiles();
void fileHandling_AI();
void fileHandling_2_humans();
void handlingPlayers();
int getBoardChoice();
int getLevelChoice();
void handleBoardChoice(int b, int l);
char win3();
char win4();
void game3();
void game4();
void ClearBuffer();
void reset3();
void reset4();
int spaces3();
int spaces4();
void playerMove3();
void playerMove4();
void player2Move3();
void player2Move4();
void printWinner3(char winner);
void printWinner4(char winner);
void AI_move_easy3();
void AI_move_easy4();
void AI_move_medium3();
int checkWinMove3(int row, int column, char player);
void AI_move_medium4();
int checkWinMove4(int row, int column, char player);
int minimax(char board[3][3], int depth, int isMaximizing);
void AI_move_hard3();
int minimax_4x4(char board[4][4], int depth, int isMaximizing);
void AI_move_hard4();
void clearScreen();

/* ==================================================================== */
/* ========================== main function =========================== */
/* ==================================================================== */

int main()
{
    char winner = ' ';
    char response = ' ', res_tourn = ' ';
    int tournament;
    char name[100]; // for the profile

    printf("----Welcome to Tic Tac Toe game!!!----");

    handlingPlayers();
}

/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        // For Linux or macOS:
        system("clear");
    #endif
}

void addProfile()
{
    Profile profile;   // calling for the structure
    fflush(stdin);     // clear the output buffer
    printf("\n\nPlease, enter your name: ");
    scanf("%[^\n]", profile.name);
    while (getchar() != '\n');  // take the input with spaces
    profile.wins = 0;
    profile.losses = 0;

    //creating the unique file
    snprintf(filename, sizeof(filename),"%s.txt", profile.name);

    FILE *file = fopen(filename, "w");
    if ( !file )    // check if the file was successfully created or opened.
    {
        printf("Error creating file for profile!\n");
        return;
    }
    //write the data to the right address
    fwrite(&profile, sizeof(Profile), 1, file);
    fclose(file);
    printf("\nProfile is saved. Thank you.");
}

void viewProfiles( int w, int l)
{
    char filename[100];
    Profile profile;

    printf("Enter the profile name to view: ");
    scanf(" %[^\n]", filename);

    //looking for the file to open
    snprintf(filename, sizeof(filename), "%s.txt", filename);

    FILE *file = fopen(filename, "r");
    if ( !file )
    {
        printf("\nProfile is not find.");
        return;
    }

    fread(&profile, sizeof(Profile), 1, file);
    profile.wins = w;
    profile.losses = l;

    fseek(file, 0, SEEK_SET);
    //changing the data for wins and losses to new ones
    fwrite(&profile, sizeof(Profile), 1, file);
    fclose(file);

    fread(&profile, sizeof(Profile), 1, file);
    fclose(file);
    printf("\n---------------------\n");    //interface of user's profile
    printf("Profile data:\n");
    printf("Name: %s\n", profile.name);
    printf("Wins: %d\n", profile.wins);
    printf("Losses: %d\n", profile.losses);
}

void fileHandling_AI()
{
    char choice = ' ' ;
    while(1)
    {
        printf("\n----Profile Manager----\n");   // give the choice for the user about profile
        printf("Would you like: \n");
        printf("View your profile.(Y/N)\n");
        scanf(" %c", &choice);
        choice = toupper(choice);
        switch(choice)
        //using swith case to run the needed function
        {
            case 'Y':
                viewProfiles(score_x[1], score_x[0]);
                break;

            case 'N':
                printf("Thank you!");
                return;

            default:
                printf("Invalid choice! Please enter Y or N.\n");
                break;

        }

    }
}

void fileHandling_2_humans()
{
    int choice ;
    while(1)  //profile manager for 2 users, so it will safe tha data for each user
    {
        printf("\n----Profile Manager----\n");
        printf("Would you like: \n");
        printf("1. PRESS 1 to view User's 1(X) profile.\n");
        printf("2. PRESS 2 to view User's 2(O) profile.\n");
        printf("3. Exit the program.\n");
        scanf(" %d", &choice);
        switch(choice)
        //is able to show profile for each user
        {
            case 1:
                viewProfiles(score_x[1], score_x[0]);  // change the data to new scores,  and then will show the profile
                break;

            case 2:
                viewProfiles(score_o[1], score_o[0]);  // change the data to new scores,  and then will show the profile (user 2)
                break;

            case 3:
                printf("Thank you!");
                return;

            default:
                printf("Invalid choice! Please enter Y or N.\n");
                break;

        }

    }
}

void handlingPlayers()
{
    char winner = ' ';
    char response = ' ', res_tourn = ' ';
    int tournament;
    char name[100];

    do
    {
        printf("\n\nPress 1, if you would like to play with AI bots.\n");
        printf("Press 2, if you would like to play with 2nd player.\n");
        //getting the input for 1 or 2 users functions
        scanf("%d", &num_players);

        if (num_players != 1 && num_players != 2)
        {
            printf("Invalid choice! Please enter either 1 or 2.\n");
            ClearBuffer();
            continue;
        }

    } while (num_players != 1 && num_players != 2);  //handle invalid choice

    if (num_players == 1)  // game with ai
    {
        addProfile();
        printf("\nWould you like to try tournament?(Y/N)\n");
        scanf(" %c", &res_tourn);
        res_tourn = toupper(res_tourn);
        if (res_tourn == 'Y')  // enetering the loop for tournament
        {
            printf("What score would you like to reach during tournament? \n");
            scanf("%d", &tournament);

            if (tournament <= 0)   //checking for invalid input
            {
                printf("Invalid tournament score! Exiting.\n");
                return 1; // Exit program for invalid input
            }

            do
            {
                winner = ' ';

                int boardChoice;
                int levelChoice;

                reset3();
                reset4();
                // calling for function to get the board and level choices
                boardChoice = getBoardChoice();
                levelChoice = getLevelChoice();
                handleBoardChoice (boardChoice, levelChoice); //arguments for this functions to call the needed board and level of ai

            }while (score_x[1] < tournament );  //will run the function until target for score will be reached
        }

    else
    {
        do
        {

        winner = ' ';
        response = ' ';


        int boardChoice;
        int levelChoice;

        reset3();
        reset4();

        boardChoice = getBoardChoice();
        levelChoice = getLevelChoice();
        handleBoardChoice (boardChoice, levelChoice);

        printf("\nWould you like to play again? (Y/N): ");
        scanf(" %c", &response);
        response = toupper(response);
        }while (response == 'Y');  // will run the single game, then will ask for one more game. It will play until the user press NO more games
    }
    //going to profile manager
    fileHandling_AI();
    return 0;
    }
    //game with another human
    if (num_players == 2)
    {
        // adding different accounts to record data
        printf("--------------------------------------");
        printf("\nFor Player 1:\n");
        addProfile();

        printf("\n--------------------------------------");
        printf("\n\nFor Player 2:\n");
        addProfile();

        printf("\nWould you like to try tournament?(Y/N)\n");
        scanf(" %c", &res_tourn);
        res_tourn = toupper(res_tourn);
        if (res_tourn == 'Y')
        {
            printf("What score would you like to reach during tournament? \n");
            scanf("%d", &tournament);

            if (tournament <= 0)
            {
                printf("Invalid tournament score! Exiting.\n");
                return 1; // Exit program for invalid input
            }

            do
            {
                winner = ' ';

                int boardChoice;
                int levelChoice;

                reset3();
                reset4();
                // choice for board in which users would like to play
                boardChoice = getBoardChoice();
                if (boardChoice == 1)
                {
                   int count = 0;
                   do
                   {
                        game3();
                        //program will call the function for move of user 1 and 2 in order( x first)
                        (count % 2 == 0) ? playerMove3() : player2Move3();
                        game3();
                        count++;

                   }while(win3() == ' ');
                printWinner3(win3());  // printing who won
                }

                if (boardChoice == 2)  // if choice was for 4x4 board
                {
                   int count = 0;
                   do
                   {
                        game4();
                        //program will call the function for move of user 1 and 2 in order( x first)
                        (count % 2 == 0) ? playerMove4() : player2Move4();
                        game4();
                        count++;

                   }while(win4() == ' ');
                printWinner4(win4());    // printing who won
                }

            }while (score_x[1] < tournament && score_o[1] < tournament);  // the program will run until one of players will reach the chosen target (wins)
            fileHandling_2_humans();
        }
        //choice of the single game for 2 humans
        if (res_tourn == 'N')
        {
            winner = ' ';

            int boardChoice;
            int levelChoice;
          do
          {
            reset3();
            reset4();
            //choice of board for single game for 2 humans
            boardChoice = getBoardChoice();
            if (boardChoice == 1)
            {
                int count = 0;
                do
                {
                    game3();
                    (count % 2 == 0) ? playerMove3() : player2Move3();
                    game3();
                    count++;

                }while(win3() == ' ');
                //printing the winner
                printWinner3(win3());
            }
            // board choice 4x4 for single game
            if (boardChoice == 2)
            {
                int count = 0;
                do
                {
                    game4();
                    //program will call the function for move of user 1 and 2 in order( x first)
                    (count % 2 == 0) ? playerMove4() : player2Move4();
                    game4();
                    count++;

                }while(win4() == ' ');
                printWinner4(win4());
            }
            printf("\nWould you like to play again? (Y/N): ");
            scanf(" %c", &response);
            response = toupper(response);
          }while (response == 'Y'); //the program will run until the answer will be Y
          fileHandling_2_humans();
        }
    }
}

int getBoardChoice()  // getting the board choice for functions above
{
    int boardChoice;

    do
    {
        printf("\nChoose the board type (1) 3x3, (2) 4x4: ");

        scanf("%d", &boardChoice);


        if (boardChoice != 1 && boardChoice != 2)  // checking for invalid input
        {
            printf("Invalid choice! Please enter either 1 or 2.\n");
            ClearBuffer();  // getting the input from user and clearing the buffer
            continue;
        }

    } while (boardChoice != 1 && boardChoice != 2);  // the loop will run until the answer will be 1 or 2

    return boardChoice;
}

int getLevelChoice() {

    int levelChoice;

    do
    {
        printf("Choose the level type (1) easy, (2) medium, (3) hard: ");

        scanf("%d", &levelChoice);


        if (levelChoice != 1 && levelChoice != 2 && levelChoice != 3)  // checking for invalid input
        {
            printf("Invalid choice! Please enter either 1, 2, or 3.\n");
            ClearBuffer();  // will clear the buffer after the wrong input so there will be no infinity loop
            continue;
        }

    } while (levelChoice != 1 && levelChoice != 2 && levelChoice != 3);  // the loop will run until the answer will be 1 or 2 or 3

    return levelChoice;
}

void handleBoardChoice(int b, int l)  //2 arguments for board and level
{
    if (b == 1)
    {
        printf("You chose a 3x3 board:\n");
        if (l == 1)
        {
            printf("You chose easy level.");
            do
            {
                clearScreen();
                game3();   // printing the board
                playerMove3();  // ask for function, so user will make the move
                if (win3() != ' ')  // when the game is over
                {
                    clearScreen();
                    game3();
                    printWinner3(win3());
                    break;
                }
                if (spaces3() == 0)
                {
                    clearScreen();
                    game3();
                    printWinner3(' '); // Declare a draw when no spaces are left
                    break;
                }

                AI_move_easy3();
                if (win3() != ' ')
                {
                    clearScreen();
                    game3();
                    printWinner3(win3());  // printing the winner if there is one after the computer move
                    break;
                }
                if (spaces3() == 0)
                {
                    clearScreen();
                    game3();
                    printWinner3(' '); // Declare a draw when no spaces are left
                    break;
                }
            } while (1);
        }

        // the same logic with 3x3 board and medium level
        if (l == 2)
        {
            printf("You chose medium level.");
            do
            {
                clearScreen();
                game3();
                playerMove3();
                if (win3() != ' ')
                {
                    clearScreen();
                    game3();
                    printWinner3(win3());
                    break;
                }
                if (spaces3() == 0)
                {
                    clearScreen();
                    game3();
                    printWinner3(' '); // Declare a draw when no spaces are left
                    break;
                }

                AI_move_medium3();
                if (win3() != ' ')
                {
                    clearScreen();
                    game3();
                    printWinner3(win3());
                    break;
                }
                if (spaces3() == 0)
                {
                    clearScreen();
                    game3();
                    printWinner3(' '); // Declare a draw when no spaces are left
                    break;
                }
            } while (1);
        }

        // the same logic with 3x3 board and hard level
        if (l == 3)
        {
            printf("You chose hard level.");
            do
            {
                clearScreen();
                game3();
                playerMove3();
                if (win3() != ' ')
                {
                    clearScreen();
                    game3();
                    printWinner3(win3());
                    break;
                }

                if (spaces3() == 0)
                {
                    clearScreen();
                    game3();
                    printWinner3(' '); // Declare a draw when no spaces are left
                    break;
                }

                AI_move_hard3();

                if (win3() != ' ')
                {
                    clearScreen();
                    game3();
                    printWinner3(win3());
                    break;
                }

                if (spaces3() == 0)
                {
                    clearScreen();
                    game3();
                    printWinner3(' '); // Declare a draw when no spaces are left
                    break;
                }
            } while(1);
        }
    }

    // the same logic with 4x4 board and easy level
    else if (b == 2)
    {
        printf("You chose a 4x4 board.\n");
        if (l == 1)
        {
            printf("You chose easy level.");
            do
            {
                clearScreen();
                game4();
                playerMove4();
                if (win4() != ' ')
                {
                    clearScreen();
                    game4();
                    printWinner4(win4());
                    break;
                }
                if (spaces4() == 0)
                {
                    clearScreen();
                    game4();
                    printWinner4(' '); // Declare a draw when no spaces are left
                    break;
                }

                AI_move_easy4();
                if (win4() != ' ')
                {
                    clearScreen();
                    game4();
                    printWinner4(win4());
                    break;
                }
                if (spaces4() == 0)
                {
                    clearScreen();
                    game4();
                    printWinner4(' '); // Declare a draw when no spaces are left
                    break;
                }
            } while (1);
        }

        // the same logic with 4x4 board and medium level
        if (l == 2)
        {
            printf("You chose medium level.");
            do
            {
                clearScreen();
                game4();
                playerMove4();
                if (win4() != ' ')
                {
                    clearScreen();
                    game4();
                    printWinner4(win4());
                    break;
                }
                if (spaces4() == 0)
                {
                    clearScreen();
                    game4();
                    printWinner4(' '); // Declare a draw when no spaces are left
                    break;
                }

                AI_move_medium4();
                if (win4() != ' ')
                {
                    clearScreen();
                    game4();
                    printWinner4(win4());
                    break;
                }
                if (spaces4() == 0)
                {
                    clearScreen();
                    game4();
                    printWinner4(' '); // Declare a draw when no spaces are left
                    break;
                }
            } while (1);
        }

        // the same logic with 4x4 board and hard level
        if (l == 3)
        {
            printf("You chose hard level.");

            AI_move_hard4();
        }
    }
}

//returning the win character ( X or O)
char win3()
{

    for(int i = 0; i < 3; i++)  //checking for rows
    {
        if(num3[i][0] == num3[i][1] && num3[i][0] == num3[i][2])
        {
            return num3[i][0];
        }
    }

    for(int i = 0; i < 3; i++)   //checking for columns
    {
        if(num3[0][i] == num3[1][i] && num3[0][i] == num3[2][i])
        {
            return num3[0][i];
        }
    }

    //checking for diagonals
    if(num3[0][0] == num3[1][1] && num3[1][1] == num3[2][2])
    {
        return num3[0][0];
    }

    if(num3[0][2] == num3[1][1] && num3[1][1] == num3[2][0])
    {
        return num3[0][2];
    }

    return ' '; // No winner

}

char win4()
{
   for (int i = 0; i < 4; i++)  //checking for rows
   {
        if (num4[i][0] == num4[i][1] && num4[i][0] == num4[i][2] && num4[i][0] == num4[i][3])
        {
            return num4[i][0];
        }
   }

   for (int i = 0; i < 4; i++)  //checking for columns
   {
        if (num4[0][i] == num4[1][i] && num4[0][i] == num4[2][i] && num4[0][i] == num4[3][i])
        {
            return num4[0][i];
        }
   }

    //checking for diagonals
    if (num4[0][0] == num4[1][1] && num4[0][0] == num4[2][2] && num4[0][0] == num4[3][3])
    {
        return num4[0][0];
    }

    if (num4[0][3] == num4[1][2] && num4[0][3] == num4[2][1] && num4[0][3] == num4[3][0])
    {
        return num4[0][3];
    }

    return ' '; // No winner

}



void game3() // Function to draw the player's game board 3x3
{
    printf("\n\n\tTic Tac Toe\n\n");

    printf("\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c\n", num3[0][0], num3[0][1], num3[0][2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c\n", num3[1][0], num3[1][1], num3[1][2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c\n", num3[2][0], num3[2][1], num3[2][2]);
    printf("     |     |     \n\n");
}

void game4() // Function to draw the player's game board 4x4
{
    printf("\n\n\tTic Tac Toe\n\n");

    printf("     |     |     |    \n");
    printf("  %c  |  %c  |  %c  |  %c   \n", num4[0][0], num4[0][1], num4[0][2], num4[0][3]);
    printf("_____|_____|_____|_____\n");
    printf("     |     |     |       \n");
    printf("  %c  |  %c  |  %c  |  %c   \n", num4[1][0], num4[1][1], num4[1][2], num4[1][3]);
    printf("_____|_____|_____|_____\n");
    printf("     |     |     |       \n");
    printf("  %c  |  %c  |  %c  |  %c  \n", num4[2][0], num4[2][1], num4[2][2], num4[2][3]);
    printf("_____|_____|_____|_____\n");
    printf("     |     |     |       \n");
    printf("  %c  |  %c  |  %c  |  %c  \n", num4[3][0], num4[3][1], num4[3][2], num4[3][3]);
    printf("     |     |     |       \n");
}

//function for clearing the buffer after the invalid input, so the program will not read the wrong answer
void ClearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//reseting the 3x3 board after each game
void reset3()
{
    for (int i = 0; i <= 2; i++)
    {
        for (int k = 0; k <= 2; k++)
        {
            num3[i][k] = ' ';
        }
    }
}

//reseting the 4x4 board after each game
void reset4()
{
    for (int i = 0; i <= 3; i++)
    {
        for (int k = 0; k <= 3; k++)
        {
            num4[i][k] = ' ';
        }
    }
}

//checking the free spaces for 3x3 board
int spaces3()
{
    int spaces3 = 0;
    for(int i = 0; i < 3; i++)
    {
      for(int j = 0; j < 3; j++)
      {
         if(num3[i][j] == ' ')
         {
            spaces3++;
         }
      }
   }
   return spaces3;
}

//checking the free spaces for 3x3 board
int spaces4()
{
    int spaces4 = 0;
    for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 4; j++)
      {
         if(num4[i][j] == ' ')
         {
            spaces4++;
         }
      }
   }
   return spaces4;
}

//function for move for 1st player (human vs human) or player (vs AI)  3x3 board
void playerMove3()
{
  int row;
  int column;

  printf("It is your turn (X)!\n");

  do
  {
    printf("Enter the row number (from top to bottom)(1-3): ");
    scanf("%d", &row);
    row -=1;  //the input will be suited for row array

    printf("Enter the column number (from left to right)(1-3): ");
    scanf("%d", &column);
    column-=1;  //the input will be suited for row array

    if (num3[row][column] != ' ')
    {
        printf("Invalid square! Please try again.\n");
    }
    else
    {
        num3[row][column] = 'X';
        break;
    }

  } while (num3[row][column] != ' ');  //user will be able to make mistake, and the program will not break

}

//function with same logic for move for 1st player (human vs human) or player (vs AI)  4x4 board
void playerMove4()
{
  int row;
  int column;

  printf("It is your turn (X)!\n");

  do
  {
    printf("Enter the row number (from top to bottom)(1-4): ");
    scanf("%d", &row);
    row-=1;
    printf("Enter the column number (from left to right)(1-4): ");
    scanf("%d", &column);
    column -=1;

    if(num4[row][column] != ' ')
    {
        printf("Invalid square! Please try again.\n");
    }

    else
    {
        num4[row][column] = 'X';
        break;
    }

  } while (num4[row][column] != ' ');

}

//function for move for 2nd player (human vs human) 3x3 board
void player2Move3() {
    int row, column;

    printf("It is Player 2's turn (O)!\n");

    do
    {
        printf("Enter the row number (from top to bottom)(1-3): ");
        scanf("%d", &row);
        row -= 1;

        printf("Enter the column number (from left to right)(1-3): ");
        scanf("%d", &column);
        column -= 1;

        if (num3[row][column] != ' ')
        {
            printf("Invalid square! Please try again.\n");
        }

        else
        {
            num3[row][column] = 'O';
            break;
        }
    } while (num3[row][column] != ' ');
}

//function for move for 2nd player (human vs human) 4x4 board
void player2Move4()
{
    int row, column;

    printf("It is Player 2's turn (O)!\n");

    do
    {
        printf("Enter the row number (from top to bottom)(1-4): ");
        scanf("%d", &row);
        row -= 1;

        printf("Enter the column number (from left to right)(1-4): ");
        scanf("%d", &column);
        column -= 1;

        if (num4[row][column] != ' ')
        {
            printf("Invalid square! Please try again.\n");
        }

        else
        {
            num4[row][column] = 'O';
            break;
        }
    } while (num4[row][column] != ' ');
}

//printing the winner for 3x3 board
void printWinner3(char winner)  //it will print the needed result depends on the argument
{
    if (winner == 'X')
    {
        printf("Player X WON!");
        score_x[1]++;  //+1 for win data for user 1
        score_o[0]++;  //+1 for loss data for user 2
    }

    else if (winner == 'O')
    {
        printf("Player O WON!");
        score_o[1]++;   //+1 for win data for user 2
        score_x[0]++;  //+1 for loss data for user 1
    }

    else
    {
        printf("IT'S A DRAW!");
    }
}

// function with same logic with printing the winner for 4x4 board
void printWinner4(char winner)
{
    if (winner == 'X')
    {
        printf("Player X WON!");
        score_x[1]++;
        score_o[0]++;
    }
    else if (winner == 'O')
    {
        printf("Player O WON!");
        score_o[1]++;
        score_x[0]++;
    }
    else{
        printf("IT'S A DRAW!");
    }
}

//function for easy AI move 3x3 board
void AI_move_easy3()
{
   //creates a seed based on current time
   srand(time(0));
   int row;
   int column;

   if(spaces3() < 9)
   {
      do
      {
         //choose the random free position in row and column to make the move
         row = rand() % 3;
         column = rand() % 3;
      } while (num3[row][column] != ' ');

      num3[row][column] = 'O';
   }
  else
   {
      printWinner4(' '); //no spaces = printing the winner (game is finished)
   }
}

//same logic for function for easy AI move for 4x4 board
void AI_move_easy4()
{
   //creates a seed based on current time
   srand(time(0));
   int row;
   int column;

   if(spaces4() < 16)
   {
      do
      {
         row = rand() % 4;
         column = rand() % 4;
      } while (num4[row][column] != ' ');

      num4[row][column] = 'O';
   }
   else
   {
      printWinner4(' ');
   }
}

//function for medium AI move
void AI_move_medium3()
{
    int row, column;

    //will check for each square the winning position and if the square is empty, will make the move
    for (row = 0; row < 3; row++)
    {
        for (column = 0; column < 3; column++)
        {
            if (num3[row][column] == ' ' && checkWinMove3(row, column, 'X'))
            {
                num3[row][column] = 'O'; // Block the player's winning move
                return;
            }
        }
    }

    //if no blocking is needed, it will make the random move
    AI_move_easy3();

}

// Check if placing a mark in the given row and column will result in a win
int checkWinMove3(int row, int column, char player)
{
    num3[row][column] = player; // Temporarily make the move
    int winner = win3(); // Check if it's a winning move
    num3[row][column] = ' '; // Undo the move
    return (winner == player);
}


//same logic for 4x4 board (medium AI move)
void AI_move_medium4()
{
    int row, column;

    for (row = 0; row < 4; row++)
    {
        for (column = 0; column < 4; column++)
        {
            if (num4[row][column] == ' ' && checkWinMove4(row, column, 'X'))
            {
                num4[row][column] = 'O'; // Block the player's winning move
                return;
            }
        }
    }


    AI_move_easy4();

}

// Check if placing a mark in the given row and column will result in a win
int checkWinMove4(int row, int column, char player)
{
    num4[row][column] = player; // Temporarily make the move
    int winner = win4(); // Check if it's a winning move
    num4[row][column] = ' '; // Undo the move
    return (winner == player);
}

int minimax(char board[3][3], int depth, int isMaximizing) {
    char winner = win3(); // check if there is a winner
    if (winner == 'O')
    {
        return 10 - depth; // ai wins, score depends on depth
    }
    if (winner == 'X')
    {
        return depth - 10; // player wins, score depends on depth
    }

    if (spaces3() == 0)
    {
        return 0; // no empty spaces, it's a draw
    }

    if (isMaximizing)
    {
        int bestScore = -1000; // start with lowest possible score
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (board[row][col] == ' ')
                {
                    board[row][col] = 'O'; // ai makes a move
                    int score = minimax(board, depth + 1, 0); // recursive call to evaluate move
                    board[row][col] = ' '; // undo move
                    bestScore = (score > bestScore) ? score : bestScore; // keep track of best score
                }
            }
        }
        return bestScore; // return the best score found
    }
    else
    {
        int bestScore = 1000; // start with highest possible score
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (board[row][col] == ' ')
                {
                    board[row][col] = 'X'; // player makes a move
                    int score = minimax(board, depth + 1, 1); // recursive call to evaluate move
                    board[row][col] = ' '; // undo move
                    bestScore = (score < bestScore) ? score : bestScore; // keep track of best score
                }
            }
        }
        return bestScore; // return the best score found
    }
}

void AI_move_hard3() {
    int bestScore = -1000; // start with lowest possible score
    int bestMoveRow = -1; // store row of the best move
    int bestMoveCol = -1; // store column of the best move

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (num3[row][col] == ' ')
            {
                num3[row][col] = 'O'; // ai makes a temporary move
                int moveScore = minimax(num3, 0, 0); // evaluate move using minimax
                num3[row][col] = ' '; // undo the move
                if (moveScore > bestScore)
                {
                    bestScore = moveScore; // update best score
                    bestMoveRow = row; // update best row for the move
                    bestMoveCol = col; // update best column for the move
                }
            }
        }
    }

    if (bestMoveRow != -1 && bestMoveCol != -1)
    {
        num3[bestMoveRow][bestMoveCol] = 'O'; // make the best move
    }
}


//same logic for hard AI move for 4x4 board
int minimax_4x4(char board[4][4], int depth, int isMaximizing) {
    char winner = win4(); // Check if someone has won
    if (winner == 'O')
    {
        return 10 - depth; // AI wins
    }

    if (winner == 'X')
    {
        return depth - 10; // Player wins
    }

    if (spaces4() == 0)
    {
        return 0;         // No more spaces, it's a draw
    }

    if (isMaximizing)
    { // AI's turn
        int bestScore = -1000;
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (board[row][col] == ' ') // If the spot is empty
                {
                    board[row][col] = 'O'; // Try this move
                    int score = minimax_4x4(board, depth + 1, 0); // Recurse
                    board[row][col] = ' '; // Undo the move
                    if (score > bestScore)
                    {
                            bestScore = score;
                    }
                }
            }
        }
        return bestScore;

    }

    else
    {
        // Player's turn
        int bestScore = 1000;
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (board[row][col] == ' ')
                {
                    // If the spot is empty
                    board[row][col] = 'X'; // Simulate player's move
                    int score = minimax_4x4(board, depth + 1, 1); // Recurse
                    board[row][col] = ' '; // Undo the move
                    if (score < bestScore)
                    {
                        bestScore = score;
                    }
                }
            }
        }
        return bestScore;
    }
}


void AI_move_hard4()
{
    int bestScore = -1000; // Start with a very low score
    int bestMoveRow = -1;
    int bestMoveCol = -1;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (num4[row][col] == ' ') // If the spot is empty
            {
                num4[row][col] = 'O'; // Try the move
                int moveScore = minimax_4x4(num4, 0, 0); // Get score for the move
                num4[row][col] = ' '; // Undo the move
                if (moveScore > bestScore) // If it's the best score so far
                {
                    bestScore = moveScore;
                    bestMoveRow = row;
                    bestMoveCol = col;
                }
            }
        }
    }

    if (bestMoveRow != -1 && bestMoveCol != -1)
    {
        num4[bestMoveRow][bestMoveCol] = 'O'; // Make the best move
    }
}
