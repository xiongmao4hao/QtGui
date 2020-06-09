#pragma once

#include <stdlib.h>

#include <k4a/k4a.hpp>
#include "windows_thread.h"
#include "windows_time.h"
#include "kinect_cv_dk.h"
#include "kinect_angle.h"

#include <array>
#include <iostream>
#include <map>
#include <vector>

#include <BodyTrackingHelpers.h>
#include <Utilities.h>
#include <Window3dWrapper.h>

#include <cmath>
#include <time.h>

#include <math.h>




int record_main(float(&joints_Angel)[ANGLE_NUM], float (& dis)[1]);