#include "aleatoriedadwidget.h"
#include "ui_aleatoriedadwidget.h"

#include "generador.h"

#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTableWidget>
#include <QDataStream>
#include <QFileDialog>

AleatoriedadWidget::AleatoriedadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AleatoriedadWidget)
{
    ui->setupUi(this);
    esconder(2);

    setupAction();

    createMenus();

    setupToolBar();
}

void AleatoriedadWidget::setupAction(){

    guardarAction = new QAction(QIcon(":/imagenes/interfaz/Save.png"),tr("Guardar"),this);
    guardarAction->setShortcut(tr("Ctrl+G"));
    guardarAction->setStatusTip(tr("Guardar el numeros generados"));
    connect(guardarAction, SIGNAL(triggered()),
            this, SLOT(saveGenerador()));

    exportarAction = new QAction(QIcon(":/imagenes/interfaz/upload.png"),tr("Exportar"),this);
    exportarAction->setShortcut(tr("Ctrl+E"));
    exportarAction->setStatusTip(tr("Exportar los números generados a las pruebas de aleatoridad"));
    connect(exportarAction, SIGNAL(triggered()),
            this, SIGNAL(exportarGenerador()));

    action_Sobre = new QAction(QIcon(":/SESimula.ico"),tr("&Acerca de &SESimula"),this);
    action_Sobre_Qt = new QAction(QIcon(":/interfaz/assistant.png"),tr("&Acerca de &Qt"),this);

}

void AleatoriedadWidget::saveGenerador(){
    int n = ui->tableWidget_2->rowCount();
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
        QApplication::setOverrideCursor(Qt::WaitCursor);
        out << ui->tableWidget_2->rowCount();
        for(int i=0;i<n;i++)
            out << ui->tableWidget_2->item(i,0)->data(Qt::DisplayRole).toDouble();
        QApplication::restoreOverrideCursor();
    }
    else
        QMessageBox::warning(this, tr("SESimula"),
                             tr("No se puede guardar el archivo."));
}

void AleatoriedadWidget::createMenus(){

    aleatoriedadMenuBar = new QMenuBar;
    menu_Archivo = new QMenu(tr("&Archivo"),aleatoriedadMenuBar);
    menu_Archivo->addAction(guardarAction);
    menu_Archivo->addAction(exportarAction);
    menu_Archivo->addSeparator();
    QAction *tmp = menu_Archivo->addAction(QIcon::fromTheme("application-exit"),
                                   tr("&Salir"), this, SLOT(close()));
    tmp->setMenuRole(QAction::QuitRole);
    tmp->setShortcut(QKeySequence(tr("CTRL+Q")));
    menu_Archivo->addAction(tmp);

    menu_Ayuda = new QMenu(tr("&Ayuda"));
    menu_Ayuda->addAction(action_Sobre);
    menu_Ayuda->addAction(action_Sobre_Qt);

    aleatoriedadMenuBar->addAction(menu_Archivo->menuAction());
    aleatoriedadMenuBar->addAction(menu_Ayuda->menuAction());


}

void AleatoriedadWidget::setupToolBar(){

    aleatoriedadToolBar = new QToolBar(tr("Aleatoriedad"));

    aleatoriedadToolBar->addAction(guardarAction);
    aleatoriedadToolBar->addSeparator();
    aleatoriedadToolBar->addAction(exportarAction);
}

AleatoriedadWidget::~AleatoriedadWidget()
{
    delete ui;
}

void AleatoriedadWidget::on_spinBox_2_valueChanged(int arg1)
{
    ui->tableWidget_2->setRowCount(arg1);
    generacion();
    ui->tableWidget_2->selectRow(arg1 - 1);
}

