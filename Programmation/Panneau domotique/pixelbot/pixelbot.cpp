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

    m_spriteGenerator = new SpriteGenerator();

    m_previewWidget = new PreviewWidget(m_spriteGenerator, this);
    m_gbPreview = new QGroupBox("Aperçu", this);
    m_gbPreview->setLayout(m_previewWidget->layout());

    m_pBExport = new QPushButton("Générer les sprites", this);

    //Layout
    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_gbSettings);
    m_layout->addWidget(m_gbPreview);
    m_layout->addWidget(m_pBExport);
    setLayout(m_layout);

    connect(m_settingsWidget, SIGNAL(newSettings(Settings)), m_spriteGenerator, SLOT(updateSettings(Settings)));
    m_settingsWidget->sendSettings();
    connect(m_settingsWidget, SIGNAL(newSettings(Settings)), m_previewWidget, SLOT(updateSettings(Settings)));
    m_settingsWidget->sendSettings();
}

PixelBot::~PixelBot()
{

}
