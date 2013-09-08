#include "Angel.h"
#include "vec.h"
#include "mat.h"
#include <complex>

using namespace std;
using namespace Angel;

GLuint WIDTH_PIXELS = 500;
GLuint HEIGHT_PIXELS = 500;
int height = (int)WIDTH_PIXELS;
int width = (int)HEIGHT_PIXELS;
size_t totalPoints = height * width;
vec2 * pointArray;
vec3 * colorArray;
vec3 translateToColor(double);
int maxIterations = 100;

enum fractalType{Julia, Mandelbrot, Mixed, Greater};
enum fractalType fractal = Julia;
string fractalTypeArray[4] = {"Julia", "Mandelbrot", "Mixed - Added", "Mixed - Greater"};

enum colorSet{HSV, RGB, RGBShift};
enum colorSet colorType = HSV;
string colorSetArray[3] = {"HSV", "RGB shifted 23", "RGB shifted 25"};

enum juliaSet{first, second, third, fourth, fifth, sixth, seventh, eighth, ninth, tenth, eleventh, twelth};
enum juliaSet juliaNumber = first;

complex<double> juliaSetArray [12] = {
	complex<double>(-0.8, 0.156),
	complex<double>(-0.4, 0.6),
	complex<double>(-.62772, .42193),
	complex<double>(0.3515, -0.07467),
	complex<double>(-0.391, -0.587),
	complex<double>(0.233, 0.53780),
	complex<double>(-0.74543, 0.11301),
	complex<double>(-0.74434, -0.10722),
	complex<double>(0.285, 0.01),
	complex<double>(0.45, 0.1428),
	complex<double>(-0.70176, -0.3842),
	complex<double>(-0.835, -0.2321)
};

complex<double> juliaConstant = juliaSetArray[juliaNumber];

double zoomLevel = 1.0;

void generatePointArray()
{
	int currentX = 0;
	int currentY = 0;
	double scaleX = width / 2.0;
	double scaleY = height / 2.0;
	double x;
	double y;

	pointArray = new vec2[totalPoints];

	for(int i = 0; i < totalPoints; i++)
	{

		x = (currentX - (width/2)) / scaleX;
		y = ((-currentY) + (height/2)) / scaleY;

		if(currentX + 1 == width)
		{
			++currentY;
			currentX = 0;
		}
		else
			++currentX;
		
		pointArray[i] = vec2(x, y);
	}
}



double recursiveColor(complex<double> complexNum, complex<double> constant, int iterations)
{
	if(iterations == maxIterations)
		return double(iterations);
	if(sqrt(complexNum.real() * complexNum.real() + complexNum.imag() * complexNum.imag()) > 2)
		return double(iterations);
	else
	{
		complexNum = complex<double>(complexNum.real() * complexNum.real() - (complexNum.imag() * complexNum.imag()) + constant.real(), 2 * complexNum.real() * complexNum.imag() + constant.imag());
		return recursiveColor(complexNum, constant, ++iterations);
	}
}

