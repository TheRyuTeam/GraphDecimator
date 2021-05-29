#pragma once
#include "qcustomplot.h"

void messageBox(const QString& text);

QSharedPointer<QCPGraphDataContainer> getDataByKeyRange(const QSharedPointer<QCPGraphDataContainer>&, const QCPRange&);

void setRanges(QCustomPlot*, const QCPRange&, const QCPRange&);

bool replotGraph(QCustomPlot*, int, const QSharedPointer<QCPGraphDataContainer>&);
