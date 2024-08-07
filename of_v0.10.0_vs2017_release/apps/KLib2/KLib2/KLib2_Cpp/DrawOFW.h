#ifndef DRAWOFW_H
#define DRAWOFW_H

#include "src/ofApp.h"

class DrawOFW
{
private:
	int nrow;
	int ncol;
	int size;

public:
	DrawOFW(int nrow, int ncol, int size);
	void draw(int **frame, int scale);
	void drawForce(double** frame, int scale);
};

#endif