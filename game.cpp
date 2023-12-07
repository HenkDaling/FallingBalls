/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include "game.h"
#include <chrono>
#include <iostream>

#include <algorithm>
#include <cstdint>
#include <iomanip>

using namespace std::chrono;
using namespace std;

// Constructor for the 'game' class
game::game(SDL_Plotter &g)
{
    // Initializing game elements and objects

    // Initialize and display the direction arrow
    arrowImg = Image(arrow);
    directionArrow = (arrowImg);
    directionArrow.setLocation(point2D(g.getCol() / 2.0, 20));
    directionArrow.show();

    // Initialize and display the ball
    b = ball(directionArrow.getPosition() - point2D(beachBall.width / 2, beachBall.height / 2), Vector2D::createFromCartesian(0, 0));
    b.setLocation(point2D(g.getCol() / 2.0, 20) - point2D(beachBall.width / 2, beachBall.height / 2));
    b.setBoundry(polygon::makeCircle((b.getPosition() + point2D(42, 42)), 42, 30));
    b.show();

    // Initialize and display the box
    box.setLocation(point2D(0, 0));
    box.setBoundry(polygon::makeCornerSquare(box.getPosition(), 800, 10));
    box.show();

    // Initialize and display the left wall
    wallLeft.setLocation(point2D(0.0, 800));
    wallLeft.setBoundry(polygon::makeCornerSquare(wallLeft.getPosition(), 10, 800));
    wallLeft.show();

    // Initialize and display the right wall
    wallRight.setLocation(point2D(830, 800));
    wallRight.setBoundry(polygon::makeCornerSquare(wallRight.getPosition(), 10, 800));
    wallRight.show();

    walls.push_back(wallLeft);
    walls.push_back(wallRight);
    walls.push_back(box);

    level = 1;

    // Generate the initial level of obstacles
    generateLevel(1, obst);
}

// Destructor for the 'game' class
game::~game()
{
}

// Function to generate obstacles for a given level
void game::generateLevel(int level, vector<obstacle> &o)
{
    for (int i = 0; (i < 4); i++)
    {
        obstacle newO;

        // Randomly set position, color, and shape for each obstacle
        int randomX = rand() % 720 + 40;
        int randomY = 700 - rand() % 100;
        int R = rand() % 100 + 155;
        int G = rand() % 100 + 155;
        int B = rand() % 100 + 155;

        newO.updatePosition(point2D(randomX, randomY));

        int shape = rand() % 3;

        switch (shape)
        {
        case 0:
            newO.setBoundry(polygon::makeCenterSquare(newO.getPosition(), 100, 100));
            break;
        case 1:
            newO.setBoundry(polygon::makeCircle(newO.getPosition(), 50, 6));
            break;
        case 2:
            newO.setBoundry(polygon::makeEquilateralTriangle(newO.getPosition(), 100));
            break;
        }

        newO.show();
        newO.setCount(level + rand() % level / 2);
        newO.setColor(color(R, G, B));

        o.push_back(newO);
    }
}

// Function to move obstacles upwards
void game::moveUp(vector<obstacle> &o)
{
    point2D pos;

    for (size_t i = 0; i < o.size(); i++)
    {
        pos = o.at(i).getPosition();
        pos.y -= 150.0;
        o.at(i).updatePosition(pos);
    }
}

