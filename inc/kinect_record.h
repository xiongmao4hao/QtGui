#include <k4a/k4a.hpp>
#include <thread>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <vector>
#include <array>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <k4arecord\types.h>
#include <k4arecord\record.h>
#include "pixel.h"
#include "depthpixelcolorizer.h"
#include "staticimageproperties.h"
#include "qtgui.h"
#include <QtWidgets/QApplication>
#include <k4abt.h>
#include <k4abttypes.h>
#include "getAngel.h"


int record_main(float(&joints_Angel)[12]);