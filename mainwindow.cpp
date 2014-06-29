#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testWidget.h"
#include "aleatoriedadWidget.h"

#include "tracer.h"

#include "documentacion.h"
#include "indexwindow.h"
#include "contentwindow.h"
#include "helpenginewrapper.h"
#include "searchwidget.h"
#include "simuladorWidget.h"

// #define TRACING_REQUESTED

#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QPair>
#include <QtCore/QResource>
#include <QtCore/QByteArray>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>

#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QDockWidget>
#include <QTreeView>
#include <QMessageBox>
#include <QFontDatabase>
#include <QComboBox>
#include <QProgressBar>
#include <QDesktopServices>
#include <QToolButton>
#include <QToolBox>
#include <QFileDialog>
#include <QSpacerItem>

#include <QtHelp/QHelpEngineCore>
#include <QtHelp/QHelpSearchEngine>
#include <QtHelp/QHelpContentModel>
#include <QtHelp/QHelpIndexModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    helpEngineWrapper = HelpEngineWrapper::instance("sesimula.qhc");

    if(!helpEngineWrapper->setupData())
        qDebug()<<"Hay fichero";
    else
        qDebug()<<"No Hay fichero";

    m_documentacion = new Documentacion(ui->tab_1);
    ui->verticalLayout_1->addWidget(m_documentacion);

    m_aleatoriedadWidget = new AleatoriedadWidget(ui->tab_2);
    ui->verticalLayout_2->addWidget(m_aleatoriedadWidget);

    m_testWidget = new TestWidget(ui->tab_3);
    ui->verticalLayout_3->addWidget(m_testWidget);

    m_simuladorWidget = new SimuladorWidget(ui->tab_4);
    ui->verticalLayout_4->addWidget(m_simuladorWidget);

    m_contentWindow = new ContentWindow;

    m_indexWindow = new IndexWindow(this);

    QHelpSearchEngine *searchEngine = helpEngineWrapper->searchEngine();
    connect(searchEngine, SIGNAL(indexingStarted()), this, SLOT(indexingStarted()));
    connect(searchEngine, SIGNAL(indexingFinished()), this, SLOT(indexingFinished()));

    m_documentacion->createSearchWidget(searchEngine);

    m_searchWidget = m_documentacion->searchWidget();

    setupDockWidget();

    setupActions();

    createMenus();

    createToolBar();

    connect(m_aleatoriedadWidget,SIGNAL(exportarGenerador()),this, SLOT(exportar()));

    m_documentacion->setupWidget();

    contentDock->show();
    indexDock->show();
    searchDock->hide();
    documentacionToolBar->show();
    simuladorDock->hide();
    m_simuladorWidget->simuladorToolBar->hide();
    m_aleatoriedadWidget->aleatoriedadToolBar->hide();
    m_testWidget->testToolBar->hide();
}

