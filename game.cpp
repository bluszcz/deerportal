#include "game.h"

namespace efc {

int initScreenX = 1360;
int initScreenY = 768;
int currentSeason = 0;
int month = 0;

void Game::initBoard()
{

    sfxClick.setBuffer(sfxClickBuffer);
    sfxDone.setBuffer(sfxDoneBuffer);

    spriteBackground.setTexture(textureBackground);
    spriteBackgroundDark.setTexture(textures.backgroundDark);
    spriteBackgroundDark.setPosition(0,0);

    gameBackground.setTexture(textures.textureGameBackground);
    spriteLestBegin.setTexture(textures.textureLetsBegin);
    viewTiles.setViewport(sf::FloatRect(0.15f,0.1f, 1.0f, 1.0f));
    viewGui.setViewport(sf::FloatRect(0.806f,0.066f, 1, 1));


    groupHud.setFont(&gameFont);
    groupHud.setSeason(currentSeason);
    groupHud.setRoundName(roundNumber);

    sf::IntRect seasonsRect[4] = {sf::IntRect(0,0,255,255), sf::IntRect(256,0,512,255), sf::IntRect(0,255, 255, 512), sf::IntRect(255,255,512, 512)};

    sf::Sprite season1;
    season1.setTexture(textures.textureSeasons);
    season1.setTextureRect(seasonsRect[0]);
    season1.setPosition(0,400);
    season1.scale(sf::Vector2f(0.25f, 0.25f));
    season1.move(37.5, 30.5);
    season1.setColor(sf::Color(0,255,0,80));
    seasons[0] = season1;

    sf::Sprite season2;
    season2.setTexture(textures.textureSeasons);
    season2.setTextureRect(seasonsRect[1]);
    season2.setPosition(0,400);
    season2.scale(sf::Vector2f(0.25f, 0.25f));
    season2.move(37.5, 30.5);
    season2.setColor(sf::Color(200,200,50,80));
    seasons[1] = season2;

    sf::Sprite season3;
    season3.setTexture(textures.textureSeasons);
    season3.setTextureRect(seasonsRect[2]);
    season3.setPosition(0,400);
    season3.scale(sf::Vector2f(0.25f, 0.25f));
    season3.move(37.5, 30.5);
    season3.setColor(sf::Color(90,90,255,80));
    seasons[2] = season3;

    sf::Sprite season4;
    season4.setTexture(textures.textureSeasons);
    season4.setTextureRect(seasonsRect[3]);
    season4.setPosition(0,400);
    season4.scale(sf::Vector2f(0.25f, 0.25f));
    season4.move(37.5, 30.5);
    season4.setColor(sf::Color(255,0,0,80));
    seasons[3] = season4;


    PlayerHud playerHud1(&textures, std::rand() % 80, &gameFont, 32,0);
    PlayerHud playerHud2(&textures, std::rand() % 30, &gameFont, 32,1);
    PlayerHud playerHud3(&textures, std::rand() % 60, &gameFont, 32,2);
    PlayerHud playerHud4(&textures, std::rand() % 50, &gameFont, 32,3);
    players[0] = playerHud1;
    players[1] = playerHud2;
    players[3] = playerHud3;
    players[2] = playerHud4;

    players[0].setActive(true);
    setCurrentNeighbours();

    diceResultPlayer =  6;
    players[turn].characters[0].diceResult = diceResultPlayer;
    roundDice.setColor(turn);
}


void Game::setCurrentNeighbours ()
{
    currentNeighbours = players[turn].getNeighbours();
}

void Game::loadAssets()
{
    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
        std::exit(1);

    if (!textureTiles.loadFromFile("assets/img/zw-tilesets/_MAP.png"))
        std::exit(1);

    if (!gameFont.loadFromFile("assets/fnt/metal-mania.regular.ttf"))
    {
        std::exit(1);
    }
    if (!menuFont.loadFromFile("assets/fnt/metal-macabre.regular.ttf"))
    {
        std::exit(1);
    }
    if (!textureBackgroundArt.loadFromFile("assets/img/background_land.png"))
        std::exit(1);


    spriteBackgroundArt.setTexture(textureBackgroundArt);



    if (!musicGame.openFromFile("assets/audio/game.ogg"))
        std::exit(1);
    if (!musicBackground.openFromFile("assets/audio/wind2.ogg"))
        std::exit(1);
    if (!musicMenu.openFromFile("assets/audio/menu.ogg"))
        std::exit(1);


    if (!sfxClickBuffer.loadFromFile("assets/audio/click.ogg"))
        std::exit(1);

    if (!sfxDoneBuffer.loadFromFile("assets/audio/done.ogg"))
        std::exit(1);
    if (!textureBackground.loadFromFile("assets/img/background.png"))
        std::exit(1);

    menuTxt.setFont(menuFont);
    menuTxt.setCharacterSize(60);
    menuTxt.setString(gameTitle);
    int width = menuTxt.getLocalBounds().width;
    int height = menuTxt.getLocalBounds().height;
    menuTxt.setPosition(400-(width/2),300-(height/2)-150);
    menuTxt.setColor(sf::Color(255, 255, 255, 85));


}

void Game::showMenu()
{
    musicBackground.play();
    musicBackground.setLoop(true);
    menuBackground.setTexture(textures.textureMenu);
    musicBackground.setVolume(7);
    musicMenu.play();
    musicMenu.setLoop(true);
    currentState = state_menu;
}
void Game::hideMenu()
{
    musicMenu.stop();
}

void Game::showGameBoard()
{
//    musicGame.setVolume(20);
    musicGame.play();
    musicGame.setLoop(true);
    sfx.playLetsBegin();
    std::cout << "lets begin" << std::endl;
    currentState = state_lets_begin;
}

void Game::endGame()
{
    currentState = state_end_game;
    musicBackground.stop();
}

void Game::handleLeftClick(sf::Vector2f pos,
                           sf::Vector2f posGui, sf::Vector2f posFull, int mousePos) {
    if (currentState==state_game)
    {
        std::array<int,2> movements = players[turn].getMovements(diceResultPlayer);
        if ((mousePos==movements[0]) || (mousePos==movements[1]))
        {
            players[turn].setFigurePos(mousePos);

            if (boardDiamonds.ifFieldIsEmpty(mousePos)==false)
            {
                sfx.playCollect();
                if (boardDiamonds.getNumberForField(mousePos)==4)
                {
                    players[turn].cash += 1;

                };
                std::cout << "cash " << players[turn].cash << " " << " for " << turn << " " << boardDiamonds.getNumberForField(mousePos) << " result" << std::endl;
                // After this no more visible
                boardDiamonds.collectField(mousePos);
            }
            int *possibleExit = std::find(std::begin(efc::possibleExits),
                                          std::end(efc::possibleExits), mousePos);
            if (possibleExit != efc::possibleExits+4) {
//                std::cerr << "Found hahahah" << mousePos << std::endl;
                players[turn].done=true;
                numberFinishedPlayers += 1;
                if (numberFinishedPlayers > 3)
                   endGame();
            } else {
//               std::cerr << "Not found" << std::endl;
            }
            nextPlayer();
        }
        std::string resultCommand = players[turn].getElem(posGui);
        command(resultCommand);
}
else if (currentState==state_roll_dice)
    {
        sf::IntRect diceRect(roundDice.spriteDice.getGlobalBounds());
        if (diceRect.intersects(sf::IntRect(posFull.x, posFull.y, 1, 1)))
        {
            diceResultPlayer = roundDice.throwDiceSix();
            players[turn].characters[0].diceResult=diceResultPlayer;
            currentState=state_game;
        }
    }

    else if (currentState==state_gui_elem)
    {
        std::string resultCommand = guiSelectBuilding.getElem(posFull);
        if (resultCommand.find("elem_")==0)
        {
            std::string resultCommandWrapped = "build_" + resultCommand;
            command(resultCommandWrapped);
        } else if (resultCommand.find("close_gui")==0)
        {        command(resultCommand);}
    }
    if (currentState==state_menu)
    {
        downTimeCounter = 0;
        std::cout << " AA " <<downTimeCounter << std::endl;
        hideMenu();
        showGameBoard();
    }



    if (currentState==state_gui_end_round)
    {
        std::string resultCommand = guiRoundDice.getElem(pos);
        command(resultCommand);
    }
}

void Game::hideGameBoard()
{
    musicGame.play();
}
Game::Game():
    screenSize(efc::initScreenX,efc::initScreenY),
    window(sf::VideoMode(efc::initScreenX, efc::initScreenY), "Pagan Board"),
    viewFull(sf::FloatRect(00, 00, screenSize.x, screenSize.y)),
    viewGui(sf::FloatRect(00, 00, screenSize.x, screenSize.y)),
    viewTiles(sf::FloatRect(0, 0, 1360, 768)),
    selector(efc::TILE_SIZE),
    guiSelectBuilding(&textures),
    character(&textures, 3),
    turn(0),
    gameTitle("pagan   board"),
    roundDice(players),
    roundNumber(1),
    guiRoundDice(&textures),
    boardDiamonds(&textures)
{


    numberFinishedPlayers = 0;

    sf::Clock frameClock;


    guiRoundDice.active = true;
    showPlayerBoardElems = false;
    window.setVerticalSyncEnabled(true);

    std::srand (time(NULL));
    loadAssets();
    initBoard();
    showMenu();
    // run the main loop
    while (window.isOpen())
    {
        sf::Time frameTime = frameClock.restart();
        std::string resultCommand = "";
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
            sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp,viewTiles);
            sf::Vector2f localPositionGui = window.mapPixelToCoords(localPositionTmp,viewGui);
            sf::Vector2f localPositionFull = window.mapPixelToCoords(localPositionTmp,viewFull);
            int mousePosX = (int)localPosition.x / efc::TILE_SIZE;
            int mousePosY = (int)localPosition.y / efc::TILE_SIZE;
            int mousePos = efc::transCords(sf::Vector2i(mousePosX, mousePosY));
            if(event.type == sf::Event::Closed)
                window.close();
            if (currentState==state_gui_elem)
            {
                resultCommand = guiSelectBuilding.getElem(localPositionFull);
                showPlayerBoardElems = false;
                if (resultCommand.find("elem_")==0)
                    command(resultCommand);
                else
                    command("hide_gui_elem_description");
            }

            if (currentState==state_game)
            {
                if ((localPosition.x>efc::TILE_SIZE*efc::BOARD_SIZE) || (localPosition.x<0)  || (localPosition.y>efc::TILE_SIZE*efc::BOARD_SIZE) || (localPosition.y<0))
                {
                    showPlayerBoardElems = false;
                    players[turn].elems.displayNeighbours = false;
                } else {
                    showPlayerBoardElems = true;
                    players[turn].elems.displayNeighbours = true;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    handleLeftClick(localPosition, localPositionGui,
                                    localPositionFull, mousePos);
            }
            if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=efc::BOARD_SIZE*efc::TILE_SIZE) && (localPosition.y<=efc::BOARD_SIZE*efc::TILE_SIZE))
            {
                selector.setPosition((int) (localPosition.x / efc::TILE_SIZE)*efc::TILE_SIZE, ((int) localPosition.y / efc::TILE_SIZE)*efc::TILE_SIZE);
            }
        }

        update(frameTime);
        render();
    }
}

