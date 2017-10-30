#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QFontDialog>
#include <QtCore/qmath.h>

typedef struct Settings_S{
    int lumLevels;
    int characterWidth;
    int characterHeight;
    QFont font;
    bool numbers, lowercase, uppercase;
}Settings;

class SettingsWidget : public QWidget
{
    Q_OBJECT

private:
    //Paramètres matrice
        //Taille de la matrice
    QSpinBox *m_sBMatrixSize;

        //Images par secondes
    QSpinBox *m_sBFPS;

        //Niveaux de luminosité
    QSpinBox *m_sBLumLevels;

        //Fréquence de rafraîchissement
    QLabel *m_lbRefreshFrequency;

        //Taille d'un caractère
    QSpinBox *m_sBCharacterWidth;
    QSpinBox *m_sBCharacterHeight;
    QHBoxLayout *m_lCharacterSize;

    //Police de caractères
    QFont m_font;
    QLabel *m_lbFont;
    QPushButton *m_pBFont;
    QHBoxLayout *m_lFont;

    //Ensemble de caractères
    QCheckBox *m_cBNumbers;
    QCheckBox *m_cBLowercase;
    QCheckBox *m_cBUppercase;
    QVBoxLayout *m_lCharactersSet;

    //Taille des sprites
    QLabel *m_lbSpritesSize;

    QFormLayout *m_layout;

    int m_refreshFrequency;
    int m_spritesSize;//octets

public:
    explicit SettingsWidget(QWidget *parent = nullptr);

signals:
    void newSettings(Settings);

public slots:
    void calculateRefreshFrequency();
    void calculateSpritesSize();
    void changeFont();
    void setCharacterSizeRange();
    void sendSettings();
};

#endif // SETTINGSWIDGET_H
