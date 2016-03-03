/*
 * GLWidget.cpp
 *
 *  Created on: Dec 4, 2010
 *      Author: lionis
 */

#include <QtGui/QMouseEvent>
#include "GLWidget.h"

GLuint GLWidget::texId[2]; // texture IDs
GLfloat GLWidget::cursorPosition[4];
GLint GLWidget::drawPlane[3], GLWidget::clippingPlane[3];

GLWidget::GLWidget(GLint plane, QGLWidget *shareWidget, QWidget *parent) :
	QGLWidget(parent, shareWidget) {
	int i;

	for(i=0; i<3; i++)
		drawPlane[i] = clippingPlane[i] = 0;

	setMouseTracking(true);
	//volumeDATFilename = volume;
	planeVis = plane;

	xRot = yRot = zRot = 0;
	//axialDepth = sagittalDepth =  coronalDepth = 0;
	cursorPosition[Xcoord] = cursorPosition[Ycoord] = cursorPosition[Zcoord] = cursorPosition[Wcoord] = 0;
	//if(planeVis == RAYCASTING3D)
		startTimer(100);
}

GLWidget::~GLWidget() {
//	delete volumeFilename;
//	delete fragShaderFilename;
//	delete [] volume;
}

void GLWidget::loadDATfile(GLchar *volumeDATfilename, GLchar *fragShader){
	fragShaderFilename = fragShader;
	fr.readDAT(volumeDATfilename);
}

/*
//size hint function to ensure that the widget is shown at a reasonable size
QSize GLWidget::minimumSizeHint() const {
	return QSize(50, 50);
}

//size hint function to ensure that the widget is shown at a reasonable size
QSize GLWidget::sizeHint() const {
	return QSize(300, 300);
}
*/

/*
 void GLWidget::qNormalizeAngle(int &angle)
 {
 while (angle < 0)
 angle += 360 * 16;
 while (angle > 360 * 16)
 angle -= 360 * 16;
 }
 */

FileReader GLWidget::getFileReader(){
	return fr;
}

void GLWidget::setXRotation(int angle) {
	if (angle != xRot) {
		xRot = angle;
		//emit
		//xRotationChanged(angle);
		updateGL();
	}
}

void GLWidget::setYRotation(int angle) {
	if (angle != yRot) {
		yRot = angle;
		//emit
		//yRotationChanged(angle);
		updateGL();
	}
}

void GLWidget::setZRotation(int angle) {
	if (angle != zRot) {
		zRot = angle;
		updateGL();
	}
}

void GLWidget::setAxialDepth(int depth){
	cursorPosition[Zcoord] = (float)depth / fr.getMaxDim();
	updateGL();
}

void GLWidget::setSagittalDepth(int depth){
	cursorPosition[Xcoord] = (float)depth / fr.getMaxDim();
	updateGL();
}

void GLWidget::setCoronalDepth(int depth){
	cursorPosition[Ycoord] = (float)depth / fr.getMaxDim();
	updateGL();
}

void GLWidget::enableAxialPlane(int flag){
	drawPlane[AXIAL] = flag;
}

void GLWidget::enableAxialClipping(int flag){
	clippingPlane[AXIAL] = flag;
}

void GLWidget::enableSagittalPlane(int flag){
	drawPlane[SAGITTAL] = flag;
}

void GLWidget::enableSagittalClipping(int flag){
	clippingPlane[SAGITTAL] = flag;
}

void GLWidget::enableCoronalPlane(int flag){
	drawPlane[CORONAL] = flag;
}

void GLWidget::enableCoronalClipping(int flag){
	clippingPlane[CORONAL] = flag;
}

