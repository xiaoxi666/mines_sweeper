#ifndef FIELDDATA_H
#define FIELDDATA_H

#include <vector>
using namespace std;

#define Field (FieldData::getInstance())

typedef vector<int> CellColumn;
typedef vector<CellColumn> CellMatrix;

class FieldData
{
public:
    ~FieldData();
    static FieldData * getInstance();
    inline int getWidth(){return _width;}
    inline int getHeight(){return _height;}
    inline int getMines(){return _mines;}
    const CellMatrix & getCells(){return _cells;}
    void reset();
    void initCells();//初始化，申请空间
    void deployMines();//布雷
    void initVisited();//初始化visited

    void customizeWHM(int cw, int ch, int cm);//每次点击后更新（参数：宽，高，雷）

    //void dump();//布雷之后打印

    CellMatrix _cells;
    CellMatrix _visited;//是否已经扫过(扫过则为1，否则为0)

protected:
    int _width;
    int _height;
    int _mines;
    //    CellMatrix _cells;

    void updateSurrounding(int x, int y);

private:
    FieldData();//单例模式
    static FieldData *_ins;
};

#endif // FIELDDATA_H
