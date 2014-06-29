#include "documentacion.h"

#include "tracer.h"

#include "findwidget.h"
#include "helpenginewrapper.h"
#include "searchwidget.h"
#include "mainwindow.h"
#include "collectionconfiguration.h"

#include "helpviewer_qwv.h"

#include <QtCore/QTimer>

#include <QApplication>
#include <QKeyEvent>
#include <QLayout>
#include <QMenu>
#include <QPrinter>
#include <QTabBar>
#include <QTabWidget>
#include <QTextBrowser>
#include <QToolButton>
#include <QPageSetupDialog>
#include <QPrintDialog>
#include <QPrintPreviewDialog>

#include <QtHelp/QHelpSearchEngine>

QT_BEGIN_NAMESPACE

namespace {
    HelpViewer* helpViewerFromTabPosition(const QTabWidget *widget,
        const QPoint &point)
    {
        TRACE_OBJ
        QTabBar *tabBar = widget->findChild<QTabBar*>();
        for (int i = 0; i < tabBar->count(); ++i) {
            if (tabBar->tabRect(i).contains(point))
                return qobject_cast<HelpViewer*>(widget->widget(i));
        }
        return 0;
    }
    Documentacion *staticDocumentacion = 0;
}

// -- Documentacion

Documentacion::Documentacion(QWidget *parent)
    : QWidget(parent)
    , lastTabPage(0)
    , tabWidget(0)
    , findWidget(0)
    , printer(0)
//    , usesDefaultCollection(parent->usesDefaultCollection())
    , m_searchWidget(0)
{
    TRACE_OBJ
    globalActionList.clear();
    staticDocumentacion = this;
    QVBoxLayout *vboxLayout = new QVBoxLayout(this);

    vboxLayout->setMargin(0);
    tabWidget = new QTabWidget(this);

    connect(tabWidget, SIGNAL(currentChanged(int)), this,
        SLOT(currentPageChanged(int)));

    QToolButton *newTabButton = new QToolButton(this);
    newTabButton->setAutoRaise(true);
    newTabButton->setToolTip(tr("Agregar nueva pagina"));
    newTabButton->setIcon(QIcon(":/imagenes/interfaz/addtab.png"));

    tabWidget->setCornerWidget(newTabButton, Qt::TopLeftCorner);
    connect(newTabButton, SIGNAL(clicked()), this, SLOT(newTab()));

    QToolButton *closeTabButton = new QToolButton(this);
    closeTabButton->setEnabled(false);
    closeTabButton->setAutoRaise(true);
    closeTabButton->setToolTip(tr("Cerrar esta pagina"));
    closeTabButton->setIcon(QIcon(":/imagenes/interfaz/closetab.png"));

    tabWidget->setCornerWidget(closeTabButton, Qt::TopRightCorner);
    connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));

    vboxLayout->addWidget(tabWidget);

    findWidget = new FindWidget(this);
    vboxLayout->addWidget(findWidget);
    findWidget->hide();

    connect(findWidget, SIGNAL(findNext()), this, SLOT(findNext()));
    connect(findWidget, SIGNAL(findPrevious()), this, SLOT(findPrevious()));
    connect(findWidget, SIGNAL(find(QString, bool)), this,
        SLOT(find(QString, bool)));
    connect(findWidget, SIGNAL(escapePressed()), this, SLOT(activateTab()));

    QTabBar *tabBar = tabWidget->findChild<QTabBar*>();
    if (tabBar) {
        tabBar->installEventFilter(this);
        tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tabBar, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showTabBarContextMenu(QPoint)));
    }

}

Documentacion::~Documentacion()
{
    TRACE_OBJ
#ifndef QT_NO_PRINTER
    delete printer;
#endif

    QStringList zoomFactors;
    QStringList currentPages;
    for (int i = 0; i < tabWidget->count(); ++i) {
        HelpViewer *viewer = qobject_cast<HelpViewer*>(tabWidget->widget(i));
        if (viewer && viewer->source().isValid()) {
            currentPages << viewer->source().toString();
            zoomFactors << QString::number(viewer->scale());
        }
    }

    HelpEngineWrapper *helpEngine = HelpEngineWrapper::instance();
    helpEngine->setLastTabPage(tabWidget->currentIndex());
    helpEngine->setLastShownPages(currentPages);
    helpEngine->setLastZoomFactors(zoomFactors);
}

