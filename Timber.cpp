#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;

const int NUM_BRANCHES = 6;

const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;

enum class side { LEFT, RIGHT, NONE };

side branchPositions[NUM_BRANCHES];

Sprite branches[NUM_BRANCHES];
Sprite spriteBackground;
Sprite spriteTree;
Sprite spriteBee;
Sprite spriteCloud1;
Sprite spriteCloud2;
Sprite spriteCloud3;
Sprite spritePlayer;
Sprite spriteRIP;
Sprite spriteAxe;
Sprite spriteLog;

RectangleShape timeBar;

Time gameTimeTotal;

Font font;

Text messageText;
Text scoreText;

Sound chop;
Sound death;
Sound outOfTime;

side playerSide;

float timeBarStartWidth = 400;
float timeBarHeight = 80;
float timeRemaining = 6.0f;
float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
float beeSpeed = 0.0f;
float cloud1Speed = 0.0f;
float cloud2Speed = 0.0f;
float cloud3Speed = 0.0f;
float logSpeedX = 1000;
float logSpeedY = -1500;

int score = 0;

bool paused = true;
bool beeActive = false; 
bool cloud1Active = false;  
bool cloud2Active = false;
bool cloud3Active = false;
bool logActive = false;
bool acceptInput = false;

// Prototypes
void updateBranches(int seed);
void handleInput(RenderWindow& window);
void updateScene(Clock &clock);
void drawScene(RenderWindow& window);

int main()
{
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
    Clock clock;
  
    // Background
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    // Tree
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Moving Bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    // Moving 3 clouds
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    // Time bar
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    // Message text
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    messageText.setFont(font);
    messageText.setString("Press Enter to start!");
    messageText.setCharacterSize(75);
    messageText.setFillColor(Color::White);

    // Score text
    scoreText.setFont(font);
    scoreText.setString("Score = 0");
    scoreText.setCharacterSize(100);
    scoreText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) 
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;// The player starts on the left

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // The player chopping sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    chop.setBuffer(chopBuffer);

    // The player has met his end under a branch
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    death.setBuffer(deathBuffer);

    // Out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    outOfTime.setBuffer(ootBuffer);

    while(window.isOpen())
    {
        handleInput(window);

        updateScene(clock);

        window.clear();// clear everything from the last frame

        drawScene(window);
    }
    return 0;

}
void updateBranches(int seed)
{
    // Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--) 
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0

    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}
