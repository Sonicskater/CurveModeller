/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */


#include <cmath>
#include "GLFW/glfw3.h"
#include "shader.hpp"
#include "windows/CurveEditor.cpp"
#include "windows/MeshViewer.h"


int main() {

	auto win = CurveEditor("Editor",600,600);
	auto win2 = MeshViewer("Viewer",800,800);
	while (!(win.shouldClose() || win2.shouldClose())) {
        win.draw();

        win2.controlPoints = win.out_curve;

        win2.draw();
	}

	// cleaup window, and glfw before exit
	win.destroy();
	win2.destroy();
	glfwTerminate();

	return EXIT_SUCCESS;
}
