#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QKeyEvent>
#include <QDateTime>
#include <QVBoxLayout>
#include <QApplication>
#include <QFontDatabase>

const char* BACKGROUND_COLOR = "#000000";
const char* FOREGROUND_COLOR = "#00FF00";

class Clock : public QWidget {
    Q_OBJECT

    public:
        Clock() {
            int id = QFontDatabase::addApplicationFont("font.otf");
            QString family = QFontDatabase::applicationFontFamilies(id).value(0);
            QFont font(family, 72, QFont::Bold);

            timeLabel = new QLabel(this);
            timeLabel->setAlignment(Qt::AlignCenter);
            timeLabel->setFont(font);
            timeLabel->setStyleSheet(QString("QLabel { color: %1; background-color: transparent; }").arg(FOREGROUND_COLOR));

            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->addWidget(timeLabel);
            setLayout(layout);

            setWindowTitle("Clock");

            setAutoFillBackground(true);
            QPalette pal = palette();
            pal.setColor(QPalette::Window, QColor(BACKGROUND_COLOR));
            setPalette(pal);

            currentTime = QDateTime::currentDateTime();
            updateLabel();

            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, &Clock::tick);

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
    Clock clock;
    clock.show();
    return app.exec();
}

#include "main.moc"
