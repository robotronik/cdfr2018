#ifndef SPRITEGENERATOR_H
#define SPRITEGENERATOR_H

#include <QObject>
#include <QImage>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QPixmap>
#include <QTransform>
#include <QChar>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "settingswidget.h"

class SpriteGenerator : public QObject
{
    Q_OBJECT

private:
    //Paramètres
    Settings *m_settings;
    QString m_set;

public:
    explicit SpriteGenerator(Settings *settings, QObject *parent = nullptr);
    QImage drawCharacter(const QChar character);
    QString getCharacterSet();
    void exportSprites(QString fileName);
    void writeSprites(QString *str);
    void writeInfos(QString *str);
    QString getSprite(const QChar character);
    QString getFrame(unsigned char **array, int width, int height);
    QString getLine(unsigned char *line, int width);

signals:

public slots:
    void updateSettings();
};

#endif // SPRITEGENERATOR_H
