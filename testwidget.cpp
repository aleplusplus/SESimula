#include "testwidget.h"
#include "ui_testwidget.h"

#include "test.h"

#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QItemEditorFactory>
#include <QItemEditorCreatorBase>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QTableWidget>
#include <QDataStream>
#include <QFileDialog>

DoubleSpinBox::DoubleSpinBox(QWidget *widget):QDoubleSpinBox(widget){
    setDecimals(8);
    setMinimum(-999999);
    setMaximum(999999);
}

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);

    QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *doubleSpinBoxCreator =
        new QStandardItemEditorCreator<DoubleSpinBox>();

    factory->registerEditor(QVariant::Double, doubleSpinBoxCreator);

    QItemEditorFactory::setDefaultFactory(factory);

    setupAction();

    createMenus();

    setupToolBar();
}

void TestWidget::importar(QTableWidget *tabla){
    int n = tabla->rowCount();
    ui->spinBox->setValue(n);
    for (int i = 0; i < n; ++i) {
        itemTable = new QTableWidgetItem;
        itemTable->setData(Qt::DisplayRole, tabla->item(i,0)->data(Qt::DisplayRole).toDouble());
        ui->tableWidget->setItem(i,0,itemTable);
    }
}

void TestWidget::setupAction(){

    guardarAction = new QAction(QIcon(":/imagenes/interfaz/Save.png"),tr("Guardar"),this);
    guardarAction->setShortcut(tr("Ctrl+G"));
    guardarAction->setStatusTip(tr("Guardar el numeros de la prueba"));
    connect(guardarAction, SIGNAL(triggered()),
            this, SLOT(saveTest()));

    abrirAction = new QAction(QIcon(":/imagenes/interfaz/download.png"),tr("Abrir"),this);
    abrirAction->setShortcut(tr("Ctrl+O"));
    abrirAction->setStatusTip(tr("Abrir los números hacia las pruebas"));
    connect(abrirAction, SIGNAL(triggered()),
            this, SLOT(abrirTest()));

    action_Sobre = new QAction(QIcon(":/SESimula.ico"),tr("&Acerca de &SESimula"),this);
    action_Sobre_Qt = new QAction(QIcon(":/interfaz/assistant.png"),tr("&Acerca de &Qt"),this);

}

void TestWidget::saveTest(){

    int n = ui->tableWidget->rowCount();
    if(n)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar"), QString(),
                                                        tr("SESimula archivo (*.ses)"));

        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("SESimula"),
                                 tr("No se puede escribir el archivo %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        }

        QDataStream out(&file);
        double aux;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        out << ui->tableWidget->rowCount();
        for(int i=0;i<n;i++)
        {
            aux = ui->tableWidget->item(i,0)->data(Qt::DisplayRole).toDouble();
            out << aux;
        }
        QApplication::restoreOverrideCursor();
    }
    else
        QMessageBox::warning(this, tr("SESimula"),
                             tr("No se puede guardar el archivo."));
}

void TestWidget::abrirTest(){
    QString fn = QFileDialog::getOpenFileName(this, tr("Abrir archivo ..."),
                                              QString(), tr("SESimula archivo (*.ses)"));
    if (!fn.isEmpty())
        load(fn);
}

void TestWidget::load(const QString &f)
{
    if (!QFile::exists(f))
        QMessageBox::warning(this, tr("SESimula"),
                             tr("No se puede abrir el archivo."));
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        QMessageBox::warning(this, tr("SESimula"),
                             tr("No se puede abrir el archivo."));

    QDataStream in(&file);
    int n;
    double aux;
    in >> n;
    ui->spinBox->setValue(n);
    for (int i = 0; i < n; ++i) {
        in >> aux;
        itemTable = new QTableWidgetItem;
        itemTable->setData(Qt::DisplayRole, aux);
        ui->tableWidget->setItem(i,0,itemTable);
    }

}

void TestWidget::createMenus(){
    testMenuBar = new QMenuBar;

    menu_Archivo = new QMenu(tr("&Archivo"),testMenuBar);
    menu_Archivo->addAction(abrirAction);
    menu_Archivo->addAction(guardarAction);

    QAction *tmp = menu_Archivo->addAction(QIcon::fromTheme("application-exit"),
                                   tr("&Salir"), this, SLOT(close()));
    tmp->setMenuRole(QAction::QuitRole);
    tmp->setShortcut(QKeySequence(tr("CTRL+Q")));
    menu_Archivo->addAction(tmp);

    menu_Ayuda = new QMenu(tr("&Ayuda"));
    menu_Ayuda->addAction(action_Sobre);
    menu_Ayuda->addAction(action_Sobre_Qt);

    testMenuBar->addAction(menu_Archivo->menuAction());
    testMenuBar->addAction(menu_Ayuda->menuAction());

}

void TestWidget::setupToolBar(){

    testToolBar = new QToolBar(tr("Test"));

    testToolBar->addAction(abrirAction);
    testToolBar->addSeparator();
    testToolBar->addAction(guardarAction);
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::on_spinBox_valueChanged(int arg1)
{

    ui->tableWidget->setRowCount(arg1);

    double aux = 0;
    itemTable = new QTableWidgetItem;
    itemTable->setData(Qt::DisplayRole, aux);

    ui->tableWidget->setItem(arg1-1, 0, itemTable);

    ui->tableWidget->selectRow(arg1 - 1);
}

void TestWidget::on_pushButton_clicked()
{
    Test *test= new Test(ui->tableWidget) ;

    if(!ui->toolBox->currentIndex()){
        if(!ui->toolBox_2->currentIndex()){
            if(test->kolomogorov_smirnox(ui->comboBox->currentIndex()))
                ui->label_6->setText("Uniforme");
            else
                ui->label_6->setText("No Uniforme");
        }
        else{
            if(test->chi_cuadrado(ui->comboBox->currentIndex(),ui->spinBox->value()))
                ui->label_6->setText("Uniforme");
            else
                ui->label_6->setText("No Uniforme");
        }
    }
    else{
        switch (ui->toolBox_3->currentIndex()) {
        case 0:
            if(test->corridas_ascendetes_descendentes(ui->doubleSpinBox->value()))
                ui->label_6->setText("Independiente");
            else
                ui->label_6->setText("No Independiente");
            break;
        case 1:
            if(test->corridas_encima_debajo(ui->doubleSpinBox_2->value()))
                ui->label_6->setText("Independiente");
            else
                ui->label_6->setText("No Independiente");
            break;
        case 2:
            if(test->longitud_corridas(ui->comboBox_3->currentIndex(),ui->comboBox->currentIndex()))
                ui->label_6->setText("Independiente");
            else
                ui->label_6->setText("No Independiente");
            break;
        default:
            if(test->autocorrelacion(ui->spinBox_3->value(),ui->spinBox_4->value(),ui->doubleSpinBox_6->value()))
                ui->label_6->setText("Independiente");
            else
                ui->label_6->setText("No Independiente");
            break;
        }
    }
}
