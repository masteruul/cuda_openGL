//#define GLEW_STATIC
#include <gl/glew.h> //header untuk gl wrangler extension
#include <gl/freeglut.h>//header untuk fungsi glut
#include <cuda_runtime.h> //header untuk menjalankan CUDA di runtime
#include "cuda_gl_interop.h" //header untuk fungsi-fungsi cuda yang mendukung inter-operability dengan openGL

//cuda method: fungsi yang dijalankan dengan CUDA
//extern berarti fungsi/prosedur atau variabel tersebut didefinisikan di file lain (file eksternal) dalam 1 project
extern void initCuda(int argc, char** argv);
extern void runCuda();
extern void renderCuda(int);

//fungsi-fungsi callback: fungsi untuk mouse, keyboard, dan display
extern void display();
extern void keyboard(unsigned char key, int x, int y);


//variabel untuk ukuran window dan waktu
unsigned int w = 512, h = 512, timer = 0;

//fungsi di openGL
void initGL(int argc, char** argv);

void main(int argc, char** argv) {
	//membuat context openGL
	initGL(argc, argv);
	initCuda(argc, argv);

	//eksekusi callback
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	//rendering mainloop: memanggil ulang fungsi main secara terus-menerus
	glutMainLoop();

	//clean up
	cudaThreadExit();
	exit(EXIT_SUCCESS);
}
void initGL(int argc, char** argv) {
	//generate window untuk openGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //
	glutInitWindowSize(w, h);
	glutCreateWindow("cuda gl interop");
	//meregister callback
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	//inisiasi glew untuk buffer data openGL
	glewInit();
	//set up view port and mode
	glViewport(0, 0, w, h);
	glClearColor(0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);

	//set view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
}