#include <QtGui>
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QToolButton>
#include <QFontComboBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QToolBox>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include <QTableWidget>
#include <QTableWidgetItem>

#include "simuladorWidget.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "arrow.h"
#include "mainwindow.h"



const int InsertTextButton = 10;

//! [0]
SimuladorWidget::SimuladorWidget(QWidget *parent):QWidget(parent)
{
    createActions();
    createToolBox();
    createMenus();

    scene = new DiagramScene(menu_Items, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
        this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
        this, SLOT(itemSelected(QGraphicsItem*)));

    createToolbars();

    QVBoxLayout *layout = new QVBoxLayout;
    view = new QGraphicsView(scene);
    layout->addWidget(view);
    setLayout(layout);

}
//! [0]

//! [1]
void SimuladorWidget::backgroundButtonGroupClicked(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            myButton->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Grid Azul"))
        scene->setBackgroundBrush(QPixmap(":/imagenes/interfaz/background1.png"));
    else if (text == tr("Grid Blanco"))
        scene->setBackgroundBrush(QPixmap(":/imagenes/interfaz/background2.png"));
    else if (text == tr("Grid Gris"))
        scene->setBackgroundBrush(QPixmap(":/imagenes/interfaz/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/imagenes/interfaz/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void SimuladorWidget::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
    if (buttonGroup->button(id) != button)
        button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]

//! [3]
void SimuladorWidget::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
         if (item->type() == DiagramItem::Type) {
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
         }
         scene->removeItem(item);
         delete item;
     }
}
//! [3]

//! [4]
void SimuladorWidget::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void SimuladorWidget::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue &&
            item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void SimuladorWidget::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue &&
            item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void SimuladorWidget::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
//! [7]

//! [8]
void SimuladorWidget::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void SimuladorWidget::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void SimuladorWidget::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void SimuladorWidget::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void SimuladorWidget::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                ":/imagenes/interfaz/textpointer.png",
                qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void SimuladorWidget::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                 ":/imagenes/interfaz/floodfill.png",
                 qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void SimuladorWidget::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                 ":/imagenes/interfaz/linecolor.png",
                 qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void SimuladorWidget::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void SimuladorWidget::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void SimuladorWidget::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void SimuladorWidget::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void SimuladorWidget::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
//    QColor color = textItem->defaultTextColor();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void SimuladorWidget::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Condicional"),
                               DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Proceso"),
                      DiagramItem::Step),0, 1);
    layout->addWidget(createCellWidget(tr("Entrada/Salida"),
                      DiagramItem::Io), 1, 0);
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(":/imagenes/interfaz/textpointer.png"));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Texto")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);

    layout->setRowStretch(2, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Grid Azul"),
                ":/imagenes/interfaz/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Grid Blanco"),
                ":/imagenes/interfaz/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Grid Gris"),
                    ":/imagenes/interfaz/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                ":/imagenes/interfaz/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


//! [22]
    toolBox = new QToolBox;
    toolBox->addItem(itemWidget, tr("Componentes"));
    toolBox->addItem(backgroundWidget, tr("Fondos"));
}
//! [22]