void AleatoriedadWidget::on_comboBox_2_1_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->widget_2_1->hide();
        break;
    case 1:
        ui->widget_2_1->show();
        ui->spinBox_2_1->setEnabled(true);
        ui->spinBox_2_3->setEnabled(true);
        ui->label_2_1->setText("A =");
        ui->label_2_2->setText("Semilla =");
        ui->label_2_2->show();
        ui->label_2_3->show();
        ui->label_2_4->show();
        ui->spinBox_2_2->show();
        ui->spinBox_2_3->show();
        ui->spinBox_2_4->show();
        break;
    case 2:
        ui->widget_2_1->show();
        ui->spinBox_2_1->setEnabled(true);
        ui->spinBox_2_3->setEnabled(false);
        ui->spinBox_2_3->setValue(0);
        ui->label_2_1->setText("A =");
        ui->label_2_2->setText("Semilla =");
        ui->label_2_2->show();
        ui->label_2_3->show();
        ui->label_2_4->show();
        ui->spinBox_2_2->show();
        ui->spinBox_2_3->show();
        ui->spinBox_2_4->show();
        break;
    case 3:
        ui->widget_2_1->show();
        ui->spinBox_2_1->setEnabled(false);
        ui->spinBox_2_1->setValue(0);
        ui->spinBox_2_3->setEnabled(true);
        ui->label_2_1->setText("A =");
        ui->label_2_2->setText("Semilla =");
        ui->label_2_2->show();
        ui->label_2_3->show();
        ui->label_2_4->show();
        ui->spinBox_2_2->show();
        ui->spinBox_2_3->show();
        ui->spinBox_2_4->show();
        break;
    case 4:
        ui->widget_2_1->show();
        ui->label_2_1->setText("Semilla =");
        ui->spinBox_2_1->setEnabled(true);
        ui->label_2_2->hide();
        ui->label_2_3->hide();
        ui->label_2_4->hide();
        ui->spinBox_2_3->hide();
        ui->spinBox_2_3->hide();
        ui->spinBox_2_2->hide();
        break;
    default:
        ui->widget_2_1->show();
        ui->label_2_1->setText("Semilla 1 =");
        ui->spinBox_2_1->setEnabled(true);
        ui->label_2_2->setText("Semilla 2 =");
        ui->label_2_2->show();
        ui->label_2_3->hide();
        ui->label_2_4->hide();
        ui->spinBox_2_2->show();
        ui->spinBox_2_3->hide();
        ui->spinBox_2_4->hide();
        break;
    }
}

