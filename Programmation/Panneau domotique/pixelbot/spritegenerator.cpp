#include "spritegenerator.h"

SpriteGenerator::SpriteGenerator(QObject *parent) : QObject(parent)
{

}

void SpriteGenerator::updateSettings(Settings settings){
    m_settings = settings;
}

QImage SpriteGenerator::drawCharacter(const QChar character){
    //Calcul de la taille de la police de caractères
    int max_width = m_settings.characterWidth;
    int max_height = m_settings.characterHeight;
    int current_size = 0;
    QRect current_rect, last_rect;

    do{
        last_rect = current_rect;
        current_size++;
        m_settings.font.setPixelSize(current_size);
        QFontMetrics fontMetrics(m_settings.font);
        current_rect = fontMetrics.tightBoundingRect(QString(character));
    }while((current_rect.width() <= max_width) && (current_rect.height() <= max_height));
    current_rect = last_rect;
    current_size--;
    m_settings.font.setPixelSize(current_size);

    //Ecriture du caractère dans un pixmap
    QPixmap pixmap(m_settings.characterWidth, m_settings.characterHeight);
    QPainter *painter = new QPainter(&pixmap);

    painter->setFont(m_settings.font);
    painter->fillRect(pixmap.rect(), QColor(255,255,255));

    int x = -current_rect.x() + (max_width-current_rect.width())/2;
    int y = -current_rect.y() + (max_height-current_rect.height())/2;
    painter->drawText(QPoint(x, y), QString(character));
    delete(painter);
    return pixmap.toImage();
}
