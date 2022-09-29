#include <iostream>
#include <Windows.h>
#include <conio.h>
#define HEIGHT 24
#define WEIDTH 119
#define WINSCORE 50


using namespace std; 
enum DIRECTION_PLAYER { STOP = 0, UP, DOWN };
enum DIRECTION_BALL {PAUSE = 0 , RIGHT_UP = 1 , LEFT_UP = 2, RIGHT_DOWN  = 3, LEFT_DOWN = 4};

class player {
public:
    int score;
    int x;
    DIRECTION_PLAYER dir;
};

class ball {
 public :
     int x, y;
     DIRECTION_BALL dir;
};

player player1, player2;
ball ball0;
int barreLength = 5;
bool roundOver;
bool gameOver;



bool inRangeOfBar(int i, player playerX) {

    for (int j = 0; j < barreLength; j++)
        if (i == playerX.x - 2 + j)
            return true;
    return false;
}

void move_ball_random(int n) {

    switch (n) {
    case 1:
        ball0.dir = RIGHT_UP;
        break;
    case 2:
        ball0.dir = LEFT_UP;
        break;
    case 3:
        ball0.dir = RIGHT_DOWN;
        break;
    case 4:
        ball0.dir = LEFT_DOWN;
        break;
    }
}

void draw() {

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
    for (int i = 0; i < WEIDTH; i++)
        cout << "\xB2";
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WEIDTH; j++) {
            if (ball0.x == i && ball0.y == j)
                cout << "O";
            else
                if ((inRangeOfBar(i, player1) && j == 0) || (inRangeOfBar(i, player2) && j == WEIDTH - 1))
                    cout << "\xDE";

                else
                    cout << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < WEIDTH; i++)
        cout << "\xB2";
    cout << endl;
    cout << "score : " << player1.score << "\t\t\t\t\t\t\t\t\t\t" << "score : " << player2.score << endl;
}

void checkWin() {

    if (player1.score == WINSCORE) {
        cout << " PLAYER 1 HAS WON" << endl;
        gameOver = true;
        draw();
    }
    if (player2.score == WINSCORE) {
        cout << " PLAYER 2 HAS WON" << endl;
        gameOver = true;
        draw();
    }
}

void resetField() {
    
    srand(time(NULL));
    roundOver = false;
    player1.x = player2.x = (HEIGHT / 2);
    ball0.x = HEIGHT / 2;
    ball0.y = WEIDTH / 2;
    player1.dir = STOP;
    player2.dir = STOP;
    ball0.dir = PAUSE;
    draw();
    _getch();
    move_ball_random(rand()%4 + 1);
}

bool ballColidesBar() {
    if ((inRangeOfBar(ball0.x, player1) && ball0.y == 1) || (inRangeOfBar(ball0.x, player2) && ball0.y == WEIDTH - 2))
        return true;
    return false;
}

bool ballColidesCeeling() {
    if (ball0.x < 1 || ball0.x > HEIGHT - 2)
        return true;
    return false;
}

void ballOutOfBounds() {

    if (ball0.y < 1) {
        player2.score += 10;
        roundOver = true;
    }
    else
    if (ball0.y > WEIDTH - 1) {
            player1.score += 10;
            roundOver = true;
    }
}

void changeBallCourseBar() {

    switch (ball0.dir) {
    case RIGHT_UP:
        ball0.dir = LEFT_UP;
        break;
    case LEFT_UP:
        ball0.dir = RIGHT_UP;
        break;
    case RIGHT_DOWN:
        ball0.dir = LEFT_DOWN;
        break;
    case LEFT_DOWN:
        ball0.dir = RIGHT_DOWN;
        break;
    }
}

void changeBallCourseCeeling() {
    
    switch (ball0.dir) {
    case RIGHT_UP:
        ball0.dir = RIGHT_DOWN;
        break;
    case LEFT_UP:
        ball0.dir = LEFT_DOWN;
        break;
    case RIGHT_DOWN:
        ball0.dir = RIGHT_UP;
        break;
    case LEFT_DOWN:
        ball0.dir = LEFT_UP;
        break;
    }

}

void move_ball() {
    switch (ball0.dir) {
    case RIGHT_UP:
        ball0.x--;
        ball0.y++;
         break;
    case LEFT_UP:
        ball0.x--;
        ball0.y--;
         break;
    case RIGHT_DOWN:
        ball0.x++;
        ball0.y++;
         break;
    case LEFT_DOWN:
        ball0.x++;
        ball0.y--;
         break;
    }
}

void move_bar(player& playerX) {
    
    
    switch (playerX.dir) {
    case UP:
        if(playerX.x > 0 + barreLength / 2)
        playerX.x--;
        break;
    case DOWN:
        if (playerX.x < HEIGHT - 1 - barreLength / 2)
        playerX.x++;
        break;
    }
    playerX.dir = STOP;
}

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void setup() {
    
   
    hidecursor();
    gameOver = false;
    player1.score = 0;
    player2.score = 0;
    player1.x = player2.x = (HEIGHT / 2);
    ball0.x = HEIGHT / 2;
    ball0.y = WEIDTH / 2;
    player1.dir = STOP;
    player2.dir = STOP;
    ball0.dir = PAUSE;
}

void input() {
    
    
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'z':
            player1.dir = UP;
            break;
        case 's':
            player1.dir = DOWN;
            break;
        case 'i' : 
            player2.dir = UP;
            break;
        case 'k' :
            player2.dir = DOWN;
            break;
        }

    }

}



void logic() {
   
    move_bar(player1);
    move_bar(player2);
    move_ball();
    if (ballColidesCeeling())
        changeBallCourseCeeling();
    else
     if (ballColidesBar())
        changeBallCourseBar();
    
    ballOutOfBounds();
    checkWin();
   
}

int main()
{
    setup();
    while (!gameOver) {
        resetField();
        while (!roundOver) {
            draw();
            input();
            logic();
        }
    }
    
}

