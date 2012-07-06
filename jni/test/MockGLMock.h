#include "gmock/gmock.h"

namespace engine {
	namespace gl {
		
		class MockGLMock : public GLMock {
		public:
			MOCK_METHOD1(glStrError,
						 const char*(GLint error));
			MOCK_METHOD1(checkGLError,
						 void(const char* op));
			MOCK_METHOD1(setGLLineWidth,
						 void(GLfloat width));
			MOCK_METHOD0(clearColorBuffer,
						 void());
			MOCK_METHOD0(clearDepthBuffer,
						 void());
			MOCK_METHOD0(clearColorAndDepthBuffer,
						 void());
			MOCK_METHOD0(loadIdentity,
						 void());
			MOCK_METHOD3(translate,
						 void(GLfloat x, GLfloat y, GLfloat z));
			MOCK_METHOD4(rotate,
						 void(GLfloat angle, GLfloat x, GLfloat y, GLfloat z));
			MOCK_METHOD3(scale,
						 void(GLfloat x, GLfloat y, GLfloat z));
			MOCK_METHOD4(vertex,
						 void(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer));
			MOCK_METHOD4(color,
						 void(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer));
			MOCK_METHOD0(enableTexture2D,
						 void());
			MOCK_METHOD0(disableTexture2D,
						 void());
			MOCK_METHOD4(texCoord,
						 void(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer));
			MOCK_METHOD3(drawArrays,
						 void(GLenum mode, GLint first, GLsizei count));
			MOCK_METHOD0(genTexture,
						 GLuint());
			MOCK_METHOD1(getString,
						 std::string(GLenum name));
			MOCK_METHOD0(getVendor,
						 std::string());
			MOCK_METHOD0(getRenderer,
						 std::string());
			MOCK_METHOD0(getVersion,
						 std::string());
			MOCK_METHOD0(getExtensions,
						 std::string());
			MOCK_METHOD1(bindTexture2D,
						 void(GLuint texture));
			MOCK_METHOD2(deleteTextures,
						 void(GLsizei n, const GLuint *textures));
			MOCK_METHOD3(texImage2D,
						 void(GLsizei width, GLsizei height, GLvoid* imageData));
			MOCK_METHOD2(texParameter,
						 void(GLenum pname, GLint param));
			MOCK_METHOD5(compressedTexImage2D,
						 void(GLenum internalFormat, GLsizei width, GLsizei height, GLsizei imageSize, const GLvoid* data));
			MOCK_METHOD1(matrixMode,
						 void(GLenum mode));
			MOCK_METHOD6(ortho,
						 void(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar));
			MOCK_METHOD1(multMatrix,
						 void(const GLfloat* m));
			MOCK_METHOD0(MatrixScope,
						 void());
			MOCK_METHOD0(DestMatrixScope,
						 void());
			MOCK_METHOD0(ColorArrayScope,
						 void());
			MOCK_METHOD0(DestColorArrayScope,
						 void());
			MOCK_METHOD1(CullFaceScope,
						 void(GLenum mode));
			MOCK_METHOD0(DestCullFaceScope,
						 void());
			MOCK_METHOD0(VertexArrayScope,
						 void());
			MOCK_METHOD0(DestVertexArrayScope,
						 void());
			MOCK_METHOD0(TextureCoordArrayScope,
						 void());
			MOCK_METHOD0(DestTextureCoordArrayScope,
						 void());
			MOCK_METHOD0(Texture2DScope,
						 void());
			MOCK_METHOD0(DestTexture2DScope,
						 void());
		};
		
	}  // namespace gl
}  // namespace engine
