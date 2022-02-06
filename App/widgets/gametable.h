#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QLabel>
#include <QFontMetrics>
#include <QEvent>
#include <QMouseEvent>
#include <QMainWindow>

class GameTable : public QLabel
{
    Q_OBJECT
private:
    qsizetype tableSize;
    unsigned short int **table;
    unsigned short int TABLE_MARGINS;
    unsigned short int cellSize;
    short int hoveredCellPosition;

public:
    explicit GameTable(QWidget *parent = nullptr);
    ~GameTable();

    void restoreTable(qsizetype _tableSize);
    void shuffleTable();

    void setHoveredCell(short int pos);
    void setHoveredCell(unsigned short int posX, unsigned short int posY);
    bool eventFilter(QObject *obj, QEvent *event);

    void restoreCellSize();

protected:
    void paintEvent(QPaintEvent *event);
    void drawBackground(QPainter *painter);
    void drawCell(QPainter *painter, short int pos);
    void drawTable(QPainter *painter);

};

#endif // GAMETABLE_H
