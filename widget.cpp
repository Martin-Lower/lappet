#include "widget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this)),// 这里是复用，前边是继承，timer是初始化列表
    menu(new QMenu(this))
{

    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);// 隐藏任务栏 去除边框  顶层显示
    this->setAttribute(Qt::WA_TranslucentBackground);   // 背景透明
    this->installEventFilter(new DragFilter);   // 拖拽效果


    connect(timer,&QTimer::timeout,[this](){
        static int index = 0;// 记录当前动作的图片索引
        auto paths = this->action_map.value(this->cur_role_act);
        // if(paths == )
        this->cur_role_pix = paths[index++%paths.size()];
        this->update();// paintEvent的调用API
    });
    initMenu();

    loadRoleActRes();

    showActAnimation(RoleAct::Standby_one);
}

Widget::~Widget(){}

void Widget::showActAnimation(RoleAct k)
{
    timer->stop();
    this->cur_role_act = k;
    timer->start(200);
    // 停止原动画，进行下一个动作
}

void Widget::onMenuTriggered(QAction *action)
{
    QMetaEnum me = QMetaEnum::fromType<RoleAct>();
    bool isok;
    int k = me.keyToValue(action->text().toStdString().c_str(),&isok);
    if(!isok)return;

    showActAnimation(static_cast<RoleAct>(k));
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pix;
    pix.load(this->cur_role_pix.toLocalFile());

    painter.drawPixmap(0,0,pix);


}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    this->menu->popup(QCursor::pos());//菜单弹出
}

// 加载图片
void Widget::loadRoleActRes()
{
    // 图片已经加载到了资源文件里，做个映射
    auto addRes=[this](RoleAct k,QString path,int s,int e,int model){
        QList<QUrl> paths;
        char buf[261];
        if(model==1){// 从头到尾循环
            for(int i=s;i<=e;i++){
                memset(buf,0,sizeof(buf));
                sprintf_s(buf,path.toStdString().c_str(),i);
                paths.append(QUrl::fromLocalFile(buf));
            }
        }
        else if(model == 2){// 从尾到头循环
            for(int i=e;i>=s;i--){
                memset(buf,0,sizeof(buf));
                sprintf_s(buf,path.toStdString().c_str(),i);
                paths.append(QUrl::fromLocalFile(buf));
            }
        }
        else{// 从头到尾再从尾到头循环
            for(int i=s;i<=e;i++){
                memset(buf,0,sizeof(buf));
                sprintf_s(buf,path.toStdString().c_str(),i);
                paths.append(QUrl::fromLocalFile(buf));
            }
            for(int i=e-1;i>=s;i--){
                sprintf_s(buf,path.toStdString().c_str(),i);
                paths.append(QUrl::fromLocalFile(buf));
            }
        }
        action_map.insert(k,paths);
    };

    addRes(RoleAct::Standby_one,":/witch/witch/图层 %d.png",1,4,3);
    addRes(RoleAct::Standby_two,":/witch2/witch2/图层 %d.png",1,5,3);
    addRes(RoleAct::Trans,":/trans/trans/图层 %d.png",1,20,1);
    addRes(RoleAct::Act,":/act/act/图层 %d.png",1,5,3);
    addRes(RoleAct::Act2,":/act2/act2/图层 %d.png",1,4,3);

    // // 外加的资源，鼠标上的特效
    // QList<QUrl> paths;
    // char buf[261];
    // // 红色粒子特效
    // for(int i=s;i<=e;i++){
    //     memset(buf,0,sizeof(buf));
    //     sprintf_s(buf,path.toStdString().c_str(),i);
    //     paths.append(QUrl::fromLocalFile(buf));
    // }// 再说吧


}

void Widget::initMenu()
{
    menu->addAction("Standby_one");
    menu->addAction("Standby_two");
    menu->addAction("Trans");
    menu->addAction("Act");
    menu->addAction("Act2");

    connect(this->menu,&QMenu::triggered,this,&Widget::onMenuTriggered);
}
