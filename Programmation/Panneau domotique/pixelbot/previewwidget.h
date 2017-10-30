#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>
#include "settingswidget.h"
#include "spritegenerator.h"

class PreviewWidget : public QWidget
{
    Q_OBJECT

private:
    //Sprite
    QLabel *m_sprite;

    //Sélection
    QComboBox *m_cBSet;
    QPushButton *m_pBNext;
    QPushButton *m_pBPrevious;
    QHBoxLayout *m_lSelector;

    //Layout principal
    QVBoxLayout *m_layout;

    //Paramètres et générateur de sprites
    SpriteGenerator *m_spriteGenerator;
    Settings m_settings;

public:
    explicit PreviewWidget(SpriteGenerator *spriteGenerator, QWidget *parent = nullptr);
    void setCharactersSet();

signals:

public slots:
    void updateSettings(Settings);
    void next();
    void previous();
    void setButtonsStates();
    void updateLabel(QString);
};

#endif // PREVIEWWIDGET_H
