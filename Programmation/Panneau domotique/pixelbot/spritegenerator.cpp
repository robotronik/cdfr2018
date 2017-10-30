#include "spritegenerator.h"

SpriteGenerator::SpriteGenerator(QObject *parent) : QObject(parent)
{

}

void SpriteGenerator::updateSettings(Settings settings){
    m_settings = settings;
}

QImage SpriteGenerator::drawCharacter(const QChar character){
    QPixmap pixmap(m_settings.characterWidth, m_settings.characterHeight);
    int max = 1;
    while(m_settings.font.);
}
