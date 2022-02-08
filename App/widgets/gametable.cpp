#include "gametable.h"

GameTable::GameTable(QWidget *parent) : QLabel(parent), TABLE_MARGINS(7)
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
    setFocusPolicy(Qt::StrongFocus);
    hoveredCellPosition = -1;
    isDisabled = true;
    this->installEventFilter(this);
    this->setMouseTracking(true);
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
    qsizetype i1 = -1, zero_pos = tableSize * tableSize - 1;
    unsigned short int opt;
    QList<int> posibleMoves;

    for (unsigned short int i = 0; i < tableSize * tableSize * tableSize * tableSize; i++)
    {
        posibleMoves = {};
        if (zero_pos - tableSize >= 0)
            posibleMoves.push_back(-tableSize);
        if (zero_pos - 1 >= 0)
            posibleMoves.push_back(-1);
        if (zero_pos + 1 < tableSize * tableSize)
            posibleMoves.push_back(1);
        if (zero_pos + tableSize < tableSize * tableSize)
            posibleMoves.push_back(tableSize);
        opt = rand() % posibleMoves.size();

        i1 = zero_pos + posibleMoves[opt];

        std::swap(table[i1 / tableSize][i1 % tableSize], table[zero_pos / tableSize][zero_pos % tableSize]);
        zero_pos = i1;
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
        painter->fillPath(path, QBrush(QColor(223, 193, 143)));
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
    }else if (event->type() == QEvent::MouseButtonPress)
    {
        if (!isDisabled)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            unsigned short int posX, posY;
            posX = mouseEvent->position().x();
            posY = mouseEvent->position().y();

            Move(posX, posY);
            repaint();
            if (checkTable())
                emit gameEnded();
        }
    }else if (event->type() == QEvent::KeyPress)
    {
        if (!isDisabled)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            int key = keyEvent->key();
            if (key == Qt::Key_Left || key == Qt::Key_A)
            {
                Move(directions::LEFT);
            }
            else if (key == Qt::Key_Up || key == Qt::Key_W)
            {
                Move(directions::UP);
            }
            else if (key == Qt::Key_Right || key == Qt::Key_D)
            {
                Move(directions::RIGHT);
            }
            else if (key == Qt::Key_Down || key == Qt::Key_S)
            {
                Move(directions::DOWN);
            }
            if (checkTable())
                emit gameEnded();
        }
    }
    return false;
}

void GameTable::restoreCellSize()
{
    cellSize = qRound(static_cast<double>(this->size().width() - (tableSize + 1) * TABLE_MARGINS) / tableSize);
}

void GameTable::Move(unsigned short posX, unsigned short posY)
{
    unsigned short int cellId = (posY / (this->size().height() / tableSize) * tableSize + posX / (this->size().width() / tableSize));

    if (cellId - 1 >= 0 && cellId % tableSize != 0 && table[(cellId - 1) / tableSize][(cellId - 1) % tableSize] == 0)
    {
        std::swap(table[(cellId - 1) / tableSize][(cellId - 1) % tableSize], table[cellId / tableSize][cellId % tableSize]);
        return;
    }
    if (cellId - tableSize >= 0 && table[(cellId - tableSize) / tableSize][(cellId - tableSize) % tableSize] == 0)
    {
        std::swap(table[(cellId - tableSize) / tableSize][(cellId - tableSize) % tableSize], table[cellId / tableSize][cellId % tableSize]);
        return;
    }
    if (cellId + 1 < tableSize * tableSize && cellId % tableSize != tableSize - 1 && table[(cellId + 1) / tableSize][(cellId + 1) % tableSize] == 0)
    {
        std::swap(table[(cellId + 1) / tableSize][(cellId + 1) % tableSize], table[cellId / tableSize][cellId % tableSize]);
        return;
    }
    if (cellId + tableSize < tableSize * tableSize && table[(cellId + tableSize) / tableSize][(cellId + tableSize) % tableSize] == 0)
    {
        std::swap(table[(cellId + tableSize) / tableSize][(cellId + tableSize) % tableSize], table[cellId / tableSize][cellId % tableSize]);
        return;
    }
}

void GameTable::Move(directions d)
{
    qsizetype zero_pos = 0;

    for (qsizetype i = 0; i < tableSize * tableSize; i++)
    {
        if (table[i / tableSize][i % tableSize] == 0)
        {
            zero_pos = i;
            break;
        }
    }

    if (d == directions::UP)
    {
        if (zero_pos + tableSize < tableSize * tableSize)
        {
            std::swap(table[zero_pos / tableSize][zero_pos % tableSize], table[(zero_pos + tableSize) / tableSize][(zero_pos + tableSize) % tableSize]);
        }
    }
    if (d == directions::DOWN)
    {
        if (zero_pos - tableSize >= 0)
        {
            std::swap(table[zero_pos / tableSize][zero_pos % tableSize], table[(zero_pos - tableSize) / tableSize][(zero_pos - tableSize) % tableSize]);
        }
    }
    if (d == directions::LEFT)
    {
        if (zero_pos + 1 < tableSize * tableSize && zero_pos % tableSize != tableSize - 1)
        {
            std::swap(table[zero_pos / tableSize][zero_pos % tableSize], table[(zero_pos + 1) / tableSize][(zero_pos + 1) % tableSize]);
        }
    }
    if (d == directions::RIGHT)
    {
        if (zero_pos - 1 >= 0 && zero_pos % tableSize != 0)
        {
            std::swap(table[zero_pos / tableSize][zero_pos % tableSize], table[(zero_pos - 1) / tableSize][(zero_pos - 1) % tableSize]);
        }
    }

    repaint();
}

void GameTable::StartGame()
{
    this->shuffleTable();
    isDisabled = false;
    repaint();
}

void GameTable::StopGame()
{
    this->restoreTable(tableSize);
    this->repaint();
    isDisabled = true;
}

bool GameTable::checkTable()
{
    for (qsizetype i = 0; i < tableSize * tableSize - 1; i++)
    {
        if (table[i / tableSize][i % tableSize] != i + 1)
            return false;
    }
    return true;
}
