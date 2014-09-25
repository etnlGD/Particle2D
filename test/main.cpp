#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Particle2D.h>
#include <random>
#include "IParticleRenderer.h"
#include <time.h>
using namespace std;
using namespace particle2d;

static GLuint createShader(GLenum shaderType, const char* source)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE)
		return shader;

	GLsizei length = 1024;
	char* string = new char[length];
	glGetShaderInfoLog(shader, length, NULL, string);
	cerr << "Compile shader failed: " << endl;
	cerr << string << endl;
	delete[] string;

	return 0;
}

static GLuint initProgram(const char* vsSource, const char* psSource)
{
	GLuint program = glCreateProgram();

	GLuint vs = createShader(GL_VERTEX_SHADER,	vsSource);
	GLuint ps = createShader(GL_FRAGMENT_SHADER,psSource);

	glAttachShader(program, vs);
	glAttachShader(program, ps);
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		GLuint bufSize = 1024;
		GLchar* buffer = new GLchar[bufSize];
		glGetProgramInfoLog(program, bufSize, NULL, buffer);
		cerr << "Link failed:" << endl;
		cerr << buffer << endl;
		delete[] buffer;

		return 0;
	}

	return program;
}

class PointRenderer : public IParticleRenderer
{
	struct VertexData
	{
		vec2 pos;
		vec4 color;
		float size;
	};

	static const char* vs;
	static const char* ps;
	static int program;

public:
	PointRenderer() : source(nullptr) 
	{
		assert(vbo != 0);
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);
		assert(vao != 0);
		
		glBindVertexArray(vao);
		glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 2*4);
		glEnableVertexAttribArray(1);

		glVertexAttribFormat(2, 1, GL_FLOAT, GL_FALSE, 6*4);
		glEnableVertexAttribArray(2);

		glBindVertexBuffer(0, vbo, 0, 7*4);
		glBindVertexBuffer(1, vbo, 0, 7*4);
		glBindVertexBuffer(2, vbo, 0, 7*4);
		glBindVertexArray(0);
	}

	virtual void onParticleSwapped(ParticleIterator pos) 
	{
		// have nothing to do here
	}

	virtual void setParticleSource(const ParticleVector& pv) 
	{
		source = &pv;
	}

	virtual void render() 
	{
		updateVBO();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glEnable(GL_PROGRAM_POINT_SIZE);

		if (!program)
			program = initProgram(vs, ps);

		if (!program)
		{
			cout << "no program created" << endl;
			exit(0);
		}

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, source->size());
	}

private:
	void updateVBO()
	{
		VertexData* data = new VertexData[source->size()];
		VertexData* vertexPtr = data;
		for (const SParticle& p : *source)
		{
			vertexPtr->pos = p.pos;
			vertexPtr->color = p.color.toVec4();
			vertexPtr->size = p.size.x * p.size.y;

// 			cout << "--\n";
// 			cout << p.pos.x  << ", " << p.pos.y << endl;
//			cout << vertexPtr->size << endl;
			++vertexPtr;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * source->size(), data, GL_DYNAMIC_DRAW);
		delete[] data;
	}

private:
	GLuint vbo;
	GLuint vao;
	const ParticleVector* source;
};

const char* PointRenderer::vs =
	"#version 330\n"
	"layout (location = 0) in vec4 pos;\n"
	"layout (location = 1) in vec4 color;\n"
	"layout (location = 2) in float size;\n"
	"out vec4 vcolor;"
	"void main() {"
	"	vcolor = color;"
	"	gl_Position = vec4(pos.xy, 0.0, 1.0);"
	"	gl_PointSize= size;"
	"}";

const char* PointRenderer::ps = 
	"#version 330\n"
	"in vec4 vcolor;"
	"out vec4 color;"
	"void main() {"
	"	color = vcolor;"
	"}";

int PointRenderer::program = 0;

static PointRenderer* renderer;
static CParticleSystem2D* psys;
static void initParticleSystem()
{
	psys = new CParticleSystem2D(256);
	
	{
		CParticleEmitter* emitter = new CParticleEmitter;
		emitter->setPPSDistribution<default_random_engine>(uniform_real_distribution<float>(60, 80));

		{
			CParticleBoxInitializer* initer = new CParticleBoxInitializer(-1, 1, 1.05, 1);
			emitter->addParticleInitializer(initer);
			initer->drop();
		}

		{
			CParticleSpeedInitializer* initer = new CParticleSpeedInitializer(vec2(0, -0.6f), degree(20));
			emitter->addParticleInitializer(initer);
			initer->drop();
		}

		{
			CParticleBasicInitializer* initer = new CParticleBasicInitializer;
			initer->setLifetimeDistribution<default_random_engine>(uniform_real_distribution<float>(3.f, 3.333f));
			
			initer->setSizeDistribution<default_random_engine>(
				uniform_real_distribution<float>(1.f, 2.2f),
				uniform_real_distribution<float>(1.f, 2.2f));

			emitter->addParticleInitializer(initer);
			initer->drop();
		}

		psys->addEmitter(emitter);
		emitter->drop();
	}
	
	{
		CParticleFadeOutAffector* affector = new CParticleFadeOutAffector;
		affector->setTargetColor(vec4());
		psys->addAffector(affector, 0.75f, 1.f);
		affector->drop();
	}

	renderer = new PointRenderer;
	psys->setRenderer(renderer);
}

static void init()
{
	initParticleSystem();
	glClearColor(0.f, 0.f, 0.f, 1.f);
}

clock_t prevClock;
static void onDraw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	clock_t now = clock();
	psys->update((float) (now - prevClock) / CLOCKS_PER_SEC);
	prevClock = now;

	renderer->render();

	glFlush();
	glutSwapBuffers();
}

static void onReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

static void onKeyDown(unsigned char key, int x, int y)
{
	(void) x;
	(void) y;
	switch (key) {
	case 27:  /*  Escape key  */
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (800, 480);
	glutCreateWindow (argv[0]);
	glutCreateMenu(NULL);

	if ( glewInit() ) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit( EXIT_FAILURE );
	}

	init();
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyDown);
	glutIdleFunc(onDraw);
	glutMainLoop();
	return 0;
}