void Game::update(sf::Time frameTime) {
    if (currentState==state_game)
    {
        std::array<int,2> currentMovements = players[turn].getMovements(diceResultPlayer);
        if (currentMovements[0]>-1)
        {
            prevRotateElem.spriteRotate.setPosition(players[turn].characters[0].leftChar.getPosition());
            prevRotateElem.spriteRotate.move(4,16);
            prevRotateElem.update(frameTime);
            prevRotateElem.setColor(turn);
        }
        if (currentMovements[1]>-1)
        {
            nextRotateElem.spriteRotate.setPosition(players[turn].characters[0].rightChar.getPosition());
            nextRotateElem.spriteRotate.move(4,16);
            nextRotateElem.update(frameTime);
            nextRotateElem.setColor(turn);
        }



    }
    for (int i=0;i<4;i++)
    {
        players[i].play();
    }


    for (int i=0;i<4;i++)
    {
        players[i].update(frameTime, busyTiles);
    }

    if (currentState==state_lets_begin)
    {
        downTimeCounter += frameTime.asSeconds();
    std::cout << downTimeCounter << std::endl;

    spriteLestBegin.setColor(sf::Color(255,255,255,255-(downTimeCounter*35)));
    if (downTimeCounter>6)
    {
        currentState = state_roll_dice;
    }

    }

}

