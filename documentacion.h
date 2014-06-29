#ifndef DOCUMENTACION_H
#define DOCUMENTACION_H

#include <QtCore/QUrl>
#include <QWidget>

QT_BEGIN_NAMESPACE

class FindWidget;
class HelpViewer;
class MainWindow;
class QHelpSearchEngine;
class QTabWidget;
class SearchWidget;
class QPrinter;

class Documentacion : public QWidget
{
    Q_OBJECT

public:
    Documentacion(QWidget *parent);
    ~Documentacion();

    void setupWidget();
    bool hasSelection() const;
    QUrl currentSource() const;
    QString currentTitle() const;
    bool isHomeAvailable() const;
    bool isForwardAvailable() const;
    bool isBackwardAvailable() const;
    QList<QAction*> globalActions() const;
    void setGlobalActions(const QList<QAction*> &actions);

    HelpViewer *viewerAt(int index) const;
    HelpViewer *currentHelpViewer() const;

//    bool searchWidgetAttached() const;
    void createSearchWidget(QHelpSearchEngine *searchEngine);
    SearchWidget *searchWidget();
//    void activateSearchWidget(bool updateLastTabPage = false);
//    void removeSearchWidget();

    int availableHelpViewer() const;
    bool enableTabCloseAction() const;

    void closeOrReloadTabs(const QList<int> &indices, bool tryReload);
    void closeTabAt(int index);
    QMap<int, QString> currentSourceFileList() const;

    static Documentacion *instance();

public slots:
    void zoomIn();
    void zoomOut();
    void nextPage();
    void resetZoom();
    void previousPage();
    void copySelection();
    void showTextSearch();
    void print();
    void pageSetup();
    void printPreview();
    void updateBrowserFont();
    void setSource(const QUrl &url);
    void setSourceInNewTab(const QUrl &url, qreal zoom = 0.0);
    HelpViewer *newEmptyTab();
    void home();
    void forward();
    void backward();

    void activateTab(bool onlyHelpViewer = false);

    void findNext();
    void findPrevious();
    void find(const QString &text, bool forward);

signals:
    void currentViewerChanged();
    void copyAvailable(bool yes);
    void sourceChanged(const QUrl &url);
    void highlighted(const QString &link);
    void forwardAvailable(bool available);
    void backwardAvailable(bool available);
    void addBookmark(const QString &title, const QString &url);

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void newTab();
    void closeTab();
    void setTabTitle(const QUrl& url);
    void currentPageChanged(int index);
    void showTabBarContextMenu(const QPoint &point);
    void printPreview(QPrinter *printer);
    void setSourceFromSearch(const QUrl &url);
    void setSourceFromSearchInNewTab(const QUrl &url);
    void highlightSearchTerms();

private:
    void connectSignals();
    bool eventFilter(QObject *object, QEvent *e);
    bool findInWebPage(const QString &ttf, bool forward);
    bool findInTextBrowser(const QString &ttf, bool forward);
    void initPrinter();
    QString quoteTabTitle(const QString &title) const;
    void setLastShownPages();

private:
    int lastTabPage;
    QList<QAction*> globalActionList;

    QTabWidget *tabWidget;
    FindWidget *findWidget;
    QPrinter *printer;
    bool usesDefaultCollection;

    SearchWidget *m_searchWidget;
};

QT_END_NAMESPACE

#endif // DOCUMENTACION_H