void handleInput(RenderWindow &window)
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::KeyReleased && !paused)
        {
            acceptInput = true;

            // hide the axe
            spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        window.close();
    }
    if (Keyboard::isKeyPressed(Keyboard::Return))
    {
        paused = false;
        score = 0;
        timeRemaining = 6;

        // Make all the branches disappear - starting in the second position
        for (int i = 1; i < NUM_BRANCHES; i++)
        {
            branchPositions[i] = side::NONE;
        }

        spriteRIP.setPosition(675, 2000); // Make sure the gravestone is hidden
        spritePlayer.setPosition(580, 720); // Move the player into position
        acceptInput = true;
    }
    // Wrap the player controls to
    if (acceptInput)
    {
        // First handle pressing the right cursor key

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            playerSide = side::RIGHT;
            score++;

            timeRemaining += (2 / score) + .15;
            spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
            spritePlayer.setPosition(1200, 720);

            updateBranches(score);

            spriteLog.setPosition(810, 720);

            logSpeedX = -5000;
            logActive = true;
            acceptInput = false;

            chop.play();// Play a chop sound
        }

        // Handle the left cursor key
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {

            playerSide = side::LEFT;
            score++;

            timeRemaining += (2 / score) + .15;
            spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
            spritePlayer.setPosition(580, 720);

            updateBranches(score);

            spriteLog.setPosition(810, 720);
            logSpeedX = 5000;
            logActive = true;
            acceptInput = false;

            chop.play();// Play a chop sound
        }
    }
}
void updateScene(Clock &clock)
{
    if (!paused)
    {
        Time dt = clock.restart(); // restart the clock every frame
        timeRemaining -= dt.asSeconds();// Subtract from the amount of time remaining
        timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
        if (timeRemaining <= 0.0f) {
            paused = true; // GAME OVER
            messageText.setString("Out of time!!");

            FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

            outOfTime.play();// Play the out of time sound
        }

        if (!beeActive)
        {
            // speed
            srand((int)time(0));
            beeSpeed = (rand() % 200) + 200;

            // hight
            srand((int)time(0) * 10);
            float height = (rand() % 500) + 500;
            spriteBee.setPosition(2000, height);

            beeActive = true;
        }
        else
        {
            // move the bee
            spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

            // Has the bee reached the left-hand edge of the screen?
            if (spriteBee.getPosition().x < -100)
            {
                beeActive = false;
            }
        }
        // Cloud 1
        if (!cloud1Active)
        {
            srand((int)time(0) * 10);
            cloud1Speed = (rand() % 200);

            srand((int)time(0) * 10);
            float height = (rand() % 150);
            spriteCloud1.setPosition(-200, height);

            cloud1Active = true;
        }
        else
        {
            spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

            // Has the cloud reached the right hand edge of the screen?
            if (spriteCloud1.getPosition().x > 1920)
            {
                cloud1Active = false;
            }
        }
        // Cloud 2
        if (!cloud2Active)
        {

            srand((int)time(0) * 20);
            cloud2Speed = (rand() % 200);

            srand((int)time(0) * 20);
            float height = (rand() % 300) - 150;
            spriteCloud2.setPosition(-200, height);

            cloud2Active = true;
        }
        else
        {
            spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

            // Has the cloud reached the right hand edge of the screen?
            if (spriteCloud2.getPosition().x > 1920)
            {
                cloud2Active = false;

            }
        }
        // Cloud 3
        if (!cloud3Active)
        {

            srand((int)time(0) * 30);
            cloud3Speed = (rand() % 200);

            srand((int)time(0) * 30);
            float height = (rand() % 450) - 150;
            spriteCloud3.setPosition(-200, height);

            cloud3Active = true;
        }
        else
        {
            spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

            // Has the cloud reached the right hand edge of the screen?
            if (spriteCloud3.getPosition().x > 1920)
            {
                cloud3Active = false;
            }
        }

        // Update the score text
        std::stringstream ss;
        ss << "Score = " << score;
        scoreText.setString(ss.str());

        // Update the branch sprites
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            float height = i * 150;
            if (branchPositions[i] == side::LEFT)
            {
                branches[i].setPosition(610, height);// Move the sprite to the left side
                branches[i].setRotation(180);// Flip the sprite round the other way
            }
            else if (branchPositions[i] == side::RIGHT)
            {
                branches[i].setPosition(1330, height);// Move the sprite to the right side
                branches[i].setRotation(0);// Set the sprite rotation to normal
            }
            else
            {
                branches[i].setPosition(3000, height);// Hide the branch
            }
        }

        // Handle a flying log
        if (logActive)
        {
            spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

            // Has the log reached the right-hand edge?
            if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
            {
                // Set it up ready to be a whole new cloud next frame
                logActive = false;
                spriteLog.setPosition(800, 600);

            }
        }

        // has the player been squished by a branch?
        if (branchPositions[5] == playerSide)
        {
            // death
            paused = true;
            acceptInput = false;

            // Draw the gravestone
            spriteRIP.setPosition(525, 760);

            // hide the player
            spritePlayer.setPosition(2000, 660);

            // Change the text of the message
            messageText.setString("SQUISHED!!");

            // Center it on the screen
            FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

            death.play();// Play the death sound
        }
    }
}
void drawScene(RenderWindow& window)
{
    window.draw(spriteBackground);
    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);

    // Draw the branches
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        window.draw(branches[i]);
    }
    window.draw(spriteTree);
    window.draw(spritePlayer);
    window.draw(spriteAxe);
    window.draw(spriteLog);
    window.draw(spriteRIP);
    window.draw(spriteBee);
    window.draw(scoreText);
    window.draw(timeBar);
    if (paused)
    {
        window.draw(messageText);
    }

    // show everything we just drew
    window.display();
}