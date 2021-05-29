#pragma once
#include <QTreeWidget>
#include <opcuaclient/UaBrowser.h>

#include "util.h"

ELESYOPCUACLIENT_START

class QUaBrowserItem: public QTreeWidgetItem
{

public:
    QUaBrowserItem(const std::string _id = std::string(), const UaQualifiedString &_uaqstr = UaQualifiedString())
        : QTreeWidgetItem()
        , id { QString::fromStdString(_id) }
        , browseName{ QString::fromStdString(_uaqstr.text) }
        , namespaceIndex { _uaqstr.namespaceIndex }
    {
        setText(0, browseName);
    }

    QUaBrowserItem(QTreeWidget *_view, const std::string _id = std::string(), const UaQualifiedString &_uaqstr = UaQualifiedString())
        : QTreeWidgetItem(_view)
        , id { QString::fromStdString(_id) }
        , browseName{ QString::fromStdString(_uaqstr.text) }
        , namespaceIndex { _uaqstr.namespaceIndex }
    {
        setText(0, browseName);
    }

    QUaBrowserItem(QTreeWidgetItem *_parent, const std::string _id = std::string(), const UaQualifiedString &_uaqstr = UaQualifiedString())
        : QTreeWidgetItem(_parent)
        , id { QString::fromStdString(_id) }
        , browseName{ QString::fromStdString(_uaqstr.text) }
        , namespaceIndex { _uaqstr.namespaceIndex }
    {
        setText(0, browseName);
    }

    QUaBrowserItem* child(int index)
    {
        return dynamic_cast<QUaBrowserItem*>(QTreeWidgetItem::child(index));
    }

    void addChild(QUaBrowserItem *child)
    {
        QTreeWidgetItem::addChild(child);
    }

    void addChildren(const QList<QUaBrowserItem*> &list)
    {
        for(const auto &child : list) {
            addChild(child);
        }
    }

    QString     id;
    QString     browseName;
    unsigned    namespaceIndex;

};

class QUaBrowser : public QTreeWidget
{
Q_OBJECT
public:
    using Ptr = std::shared_ptr<QUaBrowser>;
    QUaBrowser(QWidget *parent = nullptr);

    void setConnection(UaConnection::Ptr conn);

    QList<QUaBrowserItem*> readCurrentDir(const QString &nodeId,
                                           BrowseDirection direction = BrowseDirection::forward,
                                           ReferenceCategory refCategory = ReferenceCategory::hierarchical);

    inline UaStatusCode lastError() const { return status; }
    inline bool isValid() const { return status == uaGood; }
    inline bool isConnected() const;
    void checkConnected();
    bool browserInit();

private:
    QUaBrowserItem* createItem(const QString &text, const BrowseResult &browseResult);

    UaConnection::Ptr connection;
    UaBrowser::Ptr    browser;
    UaStatusCode      status;
    QIcon             varIcon { QPixmap("../data.png") };

private slots:
    void directoryOpened(QTreeWidgetItem* current);

protected:
    virtual void startDrag(Qt::DropActions supportedActions) override
    {
        auto item = selectedItems();
        item[0]->setText(0, dynamic_cast<QUaBrowserItem*>(item[0])->id);
        QTreeWidget::startDrag(supportedActions);
        item[0]->setText(0, dynamic_cast<QUaBrowserItem*>(item[0])->browseName);
    }
};

ELESYOPCUACLIENT_END

