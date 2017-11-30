//#define GLEW_STATIC
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "cuda_runtime.h"
#include "cuda_gl_interop.h"

//keyboard control var
int animFlag = 1;
float animTime = 0.0f;
float animInc = 0.1f;

//variabel eksternal dari simplePBO.cpp
extern GLuint pbo;
extern GLuint textureID;
extern unsigned int iw, ih;

extern void runCuda();

void display() {
	//run kernel
	runCuda();

	//buat texture dari buffer
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);

	//bind texture dari pbo
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iw, ih, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	//draw a quad
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);//membuat texture dari 2d ke objek 3d
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();

	//buffer di swap agar gambar bisa ditampilkan
	glutSwapBuffers();

	//if animFlag = true, display digambar ulang
	if (animFlag) {
		glutPostRedisplay();
		animTime += animInc;
	}
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case(27):
		exit(0);
		break;
	case 'a'://toggle animation
	case 'A':
		animFlag = (animFlag ? 0 : 1);
		break;
	case '-'://menurunkan increment
		animInc -= 0.01;
		break;
	case '+'://menaikkan increment
		animInc += 0.01;
		break;
	case 'r'://reset increment
		animInc = 0.01;
		break;
	default:
		break;
	}

	//gambar ulang
	glutPostRedisplay();
}