void MainWindow::exportar(){
    m_testWidget->importar(m_aleatoriedadWidget->verTablaWidget());
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::setupDockWidget(){

    contentDock = new QDockWidget(tr("Contenido"), this);
    contentDock->setObjectName(QLatin1String("ContentWindow"));
    contentDock->setWidget(m_contentWindow);
    addDockWidget(Qt::RightDockWidgetArea, contentDock);

    indexDock = new QDockWidget(tr("Indice"), this);
    indexDock->setObjectName(QLatin1String("IndexWindow"));
    indexDock->setWidget(m_indexWindow);
    addDockWidget(Qt::RightDockWidgetArea, indexDock);

    searchDock = new QDockWidget(tr("Busqueda"),this);
    searchDock->setWidget(m_searchWidget);
    addDockWidget(Qt::RightDockWidgetArea,searchDock);

    simuladorDock = new QDockWidget(tr("Simulador"),this);
    simuladorDock->setWidget(m_simuladorWidget->toolBox);
    addDockWidget(Qt::RightDockWidgetArea,simuladorDock);

    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

}

void MainWindow::indexingStarted()
{
    TRACE_OBJ
    if (!m_progressWidget) {
        m_progressWidget = new QWidget();
        QLayout* hlayout = new QHBoxLayout(m_progressWidget);

        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

        QLabel *label = new QLabel(tr("Actualizar la Buaqueda en el indice"));
        label->setSizePolicy(sizePolicy);
        hlayout->addWidget(label);

        QProgressBar *progressBar = new QProgressBar();
        progressBar->setRange(0, 0);
        progressBar->setTextVisible(false);
        progressBar->setSizePolicy(sizePolicy);

        hlayout->setSpacing(6);
        hlayout->setMargin(0);
        hlayout->addWidget(progressBar);

        statusBar()->addPermanentWidget(m_progressWidget);
    }
}

void MainWindow::indexingFinished()
{
    TRACE_OBJ
    statusBar()->removeWidget(m_progressWidget);
    delete m_progressWidget;
    m_progressWidget = 0;
}

void MainWindow::setupActions(){

    m_newTabAction = new QAction(tr("Nuevo &Tab"), m_documentacion);
    m_newTabAction->setShortcut(QKeySequence::AddTab);
    connect(m_newTabAction, SIGNAL(triggered()),
            m_documentacion, SLOT(newTab()));

    m_printAction = new QAction(QIcon(":/imagenes/interfaz/print.png"),tr("&Imprimir"), m_documentacion);
    m_printAction->setPriority(QAction::LowPriority);
    connect(m_printAction, SIGNAL(triggered()),
            m_documentacion, SLOT(newTab()));
    m_printAction->setShortcut(QKeySequence::Print);

    m_closeTabAction = new QAction(tr("&Cerrar Tab"), m_documentacion);
    m_closeTabAction->setShortcuts(QKeySequence::Close);
    connect(m_closeTabAction, SIGNAL(triggered()),
            m_documentacion, SLOT(newTab()));

    m_copyAction = new QAction(tr("&Copiar texto selecionado"), m_documentacion);
    connect(m_copyAction, SIGNAL(triggered()), m_documentacion, SLOT(copySelection()));
    m_copyAction->setPriority(QAction::LowPriority);
    m_copyAction->setIconText("&Copy");
    m_copyAction->setIcon(QIcon(":/imagenes/interfaz/copy-document.png"));
    m_copyAction->setShortcuts(QKeySequence::Copy);
    m_copyAction->setEnabled(false);

    m_findAction = new QAction(tr("&Buscar en el texto..."), m_documentacion);
    connect(m_findAction, SIGNAL(triggered()), m_documentacion, SLOT(showTextSearch()));
    m_findAction->setIconText(tr("&Find"));
    m_findAction->setIcon(QIcon(":/imagenes/interfaz/search.png"));
    m_findAction->setShortcuts(QKeySequence::Find);

    m_zoomInAction = new QAction(tr("Zoom &+"), m_documentacion);
    connect(m_zoomInAction, SIGNAL(triggered()),
            m_documentacion, SLOT(zoomIn()));
    m_zoomInAction->setPriority(QAction::LowPriority);
    m_zoomInAction->setIcon(QIcon(":/imagenes/interfaz/button-add.png"));
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);

    m_zoomOutAction = new QAction(tr("Zoom &-"), m_documentacion);
    connect(m_zoomOutAction, SIGNAL(triggered()),
            m_documentacion, SLOT(zoomOut()));
    m_zoomOutAction->setPriority(QAction::LowPriority);
    m_zoomOutAction->setIcon(QIcon(":/imagenes/interfaz/button-minus.png"));
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    m_resetZoomAction = new QAction(tr("Zoom &Normal"), m_documentacion);
    connect(m_resetZoomAction, SIGNAL(triggered()),
            m_documentacion, SLOT(resetZoom()));
    m_resetZoomAction->setPriority(QAction::LowPriority);
    m_resetZoomAction->setIcon(QIcon(":/imagenes/interfaz/number-one.png"));
    m_resetZoomAction->setShortcut(tr("Ctrl+0"));

    m_homeAction = new QAction(tr("&Inicio"), m_documentacion);
    connect(m_homeAction, SIGNAL(triggered()),
            m_documentacion, SLOT(home()));
    m_homeAction->setShortcut(tr("ALT+Home"));
    m_homeAction->setIcon(QIcon(":/imagenes/interfaz/arrow-up-bold-round.png"));

    m_backAction = new QAction(tr("&Atras"), m_documentacion);
    connect(m_backAction, SIGNAL(triggered()),
            m_documentacion, SLOT(backward()));
    m_backAction->setEnabled(false);
    m_backAction->setShortcuts(QKeySequence::Back);
    m_backAction->setIcon(QIcon(":/imagenes/interfaz/arrow-curve-left.png"));

    m_nextAction = new QAction(tr("&Adelante"), m_documentacion);
    connect(m_nextAction, SIGNAL(triggered()),
            m_documentacion, SLOT(forward()));
    m_nextAction->setPriority(QAction::LowPriority);
    m_nextAction->setEnabled(false);
    m_nextAction->setShortcuts(QKeySequence::Forward);
    m_nextAction->setIcon(QIcon(":/imagenes/interfaz/arrow-curve-right.png"));

    m_syncAction = new QAction(tr("Actualizar"), this);
    connect(m_syncAction, SIGNAL(triggered()),
            this, SLOT(syncContents()));
    m_syncAction->setIconText(tr("Actualizar"));
    m_syncAction->setIcon(QIcon(":/imagenes/interfaz/arrow-curve-recycle.png"));

    action_Sobre = new QAction(QIcon(":/SESimula.ico"),tr("&Acerca de &SESimula"),this);
    action_Sobre_Qt = new QAction(QIcon(":/interfaz/assistant.png"),tr("&Acerca de &Qt"),this);
}

