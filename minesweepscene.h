#ifndef MINESWEEPSCENE_H
#define MINESWEEPSCENE_H

#include <QGraphicsScene>
#include "cellItem.h"
#include <vector>
using namespace std;


typedef vector<cellItem*> ItemColumn;
typedef vector<ItemColumn> ItemMatrix;

class mineSweepScene : public QGraphicsScene
{
public:
    mineSweepScene(QObject *parent = Q_NULLPTR);
    ~mineSweepScene();

    void initItems();
    void clearItems();

//protected:
    ItemMatrix _items;

};

#endif // MINESWEEPSCENE_H