// Function to handle the game logic during active gameplay
gameState game::playingGame(SDL_Plotter &g)
{
    gameState nextState;
    bool pause = false;
    bool gameOver = false;

    Image bPause = Image(pauseButton);
    Object buttonPause(bPause);
    Image bQuit = Image(quitButton);
    Object buttonQuit(bQuit);
    Image bgImage(backgroundImg);
    double a = 0.001;
    int mouseX, mouseY;
    Vector2D direction;
    vector<obstacle> valid;

    dt = dt + 1;

    g.clear();
    draw::drawImagePixels(point2D(0, 0), bgImage, g);

    // Process obstacles and remove dead ones
    for (size_t i = 0; i < obst.size(); i++)
    {
        if (obst.at(i).isDead())
        {
            obst.at(i).hide();
        }
        else
        {
            valid.push_back(obst.at(i));
            if (obst.at(i).isTop())
            {
                gameOver = true;
            }
        }
    }
    obst.clear();
    obst.insert(obst.begin(), valid.begin(), valid.end());

    // Logic for advancing to the next level
    if ((b.getPosition().y > 800))
    {
        level++;
        moveUp(obst);
        generateLevel(level, obst);

        b.setLocation(arrowPos - point2D(beachBall.width / 2, beachBall.height / 2));
        b.giveVelocity(Vector2D::createFromCartesian(0, 0));
        b.setForce(Vector2D::createFromCartesian(0, 0));
    }

    // Update arrow position and direction
    arrowPos = directionArrow.getPosition();
    arrowPos.y = arrowPos.y + 0.5 * arrow.height;

    g.getMouseLocation(mouseX, mouseY);
    direction = Vector2D::createFromCartesian(mouseX - arrowPos.x, mouseY - arrowPos.y);
    direction.normalize();

    // Display arrow and target
    directionArrow.drawImage(g, direction.getAngle(), arrowPos, color(255, 255, 255));
    draw::drawFillPolygon(polygon::makeCircle(arrowPos, 20, 20).getPoints(), g, color(255, 0, 0));

    // Display pause and quit buttons
    buttonPause.setLocation(point2D(750, 10));
    buttonPause.setBoundry(polygon::makeCornerSquare(buttonPause.getPosition(), 75, -75));
    buttonPause.drawObject(g, draw_polygon_outline);
    buttonPause.show();

    buttonQuit.setLocation(point2D(750, 95));
    buttonQuit.setBoundry(polygon::makeCornerSquare(buttonQuit.getPosition(), 75, -75));
    buttonQuit.drawObject(g, draw_polygon_outline);
    buttonQuit.show();

    // Process mouse click events
    if (g.mouseClick())
    {
        int x, y;
        g.getMouseLocation(x, y);

        // Apply force to the ball and handle button clicks
       
        b.applyForce(Vector2D(6, directions.up));

        if (buttonPause.isInside(point2D(x, y)))
        {
            pause = true;
        }
        else if (buttonQuit.isInside(point2D(x, y)))
        {
            g.setQuit(true);
        }
        else
        {
            g.getMouseClick();
            b.setLocation(arrowPos - point2D(beachBall.width / 2, beachBall.height / 2));
            b.giveVelocity(direction * 100.0);
        }
    }

    // Update ball position and check collisions with walls
    static double ang = 0;
    ang += 0.01;
    b.update(dt);
    b.drawImage(g, 0, b.getBoundry().calculateCenter(), color(255, 255, 255));

    for (Object &wall : walls)
    {
        wall.drawObject(g, draw_polygon_fill, color(0, 200, 200));
        b.isColiding(wall);
    }

    // Rotate and display obstacles
    for (size_t i = 0; i < obst.size(); i++)
    {
        obst.at(i).rotate(a, obst.at(i).getPosition());
        obst.at(i).drawObstacle(g);
    }

    // Check collisions between ball and obstacles
    for (size_t i = 0; i < obst.size(); i++)
    {
        if (!obst.at(i).isDead())
        {
            obst.at(i).isColiding(b);
            if (b.isColiding(obst.at(i)))
            {
                cout << "checking\n";
                obst.at(i).handleCollision(b);
            }
        }
    }

    // Draw pause and quit buttons
    buttonPause.drawImage(g);
    buttonQuit.drawImage(g);

    // Update game state based on user actions
    if (pause)
    {
        nextState = game_paused;
    }
    else if (gameOver)
    {
        nextState = game_gameOver;
    }
    else
    {
        nextState = game_run;
    }

    return nextState;
}

// Function to handle the paused state of the game
gameState game::pauseGame(SDL_Plotter &g)
{
    gameState nextState = game_paused;

    // Data abstraction
    Image sButtonImage(ButtonImg);
    Image banner(bannerImg);
    Object startButton(sButtonImage);
    point2D mouseClick;

    point2D buttonLoc(330, 500);
    point2D textLoc = buttonLoc + point2D(60, 30);
    startButton.setLocation(buttonLoc);
    startButton.setBoundry(polygon::makeCornerSquare(buttonLoc, 200, -80));

    if (g.mouseClick())
    {
        point mouse = g.getMouseClick();
        mouseClick.x = mouse.x;
        mouseClick.y = mouse.y;

        if (startButton.isInside(mouseClick))
        {
            nextState = game_run;
        }

        g.getMouseClick();
    }

    static double dt = 0;
    static bool init = false;
    static bool inits = false;
    dt += 1;
    static Uint32 *screen = new Uint32[800 * 840];

    // Initialize screen pixels
    if (!init)
    {
        for (int y = 0; y < 800; y++)
        {
            for (int x = 0; x < 840; x++)
            {
                screen[y * 800 + x] = g.getColor(x, y);
            }
        }
        init = true;
    }

    if (!inits)
    {
        for (int y = 0; y < 800; y++)
        {
            for (int x = 0; x < 840; x++)
            {
                screen[y * 800 + x] = g.getColor(x, y);
            }
        }
        inits = true;
    }

    // Apply effects to the screen pixels
    for (int y = 0; y < 800; y++)
    {
        for (int x = 0; x < 840; x++)
        {
            // Calculate color based on position
            double gradientFactor = 1;
            double fadeFactor = 1;
            Uint32 bg = screen[y * 800 + x];

            gradientFactor = cos((y / (rand() % 8 + 3) - dt) * -1);

            int red = (bg >> 16) & 0xFF;
            int green = (bg >> 8) & 0xFF;
            int blue = (bg & 0xFF);

            red = red;
            green = green;
            blue = blue;

            int ga = static_cast<int>(0.299 * red + 0.587 * green + 0.114 * blue) * gradientFactor * 0.5;

            double drawY = y + gradientFactor;

            if (x >= 0 && drawY >= 0 && x < 840 && drawY < 800)
            {
                // Draw pixel
                g.plotPixel(x, drawY, ga, ga, ga);
            }
        }
    }

    draw::drawImagePixels(point2D(80, 200), banner, g, 0, point2D(), color(255, 0, 255));
    startButton.drawObject(g, draw_image);

    return nextState;
}