void GLWidget::initializeGL() {

	cameraPosition[Xcoord] = 0.0f;
	cameraPosition[Ycoord] = 0.0f;
	cameraPosition[Zcoord] = 1.0f;
	cameraPosition[Wcoord] = 0.0f;

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // our texture colors will replace the untextured colors
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 2, -2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], 0, 0, 0,
			0, 1, 0);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	if(planeVis == RAYCASTING3D){
		GLint prog_link_success;
		GLchar *fragSrc;

		//fpsCurrent = 0;
		//fpsCount = 0;


		//FRAGMENT SHADER//

		programObject = glCreateProgram(); // creating a program object
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // creating a fragment shader object

		glDisable(GL_BLEND);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // our texture colors will replace the untextured colors
		glEnable(GL_DEPTH_TEST);
		fragSrc = (GLchar *) fr.read(fragShaderFilename);

		//	 quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object (NEW )
		//	 gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals (NEW )
		//	 gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords (NEW )


		glShaderSource(fragmentShader, 1, (const GLchar**) &fragSrc, NULL); // assigning the fragment source

		glCompileShader(fragmentShader);// compiling
		glAttachShader(programObject, fragmentShader);// and attaching the fragment shader onto program

		glLinkProgram(programObject);// Link the shaders into a complete GLSL program.
		glUseProgram(programObject);// Use The Program Object Instead Of Fixed Function OpenGL

		// some extra code for checking if all this initialization is ok
		glGetObjectParameterivARB(programObject, GL_OBJECT_LINK_STATUS_ARB,
				&prog_link_success);
		if (!prog_link_success) {
			fprintf(stderr, "The shaders could not be linked\n");
			exit(1);
		}

		glUniform1i(glGetUniformLocation(programObject, "VOLUME"), 0);//get uniform location for samplers
		glUniform1i(glGetUniformLocation(programObject, "TF"), 1);
		glUniform4fv(glGetUniformLocation(programObject, "cameraPosition"), 1, cameraPosition);

		loadTextures();
	}
	else{
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_1D, texId[TRANSFERFUNCTION]);
		glBindTexture(GL_TEXTURE_3D, texId[VOLUME]);
		glEnable(GL_TEXTURE_3D);
	}

	//setLight();
	//setMaterial();

	//fpsCount = 0;
	//fpsCurrent = 0;
}