Documentacion *Documentacion::instance()
{
    TRACE_OBJ
    return staticDocumentacion;
}

void Documentacion::newTab()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
#if !defined(QT_NO_WEBKIT)
    if (viewer && viewer->hasLoadFinished())
#else
    if (viewer)
#endif
        setSourceInNewTab(viewer->source());
}

void Documentacion::zoomIn()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->scaleUp();

    if (tabWidget->currentWidget() == m_searchWidget)
        m_searchWidget->zoomIn();
}

void Documentacion::zoomOut()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->scaleDown();

    if (tabWidget->currentWidget() == m_searchWidget)
        m_searchWidget->zoomOut();
}

void Documentacion::nextPage()
{
    TRACE_OBJ
    int index = tabWidget->currentIndex() + 1;
    if (index >= tabWidget->count())
        index = 0;
    tabWidget->setCurrentIndex(index);
}

void Documentacion::resetZoom()
{
    TRACE_OBJ
    if (HelpViewer *viewer = currentHelpViewer())
        viewer->resetScale();

    if (tabWidget->currentWidget() == m_searchWidget)
        m_searchWidget->resetZoom();
}

void Documentacion::previousPage()
{
    TRACE_OBJ
    int index = tabWidget->currentIndex() -1;
    if (index < 0)
        index = tabWidget->count() -1;
    tabWidget->setCurrentIndex(index);
}

void Documentacion::closeTab()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (!viewer|| tabWidget->count() == 1)
        return;

    tabWidget->removeTab(tabWidget->indexOf(viewer));
    QTimer::singleShot(0, viewer, SLOT(deleteLater()));
}

void Documentacion::setSource(const QUrl &url)
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    HelpViewer *lastViewer =
        qobject_cast<HelpViewer*>(tabWidget->widget(lastTabPage));

    if (!viewer && !lastViewer) {
        viewer = new HelpViewer(this);
        viewer->installEventFilter(this);
        lastTabPage = tabWidget->addTab(viewer, QString());
        tabWidget->setCurrentIndex(lastTabPage);
        connectSignals();
    } else {
        viewer = lastViewer;
    }

    viewer->setSource(url);
    currentPageChanged(lastTabPage);
    viewer->setFocus(Qt::OtherFocusReason);
    tabWidget->setCurrentIndex(lastTabPage);
    tabWidget->setTabText(lastTabPage, quoteTabTitle(viewer->documentTitle()));
}

void Documentacion::setupWidget()
{
    TRACE_OBJ
    HelpEngineWrapper *helpEngine = HelpEngineWrapper::instance();
    int option = helpEngine->startOption();
    if (option != ShowLastPages) {
        QString homePage = "qthelp://aleplusplus.com.sesimula.0.2/doc/index.html";
//        if (option == ShowHomePage)
//            homePage = helpEngine.homePage();
//        else if (option == ShowBlankPage)
//            homePage = QLatin1String("about:blank");
        setSource(homePage);
    } else {
        setLastShownPages();
    }
}

void Documentacion::setLastShownPages()
{
    TRACE_OBJ
    HelpEngineWrapper *helpEngine = HelpEngineWrapper::instance();
    const QStringList &lastShownPageList = helpEngine->lastShownPages();
    const int pageCount = lastShownPageList.count();
    if (pageCount == 0) {
        if (usesDefaultCollection)
            setSource(QUrl(QLatin1String("help")));
        else
            setSource(QUrl(QLatin1String("about:blank")));
        return;
    }
    QStringList zoomFactors = helpEngine->lastZoomFactors();
    while (zoomFactors.count() < pageCount)
        zoomFactors.append(CollectionConfiguration::DefaultZoomFactor);

//    const bool searchIsAttached = m_searchWidget->isAttached();
//    const bool searchWasAttached = helpEngine.searchWasAttached();
    int tabToShow = helpEngine->lastTabPage();
//    if (searchWasAttached && !searchIsAttached && tabToShow != 0)
//        --tabToShow;
//    else if (!searchWasAttached && searchIsAttached)
//        ++tabToShow;

    for (int curTab = 0; curTab < pageCount; ++curTab) {
        const QString &curFile = lastShownPageList.at(curTab);
        if (helpEngine->findFile(curFile).isValid()
            || curFile == QLatin1String("about:blank")) {
            setSourceInNewTab(curFile, zoomFactors.at(curTab).toFloat());
        } /*else if (curTab + searchIsAttached <= tabToShow)
            --tabToShow;*/
    }

    tabWidget->setCurrentIndex(tabToShow);
}