// Function to handle the game over state
gameState game::gameOver(SDL_Plotter &g)
{
    gameState nextState = game_gameOver;

    // Back to defaults
    obst.clear();
    level = 1;

    // Data abstraction
    Image sButtonImage(ButtonImg);
    Object startButton(sButtonImage);
    Image bannerGameOver(gameOverImg);

    double gradientFactor;
    double fadeFactor;
    Uint32 bg;
    static double dt = 0;
    static bool init = false;
    static Uint32 *screen = new Uint32[800 * 840];

    point2D mouseClick;
    point2D buttonLoc(330, 500);

    point mouse;

    dt += 1;

    draw::drawImagePixels(point2D(100, 200), bannerGameOver, g, 0, point2D());

    // Initialization
    if (!init)
    {
        obst.clear();

        for (int y = 0; y < 800; y++)
        {
            for (int x = 0; x < 840; x++)
            {
                screen[y * 800 + x] = g.getColor(x, y);
            }
        }
        init = true;
    }

    startButton.setLocation(buttonLoc);
    startButton.setBoundry(polygon::makeCornerSquare(buttonLoc, 200, -80));

    // Input
    if (g.mouseClick())
    {
        mouse = g.getMouseClick();
        mouseClick.x = mouse.x;
        mouseClick.y = mouse.y;

        if (startButton.isInside(mouseClick))
        {
            nextState = game_run;
        }

        g.getMouseClick();
    }

    // Output
    for (int y = 0; y < 800; y++)
    {
        for (int x = 0; x < 840; x++)
        {
            // Calculate color based on position
            gradientFactor =
            static_cast<double>(y) / 800; // Gradient based on vertical position
            bg = screen[y * 800 + x];

            gradientFactor = cos((y / (rand() % 8 + 3) - dt) * -1);

            gradientFactor *= (cos(((x - 420) / 420.0) * (3.14 / 2.0))) * 0.5;
            gradientFactor *= (cos(((y - 400) / 400.0) * (3.14 / 2.0))) * 0.5;

            fadeFactor *= (cos(((x - 420) / 420.0) * (3.14 / 2.0)));
            fadeFactor *= (cos(((y - 400) / 400.0) * (3.14 / 2.0)));

            int red = (bg >> 16) & 0xFF;
            int green = (bg >> 8) & 0xFF;
            int blue = (bg & 0xFF);

            red = red * gradientFactor;
            green = green * 0.1 + (20 * gradientFactor);
            blue = blue * 0.1 + (20 * gradientFactor);

            // Draw pixel
            g.plotPixel(x, y, red, green, blue);
        }
    }

    startButton.drawObject(g, draw_image);

    return nextState;
}

// Function to handle the start screen state
gameState game::startScreen(SDL_Plotter &g)
{
    gameState nextState = game_startScreen;

    // Data abstraction
    Image startImage(backgroundStart);
    Object startText;

    Image sButtonImage(ButtonImg);
    Object startButton(sButtonImage);
    point2D mouseClick;

    point2D buttonLoc(330, 340);
    point2D textLoc = buttonLoc + point2D(60, 30);
    point mouse;

    startButton.setLocation(buttonLoc);
    startButton.setTextLocation(textLoc);
    startButton.setBoundry(polygon::makeCornerSquare(point2D(330, 350), 200, -80));

    if (g.mouseClick())
    {
        cout << "start screen: mouse click" << endl;

        mouse = g.getMouseClick();
        mouseClick.x = mouse.x;
        mouseClick.y = mouse.y;

        if (startButton.isInside(mouseClick))
        {
            nextState = game_run;
        }

        g.getMouseClick();
    }

    static bool init = false;
    if (!init)
    {
        draw::drawImagePixels(point2D(0, 0), startImage, g);
        init = true;
    }

    static double dt = 0;
    static bool inits = false;
    dt += 1;

    static Uint32 *screen = new Uint32[800 * 840];

    if (!inits)
    {
        for (int y = 0; y < 800; y++)
        {
            for (int x = 0; x < 840; x++)
            {
                screen[y * 800 + x] = g.getColor(x, y);
            }
        }
        inits = true;
    }

    for (int y = 0; y < 800; y++)
    {
        for (int x = 0; x < 840; x++)
        {
            // Calculate color based on position
            double gradientFactor = 1;
            double fadeFactor = 1;
            Uint32 bg = screen[y * 800 + x];

            gradientFactor = cos((y / (rand() % 8 + 3) - dt) * -1);

            int red = (bg >> 16) & 0xFF;
            int green = (bg >> 8) & 0xFF;
            int blue = (bg & 0xFF);

            double drawY = y + gradientFactor;

            if (x >= 0 && drawY >= 0 && x < 840 && drawY < 800)
            {
                // Draw pixel
                g.plotPixel(x, drawY, red, green, blue);
            }
        }
    }

    startButton.drawImage(g);

    return nextState;
}
