## README.md

Devon Hockely 30045270

## Description

Built from Tutorial 20 simple curves and my own 2nd assignment.

Done with 2 seperate windows as it is still just 2 viewports, and it made the logic much easier to conatin by essentially writing 2 different programs.

Also did a fair bit of what i THINK is c++ lambda/anonymous functions in order to make it work.

Controls:

A/D to cycle through points.

W/S to crete new points ahead/behind the current one.

X to delete a point

Arrow Keys to move the currently selected point.

## Configure

### Unix makefile (Tested)
	cmake -H. -Bbuild

### Mac OSX (Tested)
	cmake -H. -Bbuild -G "Xcode"

### Microsoft Windows (Tested - if you have Visual Studio 2017)
	(Change to "Visual Studio 16" if you have Visual Studio 2019)
	cmake -H. -Bbuild -G "Visual Studio 15"

or

	cmake -H. -Bbuild -G "Visual Studio 15 Win64"

