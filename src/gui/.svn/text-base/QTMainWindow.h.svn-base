/*
 * QTMainWindow.h
 *
 *  Created on: Dec 19, 2010
 *      Author: lionis
 */

#ifndef QTMAINWINDOW_H_
#define QTMAINWINDOW_H_

#include <QWidget>
#include <QMenuBar>
#include <QMainWindow>
#include <QtGui>
#include "QTWindow.h"

class QCheckBox;
class QSlider;
class GLWidget;
class QMenu;
class QAction;
class QTMainWindow: public QMainWindow {
	Q_OBJECT

private:
    QTWindow *window;
	QAction *openAct, *exitAct, *aboutAct;
	QMenu *fileMenu, *helpMenu;
	void createMenus();
	void createActions();

private slots:
	void open();
	void about();

public:
	QTMainWindow();
	virtual ~QTMainWindow();
};

#endif /* QTMAINWINDOW_H_ */