//! [23]
void SimuladorWidget::createActions()
{
    toFrontAction = new QAction(QIcon(":/imagenes/interfaz/bringtofront.png"),
                                tr("Enviar al &Frente"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Enviar el item al frente"));
    connect(toFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/imagenes/interfaz/sendtoback.png"),
                                 tr("Enviar a &Fondo"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Enviar el item al Fondo"));
    connect(sendBackAction, SIGNAL(triggered()),
        this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/imagenes/interfaz/delete.png"),
                               tr("&Eliminar"), this);
    deleteAction->setShortcut(tr("Eliminar"));
    deleteAction->setStatusTip(tr("Eliminar item del diagrama"));
    connect(deleteAction, SIGNAL(triggered()),
        this, SLOT(deleteItem()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/imagenes/interfaz/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/imagenes/interfaz/italic.png"),
                               tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/imagenes/interfaz/underline.png"),
                                  tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));
    guardarAction = new QAction(QIcon(":/imagenes/interfaz/Save.png"),tr("Guardar"),this);
    guardarAction->setShortcut(tr("Ctrl+G"));
    guardarAction->setStatusTip(tr("Guardar el diseño del lenguaje"));
    connect(guardarAction, SIGNAL(triggered()),
            this, SLOT(guardarSimulador()));

    abrirAction = new QAction(QIcon(":/imagenes/interfaz/download.png"),tr("&Abrir"),this);
    abrirAction->setShortcut(tr("Ctrl+O"));
    abrirAction->setStatusTip(tr("Abrir el diseño del lenguaje"));
    connect(abrirAction, SIGNAL(triggered()),
            this, SLOT(abrirSimulador()));

    action_Sobre = new QAction(QIcon(":/SESimula.ico"),tr("&Acerca de &SESimula"),this);
    action_Sobre_Qt = new QAction(QIcon(":/interfaz/assistant.png"),tr("&Acerca de &Qt"),this);
}

void SimuladorWidget::abrirSimulador(){

}

void SimuladorWidget::guardarSimulador(){

}

//! [24]
void SimuladorWidget::createMenus()
{
    simuladorMenuBar = new QMenuBar;

    menu_Archivo = new QMenu(tr("&Archivo"),simuladorMenuBar);
    menu_Archivo->addAction(abrirAction);
    menu_Archivo->addAction(guardarAction);

    menu_Items = new QMenu(tr("&Items"), simuladorMenuBar);
    menu_Items->addAction(deleteAction);
    menu_Items->addSeparator();
    menu_Items->addAction(toFrontAction);
    menu_Items->addAction(sendBackAction);

    QAction *tmp = menu_Archivo->addAction(QIcon::fromTheme("application-exit"),
                                   tr("&Salir"), this, SLOT(close()));
    tmp->setMenuRole(QAction::QuitRole);
    tmp->setShortcut(QKeySequence(tr("CTRL+Q")));
    menu_Archivo->addAction(tmp);

    menu_Ayuda = new QMenu(tr("&Ayuda"), simuladorMenuBar);
    menu_Ayuda->addAction(action_Sobre);
    menu_Ayuda->addAction(action_Sobre_Qt);

    simuladorMenuBar->addAction(menu_Archivo->menuAction());
    simuladorMenuBar->addAction(menu_Items->menuAction());
    simuladorMenuBar->addAction(menu_Ayuda->menuAction());

}
//! [24]

//! [25]
void SimuladorWidget::createToolbars()
{
    simuladorToolBar = new QToolBar(tr("Simulador"));

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/imagenes/interfaz/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/imagenes/interfaz/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton,
                                int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    simuladorToolBar->addWidget(pointerButton);
    simuladorToolBar->addWidget(linePointerButton);
    simuladorToolBar->addWidget(sceneScaleCombo);

    simuladorToolBar->addSeparator();

    simuladorToolBar->addAction(deleteAction);
    simuladorToolBar->addAction(toFrontAction);
    simuladorToolBar->addAction(sendBackAction);

    simuladorToolBar->addSeparator();

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
                                                 Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(
    ":/imagenes/interfaz/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()),
                         Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
    ":/imagenes/interfaz/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()),
                                 Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
        ":/imagenes/interfaz/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    simuladorToolBar->addWidget(fontColorToolButton);
    simuladorToolBar->addWidget(fillColorToolButton);
    simuladorToolBar->addWidget(lineColorToolButton);

    simuladorToolBar->addSeparator();

    simuladorToolBar->addWidget(fontCombo);
    simuladorToolBar->addWidget(fontSizeCombo);
    simuladorToolBar->addAction(boldAction);
    simuladorToolBar->addAction(italicAction);
    simuladorToolBar->addAction(underlineAction);



}
//! [27]

//! [28]
QWidget *SimuladorWidget::createBackgroundCellWidget(const QString &text,
                        const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *SimuladorWidget::createCellWidget(const QString &text,
                      DiagramItem::DiagramType type)
{

    DiagramItem item(type, menu_Items);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [29]

//! [30]
QMenu *SimuladorWidget::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("negro") << tr("blanco") << tr("rojo") << tr("azul")
          << tr("amarillo");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon SimuladorWidget::createColorToolButtonIcon(const QString &imageFile,
                        QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon SimuladorWidget::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
