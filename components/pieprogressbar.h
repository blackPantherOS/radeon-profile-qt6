
// copyright marazmista @ 23.05.2017

#ifndef PIEPROGRESSBAR_H
#define PIEPROGRESSBAR_H

#include <QWidget>
#include <QtCharts>
#include "globalStuff.h"
#include "ui_pieprogressbar.h"

#if QT_VERSION <= 0x060000
using namespace QtCharts;
#endif

namespace Ui {
class PieProgressBar;
}


class PieProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit PieProgressBar(QWidget *parent = nullptr) : QWidget(parent),
        ui(new Ui::PieProgressBar),
        data(new QPieSeries(this)),
        chart(new QChart),
        chartView(new QChartView(chart, this))
    {
        ui->setupUi(this);
        init();
    }

    explicit PieProgressBar(const int max, ValueID id, QColor fillColor, QWidget *parent = nullptr) : QWidget(parent),
        ui(new Ui::PieProgressBar),
        data(new QPieSeries(this)),
        chart(new QChart),
        chartView(new QChartView(chart, this))
    {
        ui->setupUi(this);

        maxValue = max;
        dataId = id;
        fill = fillColor;
        bg = QWidget::palette().color(QWidget::backgroundRole());
        init();
    }

    ~PieProgressBar() {
        delete chart;
        delete ui;
    }

    void updateValue(const GPUDataContainer &gpuData) {
        data->slices().at(1)->setValue(gpuData.value(dataId).value);
        data->slices().at(2)->setValue(maxValue - gpuData.value(dataId).value);
        primaryLabel.setText(gpuData.value(dataId).strValue);

        if (secondaryDataIdEnabled)
            secondaryLabel.setText(gpuData.value(secondaryDataId).strValue);
    }

    void setFillColor(const QColor &c) {
        fill = c;
        data->slices().at(1)->setBrush(fill);
    }

    void setSecondaryDataId(const ValueID id) {
        secondaryDataIdEnabled = true;
        secondaryDataId = id;
    }

    QColor getFillColor() {
        return fill;
    }

protected:
    Ui::PieProgressBar *ui;
    int maxValue = 100;
    bool secondaryDataIdEnabled = false;
    ValueID dataId, secondaryDataId;

    QPieSeries *data;
    QChart *chart;
    QChartView *chartView;
    QLabel primaryLabel, secondaryLabel;
    QColor fill, bg;

    void init() {
        QFont f;
        f.setFamily("Monospace");
        f.setPointSize(8);
        primaryLabel.setFont(f);
        secondaryLabel.setFont(f);

        QPalette p = this->palette();
        setAutoFillBackground(true);
        //p.setColor(QPalette::Background, Qt::black);
        p.setColor(QPalette::Window, Qt::black);

        data->setPieStartAngle(-250);
        data->setPieEndAngle(90);
//        data.setPieStartAngle(-210);
//        data.setPieEndAngle(130);
        data->setHoleSize(0.32);

        data->append("",maxValue / 3);
        data->append("Usage", 0);
        data->append("", maxValue);

        data->slices().at(0)->setBrush(bg);
        data->slices().at(0)->setPen(QPen(bg, 0));
        data->slices().at(1)->setPen(QPen(bg, 0));
        data->slices().at(2)->setPen(QPen(bg, 0));
        data->slices().at(0)->setLabelVisible(false);
        data->slices().at(1)->setLabelVisible(false);
        data->slices().at(2)->setLabelVisible(false);
        data->slices().at(1)->setBrush(fill);
        data->slices().at(2)->setBrush(Qt::darkGray);

        chart->addSeries(data);
        chart->legend()->setVisible(false);
        chart->setBackgroundVisible(false);

        chart->setMargins(QMargins(-18,-18,-18,-18));
        chart->setContentsMargins(0,0,0,0);
        chart->setMinimumSize(0,0);

        chartView->setMinimumSize(0,0);
        chartView->setRenderHint(QPainter::Antialiasing);

        primaryLabel.setContentsMargins(QMargins(0,18,0,0));
        secondaryLabel.setContentsMargins(QMargins(0,43,0,0));
        ui->grid->addWidget(chartView,0,0,Qt::AlignCenter);
        ui->grid->addWidget(&primaryLabel,0,0,Qt::AlignRight);
        ui->grid->addWidget(&secondaryLabel,0,0,Qt::AlignRight);
    }

};

#endif // PIEPROGRESSBAR_H
