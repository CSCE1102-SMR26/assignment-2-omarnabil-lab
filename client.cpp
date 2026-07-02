#include "client.h"
#include "ui_client.h"
#include <QTcpSocket>
#include <QCoro/QCoroNetwork> // Required to await QTcpSocket
#include <QMessageBox>        // Required for UI feedback
#include <QDebug>

client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
}

client::~client()
{
    delete ui;
}

QCoro::Task<void> client::sendNetworkMessage(QString host, quint16 port, QString message)
{
    QTcpSocket socket;

    // 2. Print a qDebug() message stating you are attempting to connect.
    qDebug() << "Attempting to connect to" << host << "on port" << port;

    // 3. Initiate the connection to the host using the socket.
    socket.connectToHost(host, port);

    // 4. Asynchronously wait for the connection to establish using co_await and qCoro.
    bool connected = co_await qCoro(socket).waitForConnected();

    // 5. Write an if-statement. If NOT connected:
    if (!connected) {
        // Show a QMessageBox::critical stating the connection failed
        QMessageBox::critical(this, "Connection Error", "Connection failed: " + socket.errorString());
        co_return; // Exit the coroutine early
    }

    // 6. Print a qDebug() message confirming connection.
    qDebug() << "Successfully connected to server!";

    // 7. Write the 'message' variable to the socket (converted to Utf8).
    socket.write(message.toUtf8());

    // 8. Tell Qt to gracefully disconnect from the host.
    socket.disconnectFromHost();

    // 9. Asynchronously wait for Qt to finish flushing the buffer and disconnecting.
    co_await qCoro(socket).waitForDisconnected();

    // 10. Show a QMessageBox::information stating the message was sent successfully.
    QMessageBox::information(this, "Success", "Message sent successfully!");

}
