/*
 * GLMock.hpp
 *
 *  Created on: July 19, 2011
 *      Author: Matthew Ricks
 */

#ifndef engine_GLMock_HPP_INCLUDED_
#define engine_GLMock_HPP_INCLUDED_

#include "boost/smart_ptr/intrusive_ptr.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "EngineConfig.hpp"

#ifdef HAVE_OPENGL_GL_H
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif

namespace engine
{
	namespace gl
	{
		class GLMock : public virtual IntrusiveCountableBase
		{
		public:
			virtual ~GLMock() {}
			
			virtual const char* glStrError(GLint error) = 0;
			virtual void checkGLError(const char* op) = 0;
			virtual void setGLLineWidth(GLfloat width) = 0;
			virtual void clearColorBuffer() = 0;
			virtual void clearDepthBuffer() = 0;
			virtual void clearColorAndDepthBuffer() = 0;
			virtual void loadIdentity() = 0;
			virtual void translate(GLfloat x, GLfloat y, GLfloat z) = 0;
			virtual void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) = 0;
			virtual void scale(GLfloat x, GLfloat y, GLfloat z) = 0;
			virtual void vertex(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) = 0;
			virtual void color(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) = 0;
			virtual void enableTexture2D() = 0;
			virtual void disableTexture2D() = 0;
			virtual void texCoord(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) = 0;
			virtual void drawArrays(GLenum mode, GLint first, GLsizei count) = 0;
			virtual GLuint genTexture() = 0;
			virtual std::string getString(GLenum name) = 0;
			virtual std::string getVendor() = 0;
			virtual std::string getRenderer() = 0;
			virtual std::string getVersion() = 0;
			virtual std::string getExtensions() = 0;
			virtual void bindTexture2D(GLuint texture) = 0;
			virtual void deleteTextures(GLsizei n, const GLuint *textures) = 0;
			virtual void texImage2D(GLsizei width, GLsizei height, GLvoid* imageData) = 0;
			virtual void texParameter(GLenum pname, GLint param) = 0;
			virtual void compressedTexImage2D(GLenum internalFormat, GLsizei width, GLsizei height, GLsizei imageSize, const GLvoid* data) = 0;
			virtual void matrixMode(GLenum mode) = 0;
			virtual void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) = 0;
			virtual void multMatrix(const GLfloat* m) = 0;
			virtual void MatrixScope() = 0;
			virtual void DestMatrixScope() = 0;
			virtual void ColorArrayScope() = 0;
			virtual void DestColorArrayScope() = 0;
			virtual void CullFaceScope(GLenum mode) = 0;
			virtual void DestCullFaceScope() = 0;
			virtual void VertexArrayScope() = 0;
			virtual void DestVertexArrayScope() = 0;
			virtual void TextureCoordArrayScope() = 0;
			virtual void DestTextureCoordArrayScope() = 0;
			virtual void Texture2DScope() = 0;
			virtual void DestTexture2DScope() = 0;
		};
		
		typedef boost::intrusive_ptr<GLMock> GLMockPtr;
		
		extern GLMock* glMock;
	}
}

#endif

