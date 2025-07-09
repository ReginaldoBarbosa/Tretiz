#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

// States of the State Machine
#define CREATE_SCENARIO 0
#define ADD_TETRAMINO 1
#define READ_KEYPAD 2
#define MOVE_DOWN 3
#define MOVE_RIGHT 10
#define MOVE_LEFT 20
#define TURN_AROUND 30
#define MOVE_DOWN_FASTER 40
#define VERIFY_ROW 4

// Define the size of the scenario
#define HEIGHT 25
#define WIDTH 10

// List of tetrominos
int I[4][4] = {{0, 0, 0, 0},
               {0, 0, 0, 0},
               {1, 1, 1, 1},
               {0, 0, 0, 0}};

int L[4][4] = {{0, 0, 0, 0},
               {0, 1, 0, 0},
               {0, 1, 0, 0},
               {0, 1, 1, 0}};

int _L[4][4] = {{0, 0, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 1, 1, 0}};

int Q[4][4] = {{0, 0, 0, 0},
               {0, 0, 1, 1},
               {0, 0, 1, 1},
               {0, 0, 0, 0}};

int S[4][4] = {{0, 0, 0, 0},
               {0, 1, 0, 0},
               {0, 1, 1, 0},
               {0, 0, 1, 0}};

int t[4][4] = {{0, 0, 0, 0},
               {0, 1, 0, 0},
               {0, 1, 1, 0},
               {0, 1, 0, 0}};

int Z[4][4] = {{0, 0, 0, 0},
               {0, 0, 1, 0},
               {0, 1, 1, 0},
               {0, 1, 0, 0}};

// Scenario of the game
int map[HEIGHT][WIDTH];

// Filter matrix for rotate the
int filter_rotate[4][4][2] = {{{0, 3}, {1, 2}, {2, 1}, {3, 0}},
                              {{-1, 2}, {0, 1}, {1, 0}, {2, -1}},
                              {{-2, 1}, {-1, 0}, {0, -1}, {1, -2}},
                              {{-3, 0}, {-2, -1}, {-1, -2}, {0, -3}}};

// Var to follow the tetrominos e apply the filter
int pos_row, pos_col;

// Var to receive the state in the state machine
int step = 0;

// Var to receive the keypad
char key = '\0';

// Var to receive the points
int points = 0;

/*****************************************************************************************************************
// Name:        getTetramino
// Function:    Add a randon Tetramino in the variable  **tetramino
// Parameter:   **tetramino is where the Tetramo will be allocated (follow the way of scanf("%",var))
// Return:      No
******************************************************************************************************************/
void getTetramino(int **tetramino)
{

    int aux = 1 + (rand() % 7);

    switch (aux)
    {
    case 1:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = I[i][j];
        break;
    case 2:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = L[i][j];
        break;
    case 3:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = _L[i][j];
        break;
    case 4:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = Q[i][j];
        break;
    case 5:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = S[i][j];
        break;
    case 6:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = t[i][j];
        break;
    case 7:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetramino[i][j] = Z[i][j];
        break;
    default:
        break;
    }
}

/*****************************************************************************************************************
// Name:        scenario_add_tetramino
// Function:    Add the **tetromino in the map
// Parameter:   No
// Return:      No
******************************************************************************************************************/
void scenario_add_tetramino()
{
    int **tetramino;

    tetramino = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tetramino[i] = (int *)malloc(4 * sizeof(int));

    getTetramino(tetramino);

    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            map[i][j] = (i >= 0 && i <= 3 && j >= 3 && j <= 6) ? map[i][j] + tetramino[i][j - 3] : map[i][j];
        }
    }
    pos_row = 0;
    pos_col = 3;
    free(tetramino);
}

/*****************************************************************************************************************
// Name:        set_scenario
// Function:    Change the value of the peaces (1) to the value related to the fixed number (2)
// Parameter:   No
// Return:      No
******************************************************************************************************************/
void set_scenario()
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            map[i][j] = (map[i][j] == 1) ? 2 : map[i][j];
        }
    }
}

