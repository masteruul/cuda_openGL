//#define GLEW_STATIC
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "cuda_runtime.h"
#include "cuda_gl_interop.h"

//external var
extern float animTime;
extern unsigned int w, h;

//variabel untuk ukuran gambar
unsigned int iw = w, ih = h;

//list untuk pemanggilan fungsi kernel yang ada di file kernel.cu
extern "C" void launch_kernel(uchar4*, unsigned int, unsigned int, float);

//var pbo: pointer memori untuk buffering object
//var textureID: pointer memori untuk buffering texture
GLuint pbo = NULL, textureID = NULL;

void createPBO(GLuint* pbo) {
	if (pbo) {
		//set up vertex data parameter
		int num_texels = iw*ih;
		int num_values = num_texels * 4;
		int size_tex_data = sizeof(GLubyte)*num_values;

		//generate buffer id
		glGenBuffers(1, pbo);

		//current unpack buffer (opengl state-based)
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pbo);

		//alikasi data untuk buffer, 4 channel, 8bit image
		glBufferData(GL_PIXEL_UNPACK_BUFFER, size_tex_data, NULL, GL_DYNAMIC_COPY);
		
		//meregister buffer object ke cuda
		cudaGLRegisterBufferObject(*pbo);
	}
}
void deletePBO(GLuint* pbo) {
	if (pbo) {
		//unregister buffer object dengan cuda
		cudaGLUnregisterBufferObject(*pbo);

		glBindBuffer(GL_ARRAY_BUFFER, *pbo);
		glDeleteBuffers(1, pbo);

		*pbo = NULL;
	}
}
void createTexture(GLuint* textureID, unsigned int size_x, unsigned int size_y) {
	//enable texturing
	glEnable(GL_TEXTURE_2D);

	//generate texture identifier
	glGenTextures(1, textureID);

	//membuat current texture
	glBindTexture(GL_TEXTURE_2D, *textureID);

	//alokasi memori texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iw, ih, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	//set the filter mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void deleteTexture(GLuint* tex) {
	glDeleteTextures(1, tex);
	*tex = NULL;
}
void cleanupCuda() {
	if (pbo) deletePBO(&pbo);
	if (textureID) deleteTexture(&textureID);
}
void runCuda() {
	uchar4* dptr = NULL;

	//mapping opengl buffer from cuda
	cudaGLMapBufferObject((void**)&dptr, pbo);

	//eksekusi kernel
	launch_kernel(dptr, iw, ih, animTime);

	//unmapping
	cudaGLUnmapBufferObject(pbo);
}
void initCuda(int argc, char** argv) {
	createPBO(&pbo);
	createTexture(&textureID, iw, ih);

	//clean up on exit
	atexit(cleanupCuda);

	runCuda();
}