#ifndef CLOCK_H
#define CLOCK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class clock;
}
QT_END_NAMESPACE

class clock : public QMainWindow
{
    Q_OBJECT

public:
    clock(QWidget *parent = nullptr);
    ~clock();

private:
    Ui::clock *ui;
};
#endif // CLOCK_H
