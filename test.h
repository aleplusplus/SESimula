// test.h: Definicion e Implementacion de la Clase Test para la verificacion de numeros aleatorios.
#ifndef TEST_H
#define TEST_H

#include "math.h"
#include <QTableWidget>
#include <QDataStream>
#include <QVector>

double const tablaKS[23][3] = {
    {0.950,	0.975,	0.995},
    {0.776,	0.842,	0.929},
    {0.642,	0.708,	0.828},
    {0.564,	0.624,	0.733},
    {0.510,	0.565,	0.669},
    {0.470,	0.521,	0.618},
    {0.438,	0.486,	0.577},
    {0.411,	0.457,	0.543},
    {0.388,	0.432,	0.514},
    {0.368,	0.410,	0.490},
    {0.352,	0.391,	0.468},
    {0.338,	0.375,	0.450},
    {0.325,	0.361,	0.433},
    {0.314,	0.349,	0.418},
    {0.304,	0.338,	0.404},
    {0.295,	0.328,	0.392},
    {0.286,	0.318,	0.381},
    {0.278,	0.309,	0.371},
    {0.272,	0.301,	0.363},
    {0.264,	0.294,	0.356},
    {0.240,	0.270,	0.320},
    {0.220,	0.240,	0.290},
    {0.210,	0.230,	0.270}
};

double const tablaChiC[37][5] = {
    {7.88,      6.63,	5.02,	3.84,	2.71},
    {10.60, 	9.21,	7.38,	5.99,	4.61},
    {12.84, 	11.34,	9.35,	7.84,	6.25},
    {14.96, 	13.28,	11.14,	9.49,	7.78},
    {16.70, 	15.10,	12.80,	11.10,	9.20},
    {18.50, 	16.80,	14.40,	12.60,	10.60},
    {20.30, 	18.50,	16.00,	14.10,	12.00},
    {22.00, 	20.10,	17.50,	15.50,	13.40},
    {23.60, 	21.70,	19.00,	16.90,	14.70},
    {25.20, 	23.20,	20.50,	18.30,	16.00},
    {26.80, 	24.70,	21.90,	19.70,	17.30},
    {28.30, 	26.20,	23.30,	21.00,	18.50},
    {29.80, 	27.70,	24.70,	22.40,	19.80},
    {31.30, 	29.10,	26.10,	23.70,	21.10},
    {32.80,     30.60,	27.50,	25.00,	22.30},
    {34.30,     32.00,	28.80,	26.30,	23.50},
    {35.70,     33.40,	30.20,	27.60,	24.80},
    {37.20, 	34.80,	31.50,	28.90,	26.00},
    {38.60,     36.20,	32.90,	30.10,	27.20},
    {40.00,     37.60,	34.20,	31.40,	28.40},
    {41.40, 	38.90,	35.50,	32.70,	29.60},
    {42.80,     40.30,	36.80,	33.90,	30.80},
    {44.20, 	41.60,	38.10,	35.20,	32.00},
    {45.60, 	43.00,	39.40,	36.40,	33.20},
    {46.90, 	44.30,	40.60,	37.70,	34.40},
    {48.30,     45.60,	41.90,	38.90,	35.60},
    {49.60,     47.00,	43.20,	40.10,	36.70},
    {51.00, 	48.30,	44.50,	41.30,	37.90},
    {52.30,     49.60,	45.70,	42.60,	39.10},
    {53.70,     50.90,	47.00,	43.80,	40.30},
    {66.80,     63.70,	59.30,	55.80,	51.80},
    {79.50,     76.20,	71.40,	67.50,	63.20},
    {92.00,     88.40,	83.30,	79.10,	74.40},
    {104.20,    100.40,	95.00,	90.50,	85.50},
    {116.30,	112.30,	106.60,	101.90,	96.60},
    {128.30,	124.10,	118.10,	113.10,	107.60},
    {140.20,	135.80,	129.60,	124.30,	118.50}
};