/*****************************************************************************************************************
// Name:        scenario_print_update
// Function:    Print the scenario.
// Parameter:   No
// Return:      No
******************************************************************************************************************/
int scenario_print_update()
{
    static unsigned long time_down = 0;
    if (GetTickCount() - time_down < 400)
    {
        return -1;
    }
    time_down = GetTickCount();
    printf("-------------------------------------------------------------------------------------\n");
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            printf("%s", (map[i][j] == 0 ? "|_" : "|X"));
        }
        printf("\n");
    }
    printf("-------------------------------------------------------------------------------------\n");
    printf("--POINTS: %i------------------------------------------------------------------------\n", points);
    printf("-------------------------------------------------------------------------------------\n");
    return 0;
}

/*****************************************************************************************************************
// Name:        scenario_zero
// Function:    Fill out the scenario with 0s.
// Parameter:   No
// Return:      No
******************************************************************************************************************/
void scenario_zero()
{
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            map[i][j] = 0;
        }
    }
}

/*****************************************************************************************************************
// Name:        get_pad_key
// Function:    get... pad...key   -_-  '_'
// Parameter:   No
// Return:      pad key
******************************************************************************************************************/
char get_pad_key()
{
    if (_kbhit())
    {
        return _getch(); // Retorna o código ASCII da tecla
    }
    return '\0'; // Nenhuma tecla foi pressionada
}

/*****************************************************************************************************************
// Name:        move_right
// Function:    move...the tetromino... to right   :(
// Parameter:   No
// Return:      0: can move; 1:can`t move
******************************************************************************************************************/
int move_right()
{
    static unsigned long time_down = 0;

    time_down = GetTickCount();
    int bool_down = 0;
    // Verify the space in righ the tetramino
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (map[i][j] == 1 && (map[i][j - 1] == 2 || (j) == 0))
            {
                bool_down = 1;
            }
        }
    }
    // if there is space, the peace is putted down.
    if (bool_down == 0)
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (map[i][j] == 1)
                {
                    map[i][j - 1] = 1;
                    map[i][j] = 0;
                }
            }
        }
        pos_col--;
    }

    return bool_down;
}

/*****************************************************************************************************************
// Name:        move_left
// Function:    move...th... really man? >:(
// Parameter:   No
// Return:      0: can move; 1:can`t move
******************************************************************************************************************/
int move_left()
{
    static unsigned long time_down = 0;

    time_down = GetTickCount();
    int bool_down = 0;
    // Verify the space in righ the tetramino
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = WIDTH - 1; j >= 0; j--)
        {
            if (map[i][j] == 1 && (map[i][j + 1] == 2 || (j) == WIDTH - 1))
            {
                bool_down = 1;
            }
        }
    }
    // if there is space, the peace is putted down.
    if (bool_down == 0)
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = WIDTH - 1; j >= 0; j--)
            {
                if (map[i][j] == 1)
                {
                    map[i][j + 1] = 1;
                    map[i][j] = 0;
                }
            }
        }
        pos_col++;
    }
    return bool_down;
}

/*****************************************************************************************************************
// Name:        move_down_tetramino
// Function:    aaaaaaaah >:()
// Parameter:   time_delay: the time to drop down the peace, normally 400ms, and 200ms when accelerated.
// Return:      0: can move; 1:can`t move
******************************************************************************************************************/
int move_down_tetramino(int time_delay)
{
    static unsigned long time_down = 0;
    if (GetTickCount() - time_down < time_delay)
    {
        return -1;
    }
    time_down = GetTickCount();
    int bool_down = 0;
    // Verify the space under the tetramino
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int j = WIDTH - 1; j >= 0; j--)
        {
            if (map[i][j] == 1 && (map[i + 1][j] == 2 || (i) == HEIGHT - 1))
            {
                bool_down = 1;
                set_scenario();
            }
        }
    }
    // if there is space, the peace is putted down.
    if (bool_down == 0)
    {
        for (int i = HEIGHT - 2; i >= 0; i--)
        {
            for (int j = WIDTH - 1; j >= 0; j--)
            {
                if (map[i][j] == 1)
                {
                    map[i + 1][j] = 1;
                    map[i][j] = 0;
                }
            }
        }
    }
    pos_row++;
    return bool_down;
}

