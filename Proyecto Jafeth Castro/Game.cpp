#include "Game.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;
using namespace sf;


void Game::startWindow3(bool isWin, int finalScore) {
    RenderWindow window(VideoMode(400, 300), "Resultado");
    Font font;
    font.loadFromFile("font/PressStart2P-Regular.ttf");

    Text resultText;
    resultText.setFont(font);
    resultText.setCharacterSize(32);
    resultText.setFillColor(Color::White);
    resultText.setPosition(50, 40);
    resultText.setString(isWin ? "¡Ganaste!" : "Perdiste");

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::Yellow);
    scoreText.setPosition(50, 100);
    scoreText.setString("Puntaje: " + to_string(finalScore));

    // Botón Volver a jugar
    RectangleShape playAgainButton(Vector2f(250, 40));
    playAgainButton.setPosition(75, 170);
    playAgainButton.setFillColor(Color(51, 255, 255));

    Text playAgainText;
    playAgainText.setFont(font);
    playAgainText.setCharacterSize(17);
    playAgainText.setFillColor(Color::Black);
    playAgainText.setString("Volver a jugar");
    playAgainText.setPosition(80, 175);

    // Botón Salir
    RectangleShape exitButton(Vector2f(250, 40));
    exitButton.setPosition(75, 220);
    exitButton.setFillColor(Color(255, 100, 100));

    Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(17);
    exitText.setFillColor(Color::Black);
    exitText.setString("Salir");
    exitText.setPosition(150, 225);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mouseF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (playAgainButton.getGlobalBounds().contains(mouseF)) {
                    window.close();
                    startWindow2(); 
                }
                if (exitButton.getGlobalBounds().contains(mouseF)) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(resultText);
        window.draw(scoreText);
        window.draw(playAgainButton);
        window.draw(playAgainText);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }
}



void Game::startWindow2()
{
    Board board;
    RenderWindow window(VideoMode(800, 600), "GEMAS ENCANTADAS");
    Texture texture;
    if (!texture.loadFromFile("img/background.png"))
        cout << "ERROR AL CARGAR LA IMAGEN";
    Sprite sprite(texture);


    Font font;
    if (!font.loadFromFile("font/VCR_OSD_MONO_1.001.ttf")) {
        cout << "ERROR AL CARGAR LA FUENTE" << endl;
    }

    Texture letterTexture;
    if (!letterTexture.loadFromFile("img/letter.png")) {
        cout << "ERROR AL CARGAR LA IMAGEN" << endl;
    }
    Sprite letter(letterTexture);

   letter.setScale(0.7f,0.7f);
    letter.setPosition(450, -30);
         

    Text movesText, scoreText, goalText;
    movesText.setFont(font);
    scoreText.setFont(font);
    goalText.setFont(font);

    movesText.setCharacterSize(17);
    scoreText.setCharacterSize(17);
    goalText.setCharacterSize(17);

    movesText.setFillColor(Color(Color::White));
    scoreText.setFillColor(Color(Color::White));
    goalText.setFillColor(Color(Color::White));
   
    scoreText.setPosition(640, 77);
    movesText.setPosition(670, 117);
    goalText.setPosition(623, 155);
    Clock clock;
    while (window.isOpen())

    {

        float FrameTime = clock.restart().asSeconds();
            board.moveGems(FrameTime);
             static bool inMatchLoop = false;
             static bool waitingReversal = false;

             if (board.getPendingMatchProcess() && board.allGemsAtTarget()) {
                 if (board.processMatchesAndScore()) {
                     inMatchLoop = true;
                 }
                 else {
                     board.setPendingMatchProcess(false);
                     if (inMatchLoop) {
                         board.decrementMovesLeft();
                         inMatchLoop = false;
                         board.clearSelection();
                     }
                     else {
            
                         board.revertLastSwap();
                         waitingReversal = true;

                     }
                 }
             }
            if (waitingReversal && board.allGemsAtTarget()) {
                board.clearSelection();
                waitingReversal = false;
            }

        movesText.setString(to_string(board.getMovesLeft()));
        scoreText.setString(to_string(board.getActualScore()));
        goalText.setString(to_string(board.getGoalScore()));

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (board.allGemsAtTarget() && !board.getPendingMatchProcess()) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    Vector2f worldPos = window.mapPixelToCoords(mousePos); // convierte las cordenadas de vector2 de enteros a flotantes
                    board.detectCicksOverGems(worldPos);
                    if (board.areTwoSelected()) {
                        board.swapSelectedGems();
                    }
                }
            }

            if (board.getActualScore() >= board.getGoalScore()) {
                window.close();
                startWindow3(true,board.getActualScore()); 
            }
            else if (board.getMovesLeft() <= 0) {
                window.close();
                startWindow3(false,board.getActualScore()); 
            }


        }
        
        window.clear(); 
        window.draw(sprite);
		board.drawGems(window);
        window.draw(letter);
        window.draw(scoreText);
        window.draw(movesText);
        window.draw(goalText);
        window.display();
    }
}

void Game::startWindow1()
{
    RenderWindow window(VideoMode(800, 600), "Gemas Encantadas");
    Texture texture;
    if (!texture.loadFromFile("img/background.png"))
        cout << "ERROR AL CARGAR LA IMAGEN";
    Sprite sprite(texture);

    Texture buttonTexture;
    if (!buttonTexture.loadFromFile("img/play_button.png")) {
        cout << "ERROR AL CARGAR LA IMAGEN";
    }
    Sprite buttonSprite(buttonTexture);
    buttonSprite.setPosition(250.f, 350.f);

    Texture titleTexture;
    if (!titleTexture.loadFromFile("img/titulo.png")) {
        cout << "ERROR AL CARGAR LA IMAGEN";
    }
    Sprite titleSprite(titleTexture);
    titleSprite.setPosition(150.f,-50.f);
    
   

    

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == Event::Closed)
                window.close();

            
            if (event.type == Event::MouseButtonPressed &&  event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                if (buttonSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    window.close(); 
                    startWindow2();      
                }
            }
        }

       
        window.clear();
        window.draw(sprite);
        window.draw(buttonSprite);
        window.draw(titleSprite);
        window.display();
    }
}