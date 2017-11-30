#include "ofApp.h"


//----------------------  Params -------------------------------

Params param;        //Definition of global variable

void Params::setup() {
	eCenter = ofPoint( ofGetMouseX(), ofGetMouseY());
	eRad = 100;
	velRad = 600;
	lifeTime = 5.0;
	rotate = 90;
    
}


//----------------------  Particle  ----------------------------

Particle::Particle() {
    
//    live = false;
    isAttracting = true;
    
}

//--------------------------------------------------------------
ofPoint randomPointInCircle( float Rad ){
    ofPoint pnt;
    float rad = ofRandom( 0, Rad );
    float angle = ofRandom( 0, M_TWO_PI );
    pnt.x = ofGetMouseX()  ;
    pnt.y = ofGetMouseY()  ;
    return pnt;
}

//--------------------------------------------------------------
void Particle::reset() {
    
    int numParticles = 500;
    for (int i = 0; i < numParticles; i++) {
        Particle newParticle;
        
    }
    pos = param.eCenter + randomPointInCircle( param.eRad );
	time = -3;
	lifeTime = param.lifeTime;
	live = true;
    
    vel.x = ofRandom(-3.9, 3.9);
    vel.y = ofRandom(-3.9, 3.9);
    
    drag = ofRandom(0.95, 0.998);
    
    force = ofPoint(0, 0);
    
    
}

//--------------------------------------------------------------
void Particle::update( float dt ){
	if ( live ) {
		//Rotate vel
		vel.rotate( 0, 0, param.rotate * dt );

		//Update pos
		pos += vel * dt;    //Euler method

		//Update time and check if particle should die
		time += dt;
		if ( time >= lifeTime ) {
			live = false;   //Particle is now considered as died
		}
	}
    
    if (isAttracting) {
        // Get mouse coords, save to
        // attractPoint
        ofPoint attractPoint = ofPoint(ofGetMouseX(), ofGetMouseY());
        // calculate force
        // then normalize it
        force = attractPoint - pos;
        force.normalize();
        
        // apply drag to velocity
        // apply force to velocity
        vel *= drag;
        vel += force * 0.6;
      
    } else {
        ofPoint repulsePoint = ofPoint(ofGetMouseX(), ofGetMouseY());
        force = repulsePoint - pos;
        
        float distance = force.length();
        force.normalize();
        
        if (distance < 100 ) {
            vel -= force * 0.6;
}
    pos += vel;
}
}

//--------------------------------------------------------------
void Particle::draw(){
	if ( live ) {
		//Compute size
		float size = ofMap( 
			fabs(time - lifeTime/2), 0, lifeTime/2, 3, 1 );

		//Compute color
		ofColor color = ofColor::red;
		float hue = ofMap( time, 0, lifeTime, 128, 255 );
		color.setHue( hue );
		ofSetColor( color );

		ofCircle( pos, size * 2 );  //Draw particle
	}
}

void Particle::toggleMode() {
    isAttracting = !isAttracting;
    reset();
}


//--------------------------------------------------------------
//----------------------  ofApp  -----------------------------
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate( 60 );	//Set screen frame rate

	//Allocate drawing buffer
	int w = ofGetWidth();
	int h = ofGetHeight();
	fbo.allocate( w, h, GL_RGB32F_ARB );

	//Fill buffer with white color
	fbo.begin();
	ofBackground(255, 255, 255);
	fbo.end();

	//Set up parameters
	param.setup();		//Global parameters
	history = 0.995;
	bornRate = 1000;

	bornCount = 100;
	time0 = ofGetElapsedTimef();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
	//Compute dt
	float time = ofGetElapsedTimef();
	float dt = ofClamp( time - time0, 0, 0.1 );
	time0 = time;

	//Delete inactive particles
	int i=0;
	while (i < p.size()) {
		if ( !p[i].live ) {
			p.erase( p.begin() + i );
		}
		else {
			i++;
		}
	}

	//Born new particles
	bornCount += dt * bornRate;      //Update bornCount value
	if ( bornCount >= 1 ) {          //It's time to born particle(s)
		int bornN = int( bornCount );//How many born
		bornCount -= bornN;          //Correct bornCount value
		for (int i=0; i<bornN; i++) {
			Particle newP;
			newP.reset();            //Start a new particle
			p.push_back( newP );     //Add this particle to array
		}
	}

	//Update the particles
	for (int i=0; i<p.size(); i++) {
		p[i].update( dt );
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground( 255, 255, 255 );  //Set white background

	//1. Drawing to buffer
	fbo.begin();

	//Draw semi-transparent white rectangle
	//to slightly clearing a buffer (depends on history value)

	ofEnableAlphaBlending();         //Enable transparency

	float alpha = (1-history) * 255;
	ofSetColor( 255, 255, 255, alpha );
	ofFill();
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	ofDisableAlphaBlending();        //Disable transparency

	//Draw the particles
	ofFill();
	for (int i=0; i<p.size(); i++) {
		p[i].draw();
	}

	fbo.end();

	//2. Draw buffer on the screen
	ofSetColor( 255, 255, 255 );
	fbo.draw( 0, 0 );
}

//--------------------------------------------------------------


void ofApp::keyPressed(int key){
   
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
   
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
   for (int i = 0; i < p.size(); i++) {
       p[i].toggleMode();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
