#include "cardsdeck.h"

CardsDeck::CardsDeck(TextureHolder *textures, sf::Font *gameFont)
{



    std::array<std::array<int,2>,4> cardsPos = {
        {
            {1087,95}, {1225, 95}, {1225, 277}, {1087, 277}
    }
    };

    this->textures = textures;
    for (int i=0;i<=3;i++)
    {
        spriteCardBases[i].setTexture(this->textures->textureCardBases[i]);
        spriteCardBases[i].setPosition(cardsPos[i][0],cardsPos[i][1]);

        textPileTitle[i].setFont(*gameFont);
        textPileTitle[i].setCharacterSize(10);
        textPileTitle[i].setPosition(cardsPos[i][0]+10,cardsPos[i][1]+100);

        for (int j=0;j<efc::PILE_SIZE;j++)
        {
            cardsList[i].cardsPile[j].cardType = efc::cardsTypes[j];
        }
        setTitles();
    }
}

void CardsDeck::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (int i=0;i<=3;i++)
    {
        target.draw(spriteCardBases[i], states);
        target.draw(textPileTitle[i], states);

    }
}

void CardsDeck::setTitles()
{
    for (int i=0;i<=3;i++)
    {
        int currentCard = cardsList[i].currentCard;
        std::string currentText = cardsList[i].cardsPile[currentCard].cardType;
        std::cout << i << " " << currentCard << " " <<  currentText << std::endl;
        textPileTitle[i].setString(currentText);



    }
}

void CardsDeck::setFonts(sf::Font *gameFont)
{
    for (int i=0;i<=3;i++)
    {

        textPileTitle[i].setFont(*gameFont);

    }
}

void CardsDeck::nextCard(int pileNumber)
{
    int currentCard = cardsList[pileNumber].currentCard;
    currentCard += 1;
    if (currentCard>efc::PILE_SIZE-1)
        currentCard = 0;

    setTitles();
}