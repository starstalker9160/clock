#include <QDir>
#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QColor>
#include <QWidget>
#include <QPalette>
#include <QDateTime>
#include <QKeyEvent>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QApplication>
#include <QJsonDocument>
#include <QFontDatabase>

struct ClockConfig {
    QString foreground = "#00ff00";
    QString background = "#000000";
    QString fontFile   = "font.otf";
};

QString getResourcePath(const QString &filename) {
#ifdef Q_OS_MAC
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("Resources");
    return dir.filePath(filename);
#else
    return QCoreApplication::applicationDirPath() + "/" + filename;
#endif
}

ClockConfig loadConfig() {
    ClockConfig config;
    QFile file(getResourcePath("config.json"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.contains("foreground") && obj["foreground"].isString())
                config.foreground = obj["foreground"].toString();
            if (obj.contains("background") && obj["background"].isString())
                config.background = obj["background"].toString();
            if (obj.contains("font") && obj["font"].isString())
                config.fontFile = obj["font"].toString();
        }
    }
    return config;
}

class Clock : public QWidget {
    Q_OBJECT
public:
    Clock(const ClockConfig &config) : cfg(config) {
        QString fontPath = getResourcePath(cfg.fontFile);
        int id = QFontDatabase::addApplicationFont(fontPath);
        QString family;
        if (id != -1) {
            family = QFontDatabase::applicationFontFamilies(id).value(0);
        }
        QFont font;
        if (!family.isEmpty()) {
            font = QFont(family, 72);
        } else {
            font = QFont("Sans Serif", 72);
        }
        timeLabel = new QLabel(this);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setFont(font);
        timeLabel->setStyleSheet(QString("QLabel { color: %1; background-color: transparent; }")
                                     .arg(cfg.foreground));
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(timeLabel);
        setLayout(layout);
        setWindowTitle("Clock");
        setAutoFillBackground(true);
        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor(cfg.background));
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
    ClockConfig cfg;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ClockConfig cfg = loadConfig();
    Clock clock(cfg);
    clock.show();
    return app.exec();
}

#include "main.moc"
