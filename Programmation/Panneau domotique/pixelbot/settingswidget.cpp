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

    //Format
    m_cBReadingDirection = new QComboBox(this);
    m_cBReadingDirection->addItem("Par ligne", QVariant(false));
    m_cBReadingDirection->addItem("Par colonne", QVariant(true));
    m_cBRotation = new QComboBox(this);
    m_cBRotation->addItem("Pas de rotation", QVariant(0));
    m_cBRotation->addItem("PI/2", QVariant(1));
    m_cBRotation->addItem("PI", QVariant(2));
    m_cBRotation->addItem("3PI/2", QVariant(3));

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

    //Layout
    m_layout = new QFormLayout();
    m_layout->addRow(QString("Taille de la matrice :"), m_sBMatrixSize);
    m_layout->addRow(QString("Images par secondes :"), m_sBFPS);
    m_layout->addRow(QString("Niveaux de luminosité :"), m_sBLumLevels);
    m_layout->addRow(QString("Fréquence de rafraîchissement :"), m_lbRefreshFrequency);
    m_layout->addRow(QString("Sens de lecture :"), m_cBReadingDirection);
    m_layout->addRow(QString("Rotation :"), m_cBRotation);
    m_layout->addRow(QString("Taille d'un caractère :"), m_lCharacterSize);
    m_layout->addRow(QString("Police de caractères :"), m_lFont);
    m_layout->addRow(QString("Ensemble de caractères :"), m_lCharactersSet);
    m_layout->addRow(QString("Espace mémoire nécessaire :"), m_lbSpritesSize);

    this->setLayout(m_layout);

    //Signaux
    connect(m_sBMatrixSize, SIGNAL(valueChanged(int)), this, SLOT(setCharacterSizeRange()));
    connect(m_pBFont, SIGNAL(released()), this, SLOT(changeFont()));

    connect(m_sBMatrixSize, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(m_sBFPS, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(m_sBLumLevels, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(m_sBCharacterHeight, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(m_sBCharacterWidth, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(m_sBLumLevels, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(m_cBNumbers, SIGNAL(stateChanged(int)), this, SLOT(updateSettings()));
    connect(m_cBLowercase, SIGNAL(stateChanged(int)), this, SLOT(updateSettings()));
    connect(m_cBUppercase, SIGNAL(stateChanged(int)), this, SLOT(updateSettings()));
    connect(m_cBReadingDirection, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSettings()));
    connect(m_cBRotation, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSettings()));

    m_settings = new Settings;
    updateSettings();
}

SettingsWidget::~SettingsWidget(){
    delete m_settings;
}

Settings* SettingsWidget::getSettings(){
    return m_settings;
}

void SettingsWidget::updateSettings(){
    m_settings->matrixSize = m_sBMatrixSize->value();
    m_settings->fps = m_sBFPS->value();
    m_settings->characterHeight = m_sBCharacterHeight->value();
    m_settings->characterWidth = m_sBCharacterWidth->value();
    m_settings->lumLevels = m_sBLumLevels->value();
    m_settings->font = m_font;
    m_settings->numbers = m_cBNumbers->isChecked();
    m_settings->lowercase = m_cBLowercase->isChecked();
    m_settings->uppercase = m_cBUppercase->isChecked();
    m_settings->transpose = m_cBReadingDirection->currentData().toBool();
    m_settings->rotation = m_cBRotation->currentData().toInt();

    m_lbSpritesSize->setText(QString::number(SettingsWidget::getSpritesSize(m_settings))+ " octets");
    m_lbRefreshFrequency->setText(QString::number(SettingsWidget::getRefreshFrequency(m_settings)) + QString(" Hz"));

    emit(newSettings());
}

int SettingsWidget::getSpritesSize(Settings* settings){
    int numberRotations = (settings->transpose)?1:0;
    numberRotations += settings->rotation;
    int width, height;
    if(numberRotations%2 == 0){
        width = settings->characterWidth;
        height = settings->characterHeight;
    }
    else{
        height = settings->characterWidth;
        width = settings->characterHeight;
    }

    int line = qCeil(((qreal) width)/8.)*8;//bits
    int frame_character = line * height;
    int character = frame_character * (settings->lumLevels-1);
    int nbCharacters = 0;
    if(settings->numbers){
        nbCharacters += 10;
    }
    if(settings->lowercase){
        nbCharacters += 26;
    }
    if(settings->uppercase){
        nbCharacters += 26;
    }
    return (character*nbCharacters)/8;
}

int SettingsWidget::getRefreshFrequency(Settings *settings){
    return settings->matrixSize * settings->fps * (settings->lumLevels - 1);
}


void SettingsWidget::changeFont(){
    bool ok;
    m_font = QFontDialog::getFont(&ok, m_font, this);
    if(ok){
        m_lbFont->setText(m_font.family());
        m_lbFont->setFont(m_font);
        updateSettings();
    }
}

void SettingsWidget::setCharacterSizeRange(){
    m_sBCharacterHeight->setRange(1, m_sBMatrixSize->value());
    m_sBCharacterWidth->setRange(1, m_sBMatrixSize->value());
}
