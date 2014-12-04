#include "../Classes/AppDelegate.h"
#include "cocos2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

USING_NS_CC;

int main(int argc, char **argv)
{
	int width = 1400;
	int heigth = 800;

	int paramCounter = 1; //start with 1 because 0 is program name
	while(argc - paramCounter >= 2){
		std::string command = argv[paramCounter++];
		if(command.compare("-n") == 0){
			CCUserDefault::sharedUserDefault()->setStringForKey("playername", argv[paramCounter++]);
		} else if(command.compare("-w") == 0){
			width = atoi (argv[paramCounter++]);
		} else if(command.compare("-h") == 0){
			heigth = atoi (argv[paramCounter++]);
		}
	}

    // create the application instance
    AppDelegate app;

    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize(width, heigth);

    return CCApplication::sharedApplication()->run();
}