bool Documentacion::hasSelection() const
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    return viewer ? viewer->hasSelection() : false;
}

QUrl Documentacion::currentSource() const
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        return viewer->source();

    return QUrl();
}

QString Documentacion::currentTitle() const
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        return viewer->documentTitle();

    return QString();
}

void Documentacion::copySelection()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->copy();
}

void Documentacion::showTextSearch()
{
    TRACE_OBJ
    findWidget->show();
}

void Documentacion::initPrinter()
{
    TRACE_OBJ
#ifndef QT_NO_PRINTER
    if (!printer)
        printer = new QPrinter(QPrinter::HighResolution);
#endif
}

void Documentacion::print()
{
    TRACE_OBJ
#ifndef QT_NO_PRINTER
    HelpViewer *viewer = currentHelpViewer();
    if (!viewer)
        return;

    initPrinter();

    QPrintDialog dlg(printer, this);
#if defined(QT_NO_WEBKIT)
    if (viewer->textCursor().hasSelection())
        dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
#endif
    dlg.addEnabledOption(QAbstractPrintDialog::PrintPageRange);
    dlg.addEnabledOption(QAbstractPrintDialog::PrintCollateCopies);
//    dlg.setWindowTitle(tr("Exportar documento a pdf"));
    if (dlg.exec() == QDialog::Accepted) {
        viewer->print(printer);
    }
#endif
}

void Documentacion::printPreview()
{
    TRACE_OBJ
#ifndef QT_NO_PRINTER
    initPrinter();
    QPrintPreviewDialog preview(printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
        SLOT(printPreview(QPrinter*)));
    preview.exec();
#endif
}

void Documentacion::printPreview(QPrinter *p)
{
    TRACE_OBJ
#ifndef QT_NO_PRINTER
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->print(p);
#endif
}

void Documentacion::pageSetup()
{
    TRACE_OBJ
#ifndef QT_NO_PRINTER
    initPrinter();
    QPageSetupDialog dlg(printer);
    dlg.exec();
#endif
}

bool Documentacion::isHomeAvailable() const
{
    TRACE_OBJ
    return currentHelpViewer() ? true : false;
}

void Documentacion::home()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->home();
}

bool Documentacion::isForwardAvailable() const
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        return viewer->isForwardAvailable();

    return false;
}

void Documentacion::forward()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->forward();
}

bool Documentacion::isBackwardAvailable() const
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        return viewer->isBackwardAvailable();

    return false;
}

void Documentacion::backward()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        viewer->backward();
}


QList<QAction*> Documentacion::globalActions() const
{
    TRACE_OBJ
    return globalActionList;
}

void Documentacion::setGlobalActions(const QList<QAction*> &actions)
{
    TRACE_OBJ
    globalActionList = actions;
}

void Documentacion::setSourceInNewTab(const QUrl &url, qreal zoom)
{
    TRACE_OBJ
    if (HelpViewer *viewer = currentHelpViewer()) {
        if (viewer->launchWithExternalApp(url))
            return;
    }

    HelpViewer *viewer = new HelpViewer(this, zoom);
    viewer->installEventFilter(this);
    viewer->setSource(url);
    viewer->setFocus(Qt::OtherFocusReason);
    tabWidget->setCurrentIndex(tabWidget->addTab(viewer,
        quoteTabTitle(viewer->documentTitle())));
    connectSignals();
}

HelpViewer *Documentacion::newEmptyTab()
{
    TRACE_OBJ
    HelpViewer *viewer = new HelpViewer(this);
    viewer->installEventFilter(this);
    viewer->setFocus(Qt::OtherFocusReason);
#if defined(QT_NO_WEBKIT)
    viewer->setDocumentTitle(tr("unknown"));
#endif
    tabWidget->setCurrentIndex(tabWidget->addTab(viewer, tr("unknown")));

    connectSignals();
    return viewer;
}

