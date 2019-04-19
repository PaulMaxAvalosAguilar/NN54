#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Connection/Serial/serialconnection.h"
#include "Connection/Bluetooth/Classic/bluetoothconnection.h"
#include "scanner.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void dataReceived(int counter, int time_interval);

private:
    Ui::MainWindow *ui;
    int count;
    Scanner sc;
};

#endif // MAINWINDOW_H
