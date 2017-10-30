#include "previewwidget.h"

PreviewWidget::PreviewWidget(SpriteGenerator *spriteGenerator, QWidget *parent) : QWidget(parent)
{
    m_spriteGenerator = spriteGenerator;

    //Sprite
    m_sprite = new QLabel("test");

    //Sélection
    m_cBSet = new QComboBox(this);
    m_cBSet->setEditable(true);
    m_cBSet->lineEdit()->setReadOnly(true);
    m_cBSet->lineEdit()->setAlignment(Qt::AlignCenter);

    m_pBNext = new QPushButton("Suivant");
    m_pBPrevious = new QPushButton("Précédent");
    m_lSelector = new QHBoxLayout();
    m_lSelector->addWidget(m_pBPrevious);
    m_lSelector->addWidget(m_cBSet);
    m_lSelector->addWidget(m_pBNext);

    //Layout principal
    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_sprite, 0, Qt::AlignCenter);
    m_layout->addLayout(m_lSelector);
    setLayout(m_layout);

    connect(m_pBNext, SIGNAL(clicked(bool)), this, SLOT(next()));
    connect(m_pBPrevious, SIGNAL(clicked(bool)), this, SLOT(previous()));
    connect(m_cBSet, SIGNAL(currentTextChanged(QString)), this, SLOT(setButtonsStates()));
    connect(m_cBSet, SIGNAL(currentTextChanged(QString)), this, SLOT(updateLabel(QString)));
}

void PreviewWidget::updateLabel(QString str){
    QImage img = m_spriteGenerator->drawCharacter(str[0]);

    QPixmap pixmap(m_settings.characterWidth*10+1, m_settings.characterHeight*10+1);
    QPainter *painter = new QPainter(&pixmap);
    painter->fillRect(pixmap.rect(), QColor(255,255,255));
    painter->setPen(QColor(0, 0, 0));

    for(int i=0; i < m_settings.characterHeight; i++){
        for(int j=0; j < m_settings.characterWidth; j++){
            painter->drawRect(QRect(j*10,i*10,10,10));
            int gray_level = 255/(m_settings.lumLevels-1);
            gray_level *= (qGray(img.pixel(j,i)))/(255/m_settings.lumLevels + 1);
            QRect rect(j*10+1, i*10+1, 9, 9);
            painter->fillRect(rect, (QColor(255-gray_level, 0, 0)));
        }
    }

    delete(painter);
    m_sprite->setPixmap(pixmap);
}

void PreviewWidget::next(){
    m_cBSet->setCurrentIndex(m_cBSet->currentIndex()+1);
    setButtonsStates();
}

void PreviewWidget::previous(){
    m_cBSet->setCurrentIndex(m_cBSet->currentIndex()-1);
    setButtonsStates();
}

void PreviewWidget::setButtonsStates(){
    m_pBNext->setEnabled(m_cBSet->count() && m_cBSet->currentIndex() < m_cBSet->count()-1);
    m_pBPrevious->setEnabled(m_cBSet->currentIndex() != 0 && m_cBSet->count());
}

void PreviewWidget::updateSettings(Settings settings){
    m_settings = settings;
    m_cBSet->setFont(m_settings.font);
    setCharactersSet();
    setButtonsStates();
}

void PreviewWidget::setCharactersSet(){
    QString set;
    if(m_settings.numbers){
        set.append("0123456789");
    }
    QString letters = QString("abcdefghijklmnopqrstuvwxyz");
    if(m_settings.lowercase){
        set.append(letters);
    }
    if(m_settings.uppercase){
        set.append(letters.toUpper());
    }

    m_cBSet->clear();
    for(int i=0; i < set.length(); i++){
        m_cBSet->addItem(QString(set[i]));
    }
}
