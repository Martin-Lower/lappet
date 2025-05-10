#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QUrl>
#include <QEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QCursor>


class QPaintEvent; // 前向声明


namespace Act{
    Q_NAMESPACE

enum RoleAct{
        Standby_one,
        Standby_two,
        Trans,
        Act,
        Act2
    };
Q_ENUM_NS(RoleAct)
}

using namespace Act;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void showActAnimation(RoleAct k);

public slots:
    void onMenuTriggered(QAction* action);

protected:
    void paintEvent(QPaintEvent* event)override;

    void contextMenuEvent(QContextMenuEvent* event);

private:
    void loadRoleActRes();               // 加载图片
    void initMenu();


private:
    QMap<RoleAct,QList<QUrl>> action_map;// 用map把动作和语句关联起来，List储存连续动作
    QMap<RoleAct,QList<QUrl>> vfx;

    QTimer* timer;// 计时器做动作

    RoleAct cur_role_act;// 当前人物动作
    QUrl cur_role_pix; // 当前动作的索引

    QMenu* menu;
};

// 定义一个类来实现拖拽效果
class DragFilter:public QObject{
public:
    bool eventFilter(QObject* obj,QEvent* event){
        auto w = dynamic_cast<QWidget*>(obj);
        if(!w) return false;
        if(event->type() == QEvent::MouseButtonPress){
            auto e = dynamic_cast<QMouseEvent*>(event);
            pos = e->pos();
        }
        else if(event->type() == QEvent::MouseMove){
            auto e = dynamic_cast<QMouseEvent*>(event);
            if(e){
                if(e->buttons()&Qt::MouseButton::LeftButton){
                    w->move(e->globalPosition().toPoint() - pos);
                }
            }
        }
        return QObject::eventFilter(obj,event);
    }
private:
    QPoint pos;
};

#endif // WIDGET_H
