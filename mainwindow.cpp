#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "configdialog.h"
#include "fielddata.h"
#include "minesweepscene.h"
//#include <QDebug>
#include <queue>
//using namespace std;

double factor_Scale=0.9;//缩放因子(自定义),其实最后限制30*19了，这个缩放因子在14寸以上屏幕不会用到。
bool IsScale=false;//是否缩放(缩放了的话鼠标坐标也要缩放)

//判赢条件:地雷全部标记&&其他全部翻开(即状态为初始化的个数为地雷数)
int remain_mines=Field->getMines();//剩余需要寻找的地雷数量
int remain_INITIAL=Field->getWidth()*Field->getHeight();//状态还是初始化的方块个数

struct Point_Px
{
    double px;
    double py;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FieldData::getInstance();

    _view = new QGraphicsView;
    this->setCentralWidget(_view);

    _scene=new mineSweepScene;
    _view->setScene(_scene);

    //qDebug()<<"0--------"<<this->width()<<_view->width()<<_scene->width();
    //设置窗口大小合适，正好包含_view即可
    this->showMaximized();//最大化窗口
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deleteOldScene(){
    //场景还原
    this->showMaximized();//最大化窗口
    //thisH=this->width();
    if(_scene->width()>this->width()){
        _view->scale(_scene->width()/this->width()/factor_Scale,_scene->width()/this->width()/factor_Scale);
        IsScale=false;
    }
    //删除旧场景
    if(_scene!=nullptr){
        delete _scene;
        _scene = nullptr;
    }
}

void MainWindow::addNewScene(){
    //布置新场景
    _scene = new mineSweepScene;
    _view->setScene(_scene);

    //元素缩放(注意只根据宽度缩放，因为上下滚动很平常，但左右滚动很难受)
    //调整元素大小(元素过多时_scene->width()很大，需要缩放)，注意缩放后，下次布置场景还需要还原
    this->showMaximized();//最大化窗口
    if(_scene->width()>this->width()){
        IsScale=true;
        _view->scale(this->width()/_scene->width()*factor_Scale,this->width()/_scene->width()*factor_Scale);//设置本次窗口大小
    }
    else{
        IsScale=false;
    }

    remain_mines=Field->getMines();//剩余需要寻找的地雷数量
    remain_INITIAL=Field->getWidth()*Field->getHeight();//状态还是初始化的方块个数
}

void MainWindow::on_actionNew_game_triggered()
{
    deleteOldScene();
    //开始新游戏不需要来考虑雷数太多的情况，因为和上一场宽高雷数目相同，只是位置变了
    Field->customizeWHM(Field->getWidth(),Field->getHeight(),Field->getMines());
    Field->reset();
    addNewScene();
}

void MainWindow::on_actionConfig_triggered()
{
    configDialog config(Field->getWidth(),Field->getHeight(),Field->getMines());
    if(config.exec()==QDialog::Accepted){
        //检查雷数，限制其不能太多
        if(config._mines<config._width*config._height*0.5){
            deleteOldScene();
            Field->customizeWHM(config._width,config._height,config._mines);
            Field->reset();
            addNewScene();
        }
        else{
            QMessageBox::warning(this,"Warning","mines too many!");
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"about","version 1.0");
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    double px=event->localPos().x();
    double py=event->localPos().y();//主窗口上边占用了35px,因此要减去这个偏移量

    QGraphicsItem *item =_view->itemAt(px, py-35);
    _cItem=dynamic_cast<cellItem*>(item);//或 _cItem=(cellItem*)item;

    if(item!=nullptr){//点击在地图范围之外不予响应

        static double cw = IsScale ?  item->boundingRect().width()*this->width()/_scene->width()*factor_Scale : item->boundingRect().width();
        static double ch = IsScale ?  item->boundingRect().height()*this->width()/_scene->width()*factor_Scale : item->boundingRect().height();

        switch(event->button())
        {
        case Qt::LeftButton:
            //只有初始化和问号状态左键才可以点击进行扫雷
            if(INITIAL==_cItem->getStatus() || QUESTION==_cItem->getStatus()){
                switch(Field->_cells[item->pos().x()/cw][item->pos().y()/ch]){//获取数据域对应数据
                //Field->_visited[item->pos().x()/cw][item->pos().y()/ch]=1;
                case -1:
                    --remain_INITIAL;
                    _cItem->setStatus(EXPLODE);
                    QMessageBox::information(this,"Result","You lost! New game will start...");
                    on_actionNew_game_triggered();
                    break;
                case 0:
                    _cItem->setStatus(BLANK);
                    --remain_INITIAL;JudgeIsWin();

                    block_blank(px,py,item->pos().x()/cw,item->pos().y()/ch);
                    //cout<<"My surrounding have no mines."<<endl;
                    break;
                case 1:
                    _cItem->setStatus(DIGIT,1);
                    --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding has 1 mine."<<endl;
                    break;
                case 2:
                    _cItem->setStatus(DIGIT,2);
                    --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 2 mines."<<endl;
                    break;
                case 3:
                    _cItem->setStatus(DIGIT,3);
                     --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 3 mines."<<endl;
                    break;
                case 4:
                    _cItem->setStatus(DIGIT,4);
                     --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 4 mines."<<endl;
                    break;
                case 5:
                    _cItem->setStatus(DIGIT,5);
                    --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 5 mines."<<endl;
                    break;
                case 6:
                    _cItem->setStatus(DIGIT,6);
                    --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 6 mines."<<endl;
                    break;
                case 7:
                    _cItem->setStatus(DIGIT,7);
                    --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 7 mines."<<endl;
                    break;
                case 8:
                    _cItem->setStatus(DIGIT,8);
                    --remain_INITIAL;JudgeIsWin();
                    //cout<<"My surrounding have 8 mines."<<endl;
                    break;
                default:
                    break;
                }
            }
            else if(FLAG==_cItem->getStatus()){//插旗位置只能通过右键解锁
                ;
            }
            break;
        case Qt::RightButton:
            if(INITIAL==_cItem->getStatus()){
                _cItem->setStatus(FLAG);
                if(-1==Field->_cells[item->pos().x()/cw][item->pos().y()/ch]){
                    --remain_mines;JudgeIsWin();

                }
            }
            else if(FLAG==_cItem->getStatus()){
                _cItem->setStatus(QUESTION);
                if(-1==Field->_cells[item->pos().x()/cw][item->pos().y()/ch]){
                    ++remain_mines;
                }

            }
            else if(QUESTION==_cItem->getStatus()){
                _cItem->setStatus(INITIAL);
            }
            break;
        default:
            break;
        }
    }
}

//px和py为鼠标指针坐标，x和y为数据矩阵坐标点(考虑到图像元素可能会缩放，因此鼠标坐标定义为double,但是这种精度下，仍然不能放太多元素，否则自动寻找时会出现越界情况，因此需限定元素上限)
void MainWindow::block_blank(double px,double py,int x,int y){
    Field->initVisited();

    QGraphicsItem *item =_view->itemAt(px, py-35);

    static double cw = IsScale ?  item->boundingRect().width()*this->width()/_scene->width()*factor_Scale : item->boundingRect().width();
    static double ch = IsScale ?  item->boundingRect().height()*this->width()/_scene->width()*factor_Scale : item->boundingRect().height();

    const int dir[8][2]={{0, 1}, {1, 0}, {0, -1}, {-1, 0},//下,右,上,左
                         {1, 1}, {1, -1}, {-1, -1}, {-1, 1}//右下,右上,左上,左下
                        };//八个方向可以扩展

    queue<pair<Point_Px,int>> que;//保存鼠标坐标和数据矩阵坐标
    que.push(pair<Point_Px,int>({px,py},x*Field->getHeight()+y));
    Field->_visited[x][y]=1;
    while(!que.empty()){
        pair<Point_Px,int> fr=que.front();
        que.pop();
        px=fr.first.px;
        py=fr.first.py;
        x=fr.second/Field->getHeight();
        y=fr.second%Field->getHeight();

        for(int i=0;i<8;++i){//8个方向扩展
            double npx=px+cw*dir[i][0];
            double npy=py+ch*dir[i][1];
            int nx=x+dir[i][0];
            int ny=y+dir[i][1];
            //超出边界判断
            //qDebug()<<npx<<npy<<nx<<ny;
            if(nx<0 || nx>=Field->getWidth() || ny<0 || ny>=Field->getHeight()
                    || 1==Field->_visited[nx][ny]){
                //qDebug()<<"miaomiao-continue";
                continue;
            }

            item =_view->itemAt(npx, npy-35);
            _cItem=dynamic_cast<cellItem*>(item);

            //插旗的不再寻路
            if(FLAG==_cItem->getStatus())
                continue;

            //如果数字为0，表示周围没有雷，则继续加入队列进行下去
            if(0==Field->_cells[nx][ny]){
                --remain_INITIAL;JudgeIsWin();
                que.push(pair<Point_Px,int>({npx,npy},nx*Field->getHeight()+ny));
                Field->_visited[nx][ny]=1;
                _cItem->setStatus(BLANK);
            }
            else{
                switch (Field->_cells[nx][ny]) {
                case 1:
                    if(_cItem->getStatus()!=DIGIT){//注意需要加上状态判断，不然会再次设置相同状态，浪费资源，且把状态个数减去1，造成计算错误
                        _cItem->setStatus(DIGIT,1);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding has 1 mine."<<endl;
                    break;
                case 2:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,2);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 2 mines."<<endl;
                    break;
                case 3:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,3);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 3 mines."<<endl;
                    break;
                case 4:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,4);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 4 mines."<<endl;
                    break;
                case 5:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,5);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 5 mines."<<endl;
                    break;
                case 6:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,6);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 6 mines."<<endl;
                    break;
                case 7:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,7);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 7 mines."<<endl;
                    break;
                case 8:
                    if(_cItem->getStatus()!=DIGIT){
                        _cItem->setStatus(DIGIT,8);
                        --remain_INITIAL;JudgeIsWin();
                    }
                    //cout<<"My surrounding have 8 mines."<<endl;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

bool MainWindow::JudgeIsWin(){
    //qDebug()<<"remain_mines:"<<remain_mines;
    //qDebug()<<"remain_INITIAL:"<<remain_INITIAL;
    if(0==remain_mines && Field->getMines()==remain_INITIAL){
        QMessageBox::information(this,"Result","You win! New game will start...");
        on_actionNew_game_triggered();
        return true;
    }
    return false;
}
