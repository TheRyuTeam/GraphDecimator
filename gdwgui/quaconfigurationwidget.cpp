#include "quaconfigurationwidget.h"
#include <QColorDialog>

QUaConfigurationWidget::QUaConfigurationWidget(QWidget *parent)
    : QWidget(parent)
{
    dteStart->setCalendarPopup(true);
    dteEnd->setCalendarPopup(true);
    dteStart->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    dteEnd->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    grid->addWidget(list,       0, 0, 5, 3);
    grid->addWidget(lStart,     0, 3);
    grid->addWidget(dteStart,   1, 3);
    grid->addWidget(lEnd,       2, 3);
    grid->addWidget(dteEnd,     3, 3);
    grid->addWidget(btnUpdate,  4, 3);
    list->setColumnCount(2);
    list->setSelectionBehavior(QAbstractItemView::SelectRows);
    list->setHorizontalHeaderLabels({ "nodeId", "color" });
    connect(dteStart,   &QDateTimeEdit::dateTimeChanged, dteEnd, &QDateTimeEdit::setMinimumDateTime);
    connect(dteEnd,     &QDateTimeEdit::dateTimeChanged, dteStart, &QDateTimeEdit::setMaximumDateTime);
    connect(btnUpdate,  &QPushButton::clicked, this, &QUaConfigurationWidget::buttonUpdateClicked);
    connect(list, &Tmp::colorChanged, this, &QUaConfigurationWidget::currentColorChanged);
    list->setSelectionMode(QAbstractItemView::MultiSelection);
    list->setDragDropMode(QAbstractItemView::DropOnly);
    list->setDropIndicatorShown(true);
    list->setDragDropOverwriteMode(false);
    list->setEditTriggers(QTableWidget::NoEditTriggers);
    list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    list->setColumnWidth(1, 150);
}

void QUaConfigurationWidget::keyPressEvent(QKeyEvent *event)
{
    if(list->hasFocus() && event->key() == Qt::Key_Delete) {
        std::vector<int> deletedIndexes;
        for(auto &x : list->selectedItems()) {
            auto row = list->row(x);
            deletedIndexes.push_back(row);
            list->removeRow(row);
        }
        emit graphsDeleted(deletedIndexes);
    }
}

void QUaConfigurationWidget::buttonUpdateClicked(bool checked)
{
    emit dateTimeRangeUpdated(dteStart->dateTime(), dteEnd->dateTime());
}

void QUaConfigurationWidget::currentColorChanged()
{
    emit colorChanged();
}