void Documentacion::connectSignals()
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    if (viewer) {
        connect(viewer, SIGNAL(copyAvailable(bool)), this,
            SIGNAL(copyAvailable(bool)));
        connect(viewer, SIGNAL(forwardAvailable(bool)), this,
            SIGNAL(forwardAvailable(bool)));
        connect(viewer, SIGNAL(backwardAvailable(bool)), this,
            SIGNAL(backwardAvailable(bool)));
        connect(viewer, SIGNAL(sourceChanged(QUrl)), this,
            SIGNAL(sourceChanged(QUrl)));
        connect(viewer, SIGNAL(highlighted(QString)), this,
            SIGNAL(highlighted(QString)));
        connect(viewer, SIGNAL(sourceChanged(QUrl)), this,
            SLOT(setTabTitle(QUrl)));
        connect(viewer, SIGNAL(printRequested()), this, SLOT(print()));
    }
}

HelpViewer* Documentacion::viewerAt(int index) const
{
    TRACE_OBJ
    return qobject_cast<HelpViewer*>(tabWidget->widget(index));
}

HelpViewer* Documentacion::currentHelpViewer() const
{
    TRACE_OBJ
    return qobject_cast<HelpViewer*>(tabWidget->currentWidget());
}

void Documentacion::activateTab(bool onlyHelpViewer)
{
    TRACE_OBJ
    if (currentHelpViewer()) {
        currentHelpViewer()->setFocus();
    } else {
        int idx = 0;
        if (onlyHelpViewer)
            idx = lastTabPage;
        tabWidget->setCurrentIndex(idx);
        tabWidget->currentWidget()->setFocus();
    }
}

void Documentacion::setTabTitle(const QUrl &url)
{
    TRACE_OBJ
    Q_UNUSED(url)
#if !defined(QT_NO_WEBKIT)
    QTabBar *tabBar = tabWidget->findChild<QTabBar*>();
    for (int tab = 0; tab < tabBar->count(); ++tab) {
        HelpViewer *viewer = qobject_cast<HelpViewer*>(tabWidget->widget(tab));
        if (viewer) {
            tabWidget->setTabText(tab,
                quoteTabTitle(viewer->documentTitle().trimmed()));
        }
    }
#else
    HelpViewer *viewer = currentHelpViewer();
    if (viewer) {
        tabWidget->setTabText(lastTabPage,
            quoteTabTitle(viewer->documentTitle().trimmed()));
    }
#endif
}

void Documentacion::currentPageChanged(int index)
{
    TRACE_OBJ
    const HelpViewer *viewer = currentHelpViewer();
    if (viewer)
        lastTabPage = index;

    QWidget *widget = tabWidget->cornerWidget(Qt::TopRightCorner);
    widget->setEnabled(viewer && enableTabCloseAction());

    widget = tabWidget->cornerWidget(Qt::TopLeftCorner);
    widget->setEnabled(viewer ? true : false);

    emit currentViewerChanged();
}

void Documentacion::showTabBarContextMenu(const QPoint &point)
{
    TRACE_OBJ
    HelpViewer *viewer = helpViewerFromTabPosition(tabWidget, point);
    if (!viewer)
        return;

    QTabBar *tabBar = tabWidget->findChild<QTabBar*>();

    QMenu menu(QLatin1String(""), tabBar);
    QAction *newPage = menu.addAction(tr("Add New Page"));

    bool enableAction = enableTabCloseAction();
    QAction *closePage = menu.addAction(tr("Close This Page"));
    closePage->setEnabled(enableAction);

    QAction *closePages = menu.addAction(tr("Close Other Pages"));
    closePages->setEnabled(enableAction);

    menu.addSeparator();

    QAction *newBookmark = menu.addAction(tr("Add Bookmark for this Page..."));
    const QString &url = viewer->source().toString();
    if (url.isEmpty() || url == QLatin1String("about:blank"))
        newBookmark->setEnabled(false);

    QAction *pickedAction = menu.exec(tabBar->mapToGlobal(point));
    if (pickedAction == newPage)
        setSourceInNewTab(viewer->source());

    if (pickedAction == closePage) {
        tabWidget->removeTab(tabWidget->indexOf(viewer));
        QTimer::singleShot(0, viewer, SLOT(deleteLater()));
    }

    if (pickedAction == closePages) {
        int currentPage = tabWidget->indexOf(viewer);
        for (int i = tabBar->count() -1; i >= 0; --i) {
            viewer = qobject_cast<HelpViewer*>(tabWidget->widget(i));
            if (i != currentPage && viewer) {
                tabWidget->removeTab(i);
                QTimer::singleShot(0, viewer, SLOT(deleteLater()));

                if (i < currentPage)
                    --currentPage;
            }
        }
    }

    if (pickedAction == newBookmark)
        emit addBookmark(viewer->documentTitle(), viewer->source().toString());
}

