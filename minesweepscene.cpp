#include "minesweepscene.h"
#include "fielddata.h"
#include <QGraphicsPixmapItem>
//#include <QDebug>

mineSweepScene::mineSweepScene(QObject *parent):
    QGraphicsScene(parent)
{
    initItems();
}

mineSweepScene::~mineSweepScene(){
    clearItems();
}

void mineSweepScene::initItems(){
    int w = Field->getWidth();
    int h = Field->getHeight();

    for(int x=0;x<w;++x){
        _items.push_back(ItemColumn());
        for(int y=0;y<h;++y){
            _items[x].push_back(new cellItem(x,y));
            static int cw = _items[0][0]->boundingRect().width();
            static int ch = _items[0][0]->boundingRect().height();

            _items[x][y]->setPos(x*cw,y*ch);
            addItem(_items[x][y]);
        }
    }
}
void mineSweepScene::clearItems(){
    int w = Field->getWidth();
    int h = Field->getHeight();

    for(int x=0;x<w;++x){
        for(int y=0;y<h;++y){
            delete _items[x][y];
            _items[x][y]=nullptr;
        }
    }
    _items.clear();
}

