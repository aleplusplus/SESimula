#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QToolBar;
class QMenuBar;
class QMenu;
//class QTableWidget;
class QTableWidgetItem;
class QSpinBox;
QT_END_NAMESPACE

//#include "test.h"

namespace Ui {
class TestWidget;
}

class DoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    DoubleSpinBox(QWidget *widget = 0);
};


class TestWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();

    void importar(QTableWidget *tabla);

    QToolBar *testToolBar;

    QMenuBar *testMenuBar;

    void createMenus();
    
private slots:
    void on_spinBox_valueChanged(int arg1);

    void saveTest();
    void abrirTest();

    void on_pushButton_clicked();

private:
    Ui::TestWidget *ui;

    QTableWidgetItem *itemTable;
    QSpinBox *itemSpinBox;

    QAction *guardarAction;
    QAction *abrirAction;
    QAction *action_Sobre;
    QAction *action_Sobre_Qt;

    QMenu *menu_Archivo;
    QMenu *menu_Ayuda;

    void setupAction();
    void setupToolBar();
    void load(const QString &f);
};

#endif // TESTWIDGET_H