void MainWindow::createMenus(){
    TRACE_OBJ

    documentacionMenuBar = new QMenuBar;
    menu_Archivo = new QMenu(tr("&Archivo"), documentacionMenuBar);
    menu_Archivo->addAction(m_newTabAction);

    menu_Archivo->addSeparator();

    menu_Archivo->addAction(m_printAction);

    menu_Archivo->addSeparator();

    menu_Archivo->addAction(m_closeTabAction);

    QAction *tmp = menu_Archivo->addAction(QIcon::fromTheme("application-exit"),
                                   tr("&Salir"), this, SLOT(close()));
    tmp->setMenuRole(QAction::QuitRole);
    tmp->setShortcut(QKeySequence(tr("CTRL+Q")));

    menu_Editar = new QMenu(tr("&Editar"), documentacionMenuBar);
    menu_Editar->addAction(m_copyAction);
    menu_Editar->addAction(m_findAction);

    QAction *findNextAction = menu_Editar->addAction(tr("Buscar &Siguiente"), m_documentacion,
        SLOT(findNext()));
    findNextAction->setShortcuts(QKeySequence::FindNext);

    QAction *findPreviousAction = menu_Editar->addAction(tr("Buscar &Anterior"),
        m_documentacion, SLOT(findPrevious()));
    findPreviousAction->setShortcuts(QKeySequence::FindPrevious);

    menu_Ver = new QMenu(tr("&Ver"), documentacionMenuBar);
    menu_Ver->addAction(m_zoomInAction);
    menu_Ver->addAction(m_zoomOutAction);
    menu_Ver->addAction(m_resetZoomAction);

    menu_Ver->addSeparator();

    menu_Ver->addAction(tr("Contenido"), contentDock, SLOT(show()),
        QKeySequence(tr("ALT+C")));
    menu_Ver->addAction(tr("Indice"), indexDock, SLOT(show()),
        QKeySequence(tr("ALT+I")));
    menu_Ver->addAction(tr("Busqueda"), searchDock, SLOT(show()),
        QKeySequence(tr("ALT+S")));

    menu_Navegar = new QMenu(tr("Navegar"), documentacionMenuBar);
    menu_Navegar->addAction(m_homeAction);
    menu_Navegar->addAction(m_backAction);
    menu_Navegar->addAction(m_nextAction);
    menu_Navegar->addAction(m_syncAction);

    menu_Navegar->addSeparator();

    tmp = menu_Navegar->addAction(tr("Próxima Pagina"), m_documentacion, SLOT(nextPage()));
    tmp->setShortcuts(QList<QKeySequence>() << QKeySequence(tr("Ctrl+Alt+Right"))
        << QKeySequence(Qt::CTRL + Qt::Key_PageDown));

    tmp = menu_Navegar->addAction(tr("Pagina Anterior"), m_documentacion, SLOT(previousPage()));
    tmp->setShortcuts(QList<QKeySequence>() << QKeySequence(tr("Ctrl+Alt+Left"))
        << QKeySequence(Qt::CTRL + Qt::Key_PageUp));

    menu_Ayuda = new QMenu(tr("&Ayuda"));
    menu_Ayuda->addAction(action_Sobre);
    menu_Ayuda->addAction(action_Sobre_Qt);

    documentacionMenuBar->addAction(menu_Archivo->menuAction());
    documentacionMenuBar->addAction(menu_Editar->menuAction());
    documentacionMenuBar->addAction(menu_Ver->menuAction());
    documentacionMenuBar->addAction(menu_Navegar->menuAction());
    documentacionMenuBar->addAction(menu_Ayuda->menuAction());

    setMenuBar(documentacionMenuBar);
}

