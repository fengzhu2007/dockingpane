#ifndef DOCKING_GUIDE_COVER_H
#define DOCKING_GUIDE_COVER_H
#include <QFrame>
namespace ady{
class DockingGuideCoverPrivate;

class DockingGuideCover : public QFrame {
    Q_OBJECT
public:
    enum Sider{
        Full = 0,
        Top,
        Bottom
    };
    DockingGuideCover(QWidget* parent);
    ~DockingGuideCover();

    void setShape(int sider,int tabOffset,int tabHeight);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

    DockingGuideCoverPrivate* d;

};

}
#endif // DOCKING_GUIDE_COVER_H
