// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef engine_GL_HPP_INCLUDED_
#define engine_GL_HPP_INCLUDED_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "GLMock.hpp"
#include "Log.hpp"

#ifdef HAVE_OPENGL_GL_H
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#else
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif
#include <stdlib.h>
#include <string>

namespace engine
{

namespace gl
{

const GLuint INVALID_TEXTURE = 0;

const char* glStrError(GLint error);

inline void checkGLError(const char* op)
{
	// because calling into the graphics chip an extra two times can be slow, only do this if running in debug mode.
#ifdef DEBUG
	if(glMock) { glMock->checkGLError(op); return; }
    GLint error = glGetError();
    if (error)
    {
        LOGI("GL ERROR! after %s glError (%d: %s)\n", op, error, glStrError(error));
    	abort();
    }
#endif
}

inline void setGLLineWidth(GLfloat width)
{
	if(glMock) { glMock->setGLLineWidth(width); return; }
	glLineWidth(width);
}

inline void clearColorBuffer()
{
	if(glMock) { glMock->clearColorBuffer(); return; }
	glClear(GL_COLOR_BUFFER_BIT);
	checkGLError("glClear(GL_COLOR_BUFFER_BIT)");
}

inline void clearDepthBuffer()
{
	if(glMock) { glMock->clearDepthBuffer(); return; }
	glClear(GL_DEPTH_BUFFER_BIT);
	checkGLError("glClear(GL_DEPTH_BUFFER_BIT)");
}

inline void clearColorAndDepthBuffer()
{
	if(glMock) { glMock->clearColorAndDepthBuffer(); return; }
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkGLError("glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)");
}

inline void loadIdentity()
{
	if(glMock) { glMock->loadIdentity(); return; }
	glLoadIdentity();
	checkGLError("glLoadIdentity()");
}

inline void translate(GLfloat x, GLfloat y, GLfloat z)
{
	if(glMock) { glMock->translate(x,y,z); return; }
	glTranslatef(x, y, z);
	checkGLError("glTranslatef");
}

inline void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	if(glMock) { glMock->rotate(angle,x,y,z); return; }
	glRotatef(angle, x, y, z);
	checkGLError("glRotatef");
}

inline void scale(GLfloat x, GLfloat y, GLfloat z)
{
	if(glMock) { glMock->scale(x,y,z); return; }
	glScalef(x, y, z);
	checkGLError("glScalef");
}

inline void vertex(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	if(glMock) { glMock->vertex(size,type,stride,pointer); return; }
	glVertexPointer(size, type, stride, pointer);
	checkGLError("glVertexPointer");
}

inline void color(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	if(glMock) { glMock->color(size,type,stride,pointer); return; }
	glColorPointer(size, type, stride, pointer);
	checkGLError("glColorPointer");
}

inline void enableTexture2D()
{
	if(glMock) { glMock->enableTexture2D(); return; }
	glEnable(GL_TEXTURE_2D);
	checkGLError("glEnable(GL_TEXTURE_2D)");
}

inline void disableTexture2D()
{
	if(glMock) { glMock->disableTexture2D(); return; }
	glDisable(GL_TEXTURE_2D);
	checkGLError("glDisable(GL_TEXTURE_2D)");
}

inline void texCoord(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	if(glMock) { glMock->texCoord(size,type,stride,pointer); return; }
	glTexCoordPointer(size, type, stride, pointer);
	checkGLError("glTexCoordPointer");
}

inline void drawArrays(GLenum mode, GLint first, GLsizei count)
{
	if(glMock) { glMock->drawArrays(mode,first,count); return; }
	glDrawArrays(mode, first, count);
	checkGLError("glDrawArrays");
}

inline GLuint genTexture()
{
	if(glMock) { return glMock->genTexture(); }
	GLuint texture;
	glGenTextures(1, &texture);
	checkGLError("glGenTextures");
	return texture;
}

inline std::string getString(GLenum name)
{
	if(glMock) { return glMock->getString(name); }
	const GLubyte* junk = glGetString(name);
	if( junk )
	{
		return std::string(reinterpret_cast<const char*>(junk));
	}
	return std::string();
}

inline std::string getVendor()
{
	if(glMock) { return glMock->getVendor(); }
	std::string text = getString(GL_VENDOR);
	checkGLError("glGetString(GL_VENDOR)");
	return text;
}
inline std::string getRenderer()
{
	if(glMock) { return glMock->getRenderer(); }
	std::string text = getString(GL_RENDERER);
	checkGLError("glGetString(GL_RENDERER)");
	return text;
}
inline std::string getVersion()
{
	if(glMock) { return glMock->getVersion(); }
	std::string text = getString(GL_VERSION);
	checkGLError("glGetString(GL_VERSION)");
	return text;
}
inline std::string getExtensions()
{
	if(glMock) { return glMock->getExtensions(); }
	std::string text = getString(GL_EXTENSIONS);
	checkGLError("glGetString(GL_EXTENSIONS)");
	return text;
}

inline void bindTexture2D(GLuint texture)
{
	if(glMock) { glMock->bindTexture2D(texture); return; }
	glBindTexture(GL_TEXTURE_2D, texture);
	checkGLError("glBindTexture(GL_TEXTURE_2D, texture)");
}

inline void deleteTextures(GLsizei n, const GLuint *textures)
{
	if(glMock) { glMock->deleteTextures(n,textures); return; }
	glDeleteTextures(n, textures);
	checkGLError("glDeleteTextures");
}

inline void texImage2D(GLsizei width, GLsizei height, GLvoid* imageData)
{
	if(glMock) { glMock->texImage2D(width,height,imageData); return; }
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) imageData);
	checkGLError("glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) imageData)");
}

