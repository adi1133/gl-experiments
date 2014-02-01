/*
 * MainLoop.h
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

#include <GLFW/glfw3.h>
class MainLoop
{
public:
	MainLoop();
	void run(GLFWwindow* window);
	virtual ~MainLoop();
};

#endif /* MAINLOOP_H_ */
