#include "registration.h"
#include "ui_registration.h"
#include <QThread>

Registration::Registration(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Registration)
{
    GetToken();
    GetPosition();
    ui->setupUi(this);
    SetPosition();

    ui->stackedWidget->insertWidget(1, &_usersList);
    ui->stackedWidget->insertWidget(2, &_userAdded);
    ui->stackedWidget->setStyleSheet("QStackedWidget { border: 1px solid #cecece; }");

    connect(&_usersList, SIGNAL(UL_move_Registration()), this, SLOT(UL_move_Registration()));
    connect(&_userAdded, SIGNAL(UA_move_Registration()), this, SLOT(UA_move_Registration()));
    connect(&_userAdded, SIGNAL(UA_move_UsersList_1()), this, SLOT(UA_move_UsersList_1()));
    connect(&_userAdded, SIGNAL(UA_move_UsersList_2()), this, SLOT(UA_move_UsersList_2()));
}

Registration::~Registration()
{
    delete ui;
}

void Registration::GetToken(){
    QUrl url("https://frontend-test-assignment-api.abz.agency/api/v1/token");
    QNetworkRequest request(url);

    tokenReply = tokenManager.get(request);

    QEventLoop loop;
    QObject::connect(tokenReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (tokenReply->error() == QNetworkReply::NoError) {
        QByteArray responseData = tokenReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (!jsonResponse.isNull() && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("token") && jsonObject["token"].isString()) {
                token = jsonObject["token"].toString();
            }
        }
    } else {
        qDebug() << "Помилка HTTP GET:" << tokenReply->errorString();
    }
}

void Registration::GetPosition(){
    QUrl url("https://frontend-test-assignment-api.abz.agency/api/v1/positions");
    QNetworkRequest request(url);

    positionReply = positionManager.get(request);

    QEventLoop loop;
    QObject::connect(positionReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (positionReply->error() == QNetworkReply::NoError) {
        QByteArray responseData = positionReply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (!jsonResponse.isNull() && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("positions") && jsonObject["positions"].isArray()) {
                QJsonArray positionsArray = jsonObject["positions"].toArray();

                for (int i = 0; i < positionsArray.size(); ++i) {

                    QJsonObject positionObject = positionsArray[i].toObject();
                    int id = positionObject["id"].toInt();
                    QString name = positionObject["name"].toString();

                    positionsNames.append(name);
                }
            }
        }
    } else {
        qDebug() << "Помилка HTTP GET:" << positionReply->errorString();
    }
}

void Registration::SetPosition(){
    ui->radioButton_1->setText(positionsNames[0]);
    ui->radioButton_2->setText(positionsNames[1]);
    ui->radioButton_3->setText(positionsNames[2]);
    ui->radioButton_4->setText(positionsNames[3]);
}

void Registration::on_Users_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Registration::onNetworkReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();

        ui->stackedWidget->setCurrentIndex(2);

        multiPart->deleteLater();
        reply->deleteLater();
        _usersList.Show_List();
    } else {
        qDebug() << "Помилка HTTP POST:" << reply->errorString();
                                                       QMessageBox::warning(this, tr("Помилка"), tr("Поля були неправильно заповнені або користувач вже є в базі"));
    }
}

void Registration::on_Add_user_clicked()
{
    QNetworkRequest request(QUrl("https://frontend-test-assignment-api.abz.agency/api/v1/users"));
    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    request.setRawHeader("Token", token.toUtf8());

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"name\"");
    QString nameText = ui->nameLine->text();
    namePart.setBody(nameText.toUtf8());
    multiPart->append(namePart);

    QHttpPart emailPart;
    emailPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"email\"");
    QString emailText = ui->emailLine->text();
    emailPart.setBody(emailText.toUtf8());
    multiPart->append(emailPart);

    QHttpPart phonePart;
    phonePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"phone\"");
    QString phoneText = ui->phoneLine->text();
    phonePart.setBody(phoneText.toUtf8());
    multiPart->append(phonePart);


    QHttpPart positionPart;
    positionPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"position_id\"");
    QString positionText;
    if (ui->radioButton_1->isChecked()) {
        positionPart.setBody("1");
        positionText = ui->radioButton_1->text();
    } else if (ui->radioButton_2->isChecked()) {
        positionPart.setBody("2");
        positionText = ui->radioButton_2->text();
    } else if (ui->radioButton_3->isChecked()) {
        positionPart.setBody("3");
        positionText = ui->radioButton_3->text();
    } else if (ui->radioButton_4->isChecked()) {
        positionPart.setBody("4");
        positionText = ui->radioButton_4->text();
    }
    multiPart->append(positionPart);

    if(filePath.isEmpty()) {
        filePath = ui->photoLine->text();
        qDebug() << "filePath2:" << filePath;
    }
    QFile *file = new QFile(filePath);
    QString fotoText = filePath;
    if (file->open(QIODevice::ReadOnly)) {
        QHttpPart photoPart;
        photoPart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");
        photoPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"photo\"; filename=\"test.jpg\"");
        photoPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"photo\"; filename=\"test.png\"");
        photoPart.setBodyDevice(file);
        multiPart->append(photoPart);
    }

    if (nameText.isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Будь ласка, заповніть всі поля."));
        return;
    } else if (emailText.isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Будь ласка, заповніть всі поля."));
        return;
    } else if (phoneText.isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Будь ласка, заповніть всі поля."));
        return;
    } else if (positionText.isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Будь ласка, заповніть всі поля."));
        return;
    } else if (fotoText.isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Будь ласка, заповніть всі поля."));
        return;
    }

    reply = manager.post(request, multiPart);
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << " Відправка запиту успішна";
    } else {
        qDebug() << "Помилка HTTP POST:" << reply->errorString();
    }

    QObject::connect(reply, &QNetworkReply::finished, this, &Registration::onNetworkReplyFinished);

    _usersList.page = 1;
    _usersList.showMoreButton->setVisible(true);
}

void Registration::on_FotoButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Виберіть фотографію"), QDir::homePath(), tr("Зображення (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if(!filePath.isEmpty()){
        ui->photoLine->setText(filePath);
    }
}

//
void Registration::UL_move_Registration()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Registration::UA_move_Registration()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Registration::UA_move_UsersList_1()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void Registration::UA_move_UsersList_2()
{
    ui->stackedWidget->setCurrentIndex(1);
}



