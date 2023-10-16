#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QPixmap>
#include <QFont>

class CustomDelegate : public QStyledItemDelegate {
public:
    CustomDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (!index.isValid()) {
            return;
        }

        QPixmap userPhoto = qvariant_cast<QPixmap>(index.data(Qt::UserRole + 1));
        QString name = index.data(Qt::UserRole + 2).toString();
        QString position = index.data(Qt::UserRole + 3).toString();
        QString email = index.data(Qt::UserRole + 4).toString();
        QString phone = index.data(Qt::UserRole + 5).toString();

        int xOffset = option.rect.x() + 10;
        int yOffset = option.rect.y() + 10;

        painter->drawPixmap(xOffset, yOffset, userPhoto);
        xOffset += userPhoto.width() + 10;

        QFont nameFont = painter->font();
        painter->setPen(QColor(Qt::black));
        nameFont.setPointSize(11);
        painter->setFont(nameFont);

        yOffset += 12;
        painter->drawText(xOffset, yOffset, name);

        yOffset += 19;
        painter->setPen(QColor("#6d6d6d"));
        painter->setFont(option.font);
        painter->drawText(xOffset, yOffset, position);

        yOffset += 19;
        painter->drawText(xOffset, yOffset, email);

        yOffset += 19;
        painter->drawText(xOffset, yOffset, phone);

        int lineY = option.rect.bottom() - 1;
        painter->setPen(QColor("#cecece"));
        painter->drawLine(option.rect.left(), lineY, option.rect.right(), lineY);

    }
};


#endif // DELEGATE_H
