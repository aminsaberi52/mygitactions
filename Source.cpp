#include <iostream>
#include <GL/glut.h>
#include <string>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 120
#define PADDLE_OFFSET 20
#define BALL_RADIUS 10
#define SCORE_LIMIT 5

int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;
int player1Score = 0;
int player2Score = 0;
bool gameover = false;
int maxScore = 0;

// Ball position and velocity
float ballX = WINDOW_WIDTH / 2;
float ballY = WINDOW_HEIGHT / 2;
float ballVelocityX = 5;
float ballVelocityY = 5;
static float ballVelocityXsaver;
static float ballVelocityYsaver;
float speed_rate;

// Paddle positions
float player1Y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
float player2Y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;

bool paused = false;

void drawRect(float x, float y, float width, float height) 
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawCircle(float x, float y, float radius) 
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) 
    {
        glVertex2f(x + (radius * cos(i * 2.0f * 3.14 / 20)),
            y + (radius * sin(i * 2.0f * 3.14 / 20)));
    }
    glEnd();
}

void drawScore()
{
    // Draw rectangle
    glColor3f(1.0f, 0, 0); // set color to white
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT - 30); // bottom left corner
    glVertex2f(WINDOW_WIDTH / 2 - 20, WINDOW_HEIGHT - 30); // bottom right corner
    glVertex2f(WINDOW_WIDTH / 2 - 20, WINDOW_HEIGHT); // top right corner
    glVertex2f(WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT); // top left corner
    glEnd();

    //glutSwapBuffers();

    glColor3f(1.0f, 1.0f, 0);
    glRasterPos2f(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT - 20);
    std::string score = std::to_string(player1Score) + " - " + std::to_string(player2Score);
    for (int i = 0; i < score.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);

    // display maximum score
    glColor3f(1.0f, 1.0f, 0);
    std::string maxScoreStr = "Max Set Score : " + std::to_string(maxScore);
    glRasterPos2f(WINDOW_WIDTH - 350, 10);
    for (int i = 0; i < maxScoreStr.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, maxScoreStr[i]);
}

void updateBall() 
{
    ballX += ballVelocityX;
    ballY += ballVelocityY;

    // Check for collisions with walls
    if (ballY > windowHeight - BALL_RADIUS || ballY < BALL_RADIUS) 
        ballVelocityY = -ballVelocityY;

    // Check for collisions with paddles
    if (ballX < PADDLE_OFFSET + BALL_RADIUS && ballY >= player1Y && ballY <= player1Y + PADDLE_HEIGHT) 
        ballVelocityX = -ballVelocityX;

    if (ballX > windowWidth - PADDLE_OFFSET - BALL_RADIUS - PADDLE_WIDTH && ballY >= player2Y && ballY <= player2Y + PADDLE_HEIGHT) 
        ballVelocityX = -ballVelocityX;

    // Check for scoring
    if (ballX < BALL_RADIUS) 
    {
        player2Score++;
        if (player2Score == maxScore) 
            gameover = true;

        ballX = WINDOW_WIDTH / 2;
        ballY = WINDOW_HEIGHT / 2;
        ballVelocityX = -ballVelocityX;
    }

    if (ballX > windowWidth - BALL_RADIUS) 
    {
        player1Score++;
        if (player1Score == maxScore) 
            gameover = true;

        ballX = WINDOW_WIDTH / 2;
        ballY = WINDOW_HEIGHT / 2;
        ballVelocityX = -ballVelocityX;
    }
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);

    // Draw paddles
    glColor3f(0.71f, 0.40f, 0.11f);
    drawRect(PADDLE_OFFSET, player1Y, PADDLE_WIDTH, PADDLE_HEIGHT);
    drawRect(windowWidth - PADDLE_OFFSET - PADDLE_WIDTH, player2Y, PADDLE_WIDTH, PADDLE_HEIGHT);

    // Draw dashed line in the middle
    glColor3f(1.0f, 1.0f, 0.5f);
    glLineStipple(1, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2f(windowWidth / 2, 0);
    glVertex2f(windowWidth / 2, windowHeight);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // Draw ball
    glColor3f(1.0f, 0.3f, 0.0f);
    drawCircle(ballX, ballY, BALL_RADIUS);

    // Draw score
    drawScore();

    // Draw game over message
    if (gameover) 
    {
        glColor3f(1.0f, 0.7f, 0.7f);
        glRasterPos2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2);
        std::string message;
        if (player1Score == maxScore) 
            message = "Player 1 wins!";

        else 
        { 
            glColor3f(0.7f, 0.7f, 1.0f);
            glRasterPos2f(WINDOW_WIDTH / 2 + 100, WINDOW_HEIGHT / 2);
            message = "Player 2 wins!"; 
        }
           

        for (int i = 0; i < message.length(); i++) 
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
    }

    // Draw instructions
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(10, windowHeight - 20);
    std::string exitMsg = "Press E to exit";
    for (int i = 0; i < exitMsg.length(); i++) 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, exitMsg[i]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(100, windowHeight - 20);
    std::string pauseMsg = "Press P to Pause";
    for (int i = 0; i < pauseMsg.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pauseMsg[i]);

    glRasterPos2f(windowWidth - 85, windowHeight - 20);
    std::string rstMsg = "Press R to reset points";
    for (int i = 0; i < rstMsg.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, rstMsg[i]);

    glRasterPos2f(windowWidth - 180, windowHeight - 20);
    std::string BallVelocityX = "ballVelocityX = " + std::to_string(ballVelocityX);
    for (int i = 0; i < BallVelocityX.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, BallVelocityX[i]);

    glRasterPos2f(windowWidth - 280, windowHeight - 20);
    std::string BallVelocityY = "ballVelocityY = " + std::to_string(ballVelocityY);
    for (int i = 0; i < BallVelocityY.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, BallVelocityY[i]);

    glRasterPos2f(10, 10);
    std::string player1Msg = "Press W & S";
    for (int i = 0; i < player1Msg.length(); i++) 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, player1Msg[i]);

    glRasterPos2f(windowWidth - 100, 10);
    std::string player2Msg = "Press up and down";
    for (int i = 0; i < player2Msg.length(); i++) 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, player2Msg[i]);

    glColor3f(0.56f, 0.93f, 0.56f);
    glRasterPos2f(10, 30);
    std::string player1 = "Player 1";
    for (int i = 0; i < player1.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, player1[i]);

    glRasterPos2f(windowWidth - 65, 30);
    std::string player2 = "Player 2";
    for (int i = 0; i < player2.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, player2[i]);

    glutSwapBuffers();
}

