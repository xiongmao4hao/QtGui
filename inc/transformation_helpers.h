// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <k4a/k4a.h>

extern float rotation_matrix_1t0_inv[3][3];
extern float translation_matrix_1t0[3];

void tranformation_helpers_write_point_cloud(const k4a_image_t point_cloud_image,
											const k4a_image_t color_image,
											const char* file_name,
											uint8_t deviceId);
