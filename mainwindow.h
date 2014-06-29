#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

QT_BEGIN_NAMESPACE

class HelpEngineWrapper;
class Documentacion;
class ContentWindow;
class IndexWindow;
class SearchWidget;
class SimuladorWidget;
class TestWidget;
class AleatoriedadWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static void activateCurrentBrowser();

private:
    void setupActions();
    void createMenus();
    void createToolBar();
    void setupDockWidget();
    
private slots:

    void on_action_Sobre_Qt_triggered();
    void on_tabWidget_currentChanged(int index);

    void indexingStarted();
    void indexingFinished();

    void syncContents();
    void activateCurrentCentralWidgetTab();
    void copyAvailable(bool yes);
    void updateNavigationItems();
    void updateTabCloseAction();

    void exportar();

private:
    Ui::MainWindow *ui;

    HelpEngineWrapper *helpEngineWrapper;
    IndexWindow *m_indexWindow;
    ContentWindow *m_contentWindow;
    SearchWidget *m_searchWidget;

    Documentacion *m_documentacion;

    AleatoriedadWidget *m_aleatoriedadWidget;

    TestWidget *m_testWidget;

    SimuladorWidget *m_simuladorWidget;

    QMenuBar *documentacionMenuBar;
    QMenu *menu_Archivo;
    QMenu *menu_Editar;
    QMenu *menu_Ver;
    QMenu *menu_Navegar;
    QMenu *menu_Ayuda;

    QDockWidget *contentDock;
    QDockWidget *indexDock;
    QDockWidget *searchDock;
    QDockWidget *simuladorDock;

    QToolBar *documentacionToolBar;

    QWidget *m_progressWidget;

    QAction *action_Sobre;
    QAction *action_Sobre_Qt;
    QAction *m_newTabAction;
    QAction *m_printAction;
    QAction *m_closeTabAction;
    QAction * m_copyAction;
    QAction * m_findAction;
    QAction * m_zoomInAction;
    QAction * m_zoomOutAction;
    QAction * m_resetZoomAction;
    QAction * m_homeAction;
    QAction * m_backAction;
    QAction * m_nextAction;
    QAction * m_syncAction;

};

QT_END_NAMESPACE

#endif // MAINWINDOW_H