double const tablaNormal[40][10] = {
    {0.50000,	0.50399,	0.50798,	0.51197,	0.51595,	0.51994,	0.52392,	0.52790,	0.53188,	0.53586},
    {0.53983,	0.54380,	0.54776,	0.55172,	0.55567,	0.55962,	0.56356,	0.56749,	0.57142,	0.57535},
    {0.57926,	0.58317,	0.58706,	0.59095,	0.59483,	0.59871,	0.60257,	0.60642,	0.61026,	0.61409},
    {0.61791,	0.62172,	0.62552,	0.62930,	0.63307,	0.63683,	0.64058,	0.64431,	0.64803,	0.65173},
    {0.65542,	0.65910,	0.66276,	0.66640,	0.67003,	0.67364,	0.67724,	0.68082,	0.68439,	0.68793},
    {0.69146,	0.69497,	0.69847,	0.70194,	0.70540,	0.70884,	0.71226,	0.71566,	0.71904,	0.72240},
    {0.72575,	0.72907,	0.73237,	0.73565,	0.73891,	0.74215,	0.74537,	0.74857,	0.75175,	0.75490},
    {0.75804,	0.76115,	0.76424,	0.76730,	0.77035,	0.77337,	0.77637,	0.77935,	0.78230,	0.78524},
    {0.78814,	0.79103,	0.79389,	0.79673,	0.79955,	0.80234,	0.80511,	0.80785,	0.81057,	0.81327},
    {0.81594,	0.81859,	0.82121,	0.82381,	0.82639,	0.82894,	0.83147,	0.83398,	0.83646,	0.83891},
    {0.84134,	0.84375,	0.84614,	0.84849,	0.85083,	0.85314,	0.85543,	0.85769,	0.85993,	0.86214},
    {0.86433,	0.86650,	0.86864,	0.87076,	0.87286,	0.87493,	0.87698,	0.87900,	0.88100,	0.88298},
    {0.88493,	0.88686,	0.88877,	0.89065,	0.89251,	0.89435,	0.89617,	0.89796,	0.89973,	0.90147},
    {0.90320,	0.90490,	0.90658,	0.90824,	0.90988,	0.91149,	0.91308,	0.91466,	0.91621,	0.91774},
    {0.91924,	0.92073,	0.92220,	0.92364,	0.92507,	0.92647,	0.92785,	0.92922,	0.93056,	0.93189},
    {0.93319,	0.93448,	0.93574,	0.93699,	0.93822,	0.93943,	0.94062,	0.94179,	0.94295,	0.94408},
    {0.94520,	0.94630,	0.94738,	0.94845,	0.94950,	0.95053,	0.95154,	0.95254,	0.95352,	0.95449},
    {0.95543,	0.95637,	0.95728,	0.95818,	0.95907,	0.95994,	0.96080,	0.96164,	0.96246,	0.96327},
    {0.96407,	0.96485,	0.96562,	0.96638,	0.96712,	0.96784,	0.96856,	0.96926,	0.96995,	0.97062},
    {0.97128,	0.97193,	0.97257,	0.97320,	0.97381,	0.97441,	0.97500,	0.97558,	0.97615,	0.97670},
    {0.97725,	0.97778,	0.97831,	0.97882,	0.97932,	0.97982,	0.98030,	0.98077,	0.98124,	0.98169},
    {0.98214,	0.98257,	0.98300,	0.98341,	0.98382,	0.98422,	0.98461,	0.98500,	0.98537,	0.98574},
    {0.98610,	0.98645,	0.98679,	0.98713,	0.98745,	0.98778,	0.98809,	0.98840,	0.98870,	0.98899},
    {0.98928,	0.98956,	0.98983,	0.99010,	0.99036,	0.99061,	0.99086,	0.99111,	0.99134,	0.99158},
    {0.99180,	0.99202,	0.99224,	0.99245,	0.99266,	0.99286,	0.99305,	0.99324,	0.99343,	0.99361},
    {0.99379,	0.99396,	0.99413,	0.99430,	0.99446,	0.99461,	0.99477,	0.99492,	0.99506,	0.99520},
    {0.99534,	0.99547,	0.99560,	0.99573,	0.99585,	0.99598,	0.99609,	0.99621,	0.99632,	0.99643},
    {0.99653,	0.99664,	0.99674,	0.99683,	0.99693,	0.99702,	0.99711,	0.99720,	0.99728,	0.99736},
    {0.99744,	0.99752,	0.99760,	0.99767,	0.99774,	0.99781,	0.99788,	0.99795,	0.99801,	0.99807},
    {0.99813,	0.99819,	0.99825,	0.99831,	0.99836,	0.99841,	0.99846,	0.99851,	0.99856,	0.99861},
    {0.99865,	0.99869,	0.99874,	0.99878,	0.99882,	0.99886,	0.99889,	0.99893,	0.99896,	0.99900},
    {0.99903,	0.99906,	0.99910,	0.99913,	0.99916,	0.99918,	0.99921,	0.99924,	0.99926,	0.99929},
    {0.99931,	0.99934,	0.99936,	0.99938,	0.99940,	0.99942,	0.99944,	0.99946,	0.99948,	0.99950},
    {0.99952,	0.99953,	0.99955,	0.99957,	0.99958,	0.99960,	0.99961,	0.99962,	0.99964,	0.99965},
    {0.99966,	0.99968,	0.99969,	0.99970,	0.99971,	0.99972,	0.99973,	0.99974,	0.99975,	0.99976},
    {0.99977,	0.99978,	0.99978,	0.99979,	0.99980,	0.99981,	0.99981,	0.99982,	0.99983,	0.99983},
    {0.99984,	0.99985,	0.99985,	0.99986,	0.99986,	0.99987,	0.99987,	0.99988,	0.99988,	0.99989},
    {0.99989,	0.99990,	0.99990,	0.99990,	0.99991,	0.99991,	0.99992,	0.99992,	0.99992,	0.99992},
    {0.99993,	0.99993,	0.99993,	0.99994,	0.99994,	0.99994,	0.99994,	0.99995,	0.99995,	0.99995},
    {0.99995,	0.99995,	0.99996,	0.99996,	0.99996,	0.99996,	0.99996,	0.99996,	0.99997,	0.99997}
};

