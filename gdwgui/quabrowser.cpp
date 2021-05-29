#include "quabrowser.h"
#include <opcuaclient/UaBrowser.h>

ELESYOPCUACLIENT_START

QUaBrowser::QUaBrowser(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragEnabled(true);
    setHeaderLabel("nodeId");
    QObject::connect(this, &QTreeWidget::itemExpanded, this, &QUaBrowser::directoryOpened);
}

void QUaBrowser::setConnection(UaConnection::Ptr conn)
{
    connection = conn;
    if(!isConnected()) {
        status = uaBad;
    }
    else if(browserInit()) {
        clear();
        auto root = new QUaBrowserItem(NODEID_OBJECTS, {"Objects", 0});
        addTopLevelItem(root);
        root->addChildren(readCurrentDir(root->id));
        for(int i = 0; i < root->childCount(); ++i) {
            auto child = root->child(i);
            if(child->childCount() == 0 ) {
                child->addChildren(readCurrentDir(child->id));
            }
        }
    }
}

QList<QUaBrowserItem*> QUaBrowser::readCurrentDir(const QString& nodeId, BrowseDirection direction, ReferenceCategory refCategory)
{
    browser->changeBrowsePosition(nodeId.toStdString());
    std::vector<std::string> tmp;
    BrowseResults result;
    status = browser->browse(direction, refCategory, tmp, result);
    QList<QUaBrowserItem*> items;
    for(size_t i = 0; i < tmp.size(); ++i) {
        auto newChild = new QUaBrowserItem(tmp[i], result[i].browseName);
        if(result[i].nodeClass == UaNodeClass::cVariable) {
            newChild->setIcon(0, varIcon);
        }
        else {
            newChild->setFlags(newChild->flags() ^ Qt::ItemIsDragEnabled);
        }
        items.append(newChild);
    }
    return items;
}

bool QUaBrowser::isConnected() const
{
    return connection->isConnected();
}

bool QUaBrowser::browserInit()
{
    status = createBrowser(connection, browser);
    return status == uaGood;
}


void QUaBrowser::directoryOpened(QTreeWidgetItem* current)
{
    auto item = dynamic_cast<QUaBrowserItem*>(current);
    if(item->childCount() == 0) {
        item->addChildren(readCurrentDir(item->id));
    }
    for(int i = 0; i < item->childCount(); ++i) {
        auto child = item->child(i);
        if(child->childCount() == 0 ) {
            child->addChildren(readCurrentDir(child->id));
        }
    }
}
ELESYOPCUACLIENT_END
