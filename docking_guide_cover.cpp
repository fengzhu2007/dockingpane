#include "docking_guide_cover.h"

namespace ady{

    DockingGuideCover::DockingGuideCover(QWidget* parent)
        :QFrame(parent){
        setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
        this->setStyleSheet("background:#3333ff;border:5px solid gray");
        setWindowOpacity(0.3);
    }






}
