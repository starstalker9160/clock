#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QDateTime>
#include <QElapsedTimer>
#include <QKeyEvent>

const char* BACKGROUND_COLOR = "#000000";
const char* FOREGROUND_COLOR = "#00FF00";

class ClockWidget : public QWidget {
    Q_OBJECT

public:
    ClockWidget() {
        int id = QFontDatabase::addApplicationFont("font.otf");
        QString family = QFontDatabase::applicationFontFamilies(id).value(0);
        QFont font(family, 72, QFont::Bold);

        timeLabel = new QLabel(this);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setFont(font);
        timeLabel->setStyleSheet(QString("QLabel { color: %1; background-color: %2; }")
                                     .arg(FOREGROUND_COLOR)
                                     .arg(BACKGROUND_COLOR));

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(timeLabel);
        setLayout(layout);

        setWindowFlag(Qt::FramelessWindowHint);
        resize(800, 600);

        currentTime = QDateTime::currentDateTime();
        updateLabel();

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ClockWidget::tick);

        int msToNextSecond = 1000 - QTime::currentTime().msec();
        QTimer::singleShot(msToNextSecond, this, [this]() {
            timer->start(1000);
            tick();
        });
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::AltModifier)) {
            if (isFullScreen())
                showNormal();
            else
                showFullScreen();
        }
    }

private slots:
    void tick() {
        currentTime = currentTime.addSecs(1);
        updateLabel();
    }

private:
    void updateLabel() {
        timeLabel->setText(currentTime.toString("HH:mm:ss"));
    }

    QLabel *timeLabel;
    QTimer *timer;
    QDateTime currentTime;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ClockWidget clock;
    clock.show();
    return app.exec();
}

#include "main.moc"