void generateColorArray()
{
	double iterations;

	colorArray = new vec3[totalPoints];

	if(fractal == 0)
	{	
		for(int i = 0; i < totalPoints; i++)
		{	
			iterations = 0;

			complex<double> constantComplex = juliaConstant;
			complex<double> complexNum = complex<double>(pointArray[i].x, pointArray[i].y);

			iterations = recursiveColor(complexNum, constantComplex, 0);

			colorArray[i] = translateToColor(iterations);
		}
	}
	else if(fractal == 1)
	{
		for(int i = 0; i < totalPoints; i++)
		{	
			iterations = 0;

			complex<double> constantComplex = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> complexNum = complex<double>(0,0);

			iterations = recursiveColor(complexNum, constantComplex, 0);

			colorArray[i] = translateToColor(iterations);
		}
	}
	else if(fractal == 2)
	{
		vec3 juliaColor;
		vec3 mandelbrotColor;
		vec3 mixedColor;

		for(int i = 0; i < totalPoints; i++)
		{
			complex<double> constantComplexM = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> complexNumM = complex<double>(0,0);
			complex<double> complexNumJ = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> constantComplexJ = juliaConstant;

			mandelbrotColor = translateToColor(recursiveColor(complexNumM, constantComplexM, 0));

 			juliaColor = translateToColor(recursiveColor(complexNumJ, constantComplexJ, 0));

			for(int i = 0; i < 3; i++)
			{
				mixedColor[i] = (juliaColor[i] + mandelbrotColor[i]);
				while(mixedColor[i] > 1.0)
					mixedColor[i] -= 1.0;
			}

			colorArray[i] = mixedColor;
		}
	}
	else
	{
		vec3 juliaColor;
		vec3 mandelbrotColor;
		vec3 mixedColor;

		for(int i = 0; i < totalPoints; i++)
		{
			complex<double> constantComplexM = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> complexNumM = complex<double>(0,0);
			complex<double> complexNumJ = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> constantComplexJ = juliaConstant;

			mandelbrotColor = translateToColor(recursiveColor(complexNumM, constantComplexM, 0));

 			juliaColor = translateToColor(recursiveColor(complexNumJ, constantComplexJ, 0));

			for(int i = 0; i < 3; i++)
			{
				if(juliaColor[i] > mandelbrotColor[i])
					mixedColor[i] = juliaColor[i];
				else
					mixedColor[i] = mandelbrotColor[i];
			}

			colorArray[i] = mixedColor;
		}
	}
}

void regenerateColorArray(char command, vec2 location)
{
	double iterations;

	vec2 center = ((pointArray[0] + pointArray[totalPoints - 1]) / 2.0);

	for (int i = 0; i < totalPoints; i++)
	{
		if(command == 'z')
			pointArray[i] = ((pointArray[i] + center) /2.0) + location * zoomLevel;
		else if(command == 'Z' && zoomLevel <= 0.5)
			pointArray[i] = (pointArray[i] - center)* 2.0 + center;
		if(fractal == 0)
		{
			iterations = 0;

			complex<double> constantComplex = juliaConstant;
			complex<double> complexNum = complex<double>(pointArray[i].x, pointArray[i].y);

			iterations = recursiveColor(complexNum, constantComplex, 0);

			colorArray[i] = translateToColor(iterations);
		}
		else if(fractal == 1)
		{
			iterations = 0;

			complex<double> constantComplex = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> complexNum = complex<double>(0,0);

			iterations = recursiveColor(complexNum, constantComplex, 0);

			colorArray[i] = translateToColor(iterations);
		}
		else if(fractal == 2)
		{
			vec3 juliaColor;
			vec3 mandelbrotColor;
			vec3 mixedColor;

			complex<double> constantComplexM = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> complexNumM = complex<double>(0,0);
			complex<double> complexNumJ = complex<double>(pointArray[i].x, pointArray[i].y);
			complex<double> constantComplexJ = juliaConstant;

			mandelbrotColor = translateToColor(recursiveColor(complexNumM, constantComplexM, 0));

 			juliaColor = translateToColor(recursiveColor(complexNumJ, constantComplexJ, 0));

			for(int i = 0; i < 3; i++)
			{
				mixedColor[i] = (juliaColor[i] + mandelbrotColor[i]);
				while(mixedColor[i] > 1.0)
					mixedColor[i] -= 1.0;
			}

			colorArray[i] = mixedColor;
		}
		else
		{
			vec3 juliaColor;
			vec3 mandelbrotColor;
			vec3 mixedColor;

			for(int i = 0; i < totalPoints; i++)
			{
				complex<double> constantComplexM = complex<double>(pointArray[i].x, pointArray[i].y);
				complex<double> complexNumM = complex<double>(0,0);
				complex<double> complexNumJ = complex<double>(pointArray[i].x, pointArray[i].y);
				complex<double> constantComplexJ = juliaConstant;

				mandelbrotColor = translateToColor(recursiveColor(complexNumM, constantComplexM, 0));

 				juliaColor = translateToColor(recursiveColor(complexNumJ, constantComplexJ, 0));

				for(int i = 0; i < 3; i++)
				{
					if(juliaColor[i] > mandelbrotColor[i])
						mixedColor[i] = juliaColor[i];
					else
						mixedColor[i] = mandelbrotColor[i];
				}

				colorArray[i] = mixedColor;
			}
		}

	}
	if(command == 'Z')
		if(zoomLevel > 0.5)
			cerr << "Cannot zoom out anymore" << endl;
		else
			zoomLevel *= 2;
	if(command == 'z')
		zoomLevel /= 2;
}

