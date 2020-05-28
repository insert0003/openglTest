/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年05月27日 13時51分53秒
 ************************************************************************/
#include <IL/il.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>

char* SkBlendName[] = {
	"kSrc",
	"kSrcOver",
	"kSrcIn",
	"kSrcOut",
	"kSrcATop",
	"kDst",
	"kDstOver",
	"kDstIn",
	"kDstOut",
	"kDstATop"
};

enum SkBlendMode {
    kSrc = 0,                             //!< replaces destination
    kSrcOver,                         //!< source over destination
    kSrcIn,                           //!< source trimmed inside destination
    kSrcOut,                          //!< source trimmed outside destination
    kSrcATop,                         //!< source inside destination blended with destination
    kDst = 5,                             //!< preserves destination
    kDstOver,                         //!< destination over source
    kDstIn,                           //!< destination trimmed by source
    kDstOut,                          //!< destination trimmed outside source
    kDstATop,                         //!< destination inside source blended with source
    kXor = 10,                             //!< each of source and destination trimmed outside the other
    kPlus,                            //!< sum of colors
    kModulate,                        //!< product of premultiplied colors; darkens destination
    kScreen,                //!< multiply inverse of pixels, inverting result; brightens destination
    kLastCoeffMode     = kScreen,     //!< last porter duff blend mode
    kOverlay,                         //!< multiply or screen, depending on destination
    kDarken,                          //!< darker of source and destination
    kLighten,                         //!< lighter of source and destination
    kColorDodge,                      //!< brighten destination to reflect source
    kColorBurn,                       //!< darken destination to reflect source
    kHardLight,                       //!< multiply or screen, depending on source
    kSoftLight,                       //!< lighten or darken, depending on source
    kDifference,                      //!< subtract darker from lighter with higher contrast
    kExclusion,                       //!< subtract darker from lighter with lower contrast
    kMultiply,                        //!< multiply source with destination, darkening image
    kLastSeparableMode = kMultiply,   //!< last blend mode operating separately on components
    kHue,                           //!< hue of source with saturation and luminosity of destination
    kSaturation,                    //!< saturation of source with hue and luminosity of destination
    kColor,                         //!< hue and saturation of source with luminosity of destination
    kLuminosity,                    //!< luminosity of source with hue and saturation of destination
    kLastMode          = kLuminosity, //!< last valid value
};

int width=980;
int height=560;

GLuint tex_layer14, tex_layer13, tex_layer15;
GLuint tex_src, tex_dst;
enum SkBlendMode blend_mode = kSrcOver;

