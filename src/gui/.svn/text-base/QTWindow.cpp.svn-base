/*
 * QTWindow.cpp
 *
 *  Created on: Dec 6, 2010
 *      Author: lionis
 */

#include "QTWindow.h"

QTWindow::QTWindow() {
//	setWindowTitle(tr("VMTK"));
//	setMinimumSize(620,537);
}

QTWindow::~QTWindow() {
	// TODO Auto-generated destructor stub
	glWidget[0]->~GLWidget();
	glWidget[1]->~GLWidget();
	glWidget[2]->~GLWidget();
	glWidget[3]->~GLWidget();
	//delete [] glWidget;

	delete xSlider;
	delete ySlider;
	delete zSlider;
	delete axialSlider;
	delete sagittalSlider;
	delete coronalSlider;
	delete drawAxial;
	delete drawSagittal;
	delete drawCoronal;
	delete axialClip;
	delete sagittalClip;
	delete coronalClip;
	delete gridLayout;
/*	delete layout00;
	delete layout01;
	delete layout10;
	delete layout11;
	delete layoutAxial;
	delete layoutSagittal;
	delete layoutCoronal;
	delete layoutAxialCheckBoxes;
	delete layoutSagittalCheckBoxes;
	delete layoutCoronalCheckBoxes;
*/
}