void AleatoriedadWidget::on_comboBox_2_2_currentIndexChanged(int index)
{
    if( index != 0 ){
        ui->comboBox_2_3->setCurrentIndex(0);
        ui->doubleSpinBox_2_1->setDecimals(4);
        ui->doubleSpinBox_2_2->setDecimals(4);
        ui->doubleSpinBox_2_3->setDecimals(4);
        ui->doubleSpinBox_2_4->setDecimals(4);
        ui->doubleSpinBox_2_1->setMaximum(999999999);
        ui->doubleSpinBox_2_2->setMaximum(999999999);
        ui->doubleSpinBox_2_3->setMaximum(999999999);
        ui->doubleSpinBox_2_4->setMaximum(999999999);
        ui->widget_2_2->show();
    }

    switch (index){
    case 0: // Ninguna
        if(ui->comboBox_2_3->currentIndex() == 0)
            ui->widget_2_2->hide();
        break;
    case 1: // Uniforme
        distroMaximoMinino();
        break;
    case 2: // Arcoseno
        distroMaximoMinino();
        break;
    case 3: //Beta
        distroMaximoMinino();
        ui->label_2_9->setText("Shape v");
        ui->label_2_10->setText("Shape w");
        ui->doubleSpinBox_2_3->setMinimum(0);
        ui->doubleSpinBox_2_3->setValue(3);
        ui->doubleSpinBox_2_4->setMinimum(0);
        ui->doubleSpinBox_2_4->setValue(3);
        mostrar(2);
        break;
    case 4: // Cauchy (or Lorentz)
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(0.5);
        esconder(2);
        mostrar(1);
        break;
    case 5: // Chi-Cuadrado
        ui->label_2_7->setText("Shape v");
        ui->doubleSpinBox_2_1->setMinimum(1);
        ui->doubleSpinBox_2_1->setValue(1);
        esconder(3);
        break;
    case 6: // Coseno
        distroMaximoMinino();
        break;
    case 7: // Double Log
        distroMaximoMinino();
        break;
    case 8: // Erlang
        ui->label_2_7->setText("Scale b");
        ui->label_2_8->setText("Shape c");
        ui->doubleSpinBox_2_1->setMinimum(0.000000001);
        ui->doubleSpinBox_2_1->setValue(1);
        ui->doubleSpinBox_2_2->setMinimum(0);
        ui->doubleSpinBox_2_2->setValue(2);
        esconder(2);
        mostrar(1);
        break;
    case 9: // Exponencial
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        esconder(2);
        mostrar(1);
        break;
    case 10: // Extreme Value
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        esconder(2);
        mostrar(1);
        break;
    case 11: // F Ratio
        ui->label_2_7->setText("Shape v");
        ui->label_2_8->setText("Shape w");
        ui->doubleSpinBox_2_3->setMinimum(1);
        ui->doubleSpinBox_2_3->setValue(4);
        ui->doubleSpinBox_2_4->setMinimum(1);
        ui->doubleSpinBox_2_4->setValue(4);
        break;
    case 12: // Gamma
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->label_2_9->setText("Shape c");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        ui->doubleSpinBox_2_3->setMinimum(0.000000001);
        ui->doubleSpinBox_2_3->setValue(2);
        esconder(1);
        mostrar(2);
        break;
    case 13: // Laplace
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        esconder(2);
        mostrar(1);
        break;
    case 14: // Logarithmic
        distroMaximoMinino();
        break;
    case 15: // Logistic
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        esconder(2);
        mostrar(1);
        break;
    case 16: // Lognormal
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Shepe mu");
        ui->label_2_9->setText("Scale sigma");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(-999999999);
        ui->doubleSpinBox_2_2->setValue(0);
        ui->doubleSpinBox_2_3->setMinimum(0.000000001);
        ui->doubleSpinBox_2_3->setValue(1);
        esconder(1);
        mostrar(2);
        break;
    case 17: // Normal
        ui->label_2_7->setText("Shepe mu");
        ui->label_2_8->setText("Scale sigma");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        break;
    case 18: // Parabolic
        distroMaximoMinino();
        break;
    case 19: // Pareto
        ui->label_2_7->setText("Shape c");
        ui->doubleSpinBox_2_1->setMinimum(0.000000001);
        ui->doubleSpinBox_2_1->setValue(1);
        esconder(3);
        break;
    case 20: // Pearson Type 5
        ui->label_2_7->setText("Scale b");
        ui->label_2_8->setText("Shape c");
        ui->doubleSpinBox_2_1->setMinimum(0.000000001);
        ui->doubleSpinBox_2_1->setValue(1);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(2);
        esconder(2);
        mostrar(1);
        break;
    case 21: // Pearson Type 6
        ui->label_2_7->setText("Scale b");
        ui->label_2_8->setText("Shepe v");
        ui->label_2_9->setText("Shepe w");
        ui->doubleSpinBox_2_1->setMinimum(0.000000001);
        ui->doubleSpinBox_2_1->setValue(1);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(2);
        ui->doubleSpinBox_2_3->setMinimum(0.000000001);
        ui->doubleSpinBox_2_3->setValue(2);
        esconder(1);
        mostrar(2);
        break;
    case 22: // Power
        ui->label_2_7->setText("Shape c");
        ui->doubleSpinBox_2_1->setMinimum(0.000000001);
        ui->doubleSpinBox_2_1->setValue(0.5);
        esconder(3);
        break;
    case 23: // Rayleigh
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        esconder(2);
        mostrar(1);
        break;
    case 24: // Students T
        ui->label_2_7->setText("Shape v");
        ui->doubleSpinBox_2_1->setMinimum(1);
        ui->doubleSpinBox_2_1->setValue(1);
        esconder(3);
        break;
    case 25: // Triangular
        distroMaximoMinino();
        ui->label_2_7->setText("Location c");
        ui->doubleSpinBox_2_3->setMinimum(-999999999);
        ui->doubleSpinBox_2_3->setValue(0.5);
        ui->label_2_7->show();
        ui->doubleSpinBox_2_3->show();
        break;
    case 26: // Weibull
        ui->label_2_7->setText("Location a");
        ui->label_2_8->setText("Scale b");
        ui->label_2_9->setText("Shape c");
        ui->doubleSpinBox_2_1->setMinimum(-999999999);
        ui->doubleSpinBox_2_1->setValue(0);
        ui->doubleSpinBox_2_2->setMinimum(0.000000001);
        ui->doubleSpinBox_2_2->setValue(1);
        ui->doubleSpinBox_2_3->setMinimum(0.000000001);
        ui->doubleSpinBox_2_3->setValue(2);
        esconder(1);
        mostrar(2);
        break;
//    default:
//        break;
    }
}