void GLWidget::paintGL() {
	float x, y, z;

//	x = ((float)fr.getVOLWIDTH()/fr.getMaxDim())*(fr.getMaxThickness()/fr.getXSliceThickness());
//	y = ((float)fr.getVOLHEIGHT()/fr.getMaxDim())*(fr.getMaxThickness()/fr.getYSliceThickness());
//	z = ((float)fr.getVOLDEPTH()/fr.getMaxDim())*(fr.getMaxThickness()/fr.getZSliceThickness());

//	x = ((float)fr.getMaxDim()/fr.getVOLWIDTH())*(fr.getXSliceThickness()/fr.getMaxThickness());
//	y = ((float)fr.getMaxDim()/fr.getVOLHEIGHT())*(fr.getYSliceThickness()/fr.getMaxThickness());
//	z = ((float)fr.getMaxDim()/fr.getVOLDEPTH())*(fr.getZSliceThickness()/fr.getMaxThickness());

	x = y = z = 1.0f;

/*
	cout << "GLWidget::paintGL xyz=(" << x << "," << y << "," << z << ")"
			<< " thick=(" << fr.getXSliceThickness()/fr.getMaxThickness() << "," << fr.getYSliceThickness()/fr.getMaxThickness() << "," << fr.getZSliceThickness()/fr.getMaxThickness() << ")"
			<< " dim=(" << (float)fr.getMaxDim()/fr.getVOLWIDTH() << "," << (float)fr.getMaxDim()/fr.getVOLHEIGHT() << "," << (float)fr.getMaxDim()/fr.getVOLDEPTH() << ")" << endl;
*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(planeVis == RAYCASTING3D){

		glRotatef(xRot, 1.0, 0.0, 0.0);
		glRotatef(yRot, 0.0, 1.0, 0.0);
		glRotatef(zRot, 0.0, 0.0, 1.0);

		glUniform4fv(glGetUniformLocation(programObject, "cursorPosition"), 1, cursorPosition);
		glUniform3iv(glGetUniformLocation(programObject, "drawPlane"), 1, drawPlane);
		glUniform3iv(glGetUniformLocation(programObject, "clippingPlane"), 1, clippingPlane);

		glBegin(GL_QUAD_STRIP);
		glTexCoord3f(0.0f, 0.0f, 0.0f);//1
		glVertex3f(-x, -y, -z);
		glTexCoord3f(0.0f, 1.0f, 0.0f);//2
		glVertex3f(-x, y, -z);
		glTexCoord3f(1.0f, 0.0f, 0.0f);//3
		glVertex3f(x, -y, -z);
		glTexCoord3f(1.0f, 1.0f, 0.0f);//4
		glVertex3f(x, y, -z);

		glTexCoord3f(1.0f, 0.0f, 1.0f);//5
		glVertex3f(x, -y, z);
		glTexCoord3f(1.0f, 1.0f, 1.0f);//6
		glVertex3f(x, y, z);

		glTexCoord3f(0.0f, 0.0f, 1.0f);//7
		glVertex3f(-x, -y, z);
		glTexCoord3f(0.0f, 1.0f, 1.0f);//8
		glVertex3f(-x, y, z);

		glTexCoord3f(0.0f, 0.0f, 0.0f);//9
		glVertex3f(-x, -y, -z);
		glTexCoord3f(0.0f, 1.0f, 0.0f);//10
		glVertex3f(-x, y, -z);
		glEnd();

		glBegin(GL_QUADS);
		glTexCoord3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-x, y, -z);
		glTexCoord3f(0.0f, 1.0f, 1.0f);
		glVertex3f(-x, y, z);
		glTexCoord3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, z);
		glTexCoord3f(1.0f, 1.0f, 0.0f);
		glVertex3f(x, y, -z);

		glTexCoord3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-x, -y, -z);
		glTexCoord3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-x, -y, z);
		glTexCoord3f(1.0f, 0.0f, 1.0f);
		glVertex3f(x, -y, z);
		glTexCoord3f(1.0f, 0.0f, 0.0f);
		glVertex3f(x, -y, -z);
		glEnd();
		//fpsCurrent++;


		//	 glPushMatrix();
		//	 //glRotatef(90., 1., 0., 0.);
		//	 glTranslatef(0.3f,0,0);
		//	 gluCylinder(quadratic,0.2f,0.0f,0.5f,32,32);
		//	 glPopMatrix();

		//cout << "GLWidget::paintGL RAYCASTING3D sharing=" << this->isSharing() << " valid=" << this->isValid() << endl;
	}
	else if(planeVis == AXIAL){
		glBegin(GL_QUADS);
		glTexCoord3f(1.0f, 1.0f, cursorPosition[Zcoord]);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord3f(1.0f, 0.0f, cursorPosition[Zcoord]);
		glVertex2f(-1.0f, 1.0f);
		glTexCoord3f(0.0f, 0.0f, cursorPosition[Zcoord]);
		glVertex2f(1.0f, 1.0f);
		glTexCoord3f(0.0f, 1.0f, cursorPosition[Zcoord]);
		glVertex2f(1.0f, -1.0f);

		glEnd();

		//cout << "GLWidget::paintGL AXIAL sharing=" << this->isSharing() << " valid=" << this->isValid() << endl;
	}
	else if(planeVis == SAGITTAL){
		glBegin(GL_QUADS);
		glTexCoord3f(cursorPosition[Xcoord], 1.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord3f(cursorPosition[Xcoord], 1.0f, 0.0f);
		glVertex2f(-1.0f, 1.0f);
		glTexCoord3f(cursorPosition[Xcoord], 0.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glTexCoord3f(cursorPosition[Xcoord], 0.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);

		glEnd();

		//cout << "GLWidget::paintGL SAGITTAL sharing=" << this->isSharing() << " valid=" << this->isValid() << endl;
	}
	else if(planeVis == CORONAL){
		glBegin(GL_QUADS);
		glTexCoord3f(0.0f, cursorPosition[Ycoord], 1.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord3f(0.0f, cursorPosition[Ycoord], 0.0f);
		glVertex2f(-1.0f, 1.0f);
		glTexCoord3f(1.0f, cursorPosition[Ycoord], 0.0f);
		glVertex2f(1.0f, 1.0f);
		glTexCoord3f(1.0f, cursorPosition[Ycoord], 1.0f);
		glVertex2f(1.0f, -1.0f);

		glEnd();

		//cout << "GLWidget::paintGL CORONAL sharing=" << this->isSharing() << " valid=" << this->isValid() << endl;
	}
}

void GLWidget::resizeGL(int w, int h) {
	int min;

	min = qMin(w, h);
	glMatrixMode(GL_PROJECTION);
	glViewport((w - min) / 2, (h - min) / 2, min, min);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	//lastPos = event->pos();
	cout << "GLWidget::mousePressEvent event=" << event->button() << " modifiers=" << endl;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
/*
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
*/
	GLfloat modelview[4][4];//, projection[4][4];
	GLfloat *poi;//, *cursorTemp;
	//GLint viewport[4];
	//float winZ;

	if(planeVis == RAYCASTING3D){
		//glGetIntegerv(GL_VIEWPORT, viewport);
		//updateGL();
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)modelview);
		//glGetDoublev(GL_PROJECTION_MATRIX, (GLdouble *)projection);

		//Read the window z value from the z-buffer
		//glReadPixels(event->x(), viewport[3] - event->y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		//Use the gluUnProject to get the world co-ordinates of
		//the point the user clicked and save in objx, objy, objz.