bool Documentacion::eventFilter(QObject *object, QEvent *e)
{
    TRACE_OBJ
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
        switch (ke->key()) {
            default: {
                return QWidget::eventFilter(object, e);
            }   break;

            case Qt::Key_Backspace: {
                HelpViewer *viewer = currentHelpViewer();
                if (viewer == object) {
#if defined(QT_NO_WEBKIT)
                    if (viewer->isBackwardAvailable()) {
#else
                    if (viewer->isBackwardAvailable() && !viewer->hasFocus()) {
#endif
                        viewer->backward();
                        return true;
                    }
                }
            }   break;
        }
    }

    if (qobject_cast<QTabBar*>(object)) {
        const bool dblClick = e->type() == QEvent::MouseButtonDblClick;
        if ((e->type() == QEvent::MouseButtonRelease) || dblClick) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
            HelpViewer *viewer = helpViewerFromTabPosition(tabWidget,
                mouseEvent->pos());
            if (viewer) {
                if ((mouseEvent->button() == Qt::MidButton) || dblClick) {
                    if (availableHelpViewer() > 1) {
                        tabWidget->removeTab(tabWidget->indexOf(viewer));
                        QTimer::singleShot(0, viewer, SLOT(deleteLater()));
                        currentPageChanged(tabWidget->currentIndex());
                        return true;
                    }
                }
            }
        }
    }

    return QWidget::eventFilter(object, e);
}

void Documentacion::keyPressEvent(QKeyEvent *e)
{
    TRACE_OBJ
    const QString &text = e->text();
    if (text.startsWith(QLatin1Char('/'))) {
        if (!findWidget->isVisible()) {
            findWidget->showAndClear();
        } else {
            findWidget->show();
        }
    } else {
        QWidget::keyPressEvent(e);
    }
}

void Documentacion::findNext()
{
    TRACE_OBJ
    find(findWidget->text(), true);
}

void Documentacion::findPrevious()
{
    TRACE_OBJ
    find(findWidget->text(), false);
}

void Documentacion::find(const QString &ttf, bool forward)
{
    TRACE_OBJ
    bool found = false;
#if defined(QT_NO_WEBKIT)
    found = findInTextBrowser(ttf, forward);
#else
    found = findInWebPage(ttf, forward);
#endif

    if (!found && ttf.isEmpty())
        found = true;   // the line edit is empty, no need to mark it red...

    if (!findWidget->isVisible())
        findWidget->show();
    findWidget->setPalette(found);
}

bool Documentacion::findInWebPage(const QString &ttf, bool forward)
{
    TRACE_OBJ
#if !defined(QT_NO_WEBKIT)
    if (HelpViewer *viewer = currentHelpViewer()) {
        bool found = false;
        QWebPage::FindFlags options;
        if (!ttf.isEmpty()) {
            if (!forward)
                options |= QWebPage::FindBackward;

            if (findWidget->caseSensitive())
                options |= QWebPage::FindCaseSensitively;

            found = viewer->findText(ttf, options);
            findWidget->setTextWrappedVisible(false);

            if (!found) {
                options |= QWebPage::FindWrapsAroundDocument;
                found = viewer->findText(ttf, options);
                if (found)
                    findWidget->setTextWrappedVisible(true);
            }
        }
        // force highlighting of all other matches, also when empty (clear)
        options = QWebPage::HighlightAllOccurrences;
        if (findWidget->caseSensitive())
            options |= QWebPage::FindCaseSensitively;
        viewer->findText(QLatin1String(""), options);
        viewer->findText(ttf, options);
        return found;
    }

    // this needs to stay, case for active search results page
    return findInTextBrowser(ttf, forward);
#else
    Q_UNUSED(ttf);
    Q_UNUSED(forward);
#endif
    return false;
}

