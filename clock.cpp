#include "clock.h"
#include "./ui_clock.h"

clock::clock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::clock)
{
    ui->setupUi(this);
}

clock::~clock()
{
    delete ui;
}