void generateArrays()
{
	cout << "Generating points..." << endl;
	zoomLevel = 1.0;
	generatePointArray();
	generateColorArray();
	cout << "Generated." << endl;
}

void regenerateArrays(char command, vec2 location)
{
	cout << "Regenerating with command '" << command << "'..." << endl;
	regenerateColorArray(command, location);
	//rebuffer colors
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
	cout << "Regenerated." << endl;
}

vec3 translateToColor(double iterations)
{

	// calculate color
	// based on hsv scale at http://basecase.org/2011/12/hsv
	double red = 0;
	double green = 0;
	double blue = 0;
	if(colorType == 0)
	{
		double ratio = 0; // percentage of progress between milestones in HSV wheel
		double milestone = maxIterations/6;
		ratio = iterations / milestone;

		if ( iterations < milestone )
		{
			red = 1.0;
			green = ratio;
			blue = 0.0;
		}
		else if ( iterations < (milestone*2) )
		{
			red = 1.0 - ratio;
			green = 1.0;
			blue = 0.0;
		}
		else if ( iterations < (milestone*3) )
		{
			red = 0.0;
			green = 1.0;
			blue = ratio;
		}
		else if ( iterations < (milestone*4) )
		{
			red = 0.0;
			green = 1.0 - ratio;
			blue = 1.0;
		}
		else if ( iterations < (milestone*5) )
		{
			red = ratio;
			green = 0;
			blue = 1.0;
		}
		else if (iterations < (milestone*6) )
		{
			red = 1.0;
			green = 0.0;
			blue = 1.0 - ratio;
		}
		else if (iterations == 255)
		{
			red = 1.0;
			green = 0.0;
			blue = 0.0;
		}
	}
	else if(colorType == 1)
	{
		double LSB = maxIterations / (1<<23);
		double value;

		union{ 
			unsigned int uInt; 
			unsigned char charHolder [4];
		} doubleAsRGB;
		
		value = iterations / LSB;
		doubleAsRGB.uInt = value;

		red = double(doubleAsRGB.charHolder[2]/255.0);
		green = double(doubleAsRGB.charHolder[1]/255.0);
		blue = double(doubleAsRGB.charHolder[0]/255.0);
	}
	else
	{
		double LSB = maxIterations / (1<<25);
		double value;

		union{ 
			unsigned int uInt; 
			unsigned char charHolder [4];
		} doubleAsRGB;
		
		value = iterations / LSB;
		doubleAsRGB.uInt = value;

		red = double(doubleAsRGB.charHolder[2]/255.0);
		green = double(doubleAsRGB.charHolder[1]/255.0);
		blue = double(doubleAsRGB.charHolder[0]/255.0);


	}

	return vec3(red, green, blue);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_POINTS,0,totalPoints);
    glFlush();
	glutSwapBuffers();
}

