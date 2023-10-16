#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include "userslist.h"
#include "useradded.h"

#include <QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QListWidget>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <algorithm>
#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>

#include <QHttpMultiPart>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>



QT_BEGIN_NAMESPACE
namespace Ui { class Registration; }
QT_END_NAMESPACE
class UsersList;

class Registration : public QMainWindow
{
    Q_OBJECT

public:
    void GetToken();
    void GetPosition();
    void SetPosition();

    Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void onNetworkReplyFinished();

    void on_Users_clicked();

    void on_Add_user_clicked();

    void UL_move_Registration();
    void UA_move_Registration();
    void UA_move_UsersList_1();
    void UA_move_UsersList_2();

    void on_FotoButton_clicked();

private:
    Ui::Registration *ui;

    UsersList _usersList;
    UserAdded _userAdded;

    QNetworkAccessManager tokenManager;
    QNetworkReply *tokenReply;
    QString token;

    QNetworkAccessManager positionManager;
    QNetworkReply *positionReply;
    QStringList positionsNames;

    QNetworkAccessManager manager;
    QHttpMultiPart *multiPart;
    QNetworkReply *reply;

    QString filePath;
};
#endif // REGISTRATION_H
