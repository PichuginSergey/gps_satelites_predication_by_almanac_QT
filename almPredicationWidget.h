#ifndef ALMPREDICATIONWIDGET_H
#define ALMPREDICATIONWIDGET_H

#include <QObject>
#include <QFrame>
#include <QTextEdit>
#include <QHBoxLayout>
#include "position.h"
#include "satellite.h"
#include "sv_time.h"
#include "gpsAlmanac.h"
#include "glonassAlmanac.h"

class AlmPredWidget : public QFrame {

public:
    AlmPredWidget();
    ~AlmPredWidget() {
        delete edit;
        delete layout;
    }

    void display(unsigned int angle_mask, GNSS& sat);

private:
    QTextEdit* edit;
    QHBoxLayout* layout;

};
#endif // ALMPREDICATIONWIDGET_H

