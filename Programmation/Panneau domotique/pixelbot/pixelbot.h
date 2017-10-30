#ifndef PIXELBOT_H
#define PIXELBOT_H

#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include "settingswidget.h"
#include "previewwidget.h"
#include "spritegenerator.h"

class PixelBot : public QWidget
{
    Q_OBJECT

private:
    //Widgets
    SettingsWidget *m_settingsWidget;
    QGroupBox *m_gbSettings;
    QGroupBox *m_gbPreview;
    PreviewWidget *m_previewWidget;
    QPushButton *m_pBExport;

    //Layouts
    QVBoxLayout *m_layout;

    //Générateur de sprites
    SpriteGenerator *m_spriteGenerator;

public:
    PixelBot(QWidget *parent = 0);
    ~PixelBot();
};

#endif // PIXELBOT_H