void keyboard(int key, int x, int y) 
{
    switch (key)
    {
    case GLUT_KEY_UP:
        player2Y += 20;
        break;
    case GLUT_KEY_DOWN:
        player2Y -= 20;
        break;
    }
    glutPostRedisplay();
}

void resetGame()
{
    player1Score = 0;
    player2Score = 0;
    gameover = false;
    ballX = WINDOW_WIDTH / 2;
    ballY = WINDOW_HEIGHT / 2;
    ballVelocityX = 5;
    ballVelocityY = 5;
    player1Y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    player2Y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
}

void update(int value)
{
    if (!gameover && !paused)
    {
        updateBall();
        glutPostRedisplay();
    }
    glutTimerFunc(16, update, 0);
}

void ballbooster()
{
    if (ballVelocityXsaver < 0 && ballVelocityYsaver < 0)
    {
        ballVelocityX -= 1;
        ballVelocityY -= 1;
    }

    else if (ballVelocityXsaver > 0 && ballVelocityYsaver < 0)
    {
        ballVelocityX += 1;
        ballVelocityY -= 1;
    }

    else if (ballVelocityXsaver < 0 && ballVelocityYsaver > 0)
    {
        ballVelocityX -= 1;
        ballVelocityY += 1;
    }

    else if (ballVelocityXsaver > 0 && ballVelocityYsaver > 0)
    {
        ballVelocityX += 1;
        ballVelocityY += 1;
    }
}

void speed_booster()
{
    if (ballVelocityX < 0 && ballVelocityY < 0)
    {
        ballVelocityX -= speed_rate;
        ballVelocityY -= speed_rate;
    }

    else if (ballVelocityX > 0 && ballVelocityY < 0)
    {
        ballVelocityX += speed_rate;
        ballVelocityY -= speed_rate;
    }

    else if (ballVelocityX < 0 && ballVelocityY > 0)
    {
        ballVelocityX -= speed_rate;
        ballVelocityY += speed_rate;
    }

    else if (ballVelocityX > 0 && ballVelocityY > 0)
    {
        ballVelocityX += speed_rate;
        ballVelocityY += speed_rate;
    }

    else if (ballVelocityX == 0 && ballVelocityY == 0)
        ballbooster();
}

void speed_decrescent()
{
    if (ballVelocityX < 0 && ballVelocityY < 0)
    {
        ballVelocityXsaver = ballVelocityX;
        ballVelocityYsaver = ballVelocityY;
        ballVelocityX += speed_rate;
        ballVelocityY += speed_rate;

        if (ballVelocityX > 0 || ballVelocityY > 0)
        {
            ballVelocityX = ballVelocityXsaver;
            ballVelocityY = ballVelocityYsaver;
        }
    }

    else if (ballVelocityX > 0 && ballVelocityY < 0)
    {
        ballVelocityXsaver = ballVelocityX;
        ballVelocityYsaver = ballVelocityY;
        ballVelocityX -= speed_rate;
        ballVelocityY += speed_rate;

        if (ballVelocityX < 0 || ballVelocityY > 0)
        {
            ballVelocityX = ballVelocityXsaver;
            ballVelocityY = ballVelocityYsaver;
        }
    }

    else if (ballVelocityX < 0 && ballVelocityY > 0)
    {
        ballVelocityXsaver = ballVelocityX;
        ballVelocityYsaver = ballVelocityY;
        ballVelocityX += speed_rate;
        ballVelocityY -= speed_rate;

        if (ballVelocityX > 0 || ballVelocityY < 0)
        {
            ballVelocityX = ballVelocityXsaver;
            ballVelocityY = ballVelocityYsaver;
        }
    }

    else if (ballVelocityX > 0 && ballVelocityY > 0)
    {
        ballVelocityXsaver = ballVelocityX;
        ballVelocityYsaver = ballVelocityY;
        ballVelocityX -= speed_rate;
        ballVelocityY -= speed_rate;

        if (ballVelocityX < 0 || ballVelocityY < 0)
        {
            ballVelocityX = ballVelocityXsaver;
            ballVelocityY = ballVelocityYsaver;
        }
    }
}

void keyboardDown(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        player1Y += 20;
        break;
    case 's':
        player1Y -= 20;
        break;
    case 'r':
        resetGame();
        break;
    case 'e':
        exit(0);
        break;
    case 'p':
        paused = !paused; // toggle game state
        break;
    case '+':
        speed_booster();
        break;
    case '-':
        speed_decrescent();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void init() 
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // change the background color to dark blue
    gluOrtho2D(0, windowWidth, 0, windowHeight);
}

void startGame() 
{
    std::cout << "Enter maximum score : ";
    std::cin >> maxScore;
    std::cout << "\nEnter the speed rate for ascending & descending : ";
    std::cin >> speed_rate;
    resetGame();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboardDown);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Pong Game");
    init();
    startGame();
    return 0;
}