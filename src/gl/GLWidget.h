/*
 * GLWidget.h
 *
 *  Created on: Dec 4, 2010
 *      Author: lionis
 */

#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#define STEP 0.01
//#define TF_ENTRIES 256
//#define VOL_WIDTH 256
//#define VOL_HEIGHT 256
//#define VOL_DEPTH 225
//#define BITS_ALLOCATED GL_UNSIGNED_BYTE //GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT
#define AXIAL 0 //plane of visualization planeVis
#define SAGITTAL 1 //plane of visualization planeVis
#define CORONAL 2 //plane of visualization planeVis
#define RAYCASTING3D 3 //plane of visualization planeVis
#define VOLUME 0
#define TRANSFERFUNCTION 1
#define Xcoord 0
#define Ycoord 1
#define Zcoord 2
#define Wcoord 3
#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

#include <QtOpenGL/QGLWidget>
#include <GL/glext.h>
#include "FileReader.h"

class GLWidget : public QGLWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    //GLWidget(GLchar *volume, GLchar *fragShader, QGLContext *context, QWidget *parent = NULL, QGLWidget *shareWidget = 0);
    GLWidget(GLint planeVis, QGLWidget *shareWidget, QWidget *parent = NULL);
    ~GLWidget();

    void loadDATfile(GLchar *volumeDATfilename, GLchar *fragShader);
/*
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
*/

    FileReader getFileReader();

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setAxialDepth(int depth);
    void setSagittalDepth(int depth);
    void setCoronalDepth(int depth);
    void enableAxialPlane(int flag);
    void enableAxialClipping(int flag);
    void enableSagittalPlane(int flag);
    void enableSagittalClipping(int flag);
    void enableCoronalPlane(int flag);
    void enableCoronalClipping(int flag);

signals:
	void depthAxialChanged(int ticks);//o evento é transformado em ticks do Slider
	void depthSagittalChanged(int ticks);//o evento é transformado em ticks do Slider
	void depthCoronalChanged(int ticks);//o evento é transformado em ticks do Slider
/*
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
*/

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    GLint xRot, yRot, zRot;
    GLint planeVis;//3D, AXIAL, SAGITTAL, CORONAL
    QPoint lastPos;
    GLchar *volumeFilename, *fragShaderFilename;
    GLfloat cameraPosition[4], rayDirection[4];
    //GLdouble raycastingModelView[4][4];
    //GLfloat axialDepth, sagittalDepth, coronalDepth;
	GLubyte *volume, tfRGBA[GL_MAX_TEXTURE_SIZE][4]; //8192 = max texture resolution
	static GLuint texId[2]; // texture IDs
	static GLfloat cursorPosition[4];
	GLuint programObject; // a handler to the GLSL program used to update
	GLuint fragmentShader; // a handler to fragment shader. This is used internally too
	//static GLuint fpsCurrent, fpsCount, tick;
	//GLdouble objX, objY, objZ; //3D position of object
	static GLint drawPlane[3], clippingPlane[3];

	FileReader fr;
	//GLUquadricObj *quadratic;

	void loadTextures();
	void loadVolume(GLchar *volumeDATFilename, GLuint *texname);
	void loadTransferFunction8bit(GLuint *texname);
	void loadTransferFunction16bit(GLuint *texname);

	GLfloat *pickPoint(GLfloat point4D[4], GLfloat rayDir[4]);
	void applyTransform(GLfloat matrix[4][4], GLfloat *point4D);
	void applyInverseTransform(GLfloat matrix[4][4], GLfloat *point4D);
	GLint getVolumeSample(GLfloat point3D[3]);
	void printMatrix4x4(GLfloat matrix[4][4]);
};
#endif /* GLWIDGET_H_ */