void AleatoriedadWidget::on_comboBox_2_3_currentIndexChanged(int index)
{
    if( index != 0 ){
        ui->comboBox_2_2->setCurrentIndex(0);
        ui->doubleSpinBox_2_1->setDecimals(0);
        ui->doubleSpinBox_2_2->setDecimals(0);
        ui->doubleSpinBox_2_3->setDecimals(0);
        ui->doubleSpinBox_2_4->setDecimals(0);
        ui->doubleSpinBox_2_1->setMaximum(999999999);
        ui->doubleSpinBox_2_2->setMaximum(999999999);
        ui->doubleSpinBox_2_3->setMaximum(999999999);
        ui->doubleSpinBox_2_4->setMaximum(999999999);
        ui->widget_2_2->show();
    }
    switch (index){
    case 0:
        if(ui->comboBox_2_2->currentIndex() == 0)
            ui->widget_2_2->hide();
        break;
    case 1: // Uniforme Discreta
        distroMaximoMinino();
        ui->doubleSpinBox_2_1->setMinimum(0);
        ui->doubleSpinBox_2_2->setMinimum(0);
        ui->doubleSpinBox_2_2->setValue(100);
        break;
    case 2: // Bernoulli Trial
        ui->label_2_7->setText("Probabilidad p");
        ui->doubleSpinBox_2_1->setMinimum(0.01);
        ui->doubleSpinBox_2_1->setMaximum(.99);
        ui->doubleSpinBox_2_1->setValue(0.5);
        ui->doubleSpinBox_2_1->setDecimals(2);
        esconder(3);
        break;
    case 3: // Binomial
        ui->label_2_7->setText("Probabilidad p");
        ui->label_2_8->setText("Intentos n");
        ui->doubleSpinBox_2_1->setMinimum(0.01);
        ui->doubleSpinBox_2_1->setMaximum(0.99);
        ui->doubleSpinBox_2_1->setValue(0.1);
        ui->doubleSpinBox_2_2->setMinimum(1);
        ui->doubleSpinBox_2_2->setValue(20);
        esconder(2);
        mostrar(1);
        break;
    case 4: // Geometric
        ui->label_2_7->setText("Probabilidad p");
        ui->doubleSpinBox_2_1->setMinimum(0.01);
        ui->doubleSpinBox_2_1->setMaximum(.99);
        ui->doubleSpinBox_2_1->setValue(0.5);
        ui->doubleSpinBox_2_1->setDecimals(2);
        esconder(3);
        break;
    case 5: // Hypergeometric
        ui->label_2_7->setText("Trail n");
        ui->label_2_8->setText("Poblacion N");
        ui->label_2_9->setText("Successes K");
        ui->doubleSpinBox_2_1->setMinimum(0);
        ui->doubleSpinBox_2_1->setValue(6);
        ui->doubleSpinBox_2_2->setMinimum(1);
        ui->doubleSpinBox_2_2->setValue(10);
        ui->doubleSpinBox_2_3->setMinimum(0);
        ui->doubleSpinBox_2_3->setValue(4);
        esconder(1);
        mostrar(2);
        break;
    case 6: // Negative Binomial
        ui->label_2_7->setText("Probabilidad p");
        ui->label_2_8->setText("Sucesos s");
        ui->doubleSpinBox_2_1->setMinimum(0.01);
        ui->doubleSpinBox_2_1->setMaximum(0.99);
        ui->doubleSpinBox_2_1->setValue(0.5);
        ui->doubleSpinBox_2_2->setMinimum(1);
        ui->doubleSpinBox_2_2->setValue(5);
        ui->doubleSpinBox_2_1->setDecimals(2);
        esconder(2);
        mostrar(1);
        break;
    case 7: // Pascal
        ui->label_2_7->setText("Probabilidad p");
        ui->label_2_8->setText("Sucesos s");
        ui->doubleSpinBox_2_1->setMinimum(0.01);
        ui->doubleSpinBox_2_1->setMaximum(0.99);
        ui->doubleSpinBox_2_1->setValue(0.5);
        ui->doubleSpinBox_2_2->setMinimum(1);
        ui->doubleSpinBox_2_2->setValue(3);
        ui->doubleSpinBox_2_1->setDecimals(2);
        esconder(2);
        mostrar(1);
        break;
    case 8: // Poisson
        ui->label_2_7->setText("Rate miu");
        ui->doubleSpinBox_2_1->setMinimum(0.01);
        ui->doubleSpinBox_2_1->setValue(2);
        ui->doubleSpinBox_2_1->setDecimals(2);
        esconder(3);
        break;
//    default:
//        break;
    }
}

