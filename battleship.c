//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include "lab_buttons.h"

#define MOVE_PIXELS   1

// Turn Debuggin off
#undef DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

#define UP_BUTTON 0x01

#define LOCAL_ID     14
#define REMOTE_ID    12
 //pixel positions for various parts of the board
#define LEFT_EDGE 239
#define RIGHT_EDGE 0
#define TOP_EDGE 319
#define BOTTOM_EDGE  80
#define circleStartX 222
#define circleStartY 98
#define AX 2
/******************************************************************************
 * Global Variables
 *****************************************************************************/


//*****************************************************************************
//*****************************************************************************
/* Takes an integer of the opponents ships, and an array of chars with the currentState
of the gae.  This currentState array keeps of track of hits or misses on the ships.  This function
checks for horizontal and vertical ships hit and if hit in a row = 3, counts it as a sunk ship for score display */
int opponentShips(int oppShips, char currentState[7][7])
{
	int currScore = 0;
	int hitInARow = 0;
	//iterates through horizontally
	for (int i = 0; i < 7; i++)
	{
		for(int j= 0; j < 7; j++)
		{
			if (currentState[i][j] == 'H')
			{
				hitInARow += 1;
			}
			else if(currentState[i][j] == 'M' || currentState[i][j] == 'n')
			{
				hitInARow = 0;
			}
			if (hitInARow == 3)
			{
				hitInARow = 0;
				currScore += 1;
			}
		}
	}
	hitInARow = 0;
	//iterates through vertically
	for (int i = 0; i < 7; i++)
	{
		for(int j= 0; j < 7; j++)
		{
			if (currentState[j][i] == 'H')
			{
				hitInARow += 1;
			}
			else if(currentState[j][i] == 'M' || currentState[j][i] == 'n')
			{
				hitInARow = 0;
			}
			if (hitInARow == 3)
			{
				hitInARow = 0;
				currScore += 1;
			}
		}
	}
	hitInARow = 0;

	return 4 - currScore;


}




