# dockingpane
qt vs like docking pane
visual studio dock panel

## demo

.h
```
class DockingPaneManager;

class DockWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit DockWindow(QWidget *parent = nullptr);
        ~DockWindow();

    private:
        Ui::DockWindow *ui;
        DockingPaneManager* m_dockingPaneManager;
};

```


.cpp

```
	DockWindow::DockWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::DockWindow)
    {
        ui->setupUi(this);

        m_dockingPaneManager = new DockingPaneManager(this);

        this->setCentralWidget(m_dockingPaneManager->widget());
        QString group="label";
        for(int i=0;i<5;i++){
            QLabel* label = new QLabel(QString("LABEL:%1").arg(i),(QWidget* )m_dockingPaneManager->workbench());
            //label->setMinimumSize(QSize(400,400));
            QString id = QString("id:%1").arg(i);
            m_dockingPaneManager->createPane(id,group,QString("title:%1").arg(i),label,DockingPaneManager::Right);
        }
    }

    DockWindow::~DockWindow()
    {
        delete ui;
    }
```



## screens

![](./screens/demo1.png)  
![](./screens/demo2.png)