#include "test.h"

extern int CENGINE_TEST_CORE_ArrayList();
extern int CENGINE_TEST_GPU_RendererGL();

int main(int argc, char **argv) {
	CENGINE_TEST_CORE_ArrayList();
	CENGINE_TEST_GPU_RendererGL();
	return 0;
}