void applyBlendMode(enum SkBlendMode blend_mode, GLuint src, GLuint dst) {
	if (blend_mode >= 5 && blend_mode < 10){
		tex_src=dst;
		tex_dst=src;
	}else{
		tex_src=src;
		tex_dst=dst;
	}
	printf("use blendmode %s. src: %d, dst: %d. \n", SkBlendName[blend_mode], tex_src, tex_dst);
	switch (blend_mode) {
		case kSrc:
		case kDst:
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		case kSrcOver:
		case kDstOver:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case kSrcIn:
		case kDstIn:
			glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			// glBlendFunc(GL_DST_ALPHA, GL_ZERO);
			break;
		case kSrcOut:
		case kDstOut:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_COLOR);
			break;
		case kScreen:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;
		// case kOverlay:
		// 	equation = GL_OVERLAY_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kDifference:
		// 	equation = GL_DIFFERENCE_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kExclusion:
		// 	equation = GL_EXCLUSION_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kMultiply:
		// 	equation = GL_MULTIPLY_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kHue:
		// 	equation = GL_HSL_HUE_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kSaturation:
		// 	equation = GL_HSL_SATURATION_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kColor:
		// 	equation = GL_HSL_COLOR_KHR;
		//     glBlendEquation(equation);
		// 	break;
		// case kLuminosity:
		// 	equation = GL_HSL_LUMINOSITY_KHR;
		//     glBlendEquation(equation);
		// 	break;
	}
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() 
{
    // Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);     // Operate on model-view matrix

	applyBlendMode(blend_mode, tex_layer13, tex_layer14);

    /* Draw Dst quad */
	glBindTexture(GL_TEXTURE_2D, tex_dst);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0,   0);
	glTexCoord2i(0, 1); glVertex2i(0,   height);
	glTexCoord2i(1, 1); glVertex2i(width, height);
	glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();

	/* Draw Src texture */
	glBindTexture(GL_TEXTURE_2D, tex_src);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0,   0);
	glTexCoord2i(0, 1); glVertex2i(0,   height);
	glTexCoord2i(1, 1); glVertex2i(width, height);
	glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();

	/* Draw Src texture */
	// glBindTexture(GL_TEXTURE_2D, tex_layer15);
	// glBegin(GL_QUADS);
	// glTexCoord2i(0, 0); glVertex2i(0,   0);
	// glTexCoord2i(0, 1); glVertex2i(0,   height);
	// glTexCoord2i(1, 1); glVertex2i(width, height);
	// glTexCoord2i(1, 0); glVertex2i(width, 0);
	// glEnd();

    glutSwapBuffers();
} 

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei newwidth, GLsizei newheight) 
{  
    // Set the viewport to cover the new window
    glViewport(0, 0, width=newwidth, height=newheight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

/* Initialize OpenGL Graphics */
void initGL(int w, int h) 
{
    glViewport(0, 0, w, h); // use a screen size of WIDTH x HEIGHT
    glEnable(GL_TEXTURE_2D);     // Enable 2D texturing
 
    glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, 0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
}

/* Load an image using DevIL and return the devIL handle (-1 if failure) */
int LoadImage(char *filename)
{
    ILboolean success;
	ILuint image;

    ilGenImages(1, &image); /* Generation of one image name */
	ilBindImage(image); /* Binding of image name */
	success = ilLoadImage(filename); /* Loading of the image filename by DevIL */
     
    if (success) {
        /* Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 
        if (!success)
			return -1;
    }else
		return -1;

	printf("image: %d. \n", image);
    return image;
}

void key(unsigned char key , int x , int y) {
	printf("Key = %c \n" , key);
	if (key >= '0' && key <= '9'){
		blend_mode = key - 48;
		display();
	}else{
		exit(0);
	}
}

int main(int argc , char ** argv) {
    int image01, image02, image03;

    if (argc < 1) /* no image file to  display */
        return -1;

	glutInit(&argc , argv);
	glutInitWindowSize(width , height);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("Test Blend");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);

    initGL(width, height);

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
		printf("wrong DevIL version");
		return -1;
	}
	ilInit();

	image01 = LoadImage(argv[1]);
    if (image01 == -1) {
        printf("Can't load picture file %s by DevIL", argv[1]);
        return -1;
    }

    /* OpenGL texture binding of the image loaded by DevIL  */
	glGenTextures(1, &tex_layer14); /* Texture name generation */
	glBindTexture(GL_TEXTURE_2D, tex_layer14); /* Binding of texture name */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 
    		     0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */

	image02 = LoadImage(argv[2]);
    if (image02 == -1) {
        printf("Can't load picture file %s by DevIL", argv[1]);
        return -1;
    } 

    /* OpenGL texture binding of the image loaded by DevIL  */
	glGenTextures(1, &tex_layer13); /* Texture name generation */
	glBindTexture(GL_TEXTURE_2D, tex_layer13); /* Binding of texture name */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 
    		     0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */

	// image03 = LoadImage(argv[3]);
    // if (image03 == -1) {
    //     printf("Can't load picture file %s by DevIL", argv[1]);
    //     return -1;
    // } 

    // /* OpenGL texture binding of the image loaded by DevIL  */
	// glGenTextures(1, &tex_layer15); /* Texture name generation */
	// glBindTexture(GL_TEXTURE_2D, tex_layer15); /* Binding of texture name */
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
	// glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 
    // 		     0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */

	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);

    /* Main loop */
    glutMainLoop();
    
    /* Delete used resources and quit */
	ilDeleteImages(1, &image01); /* Because we have already copied image data into texture data we can release memory used by image. */
	ilDeleteImages(1, &image02); /* Because we have already copied image data into texture data we can release memory used by image. */
	ilDeleteImages(1, &image03); /* Because we have already copied image data into texture data we can release memory used by image. */
	glDeleteTextures(1, &tex_layer15);
	glDeleteTextures(1, &tex_layer14);
	glDeleteTextures(1, &tex_layer13);

	return 0;
}
