#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <qcorotask.h> // Include QCoro core

QT_BEGIN_NAMESPACE
namespace Ui { class client; }
QT_END_NAMESPACE

class client : public QMainWindow {
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();

private slots:
    void on_sendButton_clicked();

private:
    Ui::client *ui;

    // The coroutine that will handle the network operation without blocking the UI
    QCoro::Task<void> sendNetworkMessage(QString host, quint16 port, QString message);
};

#endif // CLIENT_H