void AleatoriedadWidget::distroMaximoMinino(){

    ui->label_2_7->setText("Minimo");
    ui->label_2_8->setText("Maximo");
    ui->doubleSpinBox_2_1->setMinimum(-999999999.000000);
    ui->doubleSpinBox_2_1->setValue(0);
    ui->doubleSpinBox_2_2->setMinimum(-999999999.000000);
    ui->doubleSpinBox_2_2->setValue(1);
    esconder(2);
    mostrar(1);
    ui->widget_2_2->show();

}

void AleatoriedadWidget::esconder(int par){
    if(par == 1){
        ui->label_2_10->hide();
        ui->doubleSpinBox_2_4->hide();
    }
    else if (par == 2){
        ui->label_2_10->hide();
        ui->label_2_9->hide();
        ui->doubleSpinBox_2_3->hide();
        ui->doubleSpinBox_2_4->hide();
    }
    else if (par == 3){
        ui->label_2_10->hide();
        ui->label_2_9->hide();
        ui->label_2_8->hide();
        ui->doubleSpinBox_2_2->hide();
        ui->doubleSpinBox_2_3->hide();
        ui->doubleSpinBox_2_4->hide();
    }
}

void AleatoriedadWidget::mostrar(int par){
    if(par == 1){
        ui->label_2_8->show();
        ui->doubleSpinBox_2_2->show();
    }
    else if (par == 2){
        ui->label_2_8->show();
        ui->label_2_9->show();
        ui->doubleSpinBox_2_2->show();
        ui->doubleSpinBox_2_3->show();
    }
    else if (par == 3){
        ui->label_2_8->show();
        ui->label_2_9->show();
        ui->label_2_10->show();
        ui->doubleSpinBox_2_2->show();
        ui->doubleSpinBox_2_3->show();
        ui->doubleSpinBox_2_4->show();
    }

}

