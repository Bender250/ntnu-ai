#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include "flatland.h"
#include "individual.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initiate(std::unique_ptr<Individual>& i);
    void paintEvent(QPaintEvent *);

private slots:
    void on_moves_per_sec_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Individual> _i;
    QTimer _timer;
    uint64_t _steps;

    float _moves_per_sec = 1.0;

    uint64_t index_to_pos(uint64_t const& i) const {
        return (i * 50) + 25;
    }

    void draw_flatland(QPainter &p) const;
    void draw_poisson(uint64_t const& i, uint64_t const& j, QPainter &p) const;
    void draw_food(uint64_t const& i, uint64_t const& j, QPainter &p) const;
    void draw_agent(uint64_t const& i, uint64_t const& j, const Orientation &d, QPainter &p) const;

    void step();
};

#endif // MAINWINDOW_H
