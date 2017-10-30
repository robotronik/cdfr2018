#ifndef SPRITEGENERATOR_H
#define SPRITEGENERATOR_H

#include <QObject>
#include <QImage>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QPixmap>
#include <QChar>
#include "settingswidget.h"

class SpriteGenerator : public QObject
{
    Q_OBJECT

private:
    //Paramètres
    Settings m_settings;


public:
    explicit SpriteGenerator(QObject *parent = nullptr);
    QImage drawCharacter(const QChar character);

signals:

public slots:
    void updateSettings(Settings);
};

#endif // SPRITEGENERATOR_H