class Test {

public:

    Test(QTableWidget *tableWidget){
        N = tableWidget->rowCount();
        arregloItems = new QVector<double>;
        double aux;
        for(int i=0;i<N;i++){
            aux = tableWidget->item(i,0)->data(Qt::DisplayRole).toDouble();
            arregloItems->insert(i, aux);
        }
    }
    ~Test();

//    void actualizar(QTableWidget tableWidget){
//        N = tableWidget.rowCount();
//        arregloItems = new QVector<double>(N);
//        QDataStream dato;
//        for(int i=0;i<N;i++){
//            (dato << tableWidget.item(i,0)) >> arregloItems[i];
//        }
//    }

    // Pruebas de Uniformidad

    // K-S
    bool kolomogorov_smirnox(int sig){
        arregloDesviacionPositiva = new QVector<double>;
        arregloDesviacionNegativa = new QVector<double>;
        ordernar(arregloItems);
        double aux;
        for(int i=0;i<N;i++){
            aux = i+1;
            aux /= N;
            aux -= arregloItems->value(i);
            arregloDesviacionPositiva->insert(i, aux);
            aux = i;
            aux /= N;
            aux = arregloItems->value(i) - aux;
            arregloDesviacionNegativa->insert(i,aux );
        }
        double maxDP = maximo(arregloDesviacionPositiva);
        double maxDN = maximo(arregloDesviacionNegativa);
        return (maxDP < maxDN ? maxDN : maxDP) <= (tabla_kolomogorov_smirnox(N,sig));
    }

    bool chi_cuadrado(int sig, int n){
        arregloChiCuadradoClases = new QVector<double>;
        arregloChiCuadradoObservados = new QVector<int>;
        double aux = 0;
        for(int i=0;i<n;i++){
            aux += 1/n;
            arregloChiCuadradoClases->insert(i,aux);
            arregloChiCuadradoObservados->insert(i,N/n);
        }
        chi_cuadrado_observados(n);
        arreglo_chi_cuadrado(n);

        return chi <= tablaChiC[n-1][sig];

    }
    bool distancia_cuadartica();
    bool series();
    bool gaps();

    // Pruebas de Independecia

    // Corridas Acendentes y Descendentes
    bool corridas_ascendetes_descendentes(double sig){
        arregloBoleano = new QVector<bool>;
        for(int i=0;i<N-1;i++)
            arregloItems->value(i) < arregloItems->value(i+1) ? arregloBoleano->insert(i,true) : arregloBoleano->insert(i,false);
        a = numeros_corridas(arregloBoleano, N-1);
        z0 = (a -(2*N-1/3))/(sqrt(16*N-29/90));
        numero_tabla = tabla_Normal(1-sig);
        return -numero_tabla <= z0 && z0 <= numero_tabla;
    }

    // Corridas por encima y por debajo de la media
    bool corridas_encima_debajo(double sig){
        arregloBoleano = new QVector<bool>;
        for(int i=0;i<N;i++)
            arregloItems->value(i) <= 0.5 ? arregloBoleano->insert(i,false) : arregloBoleano->insert(i,true);
        a = numeros_corridas(arregloBoleano, N);
        n1=0;
        n2=0;
        observacionesED(n1,n2);
        numero_tabla = tabla_Normal(1-sig);
        z0 = ((a - 2*n1*n2)/N - 1/2)/(sqrt(2*n1*n2*(2*n1*n2 - N)/N*N*(N-1)));
        return -numero_tabla <= z0 && z0 <= numero_tabla;
    }

