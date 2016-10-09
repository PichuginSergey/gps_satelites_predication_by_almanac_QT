#include <QTextStream>
#include <future>
#include "almPredicationWidget.h"
#include "service.h"

AlmPredWidget::AlmPredWidget() {

    this->edit = new QTextEdit();
    edit->setReadOnly(true);
    layout = new QHBoxLayout;
    layout->addWidget(edit);
    setLayout(layout);
    setFrameStyle(QFrame::Box|QFrame::Sunken);
}

void AlmPredWidget::display(unsigned int angle_mask, GNSS& satellites){

    QString str;
    QTextStream stream(&str);
    stream << "PRN" << "  " << "AZI" << " " << "ELV" << '\n';
    for (auto& sat : satellites)
    {
        double elv = floor(sat.ang.elv*180/Pi + 0.5);
        double azi = floor(sat.ang.azi*180/Pi + 0.5);
        if (sat.ang.vflg &&
            sat.ang.elv > 0 && elv >= angle_mask)
                stream << sat.get_prn() << "  " << azi << " " << elv << '\n';
    }
    edit->setPlainText(str);
    show();
}

