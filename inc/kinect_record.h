#pragma once

#include <stdlib.h>

#include <k4a/k4a.hpp>
#include "windows_thread.h"
#include "windows_time.h"
#include "kinect_cv_dk.h"
#include "kinect_angle.h"




int record_main(float(&joints_Angel)[ANGLE_NUM]);