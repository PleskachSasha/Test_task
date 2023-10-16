#include "userslist.h"
#include "ui_userslist.h"

UsersList::UsersList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsersList)
{
    page = 1;
    ui->setupUi(this);
    showMoreButton = ui->Users_List_ShowMore;
    Show_List();
}

UsersList::~UsersList()
{
    delete ui;
}

void UsersList::on_Users_List_AddUser_clicked()
{
    emit UL_move_Registration();
}

void UsersList::Show_List()
{
    QUrl url("https://frontend-test-assignment-api.abz.agency/api/v1/users?page=1&count=" + QString::number(page * 6));
    QNetworkRequest request(url);

    reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (!jsonResponse.isNull() && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("users") && jsonObject["users"].isArray()) {
                QJsonArray usersArray = jsonObject["users"].toArray();
                if (totalPages == -1) {
                    totalPages = jsonObject.value("total_pages").toInt();
                }

                if (page >= totalPages) {
                    ui->Users_List_ShowMore->setVisible(false);
                }
                items.clear();

                QListView *listView = ui->listView;
                listView->setStyleSheet("QListView::item {border: 1px solid gray;}");

                model = new QStandardItemModel(listView);

                CustomDelegate *customDelegate = new CustomDelegate(this);

                listView->setItemDelegate(customDelegate);

                for (int i = 0; i < qMin(usersArray.size(), page * 6); ++i) {
                    QJsonObject userObject = usersArray[i].toObject();

                    QString name = userObject["name"].toString();
                    QString position = userObject["position"].toString();
                    QString email = userObject["email"].toString();
                    QString phone = userObject["phone"].toString();

                    QStandardItem *item = new QStandardItem();
                    item->setData(name, Qt::UserRole + 2);
                    item->setData(position, Qt::UserRole + 3);
                    item->setData(email, Qt::UserRole + 4);
                    item->setData(phone, Qt::UserRole + 5);

                    QString itemText = QString("%1\n%2\n%3\n%4\n").arg(name, position, email, phone);
                    item->setText(itemText);

                    QString photoUrl = userObject["photo"].toString();
                    QUrl imageUrl(photoUrl);
                    QNetworkRequest imageRequest(imageUrl);
                    QNetworkReply *imageReply = manager.get(imageRequest);

                    QObject::connect(imageReply, &QNetworkReply::finished, [=]() {
                        if (imageReply->error() == QNetworkReply::NoError) {
                            QByteArray imageData = imageReply->readAll();
                            QPixmap userPhoto;
                            if (userPhoto.loadFromData(imageData)) {
                                item->setIcon(QIcon(userPhoto));
                                item->setData(userPhoto, Qt::UserRole + 1);
                            } else {
                                qDebug() << "Помилка завантаження фотографії:" << imageReply->errorString();
                            }
                        } else {
                            qDebug() << "Помилка завантаження фотографії1:" << imageReply->errorString();
                        }
                        imageReply->deleteLater();
                    });

                    items.append(item);
                }
                model->appendColumn(items);
                listView->setModel(model);
            }
        }
    }
    else {
        qDebug() << "Помилка HTTP GET:" << reply->errorString();
    }
}


void UsersList::on_Users_List_ShowMore_clicked()
{
    QMovie *movie = new QMovie(":/resource/img/Skypeloader.gif");

    ui->Users_List_ShowMore->setVisible(false);
    ui->test->setVisible(true);
    ui->test->setFixedSize(40, 40);
    ui->test->setMovie(movie);
    movie->start();

    ++page;
    Show_List();

    ui->test->movie()->stop();
    ui->test->setVisible(false);

    if (page >= totalPages) {
        ui->Users_List_ShowMore->setVisible(false);
    }
    else {
        ui->Users_List_ShowMore->setVisible(true);
    }
}

