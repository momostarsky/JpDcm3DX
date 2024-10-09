//
// Created by dhz on 24-10-9.
//

#ifndef JPDCM3DY_QTDCMVIEWER_H
#define JPDCM3DY_QTDCMVIEWER_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class QtDcmViewer; }
QT_END_NAMESPACE

class QtDcmViewer : public QMainWindow {
Q_OBJECT

public:
    explicit QtDcmViewer( );

    ~QtDcmViewer() override;

private:
    Ui::QtDcmViewer *ui;
};


#endif //JPDCM3DY_QTDCMVIEWER_H