//		gluUnProject(event->x(), event->y(), winZ, (GLdouble *)modelview, (GLdouble *)projection, viewport,
//				&cursorPosition[Xcoord], &cursorPosition[Ycoord], &cursorPosition[Zcoord]);
//		gluUnProject(event->x(), event->y(), 1, (GLdouble *)modelview, (GLdouble *)projection, viewport,
//				&cursorPosition[Xcoord], &cursorPosition[Ycoord], &cursorPosition[Zcoord]);


		cursorPosition[Xcoord] = ((float)event->x()/qMin(size().width(), size().height()));
		cursorPosition[Ycoord] = 1.0f - ((float)event->y()/qMin(size().width(), size().height()));
		//cursorPosition[Zcoord] = 1.0f;
		cursorPosition[Wcoord] = 0.0f;

		cursorPosition[Xcoord] = qMin(cursorPosition[Xcoord], 1.0f);
		cursorPosition[Xcoord] = qMax(cursorPosition[Xcoord], 0.0f);
		cursorPosition[Ycoord] = qMin(cursorPosition[Ycoord], 1.0f);
		cursorPosition[Ycoord] = qMax(cursorPosition[Ycoord], 0.0f);

		rayDirection[Xcoord] = 0.0f - cameraPosition[Xcoord];
		rayDirection[Ycoord] = 0.0f - cameraPosition[Ycoord];
		rayDirection[Zcoord] = 0.0f - cameraPosition[Zcoord];
		rayDirection[Wcoord] = 0.0f;


/*
		cout << "GLWidget::mouseMoveEvent  ANTES cursorPosition=(" << cursorPosition[0] << "," << cursorPosition[1] << "," << cursorPosition[2] << ")"
		<< " rayDirection=(" << rayDirection[0] << "," << rayDirection[1] << "," << rayDirection[2] << ")" <<
		" event=(" << event->x() << "," << event->y() << ")" << endl;

		cursorPosition[Xcoord] -= 0.5f;
		cursorPosition[Ycoord] -= 0.5f;
		cursorPosition[Zcoord] -= 0.5f;

		applyInverseTransform(modelview, cursorPosition);
		applyInverseTransform(modelview, rayDirection);

		cursorPosition[Xcoord] += 0.5f;
		cursorPosition[Ycoord] += 0.5f;
		cursorPosition[Zcoord] += 0.5f;

		//poi = pickPoint(cursorPosition, rayDirection);//TODO

		cout << "GLWidget::mouseMoveEvent DEPOIS cursorPosition=(" << cursorPosition[0] << "," << cursorPosition[1] << "," << cursorPosition[2] << ")"
		<< " rayDirection=(" << rayDirection[0] << "," << rayDirection[1] << "," << rayDirection[2] << ")"
		//<< " poi=(" << poi[0] << "," << poi[1] << "," << poi[2] << ")"
		<< " event=(" << event->x() << "," << event->y() << ")" << endl;
*/

	}
}