inline void texParameter(GLenum pname, GLint param)
{
	if(glMock) { glMock->texParameter(pname,param); return; }
	glTexParameteri(GL_TEXTURE_2D, pname, param);
	checkGLError("glTexParameteri(GL_TEXTURE_2D, pname, param)");
}

inline void compressedTexImage2D(GLenum internalFormat, GLsizei width, GLsizei height, GLsizei imageSize, const GLvoid* data)
{
	if(glMock) { glMock->compressedTexImage2D(internalFormat,width,height,imageSize,data); return; }
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageSize, data);
	checkGLError("glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageSize, data)");
}
	
	inline void matrixMode(GLenum mode)
	{
		if(glMock) { glMock->matrixMode(mode); return; }
		glMatrixMode(mode);
		checkGLError("glMatrixMode");
	}
	
	inline void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
	{
		if(glMock) { glMock->ortho(left,right,bottom,top,zNear,zFar); return; }
#ifdef GL_VERSION_ES_CM_1_0
		glOrthof(left, right, bottom, top, zNear, zFar);
#else
		glOrtho(left, right, bottom, top, zNear, zFar);
#endif
		checkGLError("glOrthof");
	}
	
	inline void multMatrix(const GLfloat* m)
	{
		if(glMock) { glMock->multMatrix(m); return; }
		glMultMatrixf(m);
		checkGLError("glMultMatrixf");
	}

class MatrixScope
{
public:
	MatrixScope()
	{
		if(glMock) { glMock->MatrixScope(); return; }
		glPushMatrix();
		checkGLError("glPushMatrix()");
		glLoadIdentity();
		checkGLError("glLoadIdentity()");
	}
	~MatrixScope()
	{
		if(glMock) { glMock->DestMatrixScope(); return; }
		glPopMatrix();
		checkGLError("glPopMatrix()");
	}
};

class ColorArrayScope
{
public:
	ColorArrayScope()
	{
		if(glMock) { glMock->ColorArrayScope(); return; }
		glEnableClientState(GL_COLOR_ARRAY);
	}
	~ColorArrayScope()
	{
		if(glMock) { glMock->DestColorArrayScope(); return; }
		glDisableClientState(GL_COLOR_ARRAY);
	}
};

class CullFaceScope
{
public:
	CullFaceScope(GLenum mode)
	{
		if(glMock) { glMock->CullFaceScope(mode); return; }
		glEnable(GL_CULL_FACE);
		glCullFace(mode);
	}
	~CullFaceScope()
	{
		if(glMock) { glMock->DestCullFaceScope(); return; }
		glDisable(GL_CULL_FACE);
	}
};

class VertexArrayScope
{
public:
	VertexArrayScope()
	{
		if(glMock) { glMock->VertexArrayScope(); return; }
		glEnableClientState(GL_VERTEX_ARRAY);
	}
	~VertexArrayScope()
	{
		if(glMock) { glMock->DestVertexArrayScope(); return; }
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

class TextureCoordArrayScope
{
public:
	TextureCoordArrayScope()
	{
		if(glMock) { glMock->TextureCoordArrayScope(); return; }
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	~TextureCoordArrayScope()
	{
		if(glMock) { glMock->DestTextureCoordArrayScope(); return; }
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
};

class Texture2DScope
{
public:
	Texture2DScope()
	{
		if(glMock) { glMock->Texture2DScope(); return; }
		enableTexture2D();
	}
	~Texture2DScope()
	{
		if(glMock) { glMock->DestTexture2DScope(); return; }
		disableTexture2D();
	}
};

}


}

#endif
