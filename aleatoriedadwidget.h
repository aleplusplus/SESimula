#ifndef ALEATORIEDADWIDGET_H
#define ALEATORIEDADWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Generador;

class QAction;
class QToolBar;
class QMenuBar;
class QMenu;
class QTableWidget;
QT_END_NAMESPACE

namespace Ui {
class AleatoriedadWidget;
}

class AleatoriedadWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AleatoriedadWidget(QWidget *parent = 0);
    ~AleatoriedadWidget();

    QToolBar *aleatoriedadToolBar;

    QMenuBar *aleatoriedadMenuBar;

    void createMenus();

    QTableWidget *verTablaWidget();

signals:
    void exportarGenerador();

private slots:
    void on_spinBox_2_valueChanged(int arg1);
    void on_comboBox_2_2_currentIndexChanged(int index);
    void on_comboBox_2_3_currentIndexChanged(int index);
    void on_comboBox_2_1_currentIndexChanged(int index);
    void on_pushButton_2_clicked();

    void saveGenerador();

private:
    Ui::AleatoriedadWidget *ui;

    Generador *m_generador;

    QAction *guardarAction;
    QAction *exportarAction;
    QAction *action_Sobre;
    QAction *action_Sobre_Qt;

    QMenu *menu_Archivo;
    QMenu *menu_Ayuda;

    void distroMaximoMinino();
    void esconder(int par);
    void mostrar(int par);
    double seleccionarDistribucion(int control);

    void setupAction();

    void setupToolBar();

    void generacion();
};

#endif // ALEATORIEDADWIDGET_H