void GLWidget::wheelEvent(QWheelEvent *event){
	if(planeVis == RAYCASTING3D){
		if(event->delta() > 0 && cursorPosition[Zcoord] < 1.0f)
			cursorPosition[Zcoord] += 1.0f/fr.getMaxDim();
		else if(cursorPosition[Zcoord] > 0.0f)
			cursorPosition[Zcoord] -= 1.0f/fr.getMaxDim();

		emit depthCoronalChanged(cursorPosition[Zcoord]*fr.getMaxDim());
	}
	else if(planeVis == AXIAL){
		if(event->delta() > 0 && cursorPosition[Zcoord] < 1.0f)
			cursorPosition[Zcoord] += 1.0f/fr.getMaxDim();
		else if(cursorPosition[Zcoord] > 0)
			cursorPosition[Zcoord] -= 1.0f/fr.getMaxDim();

		emit depthAxialChanged((cursorPosition[Zcoord]*fr.getMaxDim()));
	}
	else if(planeVis == SAGITTAL){
		if(event->delta() > 0 && cursorPosition[Xcoord] < 1.0f)
			cursorPosition[Xcoord] += 1.0f/fr.getMaxDim();
		else if(cursorPosition[Xcoord] > 0)
			cursorPosition[Xcoord] -= 1.0f/fr.getMaxDim();

		emit depthSagittalChanged(cursorPosition[Xcoord]*fr.getMaxDim());
	}
	else if(planeVis == CORONAL){
		if(event->delta() > 0 && cursorPosition[Ycoord] < 1.0f)
			cursorPosition[Ycoord] += 1.0f/fr.getMaxDim();
		else if(cursorPosition[Ycoord] > 0)
			cursorPosition[Ycoord] -= 1.0f/fr.getMaxDim();

		emit depthCoronalChanged(cursorPosition[Ycoord]*fr.getMaxDim());
	}

/*
	cout << "GLWidget::wheelEvent delta=" << event->delta() << " cursorPosition=(" << cursorPosition[Xcoord]
	<< "," << cursorPosition[Ycoord] << "," << cursorPosition[Zcoord] << ")" << endl;
*/
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_Escape:
		close();
		break;
	default:
		event->ignore();
		break;
	}
	updateGL();
}

void GLWidget::timerEvent(QTimerEvent *event){
	//if(planeVis == RAYCASTING3D)
	updateGL();
}

void GLWidget::loadTextures() {
	glGenTextures(2, texId);//request 2 textures name from OpenGL

	if(fr.getBITSALLOCATED() == GL_UNSIGNED_BYTE)
		loadTransferFunction8bit(&texId[TRANSFERFUNCTION]);
	else if(fr.getBITSALLOCATED() == GL_UNSIGNED_SHORT)
		loadTransferFunction16bit(&texId[TRANSFERFUNCTION]);
	loadVolume(volumeFilename, &texId[VOLUME]);
}

void GLWidget::loadTransferFunction8bit(GLuint *texname) {
	int i;

	//Generate Transfer Function RGB
	for (i = 0; i < 50; i++) {
		tfRGBA[i][0] = (GLubyte) (0.0);
		tfRGBA[i][1] = (GLubyte) (0.0);
		tfRGBA[i][2] = (GLubyte) (0.0);
		tfRGBA[i][3] = (GLubyte) (0.0);
	}
	for (i = 50; i < 100; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.40);
		tfRGBA[i][1] = (GLubyte) (255 * 0.20);
		tfRGBA[i][2] = (GLubyte) (255 * 0.10);
		tfRGBA[i][3] = (GLubyte) (255 * 0.1);
	}
	for (i = 100; i < 150; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.20);
		tfRGBA[i][1] = (GLubyte) (255 * 0.40);
		tfRGBA[i][2] = (GLubyte) (255 * 0.10);
		tfRGBA[i][3] = (GLubyte) (255 * 0.4);
	}
	for (i = 150; i < 200; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.20);
		tfRGBA[i][1] = (GLubyte) (255 * 0.10);
		tfRGBA[i][2] = (GLubyte) (255 * 0.40);
		tfRGBA[i][3] = (GLubyte) (255 * 0.7);
	}
	for (i = 200; i < 256; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.80);
		tfRGBA[i][1] = (GLubyte) (255 * 0.70);
		tfRGBA[i][2] = (GLubyte) (255 * 0.60);
		tfRGBA[i][3] = (GLubyte) (255 * 1.0);
	}
/*
	for (i = 256; i < 65536; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.30);
		tfRGBA[i][1] = (GLubyte) (255 * 0.40);
		tfRGBA[i][2] = (GLubyte) (255 * 0.30);
		tfRGBA[i][3] = (GLubyte) (255 * 1.0);
	}
*/

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, *texname);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			tfRGBA);

}

void GLWidget::loadTransferFunction16bit(GLuint *texname) {
	int i;

	//Generate Transfer Function RGB
	for (i = 0; i < 2048; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.0);
		tfRGBA[i][1] = (GLubyte) (255 * 0.0);
		tfRGBA[i][2] = (GLubyte) (255 * 0.0);
		tfRGBA[i][3] = (GLubyte) (255 * 0.0);
	}
