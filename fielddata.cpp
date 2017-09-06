#include "fielddata.h"
#include <stdlib.h>
#include <ctime>
//#include <iostream>
//#include <qDebug>
//using namespace std;

FieldData *FieldData::_ins=nullptr;

FieldData::FieldData()
    : _width(20), _height(15), _mines(50)//初始扫雷矩阵大小
{
    reset();
    srand(time(nullptr));
}

FieldData::~FieldData() {

}

void FieldData::reset(){
    _cells.clear();
    initCells();
    deployMines();
    _visited.clear();
    initVisited();
}

//初始化，申请空间
void FieldData::initCells() {
    for(int x = 0; x < _width; ++x) {
        _cells.push_back(CellColumn(_height));
    }
}

//初始化visited
void FieldData::initVisited() {
    for(int x = 0; x < _width; ++x) {
        _visited.push_back(CellColumn(_height));
    }
    for(int x = 0; x < _width; ++x) {
        for(int y = 0; y < _height; ++y) {
            _visited[x][y] = 0;
        }
    }
}


//布雷
void FieldData::deployMines() {
    //初始化
    for(int x = 0; x < _width; ++x) {
        for(int y = 0; y < _height; ++y) {
            _cells[x][y] = 0;
        }
    }
    int cpmines = _mines;
    while(cpmines) {
        int x = rand() % _width;
        int y = rand() % _height;

        //只有之前没有布过雷的位置才可以布雷
        if(_cells[x][y] != -1) {
            --cpmines;
            _cells[x][y] = -1;

            //更新周围的雷数
            updateSurrounding(x - 1, y - 1); //左上
            updateSurrounding(x, y - 1); //上
            updateSurrounding(x + 1, y - 1); //右上
            updateSurrounding(x - 1, y); //左
            updateSurrounding(x + 1, y); //右
            updateSurrounding(x - 1, y + 1); //左下
            updateSurrounding(x, y + 1); //下
            updateSurrounding(x + 1, y + 1); //右下

        }
    }
    //dump();
}

//布雷之后，更新周围的雷数
void FieldData::updateSurrounding(int x, int y) {
    //越界判断
    if(x >= 0 && x < _width && y >= 0 && y < _height) {
        if(_cells[x][y] != -1) {
            ++_cells[x][y];
        }
    }
}

//void FieldData::dump(){
//    for(int y = 0; y < _height; ++y) {
//        for(int x = 0; x < _width; ++x) {
//            cout<<_cells[x][y]<<'\t';
//        }
//        cout<<endl;
//    }
//    //qDebug()<<"---------------------------------"<<endl;
//}

//每次点击后更新（参数：宽，高，雷）
void FieldData::customizeWHM(int cw, int ch, int cm) {
    _width=cw;
    _height=ch;
    _mines=cm;
}

FieldData * FieldData::getInstance(){
    if(_ins==nullptr){
        _ins=new FieldData;
    }
    return _ins;
}
