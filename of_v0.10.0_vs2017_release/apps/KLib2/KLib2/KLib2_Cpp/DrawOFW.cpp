#include "DrawOFW.h"

DrawOFW::DrawOFW(int nrow, int ncol, int size)
{
	this->nrow = nrow;
	this->ncol = ncol;
	this->size = size;
}

void DrawOFW::draw(int ** frame, int scale)
{
	for (int i = 0; i < nrow; i++)
	{
		for (int j = 0; j < ncol; j++)
		{
			ofSetColor(0, frame[i][j] * scale, 0);
			ofFill();
			ofDrawRectangle(j*size, i*size, size, size);
		}
	}
}