bool Documentacion::findInTextBrowser(const QString &ttf, bool forward)
{
    TRACE_OBJ
    QTextBrowser *browser = qobject_cast<QTextBrowser*>(currentHelpViewer());
    if (tabWidget->currentWidget() == m_searchWidget)
        browser = m_searchWidget->findChild<QTextBrowser*>();

    if (!browser || ttf.isEmpty())
        return false;

    QTextDocument *doc = browser->document();
    QTextCursor cursor = browser->textCursor();

    if (!doc || cursor.isNull())
        return false;

    QTextDocument::FindFlags options;

    if (cursor.hasSelection()) {
        cursor.setPosition(forward ? cursor.position() : cursor.anchor(),
            QTextCursor::MoveAnchor);
    }

    if (!forward)
        options |= QTextDocument::FindBackward;

    if (findWidget->caseSensitive())
        options |= QTextDocument::FindCaseSensitively;

    findWidget->setTextWrappedVisible(false);

    bool found = true;
    QTextCursor newCursor = doc->find(ttf, cursor, options);
    if (newCursor.isNull()) {
        QTextCursor ac(doc);
        ac.movePosition(options & QTextDocument::FindBackward
            ? QTextCursor::End : QTextCursor::Start);
        newCursor = doc->find(ttf, ac, options);
        if (newCursor.isNull()) {
            found = false;
            newCursor = cursor;
        } else {
            findWidget->setTextWrappedVisible(true);
        }
    }
    browser->setTextCursor(newCursor);
    return found;
}

void Documentacion::updateBrowserFont()
{
    TRACE_OBJ
//    const bool searchAttached = searchWidgetAttached();
//    if (searchAttached) {
//        HelpEngineWrapper &helpEngine = HelpEngineWrapper::instance();
//        m_searchWidget->setFont(helpEngine.usesBrowserFont()
//            ? helpEngine.browserFont() : qApp->font());
//    }

    const int count = tabWidget->count();
    if (HelpViewer* viewer = viewerAt(count - 1)) {
        const QFont &font = viewer->viewerFont();
        for (int i = /*searchAttached ? 1 :*/ 0; i < count; ++i)
            viewerAt(i)->setViewerFont(font);
    }
}

//bool Documentacion::searchWidgetAttached() const
//{
//    TRACE_OBJ
//    return m_searchWidget && m_searchWidget->isAttached();
//}

void Documentacion::createSearchWidget(QHelpSearchEngine *searchEngine)
{
    TRACE_OBJ
    if (m_searchWidget)
        return;

    m_searchWidget = new SearchWidget(searchEngine, this);
    connect(m_searchWidget, SIGNAL(requestShowLink(QUrl)), this,
        SLOT(setSourceFromSearch(QUrl)));
    connect(m_searchWidget, SIGNAL(requestShowLinkInNewTab(QUrl)), this,
        SLOT(setSourceFromSearchInNewTab(QUrl)));

    HelpEngineWrapper *helpEngine = HelpEngineWrapper::instance();
    m_searchWidget->setFont(!helpEngine->usesBrowserFont() ? qApp->font()
        : helpEngine->browserFont());
}

SearchWidget *Documentacion::searchWidget(){

    return m_searchWidget;

}

//void Documentacion::activateSearchWidget(bool updateLastTabPage)
//{
//    TRACE_OBJ
//    if (!m_searchWidget)
//        createSearchWidget(HelpEngineWrapper::instance().searchEngine());

//    if (!m_searchWidget->isAttached()) {
//        tabWidget->insertTab(0, m_searchWidget, tr("Search"));
//        m_searchWidget->setAttached(true);

//        if (updateLastTabPage)
//            lastTabPage++;
//    }

//    tabWidget->setCurrentWidget(m_searchWidget);
//    m_searchWidget->setFocus();
//}

//void Documentacion::removeSearchWidget()
//{
//    TRACE_OBJ
//    if (searchWidgetAttached()) {
//        tabWidget->removeTab(0);
//        m_searchWidget->setAttached(false);
//    }
//}

int Documentacion::availableHelpViewer() const
{
    TRACE_OBJ
    int count = tabWidget->count();
//    if (searchWidgetAttached())
//        count--;
    return count;
}

bool Documentacion::enableTabCloseAction() const
{
    TRACE_OBJ
    int minTabCount = 1;

//    if (searchWidgetAttached())
//        minTabCount = 2;

    return (tabWidget->count() > minTabCount);
}

QString Documentacion::quoteTabTitle(const QString &title) const
{
    TRACE_OBJ
    QString s = title;
    return s.replace(QLatin1Char('&'), QLatin1String("&&"));
}

