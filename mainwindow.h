#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QLayout>
#include <future>
#include "almpredicationwidget.h"

class AlmQCheckBox;
class MainWindow : public QFrame
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    AlmPredWidget* getAlmPredWidget(const QString& name);

    ~MainWindow();
    QPushButton* calc,* exit;
public slots:
    void pred_alm();

private:
    const char* DEFAULT_ELEVATION_ANGlE = "5";
    QLineEdit* date,* angle;
    QLabel* label1,* label2;
    QGridLayout* grid;
    std::vector<AlmPredWidget*> almWidget;
    Time	cur_time;
    Position_WGS_84 pos;
    std::vector<AlmQCheckBox*> checkBox;
};

struct sv_pred{
    sv_pred(const char* msg, Almanac* alm) :
                msg_(msg), alm_(alm), sat_(alm->generate_sv()) {}
    QString msg_;
    std::unique_ptr<Almanac> alm_;
    std::unique_ptr<GNSS> sat_;
    std::future<void> task_;

};

class AlmQCheckBox : public QCheckBox {

public:
    AlmQCheckBox(QString name) : QCheckBox(name) {}
    virtual std::shared_ptr<sv_pred> createSvPred(const Position& pos,
                                                  const Time& cur_tim) const = 0;

};

class GpsAlmQCheckBox : public AlmQCheckBox {

public:
    GpsAlmQCheckBox(QString name) : AlmQCheckBox(name) {}
    std::shared_ptr<sv_pred> createSvPred(const Position& pos,
                                         const Time& cur_time) const override;

};

class GlnAlmQCheckBox : public AlmQCheckBox {

public:
    GlnAlmQCheckBox(QString name) : AlmQCheckBox(name) {}
    std::shared_ptr<sv_pred> createSvPred(const Position& pos,
                                          const Time& cur_time) const override;

};

#endif // MAINWINDOW_H
