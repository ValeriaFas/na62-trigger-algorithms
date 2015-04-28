/*
 * ParsConfFile.h
 *
 *  Created on: 20 Apr 2015
 *      Author: vfascian
 */

#ifndef L1_RICH_ALGORITHM_PARSCONFFILE_H_
#define L1_RICH_ALGORITHM_PARSCONFFILE_H_

#include "ConfFileReader.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class ParsConfFile {
public:
	ParsConfFile();
	~ParsConfFile();

	static ParsConfFile* GetInstance();

	//int getChannelRemap(int);
	int* getGeoPmsMap();
	double* getPosPmsMap();
	double* getT0();
	int* getFocalCenterSaleve();
	int* getFocalCenterJura();
	void readT0();

private:

	static ParsConfFile* theInstance;  // singleton instance

	int nChannels;
	int nroChannels;
	int focalCenterSaleve[2];
	int focalCenterJura[2];
	int geoPmsMap[2560];
	double posPmsMap[3904];
	double timeT0[1952];
	string fileT0;

};

#endif /* L1_PARSCONFFILE_H_ */
