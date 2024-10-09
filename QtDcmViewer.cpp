//
// Created by dhz on 24-10-9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_QtDcmViewer.h" resolved

#include "QtDcmViewer.h"
#include "ui_QtDcmViewer.h"


QtDcmViewer::QtDcmViewer()   {
    this->ui = new Ui::QtDcmViewer;
    this->ui->setupUi(this);

}

QtDcmViewer::~QtDcmViewer() {
    delete ui;
}
