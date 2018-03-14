#include "pixelbot.h"
#include <QIcon>

PixelBot::PixelBot(QWidget *parent)
    : QWidget(parent)
{
    //Paramètres de la fenêtre
    setWindowIcon(QIcon(":/icons/icon.png"));

    //Widgets
    m_settingsWidget = new SettingsWidget(this);
    m_gbSettings = new QGroupBox("Paramètres", this);
    m_gbSettings->setLayout(m_settingsWidget->layout());
    m_pBExport = new QPushButton("Générer les sprites", this);

    m_leftLayout = new QVBoxLayout();
    m_leftLayout->addWidget(m_gbSettings);
    m_leftLayout->addWidget(m_pBExport);

    m_spriteGenerator = new SpriteGenerator(m_settingsWidget->getSettings(), this);

    m_previewWidget = new PreviewWidget(m_spriteGenerator, m_settingsWidget->getSettings(), this);
    m_gbPreview = new QGroupBox("Aperçu", this);
    m_gbPreview->setLayout(m_previewWidget->layout());

    //Layout
    m_layout = new QHBoxLayout();
    m_layout->addLayout(m_leftLayout);
    m_layout->addWidget(m_gbPreview);
    setLayout(m_layout);

    connect(m_settingsWidget, SIGNAL(newSettings()), m_spriteGenerator, SLOT(updateSettings()));
    m_settingsWidget->updateSettings();
    connect(m_settingsWidget, SIGNAL(newSettings()), m_previewWidget, SLOT(updateSettings()));
    m_settingsWidget->updateSettings();
    connect(m_pBExport, SIGNAL(clicked(bool)), this, SLOT(generateSprites()));
}

PixelBot::~PixelBot()
{

}

void PixelBot::generateSprites(){
    disconnect(m_settingsWidget, SIGNAL(newSettings()), m_spriteGenerator, SLOT(updateSettings()));
    //On récupère le nom du fichier de sauvegarde
    QString fileNameHeader = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Exporter les sprites (header)"), QString("sprites.h"), tr("C/C++ Header (*.h *.hpp)"));
    QString fileNameSource = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Exporter les sprites (source)"), QString("sprites.c"), tr("C/C++ Source (*.c *.cpp)"));
    if(fileNameHeader.isEmpty() || fileNameSource.isEmpty()){
        return;
    }

    //On exporte les sprites
    m_spriteGenerator->exportSprites(fileNameHeader, true);
    m_spriteGenerator->exportSprites(fileNameSource, false);

    //On ouvre le fichier exporté dans l'éditeur par défaut
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileNameHeader));
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileNameSource));
    connect(m_settingsWidget, SIGNAL(newSettings()), m_spriteGenerator, SLOT(updateSettings()));
}
