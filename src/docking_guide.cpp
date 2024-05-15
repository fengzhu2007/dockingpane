#include "docking_guide.h"
#include "ui_docking_guide.h"

#include "docking_pane_container.h"
#include "docking_workbench.h"

#include <QPixmap>
#include <QBitmap>

#include <QDebug>


namespace ady {
    DockingGuide::DockingGuide(DockingWorkbench* parent)
        :QFrame(parent),
        ui(new Ui::DockingPaneGuide){
        ui->setupUi(this);
        setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
        //setAttribute(Qt::WA_NoBackground, true);
        //setStyleSheet("background: transparent;");
        //this->setStyleSheet(QString::fromUtf8("background-image: url(:/images/docking.png);"));

        ui->left->setPosition(DockingPaneManager::Left);
        ui->right->setPosition(DockingPaneManager::Right);
        ui->top->setPosition(DockingPaneManager::Top);
        ui->bottom->setPosition(DockingPaneManager::Bottom);
        ui->center->setPosition(DockingPaneManager::Center);

        QPixmap pixmap(":/images/vs2019/docking.png");
        QPalette palette;
        palette.setBrush(this->backgroundRole(),QBrush(pixmap));
        setPalette(palette);
        setMask(pixmap.mask());
        setAutoFillBackground(true);
    }

    DockingGuide::~DockingGuide(){
        delete ui;
    }

    DockingGuideDiamond* DockingGuide::updateActive(const QPoint& pos)
    {
        QRect rc = geometry();
        QPoint p (pos.x() - rc.x(),pos.y() - rc.y());
        bool ret = ui->left->updateActive(p);
        if(ret){
            return ui->left;
        }
        ret = ui->top->updateActive(p);
        if(ret){
            return ui->top;
        }
        ret = ui->center->updateActive(p);
        if(ret){
            return ui->center;
        }
        ret = ui->right->updateActive(p);
        if(ret){
            return ui->right;
        }
        ret = ui->bottom->updateActive(p);
        if(ret){
            return ui->bottom;
        }
        return nullptr;
    }
}
