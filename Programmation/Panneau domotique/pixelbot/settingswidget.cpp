#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
{
    //Paramètres matrice
        //Taille de la matrice
    m_sBMatrixSize = new QSpinBox(this);
    m_sBMatrixSize->setValue(32);
    m_sBMatrixSize->setMinimum(1);
    m_sBMatrixSize->setSuffix(" px");

        //Images par secondes
    m_sBFPS = new QSpinBox(this);
    m_sBFPS->setValue(60);
    m_sBFPS->setMinimum(1);
    m_sBFPS->setSuffix(" FPS");

        //Niveaux de luminosité
    m_sBLumLevels = new QSpinBox(this);
    m_sBLumLevels->setValue(8);
    m_sBLumLevels->setMinimum(2);

        //Fréquence de rafraîchissement
    m_lbRefreshFrequency = new QLabel(this);
    calculateRefreshFrequency();

        //Taille d'un caractère
    m_sBCharacterWidth = new QSpinBox(this);
    m_sBCharacterWidth->setValue(8);
    m_sBCharacterWidth->setMinimum(1);
    m_sBCharacterWidth->setSuffix(" w");
    m_sBCharacterHeight = new QSpinBox(this);
    m_sBCharacterHeight->setValue(16);
    m_sBCharacterHeight->setMinimum(1);
    m_sBCharacterHeight->setSuffix(" h");
    m_lCharacterSize = new QHBoxLayout();
    m_lCharacterSize->addWidget(m_sBCharacterWidth);
    m_lCharacterSize->addWidget(m_sBCharacterHeight);
    setCharacterSizeRange();

    //Police de caractères
    m_font = QFont();
    m_lbFont = new QLabel(m_font.family());
    m_pBFont = new QPushButton("Modifier", this);
    m_lFont = new QHBoxLayout();
    m_lFont->addWidget(m_lbFont);
    m_lFont->addWidget(m_pBFont);

    //Ensemble de caractères
    m_cBNumbers = new QCheckBox("Chiffres", this);
    m_cBNumbers->setChecked(true);
    m_cBLowercase = new QCheckBox("Lettres minuscules", this);
    m_cBLowercase->setChecked(false);
    m_cBUppercase = new QCheckBox("Lettres majuscules", this);
    m_cBUppercase->setChecked(false);
    m_lCharactersSet = new QVBoxLayout();
    m_lCharactersSet->addWidget(m_cBNumbers);
    m_lCharactersSet->addWidget(m_cBLowercase);
    m_lCharactersSet->addWidget(m_cBUppercase);

    //Taille des sprites
    m_lbSpritesSize = new QLabel();
    calculateSpritesSize();

    //Layout
    m_layout = new QFormLayout();
    m_layout->addRow(QString("Taille de la matrice :"), m_sBMatrixSize);
    m_layout->addRow(QString("Images par secondes :"), m_sBFPS);
    m_layout->addRow(QString("Niveaux de luminosité :"), m_sBLumLevels);
    m_layout->addRow(QString("Fréquence de rafraîchissement :"), m_lbRefreshFrequency);
    m_layout->addRow(QString("Taille d'un caractère :"), m_lCharacterSize);
    m_layout->addRow(QString("Police de caractères :"), m_lFont);
    m_layout->addRow(QString("Ensemble de caractères :"), m_lCharactersSet);
    m_layout->addRow(QString("Espace mémoire nécessaire :"), m_lbSpritesSize);

    this->setLayout(m_layout);

    //Signaux
    connect(m_sBMatrixSize, SIGNAL(valueChanged(int)), this, SLOT(setCharacterSizeRange()));

    connect(m_sBMatrixSize, SIGNAL(valueChanged(int)), this, SLOT(calculateRefreshFrequency()));
    connect(m_sBFPS, SIGNAL(valueChanged(int)), this, SLOT(calculateRefreshFrequency()));
    connect(m_sBLumLevels, SIGNAL(valueChanged(int)), this, SLOT(calculateRefreshFrequency()));

    connect(m_pBFont, SIGNAL(released()), this, SLOT(changeFont()));

    connect(m_sBCharacterHeight, SIGNAL(valueChanged(int)), this, SLOT(calculateSpritesSize()));
    connect(m_sBCharacterWidth, SIGNAL(valueChanged(int)), this, SLOT(calculateSpritesSize()));
    connect(m_sBLumLevels, SIGNAL(valueChanged(int)), this, SLOT(calculateSpritesSize()));
    connect(m_cBLowercase, SIGNAL(clicked(bool)), this, SLOT(calculateSpritesSize()));
    connect(m_cBUppercase, SIGNAL(clicked(bool)), this, SLOT(calculateSpritesSize()));
    connect(m_cBNumbers, SIGNAL(clicked(bool)), this, SLOT(calculateSpritesSize()));

    connect(m_sBCharacterHeight, SIGNAL(valueChanged(int)), this, SLOT(sendSettings()));
    connect(m_sBCharacterWidth, SIGNAL(valueChanged(int)), this, SLOT(sendSettings()));
    connect(m_sBLumLevels, SIGNAL(valueChanged(int)), this, SLOT(sendSettings()));
    connect(m_cBNumbers, SIGNAL(stateChanged(int)), this, SLOT(sendSettings()));
    connect(m_cBLowercase, SIGNAL(stateChanged(int)), this, SLOT(sendSettings()));
    connect(m_cBUppercase, SIGNAL(stateChanged(int)), this, SLOT(sendSettings()));
    sendSettings();
}

void SettingsWidget::sendSettings(){
    Settings settings;
    settings.characterHeight = m_sBCharacterHeight->value();
    settings.characterWidth = m_sBCharacterWidth->value();
    settings.lumLevels = m_sBLumLevels->value();
    settings.font = m_font;
    settings.numbers = m_cBNumbers->isChecked();
    settings.lowercase = m_cBLowercase->isChecked();
    settings.uppercase = m_cBUppercase->isChecked();
    emit(newSettings(settings));
}

void SettingsWidget::calculateSpritesSize(){
    int line = qCeil(((qreal) m_sBCharacterWidth->value())/32.)*32;//bits
    int frame_character = line * m_sBCharacterHeight->value();
    int character = frame_character * (m_sBLumLevels->value()-1);
    int nbCharacters = 0;
    if(m_cBNumbers->isChecked()){
        nbCharacters += 10;
    }
    if(m_cBLowercase->isChecked()){
        nbCharacters += 26;
    }
    if(m_cBUppercase->isChecked()){
        nbCharacters += 26;
    }
    m_spritesSize = (character*nbCharacters)/8;
    m_lbSpritesSize->setText(QString::number(m_spritesSize)+ " octets");
}

void SettingsWidget::calculateRefreshFrequency(){
    m_refreshFrequency = m_sBMatrixSize->value() * m_sBFPS->value() * (m_sBLumLevels->value() - 1);
    m_lbRefreshFrequency->setText(QString::number(m_refreshFrequency) + QString(" Hz"));
}

void SettingsWidget::changeFont(){
    bool ok;
    m_font = QFontDialog::getFont(&ok, m_font, this);
    if(ok){
        m_lbFont->setText(m_font.family());
        m_lbFont->setFont(m_font);
        sendSettings();
    }
}

void SettingsWidget::setCharacterSizeRange(){
    m_sBCharacterHeight->setRange(1, m_sBMatrixSize->value());
    m_sBCharacterWidth->setRange(1, m_sBMatrixSize->value());
}
