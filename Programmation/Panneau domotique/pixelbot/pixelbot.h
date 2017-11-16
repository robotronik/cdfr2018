#ifndef PIXELBOT_H
#define PIXELBOT_H

#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDesktopServices>
#include "settingswidget.h"
#include "previewwidget.h"
#include "spritegenerator.h"

class PixelBot : public QWidget
{
    Q_OBJECT

private:
    //Widgets
    QVBoxLayout *m_leftLayout;
    QGroupBox *m_gbSettings;
    SettingsWidget *m_settingsWidget;
    QPushButton *m_pBExport;

    QGroupBox *m_gbPreview;
    PreviewWidget *m_previewWidget;

    //Layouts
    QHBoxLayout *m_layout;

    //Générateur de sprites
    SpriteGenerator *m_spriteGenerator;

public:
    PixelBot(QWidget *parent = 0);
    ~PixelBot();

public slots:
    void generateSprites();
};

#endif // PIXELBOT_H
