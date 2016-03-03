/*
 * QTWindow.h
 *
 *  Created on: Dec 6, 2010
 *      Author: lionis
 */

#ifndef QTWINDOW_H_
#define QTWINDOW_H_

#include <QWidget>
#include <QMenuBar>
#include <QMainWindow>
#include <QtGui>
#include <string.h>
#include "GLWidget.h"
using namespace std;

class QCheckBox;
class QSlider;
class GLWidget;
class QMenu;
class QAction;
class QTWindow: public QWidget {
	Q_OBJECT

public:
	QTWindow();
	~QTWindow();
	void initWindow(string DATfile);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QGridLayout *gridLayout;
	QVBoxLayout *layout00, *layout01, *layout10, *layout11;
	QHBoxLayout *layoutAxial, *layoutSagittal, *layoutCoronal;
	QHBoxLayout *layoutAxialCheckBoxes, *layoutSagittalCheckBoxes, *layoutCoronalCheckBoxes;
	QSlider *createRotationSlider();
	QSlider *createVolumeSlider();
	QCheckBox *createCheckBox(char *label);
	GLWidget *glWidget[4], *currentGLWidget;
	QSlider *xSlider, *ySlider, *zSlider;
	QSlider *axialSlider, *sagittalSlider, *coronalSlider;
	QCheckBox *drawAxial, *drawSagittal, *drawCoronal;
	QCheckBox *axialClip, *sagittalClip, *coronalClip;

private slots:
    void setCurrentGlWidget();
};

#endif /* QTWINDOW_H_ */
