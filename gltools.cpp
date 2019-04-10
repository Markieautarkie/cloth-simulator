#include "precomp.h" // only include this header in source files

// typedefs
typedef unsigned char uchar;
typedef unsigned int uint;

// namespace
using namespace std;

// forward declarations
bool LoadPNGFile( const char* fileName, uint& w, uint& h, vector<uchar>& image );

// functions

GLuint CreateTexture( uint* pixels, int w, int h )
{
	GLuint retVal = 0;
	glGenTextures( 1, &retVal );
	glBindTexture( GL_TEXTURE_2D, retVal );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	return retVal;
}

GLuint LoadTexture( const char* fileName, uint** data = 0 )
{
	vector<uchar> image;
	uint w, h;
	GLuint retVal = 0;
	if (LoadPNGFile( fileName, w, h, image )) 
	{
		uint* d = (uint*)image.data();
		for( int i = 0; i < w * h; i++ ) d[i] = ((d[i] & 255) << 16) + (255 << 24) + ((d[i] >> 16) & 255) + (d[i] & 0xff00);
		retVal = CreateTexture( d, w, h );
		if (data)
		{
			uint* buffer = new uint[w * h];
			memcpy( buffer, image.data(), w * h * sizeof( uint ) );
			*data = buffer;
		}
	}
	return retVal;
}

GLuint CreateVBO( const GLfloat* data, const uint size )
{
	GLuint id;
	glGenBuffers( 1, &id );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	if (data) glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
	return id;
}

void BindVBO( const uint idx, const uint N, const GLuint id )
{
	glEnableVertexAttribArray( idx );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	glVertexAttribPointer( idx, N, GL_FLOAT, GL_FALSE, 0, (void*)0 );
}

static GLuint vertexBuffer, UVBuffer, vao;
void DrawQuad( float u1, float v1, float u2, float v2 )
{
	GLfloat verts[] = { u1 * 2 - 1, 1 - v1 * 2, 0, u2 * 2 - 1, 1 - v1 * 2, 0, u1 * 2 - 1, 1 - v2 * 2, 0,
						u2 * 2 - 1, 1 - v1 * 2, 0, u1 * 2 - 1, 1 - v2 * 2, 0, u2 * 2 - 1, 1 - v2 * 2, 0 };
	GLfloat uvdata[] = { u1, v1, u2, v1, u1, v2, u2, v1, u1, v2, u2, v2 };
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, UVBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( uvdata ), uvdata, GL_STATIC_DRAW );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}

void DrawQuad()
{
	if (!vao)
	{
		// generate buffers
		vertexBuffer = CreateVBO( 0, 18 * 4 );
		UVBuffer = CreateVBO( 0, 12 * 4 );
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		BindVBO( 0, 3, vertexBuffer );
		BindVBO( 1, 2, UVBuffer );
	}
	DrawQuad( 0, 0, 1, 1 );
}

GLuint CompileShader( const char* vtext, const char* ftext )
{
	uint vertex = glCreateShader( GL_VERTEX_SHADER );
	uint pixel = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( vertex, 1, &vtext, 0 );
	glCompileShader( vertex );
	glShaderSource( pixel, 1, &ftext, 0 );
	glCompileShader( pixel );
	GLuint retVal = glCreateProgram();
	glAttachShader( retVal, vertex );
	glAttachShader( retVal, pixel );
	glBindAttribLocation( retVal, 0, "in_Position" );
	glBindAttribLocation( retVal, 1, "in_Color" );
	glLinkProgram( retVal );
	return retVal;
}

GLuint LoadShader()
{
	string vsText = "#version 330\nlayout(location=0)in vec4 u;layout(location=1)in vec2 t;uniform mat4 T;out vec2 v;out vec2 P;void main(){v=t;P=vec2(u)*0.5+vec2(0.5);gl_Position=T*u;}";
	string fsText = "#version 330\nuniform sampler2D C;uniform float a;in vec2 v;out vec4 pixel;void main(){vec4 k=texture(C,v);pixel=vec4(k.bgr,a*k.a);}";
	return CompileShader( vsText.c_str(), fsText.c_str() );
}