/*
	for (i = 0; i < 60; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * 0.0);
		tfRGBA[i][1] = (GLubyte) (255 * 0.0);
		tfRGBA[i][2] = (GLubyte) (255 * 0.0);
		tfRGBA[i][3] = (GLubyte) (255 * 0.0);
	}
	for (i = 60; i < 2048; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * ((float)(i-60))/(2048-60));
		tfRGBA[i][1] = (GLubyte) (255 * ((float)(i-60))/(2048-60));
		tfRGBA[i][2] = (GLubyte) (255 * ((float)(i-60))/(2048-60));
		tfRGBA[i][3] = (GLubyte) (255 * 0.5);
	}
*/
	for (i = 60; i < 2048; i++) {
		tfRGBA[i][0] = (GLubyte) (255 * ((float)(i-60))/(2048-60));
		tfRGBA[i][1] = (GLubyte) (255 * ((float)(i-60))/(2048-60));
		tfRGBA[i][2] = (GLubyte) (255 * ((float)(i-60))/(2048-60));
		tfRGBA[i][3] = (GLubyte) (255 * 0.5);
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, *texname);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			tfRGBA);

}

void GLWidget::loadVolume(GLchar *volumeDATFilename, GLuint *texname) {
	volume = (GLubyte *) fr.readRAW();

	// tell OpenGL we're going to be setting up the texture name it gave us
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, *texname);
	// when this texture needs to be shrunk to fit on small polygons, use linear interpolation of the volume to determine the color
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// when this texture needs to be magnified to fit on a big polygon, use linear interpolation of the volume to determine the color
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// we want the texture to repeat over the S axis, so if we specify coordinates out of range we still get textured.
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	// same as above for T axis
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// same as above for R axis
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	// this is a 3d texture, level 0 (max detail), GL should store it in RGB8 format, its WIDTHxHEIGHTxDEPTH in size,
	// it doesnt have a border, we're giving it to GL in RGB format as a series of unsigned bytes, and volume is where the texel data is.
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, fr.getVOLWIDTH(), fr.getVOLHEIGHT(),
			fr.getVOLDEPTH(), 0, GL_LUMINANCE, fr.getBITSALLOCATED(), volume);
	glEnable(GL_TEXTURE_3D);
}

GLfloat *GLWidget::pickPoint(GLfloat point4D[4], GLfloat rayDir[4]){//TODO
	GLint i, j, sample, tfsample[4];
	GLfloat *vtemp;

	vtemp = new GLfloat[3];

	for(i=0; i<3; i++)
		vtemp[i] = point4D[i];
	//cout << "GLWidget::pickNearestNonTransparentPoint3D vtemp=(" << vtemp[0] << "," << vtemp[1] << "," << vtemp[2] << ")" << endl;
/*
	cout << "GLWidget::pickNearestNonTransparentPoint3D STEP vtemp=("
			<< vtemp[Xcoord] << ","	<< vtemp[Ycoord] << "," << vtemp[Zcoord] << ")"
			<< " rayDir=(" << rayDir[Xcoord] << ","	<< rayDir[Ycoord] << "," << rayDir[Zcoord] << ")"
			<< " sample=" << sample // << " alpha=" << tfRGBA8bit[getVolumeSample(vtemp)][ALPHA]
			<< endl;
*/
	for(i=0; i<fr.getMaxDim(); i++){
/*
		if(vtemp[Xcoord] < 0.0f || vtemp[Xcoord] > 1.0f
				|| vtemp[Ycoord] < 0.0f || vtemp[Ycoord] > 1.0f
				|| vtemp[Zcoord] < 0.0f || vtemp[Zcoord] > 1.5f){
			//vtemp[Xcoord] = vtemp[Ycoord] = vtemp[Zcoord] = -3.0f;
			cout << "GLWidget::pickNearestNonTransparentPoint3D SAIU MinMaxExtents"
					<< " vtemp=[" << vtemp[0] << "," << vtemp[1] << "," << vtemp[2] << ")" << endl;
			break;
		}
		else{
*/
			sample = getVolumeSample(vtemp);
			for(j=0; j<4; j++)
				tfsample[j] = tfRGBA[sample][j];

			if(tfsample[ALPHA] > 0.0f){
				//cout << "GLWidget::pickNearestNonTransparentPoint3D SAMPLE=" << sample << " ALPHA=" << tfsample[ALPHA] << endl;
				break;
			}
//		}

		vtemp[Xcoord] += rayDir[Xcoord]*STEP;
		vtemp[Ycoord] += rayDir[Ycoord]*STEP;
		vtemp[Zcoord] += rayDir[Zcoord]*STEP;


		cout << "GLWidget::pickNearestNonTransparentPoint3D i=" << i << " sample=" << sample << " TF=("
				<< tfsample[RED] << "," << tfsample[GREEN] << "," << tfsample[BLUE] << "," << tfsample[ALPHA] << ")"
				<< " vtemp=(" << vtemp[Xcoord] << "," << vtemp[Ycoord] << "," << vtemp[Zcoord] << ")"
				<< endl;

	}

	return vtemp;
}