void mouse(GLint button, GLint state, GLint x, GLint y) 
{
	//zoom in where you click
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//convert pixel coords to [-1,1] scale
		double newX = ((double)x/width*2) - 1;
		double newY = -(((double)y/height*2) - 1);
		regenerateArrays('z', vec2(newX,newY));
        glutPostRedisplay();    
    }
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {		
		regenerateArrays('Z', vec2(NULL));
        glutPostRedisplay();    
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
    
	//quit
    case 'Q':
    case 'q':
        cout << "Normal program exit." << endl;
        exit(EXIT_SUCCESS);

	//reset view
	case ' ':
		maxIterations = 100;
		zoomLevel = 1.0;
		generateArrays();
		//rebuffer colors
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 's':
		if(fractal == 0)
			fractal = Mandelbrot;
		else if(fractal == 1)
			fractal = Mixed;
		else if(fractal == 2)
			fractal = Greater;
		else
			fractal = Julia;
		cout << "Fractal type changed to " << fractalTypeArray[fractal] << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 'S':
		if(fractal == 3)
			fractal = Mandelbrot;
		else if(fractal == 2)
			fractal = Mixed;
		else if(fractal == 1)
			fractal = Greater;
		else
			fractal = Julia;
		cout << "Fractal type changed to " << fractalTypeArray[fractal] << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 'c':
	case 'C':
		cout << "Enter a number of maximum iterations: ";
		cin >> maxIterations;
		cout << "Maximum number of iterations is now " << maxIterations << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 'j':
		switch(juliaNumber)
		{
		case 0:
			juliaNumber = second;
			break;
		case 1:
			juliaNumber = third;
			break;
		case 2:
			juliaNumber = fourth;
			break;
		case 3:
			juliaNumber = fifth;
			break;
		case 4: 
			juliaNumber = sixth;
			break;
		case 5:
			juliaNumber = seventh;
			break;
		case 6:
			juliaNumber = eighth;
			break;
		case 7: 
			juliaNumber = ninth;
			break;
		case 8: 
			juliaNumber = tenth;
			break;
		case 9:
			juliaNumber = eleventh;
			break;
		case 10:
			juliaNumber = twelth;
			break;
		case 11:
			juliaNumber = first;
			break;
		}
		juliaConstant = juliaSetArray[juliaNumber];
		cout << "Changing Julia constant to " << juliaConstant << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 'J':
		switch(juliaNumber)
		{
		case 0:
			juliaNumber = twelth;
			break;
		case 1:
			juliaNumber = first;
			break;
		case 2:
			juliaNumber = second;
			break;
		case 3:
			juliaNumber = third;
			break;
		case 4: 
			juliaNumber = fourth;
			break;
		case 5:
			juliaNumber = fifth;
			break;
		case 6:
			juliaNumber = sixth;
			break;
		case 7: 
			juliaNumber = seventh;
			break;
		case 8: 
			juliaNumber = eighth;
			break;
		case 9:
			juliaNumber = ninth;
			break;
		case 10:
			juliaNumber = tenth;
			break;
		case 11:
			juliaNumber = eleventh;
			break;
		}
		juliaConstant = juliaSetArray[juliaNumber];
		cout << "Changing Julia constant to " << juliaConstant << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 'r':
		if(colorType == 0)
			colorType = RGB;
		else if(colorType == 1)
			colorType = RGBShift;
		else
			colorType = HSV;
		cout << "Displaying using " << colorSetArray[colorType] << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
	case 'R':
		if(colorType == 0)
			colorType = RGBShift;
		else if(colorType == 1)
			colorType = HSV;
		else
			colorType = RGB;
		cout << "Displaying using " << colorSetArray[colorType] << endl;
		generateArrays();
		glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
		break;
    default:
        cerr << "Unknown key command: '" << key << "'" << endl;
        break;
    }

    glutPostRedisplay();
}

void init()
{
    // Make a vertex array object
    GLuint vertArray;                 
    glGenVertexArrays(1,&vertArray);    
    glBindVertexArray(vertArray);     

    // Make a buffer object
    GLuint buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);

    // load the data into the array	
    glBufferData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints + sizeof(vec3) * totalPoints ,NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vec2) * totalPoints,pointArray);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vec2) * totalPoints,sizeof(vec3) * totalPoints ,colorArray);
	
    // Make a shader program
	GLuint shaderProgram = initShader("vert.glsl","frag.glsl");
    glUseProgram(shaderProgram);

    // Initialize the vertex shader's vertex position attribute 
	GLuint vPosition = glGetAttribLocation(shaderProgram,"vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition,2,GL_DOUBLE,GL_FALSE,0,0);
    
    // Initialize the vertex shader's vertex color attribute 
	GLuint vColor = glGetAttribLocation(shaderProgram,"vColor");
    glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor,3,GL_DOUBLE,GL_FALSE,0,BUFFER_OFFSET(sizeof(vec2)*totalPoints));

	glDisable(GL_DEPTH_TEST);
}



int main(int argc, char** argv) 
{
	generateArrays();

	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(WIDTH_PIXELS,HEIGHT_PIXELS);
    glutInitWindowPosition(0,0);

	glutCreateWindow("Fractal Space");

	glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

	glewInit();
	init();

    glutMainLoop();


    return 0;

}
