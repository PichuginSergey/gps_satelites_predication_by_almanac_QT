#include "mainwindow.h"
#include <future>
#include <vector>
#include <QTextStream>
#include "almPredicationWidget.h"
#include "service.h"

MainWindow::MainWindow(QWidget *parent) : QFrame(parent)
{
    setWindowTitle("sv_predication");
    date = new QLineEdit();
    angle = new QLineEdit();
    angle->setText(DEFAULT_ELEVATION_ANGlE);

    label1 = new QLabel("&Date");
    label2 = new QLabel("&Angel");

    label1->setBuddy(date);
    label2->setBuddy(angle);

    calc = new QPushButton("Calculate");
    exit = new QPushButton("Exit");

    GpsAlmQCheckBox* gps = new GpsAlmQCheckBox("&GPS");
    gps->setChecked(true);
    checkBox.push_back(gps);
    GlnAlmQCheckBox* gln = new GlnAlmQCheckBox("&GLN");
    gln->setChecked(true);
    checkBox.push_back(gln);

    // Layout setup
    grid = new QGridLayout;
    grid->addWidget(gps, 0, 0);
    grid->addWidget(label1, 0, 1);
    grid->addWidget(date, 0, 2);
    grid->addWidget(gln, 1, 0);
    grid->addWidget(label2, 1, 1);
    grid->addWidget(angle, 1, 2);
    grid->addWidget(calc, 2, 0);
    grid->addWidget(exit, 2, 1);

    setLayout(grid);

    QObject::connect(calc, SIGNAL(clicked()),
                     this, SLOT(pred_alm()));

    get_cur_pos("position.ini", pos);
    get_current_time(cur_time);

    QString str;
    QTextStream stream(&str);
    stream << cur_time.get_day() << '.' << cur_time.get_month() <<
              '.' << cur_time.get_year() << ' ' <<
              cur_time.get_hour() << ':' << cur_time.get_min();
    date->setText(str);
    date->setReadOnly(true);

}

void MainWindow::pred_alm() {
    std::vector<std::shared_ptr<sv_pred>> pred_task;
    for (const AlmQCheckBox* box : checkBox)
    {
        if (box->isChecked())
        pred_task.push_back(box->createSvPred(pos, cur_time));
    }

    for (auto& elm :  pred_task)
    {
        elm->task_.get();
        AlmPredWidget* gps_wgt = getAlmPredWidget(elm->msg_);
        gps_wgt->display(angle->text().toInt(), *elm->sat_);
    }
}

AlmPredWidget* MainWindow::getAlmPredWidget(const QString& title){
    for (auto& elm :  almWidget) {
        if (elm->windowTitle() == title)
            return elm;
    }
    AlmPredWidget* alm_wgt = new AlmPredWidget;
    alm_wgt->setWindowTitle(title);
    almWidget.push_back(alm_wgt);
    return alm_wgt;
}

MainWindow::~MainWindow()
{
    delete date;
    delete angle;
    delete label1;
    delete label2;
    delete grid;
    for (auto& elm : checkBox)
        delete elm;
    for (auto& elm : almWidget)
        delete elm;
}


std::shared_ptr<sv_pred> GpsAlmQCheckBox::createSvPred(const Position& pos,
                                                       const Time& cur_time) const {
     auto alm_pred_gps = std::make_shared<sv_pred>("GPS alm", new GpsAlm);
     ReadYumaGpsAlm    s;
     alm_pred_gps->alm_->setStrategy(&s);
     alm_pred_gps->alm_->read_alm("MCCT_160825.agp");
     alm_pred_gps->task_ = std::async(Alm_predication_task(pos, cur_time, *alm_pred_gps->alm_, *alm_pred_gps->sat_));
     return alm_pred_gps;
 }
 std::shared_ptr<sv_pred> GlnAlmQCheckBox::createSvPred(const Position& pos,
                                                        const Time& cur_time) const {
     auto alm_pred_gln = std::make_shared<sv_pred>("GLN alm", new GlonassAlm);
     ReadYumaGlnAlm    s;
     alm_pred_gln->alm_->setStrategy(&s);
     alm_pred_gln->alm_->read_alm("MCCT_160825.agl");
     alm_pred_gln->task_ = std::async(Alm_predication_task(pos, cur_time, *alm_pred_gln->alm_, *alm_pred_gln->sat_));
     return alm_pred_gln;
 }

