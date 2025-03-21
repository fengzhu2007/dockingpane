#include "docking_guide_cover.h"
#include "docking_theme.h"
#include <QPainter>
#include <QDebug>
namespace ady{

class DockingGuideCoverPrivate{
public:
    int tabOffset;
    int sider;
    int tabHeight;
    int placeholderWidth;
    QColor primaryColor;
    QColor borderColor;

};


    DockingGuideCover::DockingGuideCover(QWidget* parent)
        :QFrame(parent){
        setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
        //this->setStyleSheet("background:#007acc;border:5px solid #aaa");
        setWindowOpacity(0.3);
        d = new DockingGuideCoverPrivate;
        d->placeholderWidth = 160;
        auto instance = DockingTheme::getInstance();
        d->primaryColor = instance->primaryColor();//light #007acc
        d->borderColor = instance->borderColor();//light #ccc

    }

    DockingGuideCover::~DockingGuideCover(){
        delete d;
    }

    void DockingGuideCover::setShape(int sider,int tabOffset,int tabHeight){

        auto size = this->size();
        auto width = size.width();
        if((tabOffset + d->placeholderWidth) > width ){
            d->placeholderWidth = width - tabOffset;
            if(d->placeholderWidth < 30){
                tabOffset = width - d->placeholderWidth;
            }
        }else{
            d->placeholderWidth = 160;
        }

        if(d->sider!=sider || d->tabOffset!=tabOffset || d->tabHeight!=tabHeight){
            d->tabOffset = tabOffset;
            d->sider = sider;
            d->tabHeight = tabHeight;
            this->update();
        }
    }

    void DockingGuideCover::paintEvent(QPaintEvent* event){
        QFrame::paintEvent(event);

        //fill background
        QPainter painter(this);
        painter.setBrush(d->primaryColor);
        painter.setPen(QPen(d->borderColor, 5));
        //auto rect = this->geometry();
        auto size = this->size();
        if(d->sider==Top){
            //fill backgorund
            auto rect = QRect{0,d->tabHeight,size.width(),size.height() - d->tabHeight};
            painter.fillRect(rect,d->borderColor);
            rect.adjust(5,5,-5,-5);
            painter.fillRect(rect,d->primaryColor);

            auto rc = QRect{d->tabOffset,0,d->placeholderWidth,d->tabHeight};
            painter.fillRect(rc,d->borderColor);
            rc.adjust(5,5,-5,5);
            painter.fillRect(rc,d->primaryColor);

        }else if(d->sider==Bottom){

            auto rect = QRect{0,0,size.width(),size.height() - d->tabHeight};
            painter.fillRect(rect,d->borderColor);
            rect.adjust(5,5,-5,-5);
            painter.fillRect(rect,d->primaryColor);

            auto rc = QRect{d->tabOffset,size.height() - d->tabHeight,d->placeholderWidth,d->tabHeight};
            painter.fillRect(rc,d->borderColor);
            rc.adjust(5,-5,-5,-5);
            painter.fillRect(rc,d->primaryColor);


        }else{
            auto rect = QRect{0,0,size.width(),size.height()};
            painter.fillRect(rect,d->borderColor);
            rect.adjust(5,5,-5,-5);
            painter.fillRect(rect,d->primaryColor);
        }
    }



}