void Game::nextRound() {
    turn = 0;
    std::string result = roundDice.drawRound();
    roundNumber += 1;
    month++;
    if (month==13)
        month=1;
    if (month%4==0)
        currentSeason++;
    if (currentSeason>3)
        currentSeason=0;
    if (players[turn].done==true)
        nextPlayer();
}

void Game::nextPlayer(){
    if (numberFinishedPlayers==4)
    {
        std::cout << "Everybody Finished!!!" << std::endl;
        endGame();
    }
    if (turn<4)
        players[turn].updatePlayer();
    else
        nextRound();
    turn++;

    if ((players[turn].done==true) && (turn<4))
    {
        std::cout << "Player " << turn << " is done" << std::endl;
        nextPlayer();
    }

    if ((turn==4) || (turn>4))
    {
        nextRound();
    }
    selector.changeColor(turn);
    for (int i=0;i<4;i++)
    {
        if (i==turn)
        {
            players[i].setActive(true);
            currentNeighbours = players[i].getNeighbours();
        }
        else
            players[i].setActive(false);
    }
    sfxClick.play();
    diceResultPlayer = 6;
    roundDice.setDiceTexture(diceResultPlayer);
    players[turn].characters[0].diceResult = diceResultPlayer;
    groupHud.setRoundName(roundNumber);
    groupHud.setSeason(currentSeason);
    groupHud.setMonthName(month%4);
    currentState = state_roll_dice;
    roundDice.setColor(turn);
}

