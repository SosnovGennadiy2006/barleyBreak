#include "gametable.h"

GameTable::GameTable(QWidget *parent) : QLabel(parent), TABLE_MARGINS(5)
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
    hoveredCellPosition = -1;
    this->installEventFilter(this);
    this->setMouseTracking(true);

    restoreTable(4);
    shuffleTable();
}

GameTable::~GameTable()
{
    for (qsizetype i = 0; i < tableSize; i++)
    {
        delete[] table[i];
    }
    delete table;
}

// This method restore first condition of table (solved condition)
void GameTable::restoreTable(qsizetype _tableSize)
{
    tableSize = _tableSize;
    table = new unsigned short int*[tableSize];
    for (qsizetype i = 0; i < tableSize; i++)
    {
        table[i] = new unsigned short int[tableSize];
        for (qsizetype j = 0; j < tableSize; j++)
        {
            table[i][j] = i * tableSize + j + 1;
        }
    }

    table[tableSize - 1][tableSize - 1] = 0;
}

// This method shuffle table
void GameTable::shuffleTable()
{
    qsizetype i1, i2;
    for (unsigned short int i = 0; i < 100; i++)
    {
        i1 = rand() % (tableSize * tableSize);
        i2 = rand() % (tableSize * tableSize);
        std::swap(table[i1 / tableSize][i2 % tableSize], table[i2 / tableSize][i2 % tableSize]);
    }
}

void GameTable::setHoveredCell(short int pos)
{
    hoveredCellPosition = pos;
}

void GameTable::setHoveredCell(unsigned short int posX, unsigned short int posY)
{
    if (hoveredCellPosition != (posY / (this->size().height() / tableSize) * tableSize + posX / (this->size().width() / tableSize)))
    {
        hoveredCellPosition = (posY / (this->size().height() / tableSize) * tableSize + posX / (this->size().width() / tableSize));
        repaint();
    }
}

void GameTable::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    drawBackground(&painter);

    restoreCellSize();

    drawTable(&painter);
}

void GameTable::drawBackground(QPainter *painter)
{
    painter->setOpacity(0.3);
    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, this->size().width(), this->size().height()), 10, 10);
    painter->fillPath(path, QBrush(QColor(0, 0, 0)));
    painter->setOpacity(1);
}

void GameTable::drawCell(QPainter *painter, short pos)
{
    unsigned short int number = table[pos / tableSize][pos % tableSize];
    unsigned short int posY = (pos / tableSize) * (TABLE_MARGINS + cellSize) + TABLE_MARGINS;
    unsigned short int posX = (pos % tableSize) * (TABLE_MARGINS + cellSize) + TABLE_MARGINS;

    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF(posX, posY, cellSize, cellSize), 10, 10);
    painter->setPen(QPen(QColor(61, 38, 0), 7));
    painter->drawPath(path);
    if (pos == hoveredCellPosition)
        painter->fillPath(path, QBrush(QColor(214, 132, 0)));
    else
        painter->fillPath(path, QBrush(QColor(255, 220, 163)));

    QFont font;
    font.setFamily("Bahnschrift SemiBold");
    font.setPixelSize(40);

    QFontMetrics metrics(font);
    QSize textSize = metrics.size(Qt::TextSingleLine, QString::number(number));

    painter->setFont(font);
    painter->drawText(posX + cellSize / 2 - textSize.width() / 2, posY + cellSize / 2 + textSize.height() / 3, QString::number(number));
}

void GameTable::drawTable(QPainter *painter)
{
    for (qsizetype i = 0; i < tableSize; i++)
    {
        for (qsizetype j = 0; j < tableSize; j++)
        {
            if (table[i][j] != 0)
                drawCell(painter, i * tableSize + j);
        }
    }
}

bool GameTable::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);

    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        unsigned short int posX, posY;
        posX = mouseEvent->position().x();
        posY = mouseEvent->position().y();

        this->setHoveredCell(posX, posY);
    }
    else if (event->type() == QEvent::Leave)
    {
        hoveredCellPosition = -1;
        repaint();
    }
    return false;
}

void GameTable::restoreCellSize()
{
    cellSize = (this->size().width() - (tableSize + 1) * TABLE_MARGINS) / tableSize;
}
