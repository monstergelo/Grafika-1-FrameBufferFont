#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

typedef struct{
	int r,g,b,a;
} color;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0 ; //pointer framebuffer
int fbfd = 0; //pointer framebuffer driver

void clear_screen(int width, int height);
void draw_dot(int x, int y, color* c);

int main()
{
//technical Stuff========================================================================
	
	// Membuka driver framebuffer
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: driver framebuffer tidak dapat dibuka");
        exit(1);
    }

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    // Mapping framebuffer ke memori
    int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if((int)fbp == -1){
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }

//=======================================================================================

    color c = {255,255,255,255};
    int x = 0;
    int y = 0;

    clear_screen(300, 300);
    for(x=0; x<31 ; x++)
    {
    	for(y=0; y<31 ; y++)
    	{
    		draw_dot(x,y,&c);
    	}
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;

}
//End of main

//######################################################################################
void clear_screen(int width, int height)
{
	int x = 0;
	int y = 0;

	for(x=0; x<width; x++)
	{
		for(y=0; y<height; y++)
		{
			long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + 
			   (y + vinfo.yoffset) * finfo.line_length;
			*(fbp + position) = 255;
		    *(fbp + position + 1) = 70;
		    *(fbp + position + 2) = 61;
		    *(fbp + position + 3) = 255;
		}
	}
}
//######################################################################################
void draw_dot(int x, int y, color* c)
{
	long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + 
	   (y + vinfo.yoffset) * finfo.line_length;
	if(vinfo.bits_per_pixel == 32){
		*(fbp + position) = c->b;
	    *(fbp + position + 1) = c->g;
	    *(fbp + position + 2) = c->r;
	    *(fbp + position + 3) = c->a;
	}
	else
	{//assume 16 bit color
        int b = c->b;
        int g = c->g;
        int r = c->r;
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + position)) = t;
	}
}
//######################################################################################



void draw_huruf(int x, int y)
{

}