void
Documentacion::setSourceFromSearch(const QUrl &url)
{
    TRACE_OBJ
    setSource(url);
#if defined(QT_NO_WEBKIT)
    highlightSearchTerms();
#else
    connect(currentHelpViewer(), SIGNAL(loadFinished(bool)), this,
        SLOT(highlightSearchTerms()));
#endif
}

void
Documentacion::setSourceFromSearchInNewTab(const QUrl &url)
{
    TRACE_OBJ
    setSourceInNewTab(url);
#if defined(QT_NO_WEBKIT)
    highlightSearchTerms();
#else
    connect(currentHelpViewer(), SIGNAL(loadFinished(bool)), this,
        SLOT(highlightSearchTerms()));
#endif
}

void
Documentacion::highlightSearchTerms()
{
    TRACE_OBJ
    HelpViewer *viewer = currentHelpViewer();
    if (!viewer)
        return;

    QHelpSearchEngine *searchEngine =
        HelpEngineWrapper::instance()->searchEngine();
    QList<QHelpSearchQuery> queryList = searchEngine->query();

    QStringList terms;
    foreach (const QHelpSearchQuery &query, queryList) {
        switch (query.fieldName) {
            default: break;
            case QHelpSearchQuery::ALL: {
            case QHelpSearchQuery::PHRASE:
            case QHelpSearchQuery::DEFAULT:
            case QHelpSearchQuery::ATLEAST:
                foreach (QString term, query.wordList)
                    terms.append(term.remove(QLatin1String("\"")));
            }
        }
    }

#if defined(QT_NO_WEBKIT)
    viewer->viewport()->setUpdatesEnabled(false);

    QTextCharFormat marker;
    marker.setForeground(Qt::red);

    QTextCursor firstHit;

    QTextCursor c = viewer->textCursor();
    c.beginEditBlock();
    foreach (const QString& term, terms) {
        c.movePosition(QTextCursor::Start);
        viewer->setTextCursor(c);

        while (viewer->find(term, QTextDocument::FindWholeWords)) {
            QTextCursor hit = viewer->textCursor();
            if (firstHit.isNull() || hit.position() < firstHit.position())
                firstHit = hit;

            hit.mergeCharFormat(marker);
        }
    }

    if (firstHit.isNull()) {
        firstHit = viewer->textCursor();
        firstHit.movePosition(QTextCursor::Start);
    }
    firstHit.clearSelection();
    c.endEditBlock();
    viewer->setTextCursor(firstHit);

    viewer->viewport()->setUpdatesEnabled(true);
#else
    viewer->findText("", QWebPage::HighlightAllOccurrences);
         // clears existing selections
    foreach (const QString& term, terms)
        viewer->findText(term, QWebPage::HighlightAllOccurrences);

    disconnect(viewer, SIGNAL(loadFinished(bool)), this,
        SLOT(highlightSearchTerms()));
#endif
}


void Documentacion::closeOrReloadTabs(const QList<int> &indices, bool tryReload)
{
    TRACE_OBJ
    QList<int> sortedIndices = indices;
    qSort(sortedIndices);
    for (int i = sortedIndices.count(); --i >= 0;) {
        const int tab = sortedIndices.at(i);
        bool close = true;
        if (tryReload) {
            HelpViewer *viewer =
                    qobject_cast<HelpViewer*>(tabWidget->widget(tab));
            if (HelpEngineWrapper::instance()->findFile(viewer->source()).isValid()) {
                viewer->reload();
                close = false;
            }
        }
        if (close)
            closeTabAt(tab);
    }
    if (availableHelpViewer() == 0)
        setSource(QUrl(QLatin1String("about:blank")));
}

void Documentacion::closeTabAt(int index)
{
    TRACE_OBJ
    HelpViewer *viewer = qobject_cast<HelpViewer*>(tabWidget->widget(index));
    tabWidget->removeTab(index);
    QTimer::singleShot(0, viewer, SLOT(deleteLater()));
}

QMap<int, QString> Documentacion::currentSourceFileList() const
{
    TRACE_OBJ
    QMap<int, QString> sourceList;
    for (int i = 0; i < tabWidget->count(); ++i) {
        HelpViewer *viewer = qobject_cast<HelpViewer*>(tabWidget->widget(i));
        if (viewer && viewer->source().isValid())
            sourceList.insert(i, viewer->source().host());
    }
    return sourceList;
}

QT_END_NAMESPACE
