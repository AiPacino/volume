/*
 * QTMainWindow.cpp
 *
 *  Created on: Dec 19, 2010
 *      Author: lionis
 */

#include "QTMainWindow.h"

QTMainWindow::QTMainWindow() {
	createActions();
	createMenus();

	setWindowTitle(tr("VMTK"));
	setMinimumSize(620,537);

	window = NULL;
}

QTMainWindow::~QTMainWindow() {
	// TODO Auto-generated destructor stub
}

void QTMainWindow::createMenus(){
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}

void QTMainWindow::createActions(){
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void QTMainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DAT File"), QDir::currentPath());
    if (!fileName.isEmpty()){
    	if(window)
    		window->~QTWindow();

    	window = new QTWindow();
    	window->initWindow(fileName.toStdString());
    	setCentralWidget(window);
    }
}

void QTMainWindow::about(){
    QMessageBox::about(this, tr("About VMTK"),
            tr("<p>The QT version of <b>Visualization and Manipulation ToolKit (VMTK)</b> was developed for IA369E</p>"));
}
