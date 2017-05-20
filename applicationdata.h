#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <QObject>
#include <QGraphicsView>
#include <QLabel>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
class ApplicationData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGraphicsView* view WRITE setView)
public:
    Q_INVOKABLE QDateTime getCurrentDateTime() const {
         return QDateTime::currentDateTime();
    }
    Q_INVOKABLE void play() {
        play(m_view);
    }
    Q_INVOKABLE void play(QGraphicsView *view) {
        qDebug() << view;
        QString file("c:/laoshujianu.rmvb");
        m_view = view;
        if (video)
            delete video;
        video = new QWidget(m_view);
        new QLabel(getCurrentDateTime().toString(), video);
        video->setGeometry(0,0,300,300);
        video->raise();
        video->show();
        video->setWindowOpacity(0.4);

        if (process) {
            process->write("q\n");
            process->waitForFinished();
            delete process;
        }
        QStringList args;
        args.append("-slave");
        args.append("-vo");
        args.append("direct3d");
        args << "-wid" << QString::number((ulong) video->winId());
        args << "-colorkey" << "0x020202";
        args << file;
        process = new QProcess(this);
        qDebug()<<args;
        process->start("mplayer.exe", args);
    }
    ApplicationData(QObject *parent = 0) : QObject(parent), video(0), process(0) {
        QTimer::singleShot(1000, this, SIGNAL(dataChanged()));
    }
    void setView(QGraphicsView *view) {
        m_view = view;
    }
Q_SIGNALS:
    void dataChanged();
public:
    QWidget *video;
    QProcess *process;
private:
    QGraphicsView *m_view;
};
#endif // APPLICATIONDATA_H
