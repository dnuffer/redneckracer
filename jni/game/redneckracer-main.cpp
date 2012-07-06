#include "Globals.hpp"
#include "eglut/eglut.h"

namespace
{
	using namespace rr;

	/* new window size or exposure */
	void reshape(int width, int height)
	{
		GLfloat ar = (GLfloat) width / (GLfloat) height;

		glViewport(0, 0, (GLint) width, (GLint) height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
#ifdef GL_VERSION_ES_CM_1_0
		glFrustumf(-ar, ar, -1, 1, 5.0, 60.0);
#else
		glFrustum(-ar, ar, -1, 1, 5.0, 60.0);
#endif

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -10.0);
	}

	void draw(void)
	{
		try
		{
			game().renderNextFrame();
		}
		catch (const blocxx::Exception& e)
		{
			LOGE("game().renderNextFrame() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
			abort();
		}
		catch (const std::exception& e)
		{
			LOGE("game().renderNextFrame() threw std::exception: %s", e.what());
			abort();
		}
		catch (...)
		{
			LOGE("game().renderNextFrame() threw unknown exception");
			abort();
		}
	}

	void normal_key(unsigned char key)
	{
		if( key == 'Q' || key == 'q' )
		{
			exit(0);
		}
	}

	void special_key(int special)
	{
		// FIXME!
		switch (special)
		{
		case EGLUT_KEY_LEFT:
			break;
		case EGLUT_KEY_RIGHT:
			break;
		case EGLUT_KEY_UP:
			break;
		case EGLUT_KEY_DOWN:
			break;
		default:
			break;
		}
	}

	void init(void)
	{
		try
		{
			rr::game().init("redneckracer-assets.zip");
		}
		catch (const blocxx::Exception& e)
		{
			LOGE("game().init() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
			abort();
		}
		catch (const std::exception& e)
		{
			LOGE("game().init() threw std::exception: %s", e.what());
			abort();
		}
		catch (...)
		{
			LOGE("game().init() threw unknown exception");
			abort();
		}
	}
}

int main(int argc, char** argv)
{

	eglutInitWindowSize(320, 480);
	eglutInitAPIMask(EGLUT_OPENGL_ES1_BIT);
	eglutInit(argc, argv);

	eglutCreateWindow("Redneck Racer");

	eglutReshapeFunc(reshape);
	eglutDisplayFunc(NULL);
	eglutSpecialFunc(special_key);
	eglutKeyboardFunc(normal_key);
	eglutIdleFunc(draw);

	init();

	eglutMainLoop();

	return 0;
}
