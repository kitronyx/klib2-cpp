#ifndef OFAPP_H
#define OFAPP_H

#include "ofMain.h"
#include "../DrawOFW.h"
#include "../KLib2Cpp.h"


#define SERVER_IP "127.0.0.1"
#define PORT 3800
#define MAX_BUF 5000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		KLib2Cpp* klib;
		float** vertexbuffer;

		//int camera_rotate;
		//ofEasyCam* camera;
};

#endif