void MainWindow::createToolBar()
{ 
    documentacionToolBar = addToolBar(tr("Navegación Toolbar"));
    documentacionToolBar->setObjectName(QLatin1String("documentacionToolBar"));
    documentacionToolBar->addAction(m_backAction);
    documentacionToolBar->addAction(m_syncAction);
    documentacionToolBar->addAction(m_nextAction);
    documentacionToolBar->addAction(m_homeAction);
    QAction *sep = documentacionToolBar->addSeparator();
    documentacionToolBar->addAction(m_copyAction);
    documentacionToolBar->addAction(m_printAction);
    documentacionToolBar->addAction(m_findAction);
    QAction *sep2 = documentacionToolBar->addSeparator();
    documentacionToolBar->addAction(m_zoomInAction);
    documentacionToolBar->addAction(m_zoomOutAction);
    documentacionToolBar->addAction(m_resetZoomAction);

    addToolBar(m_simuladorWidget->simuladorToolBar);
    addToolBar(m_aleatoriedadWidget->aleatoriedadToolBar);
    addToolBar(m_testWidget->testToolBar);

    QList<QAction*> actionList;
    actionList << m_backAction << m_nextAction << m_homeAction << sep
        << m_zoomInAction << m_zoomOutAction << sep2 << m_copyAction
        << m_printAction << m_findAction;
    m_documentacion->setGlobalActions(actionList);

    // content viewer connections
    connect(m_documentacion, SIGNAL(copyAvailable(bool)), this,
        SLOT(copyAvailable(bool)));
    connect(m_documentacion, SIGNAL(forwardAvailable(bool)), this,
        SLOT(updateNavigationItems()));
    connect(m_documentacion, SIGNAL(backwardAvailable(bool)), this,
        SLOT(updateNavigationItems()));
    connect(m_documentacion, SIGNAL(highlighted(QString)), statusBar(),
        SLOT(showMessage(QString)));

    // index window
    connect(m_indexWindow, SIGNAL(linkActivated(QUrl)), m_documentacion,
        SLOT(setSource(QUrl)));
    connect(m_indexWindow, SIGNAL(escapePressed()), this,
        SLOT(activateCurrentCentralWidgetTab()));

    // content window
    connect(m_contentWindow, SIGNAL(linkActivated(QUrl)), m_documentacion,
        SLOT(setSource(QUrl)));
    connect(m_contentWindow, SIGNAL(escapePressed()), this,
        SLOT(activateCurrentCentralWidgetTab()));
}

