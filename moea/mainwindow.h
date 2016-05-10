#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <evolution.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Evolution *e, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Evolution *_e;
};

#endif // MAINWINDOW_H
