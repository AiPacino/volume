/*
 * FileReader.cpp
 *
 *  Created on: Nov 30, 2010
 *      Author: lionis
 */

#include "FileReader.h"

unsigned int FileReader::VOLWIDTH, FileReader::VOLHEIGHT, FileReader::VOLDEPTH, FileReader::maxDim;
float FileReader::sliceThickness[3], FileReader::maxThickness;
unsigned int FileReader::BITSALLOCATED;

FileReader::FileReader() {
	// TODO Auto-generated constructor stub

}

string FileReader::trim(const std::string &str){
    size_t s = str.find_first_not_of(" \n\r\t");
    size_t e = str.find_last_not_of (" \n\r\t");

    if(( string::npos == s) || ( string::npos == e))
        return "";
    else
        return str.substr(s, e-s+1);
}

char *FileReader::read(char *filename) {
	char * buffer;

	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (file.is_open()) {
		size = file.tellg();
		buffer = new char[size];
		file.seekg(0, ios::beg);
		file.read(buffer, size);
		file.close();
		return buffer;
	} else{
		cout << "FileReader::read Unable to open file " << RAWfilename << endl;
		return 0;
	}
}

char *FileReader::readRAW() {
	char * buffer;

	ifstream file(RAWfilename.c_str(), ios::in | ios::binary | ios::ate);
	if (file.is_open()) {
		size = file.tellg();
		buffer = new char[size];
		file.seekg(0, ios::beg);
		file.read(buffer, size);
		file.close();
		return buffer;
	} else{
		cout << "FileReader::read Unable to open file " << RAWfilename << endl;
		return 0;
	}
}

void FileReader::readDAT(char *filename){
	//char *line;
	string line, tempStr;
	stringstream lineStream;
	ifstream DATfile(filename, ios::in);

	if (DATfile.is_open()) {
		getline(DATfile, line);
		while(!DATfile.eof()){
			if(!line.find("ObjectFileName")){
				lineStream.clear();
				lineStream.str(line);
				getline(lineStream, tempStr, ':');
				getline(lineStream, tempStr, '\n');

				RAWfilename = trim(tempStr);

				cout << "FileReader::readDAT RAWfilename=" << RAWfilename << endl;
			}
			else if(!line.find("Resolution")){
				lineStream.clear();
				lineStream.str(line);
				getline(lineStream, tempStr, ':');
				getline(lineStream, tempStr, '\n');

				tempStr = trim(tempStr);

				lineStream.clear();
				lineStream.str(tempStr);
				getline(lineStream, tempStr, ' ');
				VOLWIDTH = atof(trim(tempStr).c_str());
				maxDim = VOLWIDTH;
				getline(lineStream, tempStr, ' ');
				VOLHEIGHT = atof(trim(tempStr).c_str());
				if(maxDim < VOLHEIGHT) maxDim = VOLHEIGHT;
				getline(lineStream, tempStr, '\n');
				VOLDEPTH = atof(trim(tempStr).c_str());
				if(maxDim < VOLDEPTH) maxDim = VOLDEPTH;

				cout << "FileReader::readDAT VOLWIDTH=" << VOLWIDTH << " VOLHEIGHT=" << VOLHEIGHT << " VOLDEPTH=" << VOLDEPTH << endl;
			}
			else if(!line.find("SliceThickness")){
				lineStream.clear();
				lineStream.str(line);
				getline(lineStream, tempStr, ':');
				getline(lineStream, tempStr, '\n');

				tempStr = trim(tempStr);

				lineStream.clear();
				lineStream.str(tempStr);
				getline(lineStream, tempStr, ' ');
				sliceThickness[Xcoord] = atof(trim(tempStr).c_str());
				maxThickness = sliceThickness[Xcoord];
				getline(lineStream, tempStr, ' ');
				sliceThickness[Ycoord] = atof(trim(tempStr).c_str());
				if(maxThickness < sliceThickness[Ycoord]) maxThickness = sliceThickness[Ycoord];
				getline(lineStream, tempStr, '\n');
				sliceThickness[Zcoord] = atof(trim(tempStr).c_str());
				if(maxThickness < sliceThickness[Zcoord]) maxThickness = sliceThickness[Zcoord];

				cout << "FileReader::readDAT sliceThickness=[" << sliceThickness[Xcoord] << "," << sliceThickness[Ycoord] << "," << sliceThickness[Zcoord] << "]" << endl;
			}
			else if(!line.find("Format")){//GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT
				lineStream.clear();
				lineStream.str(line);
				getline(lineStream, tempStr, ':');
				getline(lineStream, tempStr, '\n');

				tempStr = trim(tempStr);
				if(!tempStr.compare("UCHAR"))
					BITSALLOCATED = UCHAR;
				else if(!tempStr.compare("USHORT"))
					BITSALLOCATED = USHORT;

				cout << "FileReader::readDAT BITSALLOCATED=" << BITSALLOCATED << endl;
			}

			getline(DATfile, line);
		}
		DATfile.close();
	}
	else
		cout << "FileReader::read Unable to open DAT file " << filename << endl;
}

unsigned int FileReader::getVOLHEIGHT(){
	return VOLHEIGHT;
}

unsigned int FileReader::getVOLDEPTH(){
	return VOLDEPTH;
}

unsigned int FileReader::getVOLWIDTH(){
	return VOLWIDTH;
}

float FileReader::getXSliceThickness(){
	return sliceThickness[Xcoord];
}

float FileReader::getYSliceThickness(){
	return sliceThickness[Ycoord];
}

float FileReader::getZSliceThickness(){
	return sliceThickness[Zcoord];
}

int FileReader::getBITSALLOCATED(){
	return BITSALLOCATED;
}

unsigned int FileReader::getMaxDim(){
	return maxDim;
}

float FileReader::getMaxThickness(){
	return maxThickness;
}