void MainWindow::activateCurrentBrowser()
{
    TRACE_OBJ
    Documentacion *cw = Documentacion::instance();
    if (cw) {
        cw->activateTab(true);
    }
}

void MainWindow::syncContents()
{
    TRACE_OBJ
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    const QUrl url = m_documentacion->currentSource();
    contentDock->show();
    if (!m_contentWindow->syncToContent(url))
        statusBar()->showMessage(
            tr("No podra encontrar un indice asociado al contenido."), 3000);
    qApp->restoreOverrideCursor();
}

void MainWindow::activateCurrentCentralWidgetTab()
{
    TRACE_OBJ
    m_documentacion->activateTab();
}

void MainWindow::copyAvailable(bool yes)
{
    TRACE_OBJ
    m_copyAction->setEnabled(yes);
}

void MainWindow::updateNavigationItems()
{
    TRACE_OBJ
    bool hasCurrentViewer = m_documentacion->isHomeAvailable();
    m_copyAction->setEnabled(m_documentacion->hasSelection());
    m_homeAction->setEnabled(hasCurrentViewer);
    m_syncAction->setEnabled(hasCurrentViewer);
    m_printAction->setEnabled(hasCurrentViewer);
    m_nextAction->setEnabled(m_documentacion->isForwardAvailable());
    m_backAction->setEnabled(m_documentacion->isBackwardAvailable());
    m_newTabAction->setEnabled(hasCurrentViewer);
}

void MainWindow::updateTabCloseAction()
{
    TRACE_OBJ
    m_documentacion->setEnabled(m_documentacion->enableTabCloseAction());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Sobre_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch(index){
    case 0:
        contentDock->show();
        indexDock->show();
//        searchDock->show();
        documentacionToolBar->show();
        simuladorDock->hide();
        m_simuladorWidget->simuladorToolBar->hide();
        m_aleatoriedadWidget->aleatoriedadToolBar->hide();
        m_testWidget->testToolBar->hide();
        createMenus();
//        setMenuBar(documentacionMenuBar);
        break;
    case 1:
        contentDock->hide();
        indexDock->hide();
        searchDock->hide();
        simuladorDock->hide();
        documentacionToolBar->hide();
        m_simuladorWidget->simuladorToolBar->hide();
        m_aleatoriedadWidget->aleatoriedadToolBar->show();
        m_testWidget->testToolBar->hide();
        m_aleatoriedadWidget->createMenus();
        setMenuBar(m_aleatoriedadWidget->aleatoriedadMenuBar);
        break;
    case 2:
        contentDock->hide();
        indexDock->hide();
        searchDock->hide();
        simuladorDock->hide();
        documentacionToolBar->hide();
        m_simuladorWidget->simuladorToolBar->hide();
        m_aleatoriedadWidget->aleatoriedadToolBar->hide();
        m_testWidget->testToolBar->show();
        m_testWidget->createMenus();
        setMenuBar(m_testWidget->testMenuBar);
        break;
    case 3:
        contentDock->hide();
        indexDock->hide();
        searchDock->hide();
        simuladorDock->show();
        documentacionToolBar->hide();
        m_simuladorWidget->simuladorToolBar->show();
        m_aleatoriedadWidget->aleatoriedadToolBar->hide();
        m_testWidget->testToolBar->hide();
        m_simuladorWidget->createMenus();
        setMenuBar(m_simuladorWidget->simuladorMenuBar);
        break;
    }
}
