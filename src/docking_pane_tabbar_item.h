#ifndef DOCKING_PANE_TABBAR_ITEM_H
#define DOCKING_PANE_TABBAR_ITEM_H
#include <QPushButton>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QEnterEvent>
#endif
namespace ady{
    class DockingPaneTabBarItemPrivate;
    class DockingPaneTabBarItem : public QPushButton
    {
        Q_OBJECT

    public:
        DockingPaneTabBarItem(QWidget* parent);
        ~DockingPaneTabBarItem();
        virtual QSize sizeHint() const override;
        void setOrientation(Qt::Orientation orientation);
        void setSwap(bool swap);
        void setMirrored(bool mirrored);


    protected:
        virtual void paintEvent(QPaintEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        virtual void enterEvent(QEnterEvent *event) override;
#else
        virtual void enterEvent(QEvent *event) override;
#endif
        virtual void leaveEvent(QEvent *event) override;



    private:
        DockingPaneTabBarItemPrivate* d;

    };
}
#endif // DOCKING_PANE_TABBAR_ITEM_H