    // Longitud de corridas
    bool longitud_corridas(bool test, int sig){
        if(test){
            L = N-1;
            arregloBoleano = new QVector<bool>;
            for(int i=0;i<L;i++)
                arregloItems->value(i) < arregloItems->value(i+1) ? arregloBoleano->insert(i,true) : arregloBoleano->insert(i,false);
            arreglo_longitud();
            arreglo_longitud_observado();
            arreglo_longitud_esperado(test);
        }
        else{
            L = N;
            arregloBoleano = new QVector<bool>;
            for(int i=0;i<L;i++)
                arregloItems->value(i) <= 0.5 ? arregloBoleano->insert(i,false) : arregloBoleano->insert(i,true);
            arreglo_longitud();
            arreglo_longitud_observado();
            arreglo_longitud_esperado(test);
        }
        arreglo_longitud_chi();
        X=0;
        for(int i=0;i<arregloLongitudChi->count();i++)
            X += arregloLongitudChi->value(i);
        return X <= tablaChiC[arregloLongitud->count()-1][sig];
    }

    // Autocorrelcion
    bool autocorrelacion(int i, int m, double sig){
        M = (N-i/m)-1;

        p = 1/M+1;
        double aux=0;
        for(int k=0;k<M;k++)
            aux += arregloItems->value(i+k*m)*arregloItems->value(i+(k+1)*m);
        p *= aux;
        vp = (sqrt(13*M+7))/(12*(M+1));
        z0 = p - 0.25 / vp;
        numero_tabla = tabla_Normal(1-sig);
        return -numero_tabla <= z0 && z0 <= numero_tabla;

    }

private:
    QVector<double> *arregloItems;
//    int nivel_significacion;
    int N;

    QVector<double> *arregloDesviacionPositiva;
    QVector<double> *arregloDesviacionNegativa;

    QVector<double> *arregloChiCuadradoClases;
    QVector<int> *arregloChiCuadradoObservados;
    QVector<double> *arregloChiCuadradoEsperados;
    QVector<double> *arregloChiCuadrado;

    double chi;

    QVector<bool> *arregloBoleano;
    int a,n1,n2;
    double z0;
    double numero_tabla;

    int L;
    double X;
    QVector<int> *arregloLongitud;
    QVector<int> *arregloLongitudObservado;
    QVector<double> *arregloLongitudEsperado;
    QVector<double> *arregloLongitudChi;

    int M;
    double p;
    double vp;

    // K-S

    double tabla_kolomogorov_smirnox(int n,int sigma){
        if(n < 20)
            return tablaKS[n][sigma];
        else if (n < 25 && n >= 20) {
            return tablaKS[20][sigma];
        }
        else if (n < 30 && n >= 25) {
            return tablaKS[21][sigma];
        }
        else if (n < 35 && n >= 30) {
            return tablaKS[22][sigma];
        }
        else {
            if(sigma == 0)
                return 1.22 / sqrt(n);
            else if (sigma == 1) {
                return 1.36 / sqrt(n);
            }
            else {
                return 1.63 / sqrt(n);
            }
        }

    }
    void ordernar(QVector<double> *vector){
        double aux;
        int n = vector->count();
        for(int i=0;i<n-1;i++){
            for(int j=0;j<n-1;j++){
                if(vector->value(j) > vector->value(j+1)){
                    aux=vector->value(j);
                    vector->replace(j,vector->value(j+1));
                    vector->replace(j+1,aux);
                }
            }
        }
    }
    double maximo(QVector<double> *vector){
        double max = 0;
        for(int i=0;i<N;i++)
            max = (max < vector->value(i)) ? vector->value(i) : max;
        return max;
    }

    // Corridas Ascendents Descendetes y Encima Debajo

    int numeros_corridas(QVector<bool> *tablaAD, int n){
        int corridas = 0;
        int bandera= !tablaAD->value(0);
        for(int i;i<n;i++){
            if(bandera != tablaAD->value(i)){
                corridas++;
                bandera = tablaAD->value(i);
            }
        }
        return corridas;
    }

    double tabla_Normal(double sig){
        for(int i=0;i<40;i++)
            for(int j=0;j<10;j++)
                if(tablaNormal[i][j] == sig)
                    return i+j/100;
    }

