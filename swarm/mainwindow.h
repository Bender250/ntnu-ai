#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "settings.h"
#include "swarm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_individual_count_sliderMoved(int position);

    void on_centering_weight_sliderMoved(int position);

    void on_repealing_weight_sliderMoved(int position);

    void on_alignment_weight_sliderMoved(int position);

    void on_velocity_limit_sliderMoved(int position);

    void on_perimeter_size_sliderMoved(int position);

    void on_simulation_speed_sliderMoved(int position);

    //void start_simulation();

private:
    Ui::MainWindow *ui;
    //Swarm _swarm_world;
};

#endif // MAINWINDOW_H