void GLWidget::applyTransform(GLfloat matrix[4][4], GLfloat *point4D){
	GLuint i, j;
	GLfloat temp, *vtemp;

	vtemp = new GLfloat[4];
	for(i=0; i<4; i++)
		vtemp[i] = point4D[i];

	for(i=0; i<4; i++){
		temp = 0;
		for(j=0; j<4; j++)
			temp += matrix[i][j] * vtemp[j];
		point4D[i] = temp;//os vertices foram criados sem a coord homogenea
	}

	delete []vtemp;
}

void GLWidget::applyInverseTransform(GLfloat matrix[4][4], GLfloat *point4D){
	GLuint i, j;
	GLfloat temp, *vtemp;

	vtemp = new GLfloat[4];
	for(i=0; i<4; i++)
		vtemp[i] = point4D[i];

	for(i=0; i<4; i++){
		temp = 0;
		for(j=0; j<4; j++)
			temp += matrix[j][i] * vtemp[j];
		point4D[i] = temp;//os vertices foram criados sem a coord homogenea
	}

	delete []vtemp;
}

GLint GLWidget::getVolumeSample(GLfloat point3D[3]) {//TODO
	GLint x, y, z;
	GLint sample;

/*
	x = point3D[Xcoord] * fr.getVOLWIDTH()/2 + fr.getVOLWIDTH()/2; //[-1,1]
	y = point3D[Ycoord] * fr.getVOLHEIGHT()/2 + fr.getVOLHEIGHT()/2;//[-1,1]
	z = point3D[Zcoord] * fr.getVOLDEPTH()/-2 + fr.getVOLDEPTH()/2;//[2,-2] /-4
*/

/*
	x = qMin((float)point3D[Xcoord]*fr.getVOLWIDTH(), fr.getVOLWIDTH()-1.0f); //[0,1]
	x = qMax((float)point3D[Xcoord]*fr.getVOLWIDTH(), 0.0f); //[0,1]

	y = qMin((float)point3D[Ycoord]*fr.getVOLHEIGHT(), (float)fr.getVOLHEIGHT()-1.0f); //[0,1]
	y = qMax((float)point3D[Ycoord]*fr.getVOLHEIGHT(), 0.0f); //[0,1]

	z = qMin((float)-point3D[Zcoord]*fr.getVOLDEPTH() + fr.getVOLDEPTH(), fr.getVOLDEPTH()-1.0f); //[1,0]
	z = qMax((float)-point3D[Zcoord]*fr.getVOLDEPTH() + fr.getVOLDEPTH(), 0.0f); //[1,0]
*/

	x = point3D[Xcoord]*fr.getVOLWIDTH(); //[0,1]
	y = point3D[Ycoord]*fr.getVOLHEIGHT(); //[0,1]
	z = fr.getVOLDEPTH() - point3D[Zcoord]*fr.getVOLDEPTH(); //[1,0]

	if(x < 0 || x > fr.getVOLWIDTH() || y < 0 || y > fr.getVOLHEIGHT() || z < 0 || z > fr.getVOLDEPTH())
		return 0;

	sample = volume[z*fr.getVOLHEIGHT()*fr.getVOLWIDTH() + y*fr.getVOLWIDTH() + x];

	cout << "GLWidget::getVolumeSample point3D=(" << point3D[Xcoord] << "," << point3D[Ycoord] << "," << point3D[Zcoord]
    << ") xyz=(" << x << "," << y << "," << z << ")"
    << " sample=" << sample << endl;

	return sample;
}

void GLWidget::printMatrix4x4(GLfloat matrix[4][4]){
	GLuint i, j;

	cout << "GLWidget::printMatrix4x4" << endl;
	for(i=0; i<4; i++){
		cout << "[";
		for(j=0; j<4; j++){
			cout << matrix[i][j] << "  ";
		}
		cout << "]" << endl;
	}

}