    void observacionesED(int &n1, int &n2){
        for(int i=0;i<N;i++){
            arregloItems->value(i) <= 0.5 ? n1++ : n2++;
        }
    }

    // Longitud corridas

    void arreglo_longitud(){
        arregloLongitud = new QVector<int>();
        int cont=0;
        int j=0;
        bool bandera = arregloBoleano->value(0);
        for(int i=0;i<L;i++){
            if(bandera != arregloBoleano->value(i)){
                arregloBoleano->insert(j,cont);
                j++;
                cont = 1;
                bandera = arregloBoleano->value(i);
            }
            else
                cont++;
        }
    }
    void arreglo_longitud_observado(){
        arregloLongitudObservado = new QVector<int>(3);
        int a=0,b=0,c=0;
        int n = arregloLongitud->count();
        for(int i=0;i<n;i++){
            if(arregloLongitud->value(i)==1)
                a++;
            else if (arregloLongitud->value(i)==2)
                b++;
            else
                c++;
        }
        arregloLongitudObservado->insert(0,a);
        arregloLongitudObservado->insert(1,b);
        arregloLongitudObservado->insert(2,c);
    }

    void arreglo_longitud_esperado(bool test){
        arregloLongitudEsperado = new QVector<double>(3);
        double esperado = 2*N-1/3;
        double aux;
        if(test){
            aux = 1/12*(N*5+1);
            esperado -= aux;
            arregloLongitudEsperado->insert(0, aux);
            aux = 1/60*(N*11-14);
            esperado -= aux;
            arregloLongitudEsperado->insert(1, aux);
            arregloLongitudEsperado->insert(1, esperado);
        }
        else{
            aux = 2*N*(n1/N)*(n2/N)*(n2/N);
            esperado -= aux;
            arregloLongitudEsperado->insert(0, aux);
            double aux = 2*N*(n1/N)*(n1/N)*(n2/N)*(n2/N);
            esperado -= aux;
            arregloLongitudEsperado->insert(1, aux);
            arregloLongitudEsperado->insert(1, esperado);
        }


    }

    void arreglo_longitud_chi(){
        if(arregloLongitudObservado->value(3) < 5){
            arregloLongitudObservado->replace(1,arregloLongitudObservado->value(1)+arregloLongitudObservado->value(2));
            arregloLongitudObservado->remove(2);
            arregloLongitudEsperado->replace(1,arregloLongitudEsperado->value(1)+arregloLongitudEsperado->value(2));
            arregloLongitudEsperado->remove(2);
            arregloLongitudChi = new QVector<double>(2);
            arregloLongitudChi->insert(0,pow((arregloLongitudObservado->value(0)-arregloLongitudEsperado->value(0)),2)/arregloLongitudEsperado->value(0));
            arregloLongitudChi->insert(1,pow((arregloLongitudObservado->value(1)-arregloLongitudEsperado->value(1)),2)/arregloLongitudEsperado->value(1));
        }
        else{
            arregloLongitudChi = new QVector<double>(3);
            arregloLongitudChi->insert(0,pow((arregloLongitudObservado->value(0)-arregloLongitudEsperado->value(0)),2)/arregloLongitudEsperado->value(0));
            arregloLongitudChi->insert(1,pow((arregloLongitudObservado->value(1)-arregloLongitudEsperado->value(1)),2)/arregloLongitudEsperado->value(1));
            arregloLongitudChi->insert(2,pow((arregloLongitudObservado->value(2)-arregloLongitudEsperado->value(2)),2)/arregloLongitudEsperado->value(2));
        }
    }

    void chi_cuadrado_observados(int n){
        double aux;
        for(int i=0;i<n-1;i++){
            aux = arregloItems->value(i);
            if(aux*n - int (aux*n) == 0)
                arregloChiCuadradoObservados->replace(int (aux*n),arregloChiCuadradoObservados->value(int (aux*n))+1);
            else
                arregloChiCuadradoObservados->replace(int (aux*n),arregloChiCuadradoObservados->value(int (aux*n)+1)+1);
        }

    }

    void arreglo_chi_cuadrado(int n){
        arregloChiCuadrado = new QVector<double>;
        chi =0;
        double aux;
        for(int i=0;i<n;i++){
            aux = pow(arregloChiCuadradoObservados->value(i)-arregloChiCuadradoEsperados->value(i),2)/arregloChiCuadradoEsperados->value(i);
            chi += aux;
            arregloChiCuadrado->insert(i,aux);
        }
    }

};

#endif // TEST_H
