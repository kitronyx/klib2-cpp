#include "ofApp.h"

DrawOFW* drawofw;

//--------------------------------------------------------------
void ofApp::setup(){
	klib = new KLib2Cpp(SERVER_IP, PORT);
	if (!klib->start())
	{
		printf("TCP/IP Connect Error!\n");
		return;
	}
	printf("Connected Server!");

	// node size 
	int size = 20;
	float sizeOffset = klib->bufLength / (96*48);
	if (sizeOffset >= 1) {
		size =  (int)(20 / sizeOffset);
	}

	drawofw = new DrawOFW(klib->row,klib->col, size);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!(klib->read()))
	{
		printf("TCP/IP Disconnect!\n");
		return;
	}
	//klib->printadc();
	printf("Count : %d\n", klib->getcount());
}

//--------------------------------------------------------------
void ofApp::draw(){
	int scale = 2;
	if (klib->read())
	{
		drawofw->draw(klib->adc, scale);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
