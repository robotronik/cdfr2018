#include "spritegenerator.h"

SpriteGenerator::SpriteGenerator(Settings *settings, QObject *parent) : QObject(parent)
{
    m_settings = settings;
}

void SpriteGenerator::exportSprites(QString fileName){
    //Ouverture du modèle
    QFile headerFile(":/header/sprites.h");
    if(!headerFile.open(QFile::ReadOnly | QFile::Text)){
        return;
    }
    QTextStream in(&headerFile);
    QString header = in.readAll();
    headerFile.close();

    //Génération du header
    writeSprites(&header);

    //Sauvegarde du header
    QFile saveFile(fileName);
    if(!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    QTextStream out(&saveFile);
    out << header.toUtf8();
    saveFile.close();
}

QString SpriteGenerator::getLine(unsigned char *line, int width){
    QString str;
    str.append("{");
    //Pour chaque octet
    for(int i = 0; i < width; i += 8){
        unsigned char mask = 0b10000000;
        unsigned char byte = 0;

        //Pour chaque bit
        int byteSize = (width-i < 8)?(width-i):8;
        for(int j = 0; j < byteSize; j++){
            if(line[i+j] > 0){
                line[i+j] -= 1;
                byte |= mask;
            }
            else{
                byte &= ~mask;
            }
            mask >>= 1;
        }
        str.append(QString::number(byte));
        if(i+8 < width){
            str.append(",");
        }
    }
    str.append("}");
    return str;
}

QString SpriteGenerator::getFrame(unsigned char **array, int width, int height){
    QString frame;
    //Pour chaque ligne
    for(int i = 0; i < height; i++){
        frame.append(QString("{"+getLine(array[i], width)+"}"));
        if(i < height-1){
            frame.append(QString(", "));
        }
    }
    return frame;
}

QString SpriteGenerator::getSprite(const QChar character){
    //On dessine le caractère dans une image aux bonnes dimensions
    QImage img = drawCharacter(character);

    //On effectue les transpositions/rotations
    if(m_settings->transpose){
        img = img.mirrored(true, false);
        QTransform transform;
        transform.rotate(-90);
        img = img.transformed(transform);
    }
    QTransform transform;
    transform.rotate(-m_settings->rotation*90);
    img = img.transformed(transform);
    //img.save(QString(QString(character)+".png"));

    //On convertit les couleurs en nombre de cycles allumés
    int width = img.width();
    int height = img.height();
    unsigned char **array = new unsigned char*[height];
    array[0] = new unsigned char[height*width];
    for(int i = 1; i < height; i++){
        array[i] = array[0]+i*width;
    }
    //Remplissage du tableau
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            array[i][j] = (255-qGray(img.pixel(j,i)))/(255/m_settings->lumLevels + 1);
        }
    }

    QString sprite;
    //Pour chaque frame
    for(int i = 0; i < m_settings->lumLevels-1; i++){
        sprite.append("\n  //Frame "+QString::number(i)+"\n  {"+getFrame(array, width, height)+"}");
        if(i < m_settings->lumLevels-2){
            sprite.append(",");
        }
    }

    delete(array[0]);
    delete(array);
    return sprite;
}

void SpriteGenerator::writeSprites(QString *str){
    writeInfos(str);
    int flipNumber = m_settings->transpose + m_settings->rotation;
    int rowLength = ((flipNumber%2)==0)?m_settings->characterWidth:m_settings->characterHeight;
    int rowsPerFrame = ((flipNumber%2)==0)?m_settings->characterHeight:m_settings->characterWidth;
    int bytesPerRow = (rowLength+7)/8;
    int numbersIndex = 0;
    int nextIndex = 0;
    if(m_settings->numbers){
        nextIndex += 10;
    }
    int lowercaseIndex = m_settings->lowercase?nextIndex:0;
    if(m_settings->lowercase)
    {
        nextIndex += 26;
    }
    int uppercaseIndex = m_settings->uppercase?nextIndex:0;
    if(m_settings->uppercase){
        nextIndex += 26;
    }
    int charactersNumber = nextIndex;

    str->replace(tr("$ROW_LENGTH"), QString::number(rowLength));
    str->replace(tr("$ROWS_PER_FRAME"), QString::number(rowsPerFrame));
    str->replace(tr("$BYTES_PER_ROW"), QString::number(bytesPerRow));
    str->replace(tr("$NUMBERS"), QString::number(m_settings->numbers?1:0));
    str->replace(tr("$LOWERCASE"), QString::number(m_settings->lowercase?1:0));
    str->replace(tr("$UPPERCASE"), QString::number(m_settings->uppercase?1:0));
    str->replace(tr("$INDEX_NUMBERS"), QString::number(numbersIndex));
    str->replace(tr("$INDEX_LOWERCASE"), QString::number(lowercaseIndex));
    str->replace(tr("$INDEX_UPPERCASE"), QString::number(uppercaseIndex));
    str->replace(tr("$CHARACTERS_NUMBER"), QString::number(charactersNumber));

    QString sprites("");
    for(int i=0; i < m_set.length(); i++){
        sprites.append("\n//Sprite "+QString(m_set[i])+"\n{"+getSprite(m_set[i])+"}");
        if(i < m_set.length() - 1){
            sprites.append(",");
        }
    }
    str->replace(QString("$DATA"), sprites);
}