double AleatoriedadWidget::seleccionarDistribucion(int control){

    double distro; //Numero Generador po la distribucion
    switch (control) {

    // Continuas

    case 1: // Uniforme
        distro = m_generador->uniform(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 2: // Arcoseno
        distro = m_generador->arcsine(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 3: // Beta
        distro = m_generador->beta(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value(),ui->doubleSpinBox_2_3->value(),ui->doubleSpinBox_2_4->value());
        break;
    case 4: // Cauchy (or Lorentz)
        distro = m_generador->cauchy(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 5: // Chi-Cuadrado
        distro = m_generador->chiSquare(ui->doubleSpinBox_2_1->value());
        break;
    case 6: // Coseno
        distro = m_generador->cosine(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 7: // Double Log
        distro = m_generador->doubleLog(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 8: // Erlang
        distro = m_generador->erlang(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 9: // Exponencial
        distro = m_generador->exponential(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 10: // Extreme Value
        distro = m_generador->extremeValue(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 11: // F Ratio
        distro = m_generador->fRatio(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 12: // Gamma
        distro = m_generador->gamma(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value(),ui->doubleSpinBox_2_3->value());
        break;
    case 13: // Laplace
        distro = m_generador->laplace(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 14: // Logarithmic
        distro = m_generador->logarithmic(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 15: // Logistic
        distro = m_generador->logistic(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 16: // Lognormal
        distro = m_generador->lognormal(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value(),ui->doubleSpinBox_2_3->value());
        break;
    case 17: // Normal
        distro = m_generador->normal(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 18: // Parabolic
        distro = m_generador->parabolic(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 19: // Pareto
        distro = m_generador->pareto(ui->doubleSpinBox_2_1->value());
        break;
    case 20: // Pearson Type 5
        distro = m_generador->pearson5(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 21: // Pearson Type 6
        distro = m_generador->pearson6(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value(),ui->doubleSpinBox_2_3->value());
        break;
    case 22: // Power
        distro = m_generador->power(ui->doubleSpinBox_2_1->value());
        break;
    case 23: // Rayleigh
        distro = m_generador->rayleigh(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 24: // Students T
        distro = m_generador->studentT(ui->doubleSpinBox_2_1->value());
        break;
    case 25: // Triangular
        distro = m_generador->triangular(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 26: // Weibull
        distro = m_generador->weibull(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value(),ui->doubleSpinBox_2_3->value());
        break;

//    // Discretas

    case 31: // Uniforme Discreta
        distro = m_generador->uniformDiscrete(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 32: // Bernoulli Trial
        distro = m_generador->bernoulli(ui->doubleSpinBox_2_1->value());
        break;
    case 33: // Binomial
        distro = m_generador->binomial(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 34: // Geometric
        distro = m_generador->geometric(ui->doubleSpinBox_2_1->value());
        break;
    case 35: // Hypergeometric
        distro = m_generador->hypergeometric(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value(),ui->doubleSpinBox_2_3->value());
        break;
    case 36: // Negative Binomial
        distro = m_generador->negativeBinomial(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 37: // Pascal
        distro = m_generador->pascal(ui->doubleSpinBox_2_1->value(),ui->doubleSpinBox_2_2->value());
        break;
    case 38: // Poisson
        distro = m_generador->poisson(ui->doubleSpinBox_2_1->value());
        break;
//    default:
//        distro = 0;
//        break;
    }

    return distro;
}

void AleatoriedadWidget::on_pushButton_2_clicked()
{
    generacion();
}

void AleatoriedadWidget::generacion(){

    int id_generador = ui->comboBox_2_1->currentIndex();
    int distroContinua = ui->comboBox_2_2->currentIndex();
    int distroDiscreta = ui->comboBox_2_3->currentIndex();

    if(id_generador == 0){
        m_generador = new Generador;
    }
    else if(id_generador == 1){
        m_generador = new Generador(ui->spinBox_2_1->value(),ui->spinBox_2_2->value(),ui->spinBox_2_3->value(),ui->spinBox_2_4->value());
    }
    else if(id_generador == 2){
        ui->spinBox_2_3->setValue(0);
        m_generador = new Generador(ui->spinBox_2_1->value(),ui->spinBox_2_2->value(),0,ui->spinBox_2_4->value());
    }
    else if(id_generador == 3){
        ui->spinBox_2_1->setValue(0);
        m_generador = new Generador(0,ui->spinBox_2_2->value(),ui->spinBox_2_3->value(),ui->spinBox_2_4->value());
    }
    else if(id_generador == 4){
        m_generador = new Generador(ui->spinBox_2->value());
    }
    else {
        m_generador = new Generador(ui->spinBox_2_1->value(), ui->spinBox_2_2->value());
    }

    int Ntabla = ui->spinBox_2->value();

    for(int i = 0; i < Ntabla; i++){

        if (!distroContinua && !distroDiscreta) {
            //m_generadorerar sin distribucion
        }
        else if (distroContinua) {
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(seleccionarDistribucion(distroContinua)));
            ui->tableWidget_2->setItem(i, 0, newItem);
        }
        else {
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(seleccionarDistribucion(distroDiscreta + 30))); // Sumo 30 para diferenciar en swith
            ui->tableWidget_2->setItem(i, 0, newItem);
        }

    }
}

QTableWidget *AleatoriedadWidget::verTablaWidget(){
    return ui->tableWidget_2;
}