/*****************************************************************************************************************
// Name:        turn_around
// Function:    rotate the tetromino
// Parameter:   No
// Return:      0: can move; 1:can`t move
******************************************************************************************************************/
int turn_around()
{
    static unsigned long time_down = 0;

    time_down = GetTickCount();
    int bool_down = 0;

    int filter_flag = 0;
    int k = 0, l = 0;

    // Verify the space in righ the tetramino
    for (int i = pos_row; i < pos_row + 4; i++)
    {
        for (int j = pos_col; j < pos_col + 4; j++)
        {
            if (map[i][j] == 1 && ((j + 4) > 10))
            {
                bool_down = 1;
            }
        }
    }

    if (bool_down == 0)
    {
        for (int i = pos_row, filt_row = 0; i < pos_row + 4; i++, filt_row++)
        {
            for (int j = pos_col, filt_col = 0; j < pos_col + 4; j++, filt_col++)
            {

                if ((map[i][j] == 1 || map[i][j] == 6) && map[i + filter_rotate[filt_row][filt_col][0]][j + filter_rotate[filt_row][filt_col][1]] == 0)
                {
                    map[i + filter_rotate[filt_row][filt_col][0]][j + filter_rotate[filt_row][filt_col][1]] = 5;
                    map[i][j] = (map[i][j] == 1) ? 0 : 5;
                }
                else if ((map[i][j] == 1 || map[i][j] == 6) && map[i + filter_rotate[filt_row][filt_col][0]][j + filter_rotate[filt_row][filt_col][1]] == 1)
                {
                    map[i + filter_rotate[filt_row][filt_col][0]][j + filter_rotate[filt_row][filt_col][1]] = 6;
                    map[i][j] = (map[i][j] == 1) ? 0 : 5;
                }
            }
        }
    }

    // change the peaces back to 1
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (map[i][j] == 1 || map[i][j] == 6 || map[i][j] == 5)
            {

                map[i][j] = 1;
            }
        }
    }
    return bool_down;
}

/*****************************************************************************************************************
// Name:        verify_row
// Function:    verify the row to see if there is a complete row with 2s, when it finds a complete row,
                the points is increased in 100, the row is cleaned and the peaces of the high are droped down.
// Parameter:   No
// Return:      0: can move; 1:can`t move
******************************************************************************************************************/
void verify_row()
{
    int count_col = 0;

    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int j = WIDTH - 1; j >= 0; j--)
        {
            count_col = (map[i][j] == 2) ? count_col + 1 : count_col;
        }
        if (count_col == WIDTH)
        {
            for (int j = WIDTH - 1; j >= 0; j--)
            {
                map[i][j] = 0;
            }
            points = points + 100;
            for (int k = i; k > 0; k--)
            {
                for (int l = 0; l < WIDTH; l++)
                {
                    map[k][l] = map[k - 1][l];
                }
            }
        }
        count_col = 0;
    }
}

/*****************************************************************************************************************
// Name:        state_machine_game
// Function:    State machine of the game
// Parameter:   No
// Return:      No
******************************************************************************************************************/
void state_machine_game()
{

    switch (step)
    {
    case CREATE_SCENARIO:
        scenario_zero();
        step = ADD_TETRAMINO;
        break;
    case ADD_TETRAMINO:
        scenario_add_tetramino();
        scenario_print_update();
        step = READ_KEYPAD;
        break;
    case READ_KEYPAD:
        key = get_pad_key();
        switch (key)
        {
        case 'a':
            step = MOVE_RIGHT;
            break;
        case 'd':
            step = MOVE_LEFT;
            break;
        case 'w':
            step = TURN_AROUND;
            break;
        case 's':
            step = MOVE_DOWN_FASTER;
            break;
        case '\0':
            step = MOVE_DOWN;
            break;
        default:
            break;
        }
        break;
    case MOVE_DOWN:
        if (move_down_tetramino(400) == 1)
            step = ADD_TETRAMINO;
        else
            step = VERIFY_ROW;
        scenario_print_update();
        break;
    case MOVE_RIGHT:
        move_right();
        scenario_print_update();
        step = 2;
        break;
    case MOVE_LEFT:
        move_left();
        scenario_print_update();
        step = 2;
        break;
    case TURN_AROUND:
        turn_around();
        scenario_print_update();
        step = 2;
        break;
    case MOVE_DOWN_FASTER:
        move_down_tetramino(200);
        scenario_print_update();
        step = 2;
        break;
    case VERIFY_ROW:
        verify_row();
        step = READ_KEYPAD;
        break;
    }
}

void main()
{
    srand(time(NULL));

    while (1)
    {
        state_machine_game();
    }
}