void SpriteGenerator::writeInfos(QString *str){
    str->replace(tr("$MATRIX_SIZE"), QString::number(m_settings->matrixSize));
    str->replace(tr("$FPS"), QString::number(m_settings->fps));
    str->replace(tr("$BRIGHTNESS_LEVELS"), QString::number(m_settings->lumLevels));
    str->replace(tr("$CLOCK_FREQUENCY"), QString::number(SettingsWidget::getRefreshFrequency(m_settings)));
    QString readingDirection;
    if(m_settings->transpose){
        readingDirection = tr("par colonne");
    }
    else{
        readingDirection = tr("par ligne");
    }
    str->replace(tr("$READING_DIERCTION"), readingDirection);
    QString rotation;
    switch(m_settings->rotation){
        case 0:
        rotation = tr("aucune");
        break;
    case 1:
        rotation = tr("pi/2");
        break;
    case 2:
        rotation = tr("pi");
        break;
    case 3:
        rotation = tr("3pi/2");
        break;
    default:
        break;
    }
    str->replace(tr("$ROTATION"), rotation);
    str->replace(tr("$CHARACTER_WIDTH"), QString::number(m_settings->characterWidth));
    str->replace(tr("$CHARACTER_HEIGHT"), QString::number(m_settings->characterHeight));
    str->replace(tr("$FONT"), m_settings->font.family());
    QString characters;
    if(m_settings->numbers){
        characters.append("chiffres ");
    }
    if(m_settings->lowercase){
        if(!characters.isEmpty()){
            characters.append(", ");
        }
        characters.append("minuscules ");
    }
    if(m_settings->uppercase){
        if(!characters.isEmpty()){
            characters.append(", ");
        }
        characters.append("majuscules");
    }
    str->replace(tr("$CHARACTERS_SET"), characters);
    str->replace(tr("$DATE"), QDateTime::currentDateTime().toString());
}

void SpriteGenerator::updateSettings(){
    //Ensemble de caractères
    m_set.clear();
    if(m_settings->numbers){
        m_set.append("0123456789");
    }
    QString letters = QString("abcdefghijklmnopqrstuvwxyz");
    if(m_settings->lowercase){
        m_set.append(letters);
    }
    if(m_settings->uppercase){
        m_set.append(letters.toUpper());
    }
}

QImage SpriteGenerator::drawCharacter(const QChar character){
    //Calcul de la taille de la police de caractères
    int max_width = m_settings->characterWidth;
    int max_height = m_settings->characterHeight;
    int current_size = 0;
    QRect current_rect, last_rect;

    do{
        last_rect = current_rect;
        current_size++;
        m_settings->font.setPixelSize(current_size);
        QFontMetrics fontMetrics(m_settings->font);
        current_rect = fontMetrics.tightBoundingRect(QString(character));
    }while((current_rect.width() <= max_width) && (current_rect.height() <= max_height));
    current_rect = last_rect;
    current_size--;
    m_settings->font.setPixelSize(current_size);

    //Ecriture du caractère dans un pixmap
    QPixmap pixmap(m_settings->characterWidth, m_settings->characterHeight);
    QPainter *painter = new QPainter(&pixmap);

    painter->setFont(m_settings->font);
    painter->fillRect(pixmap.rect(), QColor(255,255,255));

    int x = -current_rect.x() + (max_width-current_rect.width())/2;
    int y = -current_rect.y() + (max_height-current_rect.height())/2;
    painter->drawText(QPoint(x, y), QString(character));
    delete(painter);
    return pixmap.toImage();
}

QString SpriteGenerator::getCharacterSet(){
    return QString(m_set);
}