/*This function draws the board using the currentState array of the board and */
void drawState(int board[][7][2], char currentState[][7],  int currX, int currY)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if ( board[i][j][0] == currX & board[i][j][1] == currY)
			{
				switch(currentState[i][j])
				{
				case 'M':
				ece210_lcd_print_string("M", currX + 8, currY - 8, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
				break;
				case 'H':
				ece210_lcd_print_string("H", currX + 6, currY - 8, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
				break;
				}
			}
		}
	}
}
/* main game loop, handles player movement and transmission between boards */
int targetMode( int currX, int currY, int shipBoard1[7][7], int board[7][7][2], char currentState[7][7], int oppShips)
{

int transmission = 0;
bool loop = true;
int temp = 0;
while (loop)
{
	if (AlertButtons)
	{
		AlertButtons = false;
		if (btn_right_pressed())
		{
			if ( currX - 34 > 0)
			{
				ece210_lcd_draw_circle (currX, currY, 8, LCD_COLOR_BLACK);
				drawState(board, currentState, currX, currY);
				currX -= 34;
				ece210_lcd_draw_circle (currX, currY, 8, LCD_COLOR_RED);
			}
		}
		if (btn_up_pressed())
		{
			if ( currY + 34 < 319 )
			{
				ece210_lcd_draw_circle (currX, currY, 8, LCD_COLOR_BLACK);
				drawState(board, currentState, currX, currY);
				currY += 34;
				ece210_lcd_draw_circle (currX, currY, 8, LCD_COLOR_RED);
			}
		}
		if (btn_down_pressed())
		{
			if ( currY - 34 > 80 )
			{
				ece210_lcd_draw_circle (currX, currY, 8, LCD_COLOR_BLACK);
				drawState(board, currentState, currX, currY);
				currY -= 34;
				ece210_lcd_draw_circle (currX, currY, 8, LCD_COLOR_RED);
			}
		}
		if (btn_left_pressed())
		{
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					if ( board[i][j][0] == currX & board[i][j][1] == currY)
					{
						switch (shipBoard1[i][j])
						{
							case 0:
							currentState[i][j] = 'M';
							ece210_lcd_print_string("M", currX + 8, currY - 8, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
							oppShips = scoreCheck(currentState, oppShips,transmission);
							transmission = 1;
							break;
							case 1:
							shipBoard1[i][j] = 2;
							ece210_lcd_print_string("H", currX + 6, currY - 8, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
							currentState[i][j] = 'H';
							temp = scoreCheck(currentState, oppShips,transmission);

							if (temp == oppShips - 1)
							{
								transmission = 3;
							}
							else if ( temp == 0)
							{
								transmission = 4;
							}
							else
							{
								transmission = 2;
							}
							break;
							case 2:
							shipBoard1[i][j] = 2;
							ece210_lcd_print_string("H", currX + 8, currY - 8, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
							oppShips = scoreCheck(currentState, oppShips,transmission);
							transmission = 1;
							break;
						}
					}
				}
			}
			loop = false;
		}
	}
}
return transmission;

}
/*Iterates through currentState to display game score and whether game is  complete or not. */
int scoreCheck(char currentState[][7], int score, int transmission)
{
	int currScore = 0;
	int hitInARow = 0;
	for (int i = 0; i < 7; i++)
	{
		for(int j= 0; j < 7; j++)
		{
			if (currentState[i][j] == 'H')
			{
				hitInARow += 1;
			}
			else if(currentState[i][j] == 'M' || currentState[i][j] == 'n')
			{
				hitInARow = 0;
			}
			if (hitInARow == 3)
			{
				hitInARow = 0;
				currScore += 1;
			}
		}
	}
	hitInARow = 0;
	for (int i = 0; i < 7; i++)
	{
		for(int j= 0; j < 7; j++)
		{
			if (currentState[j][i] == 'H')
			{
				hitInARow += 1;
			}
			else if(currentState[j][i] == 'M' || currentState[j][i] == 'n')
			{
				hitInARow = 0;
			}
			if (hitInARow == 3)
			{
				hitInARow = 0;
				currScore += 1;
			}
		}
	}
	hitInARow = 0;
	if (4 - currScore < score)
	{
		ece210_lcd_add_msg("You sunk a ship!",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
		if (currScore == 4)
		{
			ece210_lcd_add_msg("You win!",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
		}
		return 4 - currScore;
	}
	else
	{
		return score;
	}
}
/*Handles game initialization and play*/
main(void)
{
	char msg[80];
	uint32_t rx_data;
	uint32_t tx_data;
	uint8_t buttons;
	uint16_t currX;
	uint16_t currY;
	int oppShips = 4;
	int userShips = 4;
	bool userTurn = false;
	bool game = true;

	ece210_initialize_board();
	ece210_wireless_init(LOCAL_ID,REMOTE_ID);

	bool userReady = false;
	bool friendReady = false;

	ece210_lcd_add_msg("Press Up Button to Start", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
	while(!(userReady && friendReady))
	{
		if(ece210_wireless_data_avaiable())
		{
			rx_data = ece210_wireless_get();
			if( rx_data == UP_BUTTON)
			{
				ece210_lcd_add_msg("Other Player is Ready", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
				friendReady = true;
			}

		}
		if(AlertButtons)
		{
			AlertButtons = false;

			// Transmit data
			buttons = ece210_buttons_read();
			if(buttons == UP_BUTTON)
			{
				ece210_lcd_add_msg("You are Ready", TERMINAL_ALIGN_CENTER, LCD_COLOR_GREEN);
				ece210_wireless_send(UP_BUTTON);
				userReady = true;
				if(friendReady == false && userReady == true){
					userTurn = true;
				}
			}
		}


	}

	ece210_lcd_draw_rectangle  (0, 239, 0, 319, LCD_COLOR_BLACK);


	for (int i = 80; i < 319; i += 34)
	{
		ece210_lcd_draw_line( 0, i, 239, i, LCD_COLOR_BLUE);
	}
	for (int i = 0; i < 239; i += 34)
	{
		ece210_lcd_draw_line( i, 80, i, 319, LCD_COLOR_BLUE);
	}

	ece210_lcd_draw_circle (circleStartX, circleStartY, 8, LCD_COLOR_RED);
	currX = circleStartX;
	currY = circleStartY;
	int board[7][7][2];    //initializes game board
	int x = LEFT_EDGE - 17;
	int y = TOP_EDGE - 17;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				if (k == 0)
				{
					board[i][j][k] = x;
					x -= 34;
					if (j ==6) x = 222;
				}
				else
				{
					board[i][j][k] = y;
					if (j == 6) y -= 34;
				}
			}
		}
	} //end game board initialization
	int shipBoard1[7][7] =
	{ { 0, 0, 1, 1, 1, 0, 0},
	{ 1, 0, 0, 0, 0, 0, 0},
	{ 1, 0, 0, 1, 1, 1, 0},
	{ 1, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0},
	{ 1, 1, 1, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0} };
	char currentState[7][7];
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			currentState[i][j] = 'n';
		}
	}

	while(game)
	{
		if (userTurn)
		{
			ece210_wireless_send(targetMode(currX,currY,shipBoard1,board,currentState,oppShips));
			userTurn = false;
		}
		else if (!userTurn)
		{
			if (ece210_wireless_data_avaiable())
			{
				ece210_lcd_draw_rectangle  (0, 239, 0, 79, LCD_COLOR_BLACK);
				switch(ece210_wireless_get())
				{
					case 1:
					ece210_lcd_print_string("Opponent missed: Your turn", 239, 5, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					userTurn = true;
					break;
					case 2:
					ece210_lcd_print_string("Your Ship hit!: Your turn", 220, 5, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					userTurn = true;
					break;
					case 3:
					ece210_lcd_print_string("Your Ship Lost!: Your turn", 220, 5, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					userShips -= 1;
					userTurn = true;
					break;
					case 4:
					ece210_lcd_print_string("Game Over", 220, 5, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					game = false;
					break;
				}
				switch (opponentShips(oppShips, currentState))
				{
					case 1:
					ece210_lcd_print_string("Opponent Ships: 1", 200, 60, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					break;
					case 2:
					ece210_lcd_print_string("Opponent Ships: 2", 200, 60, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					break;
					case 3:
					ece210_lcd_print_string("Opponent Ships: 3", 200, 60, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					break;
					case 4:
					ece210_lcd_print_string("Opponent Ships: 4", 200, 60, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
					break;
				}
			}

		}
	}
