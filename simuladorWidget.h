#ifndef SIMULADORWIDGET_H
#define SIMULADORWIDGET_H

#include <QWidget>

#include "diagramitem.h"

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QToolBar;
class QMenuBar;
QT_END_NAMESPACE

//! [0]
class SimuladorWidget : public QWidget
{
    Q_OBJECT

public:
   SimuladorWidget(QWidget *parent);
//   ~SimuladorWidget();

    QToolBox *toolBox;

    QMenuBar *simuladorMenuBar;

    QToolBar *simuladorToolBar;

    void createMenus();

private slots:
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void sceneScaleChanged(const QString &scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem *item);

    void guardarSimulador();
    void abrirSimulador();

private:
    void createToolBox();
    void createActions();
    void createToolbars();

    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              DiagramItem::DiagramType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);

    DiagramScene *scene;
    QGraphicsView *view;

    QMenu *menu_Archivo;
    QMenu *menu_Items;
    QMenu *menu_Ayuda;

    QAction *addAction;
    QAction *deleteAction;

    QAction *abrirAction;
    QAction *guardarAction;
    QAction *action_Sobre;
    QAction *action_Sobre_Qt;

    QAction *toFrontAction;
    QAction *sendBackAction;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;
};

#endif // SIMULADORWIDGET_H
