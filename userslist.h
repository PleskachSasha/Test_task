#ifndef USERSLIST_H
#define USERSLIST_H

#include <QWidget>
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
#include <QTextCharFormat>
#include <QTextCursor>
#include <delegate.h>
#include <QPushButton>
#include <QMovie>


namespace Ui {
class UsersList;
}
class Registration;

class UsersList : public QWidget
{
    Q_OBJECT

public:    
    void Show_List();
    void onReplyFinished();
    int page;
    QPushButton *showMoreButton;
    explicit UsersList(QWidget *parent = nullptr);
    ~UsersList();
signals:
    void UL_move_Registration();
private slots:
    void on_Users_List_AddUser_clicked();

    void on_Users_List_ShowMore_clicked();

private:
    Ui::UsersList *ui;

    int totalPages = -1;

    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QStandardItemModel *model;
    QList<QStandardItem *> items;
};

#endif // USERSLIST_H
