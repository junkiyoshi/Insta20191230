#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	this->sound.load("sound/Spring_In_My_Step.mp3");
	this->sound.play();

	this->n_bands_to_get = 60;
	for (int j = 0; j < this->n_bands_to_get; j++) {

		this->fft_smoothed.push_back(0);
	}

	for (int i = 0; i < 36; i++) {

		this->log.push_back(this->fft_smoothed);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSoundUpdate();
	float* val = ofSoundGetSpectrum(this->n_bands_to_get);
	for (int i = 0; i < this->n_bands_to_get; i++) {

		this->fft_smoothed[i] *= 0.98f;
		if (this->fft_smoothed[i] < val[i]) {

			this->fft_smoothed[i] = val[i];
		}
	}

	log.push_back(this->fft_smoothed);
	if (log.size() > 36) {

		this->log.erase(this->log.begin());
	}

	this->face_mesh.clear();
	this->frame_mesh.clear();

	int radius_span = 5;
	int deg_span = 10;
	for (int radius = 100; radius < 300; radius += radius_span * 4) {

		for (int deg_start = 0; deg_start < 360; deg_start += deg_span) {

			int index_1 = ofMap(radius, 100, 300, 1, 10);
			int index_2 = ofMap(deg_start, 0, 360, 0, 36);

			auto z = log[index_2][index_1] * 150;
			int param_index = this->face_mesh.getNumVertices();
			for (int deg = deg_start; deg < deg_start + deg_span; deg++) {

				int index = this->face_mesh.getNumVertices();

				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z));
				vertices.push_back(glm::vec3((radius + radius_span) * cos(deg * DEG_TO_RAD), (radius + radius_span) * sin(deg * DEG_TO_RAD), z));
				vertices.push_back(glm::vec3((radius + radius_span) * cos((deg + 1) * DEG_TO_RAD), (radius + radius_span) * sin((deg + 1) * DEG_TO_RAD), z));
				vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), z));

				vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0));
				vertices.push_back(glm::vec3((radius + radius_span) * cos(deg * DEG_TO_RAD), (radius + radius_span) * sin(deg * DEG_TO_RAD), 0));
				vertices.push_back(glm::vec3((radius + radius_span) * cos((deg + 1) * DEG_TO_RAD), (radius + radius_span) * sin((deg + 1) * DEG_TO_RAD), 0));
				vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), 0));

				this->face_mesh.addVertices(vertices);

				this->face_mesh.addIndex(index + 0);	this->face_mesh.addIndex(index + 1); this->face_mesh.addIndex(index + 2);
				this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 3);

				this->face_mesh.addIndex(index + 4);	this->face_mesh.addIndex(index + 5); this->face_mesh.addIndex(index + 6);
				this->face_mesh.addIndex(index + 4); this->face_mesh.addIndex(index + 6); this->face_mesh.addIndex(index + 7);

				this->face_mesh.addIndex(index + 0);	this->face_mesh.addIndex(index + 3); this->face_mesh.addIndex(index + 7);
				this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 4); this->face_mesh.addIndex(index + 7);

				this->face_mesh.addIndex(index + 1);	this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 6);
				this->face_mesh.addIndex(index + 1);	this->face_mesh.addIndex(index + 5); this->face_mesh.addIndex(index + 6);

				this->frame_mesh.addVertices(vertices);

				this->frame_mesh.addIndex(index + 0); this->frame_mesh.addIndex(index + 3);
				this->frame_mesh.addIndex(index + 4); this->frame_mesh.addIndex(index + 7);

				this->frame_mesh.addIndex(index + 1); this->frame_mesh.addIndex(index + 2);
				this->frame_mesh.addIndex(index + 5); this->frame_mesh.addIndex(index + 6);
			}

			this->face_mesh.addIndex(param_index + 0); this->face_mesh.addIndex(param_index + 1); this->face_mesh.addIndex(param_index + 5);
			this->face_mesh.addIndex(param_index + 0); this->face_mesh.addIndex(param_index + 4); this->face_mesh.addIndex(param_index + 5);

			this->frame_mesh.addIndex(param_index + 0); this->frame_mesh.addIndex(param_index + 1);
			this->frame_mesh.addIndex(param_index + 4); this->frame_mesh.addIndex(param_index + 5);

			this->frame_mesh.addIndex(param_index + 0); this->frame_mesh.addIndex(param_index + 4);
			this->frame_mesh.addIndex(param_index + 1); this->frame_mesh.addIndex(param_index + 5);

			param_index = this->face_mesh.getNumVertices() - 8;

			this->face_mesh.addIndex(param_index + 2); this->face_mesh.addIndex(param_index + 3); this->face_mesh.addIndex(param_index + 6);
			this->face_mesh.addIndex(param_index + 3); this->face_mesh.addIndex(param_index + 6); this->face_mesh.addIndex(param_index + 7);

			this->frame_mesh.addIndex(param_index + 2); this->frame_mesh.addIndex(param_index + 3);
			this->frame_mesh.addIndex(param_index + 6); this->frame_mesh.addIndex(param_index + 7);

			this->frame_mesh.addIndex(param_index + 2); this->frame_mesh.addIndex(param_index + 6);
			this->frame_mesh.addIndex(param_index + 3); this->frame_mesh.addIndex(param_index + 7);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(270);

	ofSetColor(39);
	this->face_mesh.draw();

	ofSetColor(239);
	this->frame_mesh.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}