void Game::drawPlayersGui(){
    for (int i=0;i<4;i++)
    {
        window.draw(players[i]);
    }
}

void Game::drawSquares() {
    if (showPlayerBoardElems)
    {
        window.draw(selector);
    }

}

void Game::drawBaseGame()
{
    window.setView(viewTiles);
//    window.draw(map);
    for (int i=0;i<4;i++)
    {
        window.draw(players[i].elems);
    }
    drawSquares();
    window.setView(viewGui);
    window.setView(viewTiles);
}

void Game::drawCharacters(){
    window.setView(viewTiles); // Yeah Katia's inspiration
    window.draw(gameBackground);
    window.setView(viewFull);
    window.draw(spriteBackgroundArt);
    window.draw(roundDice.spriteDice);
    window.setView(viewTiles);
    drawSquares();

    if (currentState==state_game)
    {
        std::array<int,2> currentMovements = players[turn].characters[0].getMovements(diceResultPlayer);
        if (currentMovements[1]>-1)
            window.draw(nextRotateElem);
        if (currentMovements[0]>-1)
            window.draw(prevRotateElem);
    }
    for (int i=0;i<4;i++)
    {
        for (auto&& j: players[i].characters)
        {
            if (currentState==state_game)
                j.drawMovements = true;
            else
               j.drawMovements = false;
            window.draw(j);
        }
    }
}

void Game::render()
{
    window.clear();
    if (currentState==state_game)
    {
        window.setView(viewFull);
        window.draw(spriteBackgroundDark);
        window.setView(viewTiles);
        drawBaseGame();
        drawCharacters();
        window.draw(boardDiamonds);
        window.setView(viewFull);
        drawPlayersGui();
        window.setView(viewFull);
        window.draw(groupHud);

    } else if (currentState==state_roll_dice) {
        window.setView(viewFull);
        window.draw(spriteBackgroundDark);
        window.setView(viewTiles);
        drawBaseGame();
        drawCharacters();
        window.draw(boardDiamonds);
        window.setView(viewFull);
        drawPlayersGui();
        window.setView(viewFull);
       window.draw(groupHud);

    } else if (currentState==state_gui_elem) {
        window.setView(viewFull);
        window.draw(spriteBackgroundDark);
        drawBaseGame();
        drawCharacters();
        window.draw(guiSelectBuilding);
        window.setView(viewFull);
        window.draw(groupHud);

    }  else if (currentState==state_menu) {
        window.draw(menuTxt);
    }  else if (currentState==state_lets_begin) {
        window.setView(viewFull);
        window.draw(spriteBackgroundDark);
        window.setView(viewTiles);
        drawBaseGame();
        drawCharacters();
        window.draw(boardDiamonds);
        window.setView(viewFull);
        drawPlayersGui();
        window.draw(spriteLestBegin);

    } else if (currentState==state_gui_end_round){
        window.setView(viewFull);
        window.draw(spriteBackgroundDark);
        drawBaseGame();
        window.draw(guiRoundDice);
        window.setView(viewFull);
        window.draw(groupHud);
    }
    window.display();
}

void Game::command(std::string command){
    if (command=="close_gui")
    {
        guiSelectBuilding.active = false;
        currentState=state_game;
        sfxClick.play();
    }
    if (command=="hide_gui_elem_description")
    {
        if (currentState==state_gui_elem) {
            guiSelectBuilding.descriptionActive = false;
        }
    }

    if (command.find("end_of_round")==0)
    {
        std::string subResult = command.substr(13);
        guiRoundDice.active = true;
        guiRoundDice.setTitle(subResult);
        currentState = state_gui_end_round;
    }

    if (command=="end_turn")
        nextPlayer();

}

sf::Vector2f Game::getMousePos(){
    sf::Vector2i mousePosTmp(sf::Mouse::getPosition(window));
    sf::Vector2f mousePosition(window.mapPixelToCoords(mousePosTmp,viewTiles));
    return mousePosition;
}



void Game::setBusyTiles() {

    busyTiles.clear();
    for (int i: efc::terrainArray)
    {
        busyTiles.insert(i);
    }

    for (int i=0;i<4;i++)
    {
        for (int j:players[i].getBusy())
        {
            busyTiles.insert(j);
        }
    }
}

}