void QTWindow::initWindow(string DATfile){
	char *DATstring;

	DATstring = (char *)DATfile.c_str();
	gridLayout = new QGridLayout;
	layout00 = new QVBoxLayout;
	layout01 = new QVBoxLayout;
	layout10 = new QVBoxLayout;
	layout11 = new QVBoxLayout;

	layoutAxial = new QHBoxLayout;
	layoutSagittal = new QHBoxLayout;
	layoutCoronal = new QHBoxLayout;

	layoutAxialCheckBoxes = new QHBoxLayout;
	layoutSagittalCheckBoxes = new QHBoxLayout;
	layoutCoronalCheckBoxes = new QHBoxLayout;

	/*
	 * Layout da janela (0,0)
	 */
	glWidget[0] = new GLWidget(RAYCASTING3D, NULL, this);
	glWidget[0]->loadDATfile(DATstring, FRAG_RAY_SRC);
	xSlider = createRotationSlider();
	ySlider = createRotationSlider();
	zSlider = createRotationSlider();

	xSlider->setValue(0);
	ySlider->setValue(0);
	zSlider->setValue(0);

	connect(xSlider, SIGNAL(valueChanged(int)), glWidget[0],
			SLOT(setXRotation(int)));
	//connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
	connect(ySlider, SIGNAL(valueChanged(int)), glWidget[0],
			SLOT(setYRotation(int)));
	//connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
	connect(zSlider, SIGNAL(valueChanged(int)), glWidget[0],
			SLOT(setZRotation(int)));
	//connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

	//Layout da janela (0,1)
	glWidget[1] = new GLWidget(AXIAL, glWidget[0]);
	axialSlider = createVolumeSlider();
	axialSlider->setValue(0);
	drawAxial = createCheckBox("Draw axial plane");
	drawAxial->setChecked(false);
	axialClip = createCheckBox("Enable axial clipping");
	axialClip->setChecked(false);
	connect(axialSlider, SIGNAL(valueChanged(int)), glWidget[1], SLOT(setAxialDepth(int)));
	connect(glWidget[1], SIGNAL(depthAxialChanged(int)), axialSlider,
			SLOT(setValue(int)));
	connect(drawAxial, SIGNAL(stateChanged(int)), glWidget[1], SLOT(enableAxialPlane(int)));
	connect(axialClip, SIGNAL(stateChanged(int)), glWidget[1], SLOT(enableAxialClipping(int)));

	//Layout da janela (1,0)
	glWidget[2] = new GLWidget(SAGITTAL, glWidget[0]);
	sagittalSlider = createVolumeSlider();
	sagittalSlider->setValue(0);
	drawSagittal = createCheckBox("Draw sagittal plane");
	drawSagittal->setChecked(false);
	sagittalClip = createCheckBox("Enable sagittal clipping");
	sagittalClip->setChecked(false);
	connect(sagittalSlider, SIGNAL(valueChanged(int)), glWidget[2], SLOT(setSagittalDepth(int)));
	connect(glWidget[2], SIGNAL(depthSagittalChanged(int)), sagittalSlider,
			SLOT(setValue(int)));
	connect(drawSagittal, SIGNAL(stateChanged(int)), glWidget[2], SLOT(enableSagittalPlane(int)));
	connect(sagittalClip, SIGNAL(stateChanged(int)), glWidget[2], SLOT(enableSagittalClipping(int)));

	//Layout da janela (1,1)
	glWidget[3] = new GLWidget(CORONAL, glWidget[0]);
	coronalSlider = createVolumeSlider();
	coronalSlider->setValue(0);
	drawCoronal = createCheckBox("Draw coronal plane");
	drawCoronal->setChecked(false);
	coronalClip = createCheckBox("Enable coronal clipping");
	coronalClip->setChecked(false);
	connect(coronalSlider, SIGNAL(valueChanged(int)), glWidget[3], SLOT(setCoronalDepth(int)));
	connect(glWidget[3], SIGNAL(depthCoronalChanged(int)), coronalSlider,
			SLOT(setValue(int)));
	connect(drawCoronal, SIGNAL(stateChanged(int)), glWidget[3], SLOT(enableCoronalPlane(int)));
	connect(coronalClip, SIGNAL(stateChanged(int)), glWidget[3], SLOT(enableCoronalClipping(int)));

	/*
	 * Montagem das janelas na tela final
	 */
/*
	connect(glWidget[0], SIGNAL(clicked()), this, SLOT(setCurrentGlWidget()));
	connect(glWidget[1], SIGNAL(clicked()), this, SLOT(setCurrentGlWidget()));
	connect(glWidget[2], SIGNAL(clicked()), this, SLOT(setCurrentGlWidget()));
	connect(glWidget[3], SIGNAL(clicked()), this, SLOT(setCurrentGlWidget()));
*/

	layoutAxialCheckBoxes->addWidget(drawAxial);
	layoutAxialCheckBoxes->addWidget(axialClip);
	layoutSagittalCheckBoxes->addWidget(drawSagittal);
	layoutSagittalCheckBoxes->addWidget(sagittalClip);
	layoutCoronalCheckBoxes->addWidget(drawCoronal);
	layoutCoronalCheckBoxes->addWidget(coronalClip);

	layoutAxial->addWidget(glWidget[1]);
	layoutAxial->addWidget(axialSlider);

	layoutSagittal->addWidget(glWidget[2]);
	layoutSagittal->addWidget(sagittalSlider);

	layoutCoronal->addWidget(glWidget[3]);
	layoutCoronal->addWidget(coronalSlider);

	layout00->addWidget(glWidget[0]);
	layout00->addWidget(xSlider);
	layout00->addWidget(ySlider);
	layout00->addWidget(zSlider);

	layout01->addLayout(layoutAxial);
	layout01->addLayout(layoutAxialCheckBoxes);

	layout10->addLayout(layoutSagittal);
	layout10->addLayout(layoutSagittalCheckBoxes);

	layout11->addLayout(layoutCoronal);
	layout11->addLayout(layoutCoronalCheckBoxes);

	gridLayout->addLayout(layout00, 0, 0, 3, 2);
	gridLayout->addLayout(layout01, 0, 2);
	gridLayout->addLayout(layout10, 1, 2);
	gridLayout->addLayout(layout11, 2, 2);

	setLayout(gridLayout);
	currentGLWidget = glWidget[0];

/*
	setWindowTitle(tr("VMTK"));
	setMinimumSize(620,537);
*/
}

QSlider *QTWindow::createRotationSlider() {
	QSlider *slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 360);
	slider->setSingleStep(1);
	slider->setPageStep(15);
	slider->setTickInterval(15);
	slider->setTickPosition(QSlider::TicksAbove);
	return slider;
}

QSlider *QTWindow::createVolumeSlider() {
	QSlider *slider = new QSlider(Qt::Vertical);
	slider->setRange(0, glWidget[0]->getFileReader().getMaxDim());
	slider->setSingleStep(1);
	slider->setPageStep(50);
	slider->setTickInterval(50);
	slider->setTickPosition(QSlider::TicksRight);
	return slider;
}

QCheckBox *QTWindow::createCheckBox(char *label){
	QCheckBox *checkBox = new QCheckBox(tr(label));

	return checkBox;
}

void QTWindow::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Escape)
		close();
	else
		QWidget::keyPressEvent(e);
}

void QTWindow::setCurrentGlWidget() {
	currentGLWidget = qobject_cast<GLWidget *> (sender());
}
