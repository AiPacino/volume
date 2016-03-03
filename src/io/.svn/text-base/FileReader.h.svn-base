/*
 * FileReader.h
 *
 *  Created on: Nov 30, 2010
 *      Author: lionis
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#define DAT_FILE "../volumes/head256.dat"
#define DAT_FILE2 "../volumes/aorta_pescoco.dat"
#define DAT_FILE3 "../volumes/tumor_rim.dat"
#define DAT_FILE4 "../volumes/aorta_pescoco.dat"
#define DAT_FILE5 "../volumes/mancha_cerebro.dat"
#define DAT_FILE6 "../volumes/traumatismo_cabeca.dat"
#define DAT_FILE7 "../volumes/tumor_figado.dat"
#define FRAG_RAY_SRC "src/gl/fragment_shader_raycasting.c"
#define Xcoord 0
#define Ycoord 1
#define Zcoord 2
#define UCHAR	0x1401
#define USHORT	0x1403

//#include <GL/glext.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
using namespace std;

class FileReader {
private:
	ifstream::pos_type size;
	string RAWfilename;
	static unsigned int VOLWIDTH, VOLHEIGHT, VOLDEPTH, maxDim;
	static float sliceThickness[3], maxThickness;
	static unsigned int BITSALLOCATED;
public:
	string trim(const std::string &str);
	char *readRAW();
	char *read(char *filename);
	void readDAT(char *filename);
	FileReader();
    unsigned int getVOLWIDTH();
    unsigned int getVOLHEIGHT();
    unsigned int getVOLDEPTH();
	float getXSliceThickness();
	float getYSliceThickness();
	float getZSliceThickness();
	int getBITSALLOCATED();
	unsigned int getMaxDim();
	float getMaxThickness();
};

#endif /* FILEREADER_H_ */
