//#include "read_record.h"
//
//
//
//using namespace cv;
//using namespace std;
//
//
////读取同步用变量
//mutex Flag1to1r, Flagr1to1r, Flagrangle1to1r;
//int flag1to1r = 0, flagr1to1r = 0, flagangle1to1r = 0, flagmasterr = 0;
//const int master_num = 0;
//
//thread* tidsr;
//
////static bool point_cloud_depth_to_color(k4a_transformation_t transformation_handle,
////	const k4a_image_t depth_image,
////	const k4a_image_t color_image,
////	std::string file_name,
////	uint8_t deviceId)
////{
////	// transform color image into depth camera geometry
////	int color_image_width_pixels = k4a_image_get_width_pixels(color_image);
////	int color_image_height_pixels = k4a_image_get_height_pixels(color_image);
////	k4a_image_t transformed_depth_image = NULL;
////	if (K4A_RESULT_SUCCEEDED != k4a_image_create(K4A_IMAGE_FORMAT_DEPTH16,
////		color_image_width_pixels,
////		color_image_height_pixels,
////		color_image_width_pixels * (int)sizeof(uint16_t),
////		&transformed_depth_image))
////	{
////		printf("Failed to create transformed depth image\n");
////		return false;
////	}
////
////	k4a_image_t point_cloud_image = NULL;
////	if (K4A_RESULT_SUCCEEDED != k4a_image_create(K4A_IMAGE_FORMAT_CUSTOM,
////		color_image_width_pixels,
////		color_image_height_pixels,
////		color_image_width_pixels * 3 * (int)sizeof(int16_t),
////		&point_cloud_image))
////	{
////		printf("Failed to create point cloud image\n");
////		return false;
////	}
////
////	if (K4A_RESULT_SUCCEEDED !=
////		k4a_transformation_depth_image_to_color_camera(transformation_handle, depth_image, transformed_depth_image))
////	{
////		printf("Failed to compute transformed depth image\n");
////		return false;
////	}
////
////	if (K4A_RESULT_SUCCEEDED != k4a_transformation_depth_image_to_point_cloud(transformation_handle,
////		transformed_depth_image,
////		K4A_CALIBRATION_TYPE_COLOR,
////		point_cloud_image))
////	{
////		printf("Failed to compute point cloud\n");
////		return false;
////	}
////
////	tranformation_helpers_write_point_cloud(point_cloud_image, color_image, file_name.c_str(), deviceId);//.c_str表示把C++字符串以c字符串的形式返回
////
////	k4a_image_release(transformed_depth_image);
////	k4a_image_release(point_cloud_image);
////
////	return true;
////}
//static int playback(char* input_path, uint8_t i, \
//	float(&joints_Angeluse)[ANGLE_NUM], float(&joints_AngelALL)[ANGLE_NUM],\
//	int num,\
//	std::string output_filename = "output.ply", int timestamp = 1000)//timestamp代表了最开始的视频无法分辨骨骼的死区
//{
//	//为了保存单独一个相机的关节角度建立的数组
//	float joints_Angel[ANGLE_NUM];
//	for (int i = 0; i < ANGLE_NUM; i++) joints_Angel[i] = NULL;
//
//	int returnCode = 1;
//	k4a_playback_t playback = NULL;
//	k4a_calibration_t calibration;
//	k4a_transformation_t transformation = NULL;
//	k4a_capture_t capture = NULL;
//	k4a_image_t depth_image = NULL;
//	k4a_image_t color_image = NULL;
//	k4a_image_t uncompressed_color_image = NULL;
//	k4a_record_configuration_t deviceConfig;
//
//	k4a_result_t result;
//	k4a_stream_result_t stream_result;
//
//
//
//	// Open recording
//	result = k4a_playback_open(input_path, &playback);
//	if (result != K4A_RESULT_SUCCEEDED || playback == NULL)
//	{
//		printf("Failed to open recording %s\n", input_path);
//		goto Exit;
//	}
//
//	VERIFY(k4a_playback_get_record_configuration(playback, &deviceConfig), "Get record0 info failed!");//获取视频信息（拍摄参数）
//
//
//	get_record_configint(deviceConfig);//各个录像的参数信息打印在终端中
//
//
//	result = k4a_playback_seek_timestamp(playback, timestamp * 1000, K4A_PLAYBACK_SEEK_BEGIN);//跳过开头的死区
//
//	if (result != K4A_RESULT_SUCCEEDED)
//	{
//		printf("Failed to seek timestamp %d\n", timestamp);
//		goto Exit;
//	}
//	printf("Seeking to timestamp: %d/%d (ms)\n",
//		timestamp,
//		(int)(k4a_playback_get_recording_length_usec(playback) / 1000));//获取整个视频有多少秒
//	//获取相机参数
//	if (K4A_RESULT_SUCCEEDED != k4a_playback_get_calibration(playback, &calibration))
//	{
//		printf("Failed to get calibration\n");
//		goto Exit;
//	}
//	//关节点追踪变量tracker的建立
//	k4abt_tracker_t tracker = NULL;
//	k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
//	VERIFY(k4abt_tracker_create(&calibration, tracker_config, &tracker), "Body tracker initialization failed!");
//	//接下来进行整个视频的挨个读取
//	for (int progress = 1; progress < (int)(k4a_playback_get_recording_length_usec(playback) / 1000); progress++)
//	{//开始
//		stream_result = k4a_playback_get_next_capture(playback, &capture);
//		if (stream_result != K4A_STREAM_RESULT_SUCCEEDED || capture == NULL)
//		{
//			printf("Failed to fetch frame\n");
//			goto Exit;
//		}
//		result = k4a_playback_seek_timestamp(playback, (timestamp + progress) * 1000, K4A_PLAYBACK_SEEK_BEGIN);//将视频进度前进一秒
//
//		if (result != K4A_RESULT_SUCCEEDED)
//		{
//			printf("Failed to seek timestamp %d\n", timestamp);
//			goto Exit;
//		}
//		//捕获并写入人体骨架
//		k4a_wait_result_t queue_capture_result = \
//			k4abt_tracker_enqueue_capture(tracker, capture, K4A_WAIT_INFINITE);//异步提取骨骼信息
//		if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)// && queue_capture_result1 == K4A_WAIT_RESULT_TIMEOUT)
//		{
//			// It should never hit timeout when K4A_WAIT_INFINITE is set.
//			printf("Error! Add capture to tracker process queue timeout!\n");
//		}
//		else if (queue_capture_result == K4A_WAIT_RESULT_FAILED)// && queue_capture_result1 == K4A_WAIT_RESULT_FAILED)
//		{
//			printf("Error! Add capture to tracker process queue failed!\n");
//		}
//		else
//		{
//			k4abt_frame_t body_frame = NULL;
//			k4a_wait_result_t pop_frame_result = \
//				k4abt_tracker_pop_result(tracker, &body_frame, K4A_WAIT_INFINITE);
//			if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
//			{
//
//				//Get the number of detecied human bodies
//				//size_t num_bodies = k4abt_frame_get_num_bodies(body_frame0);
//				//Get access to every idex of human bodies
//				k4abt_skeleton_t skeleton;
//				k4a_result_t get_body_skeleton = \
//					k4abt_frame_get_body_skeleton(body_frame, 0, &skeleton);
//
//
//				if (get_body_skeleton == K4A_RESULT_SUCCEEDED)
//				{
//
//					////***************求角度*******************
//					//float (*joints_Angel)[ANGLE_NUM] ;
//					//for (int i = 0; i < 12; i++) (*joints_Angel)[i] = NULL;//错误：不能创建空的数组初始化
//
//					JointsPositionToAngel(skeleton, &joints_Angel);//必须传入地址&，joints_Angel虽然值相同但是数据类型有问题
//					//for (int i = 0; i < ANGLE_NUM; i++)
//					//{
//					//	//printf("%f", joints_Angel[i]);
//					//	//printf("   ");
//					//	fprintf(fpa, "%f,", joints_Angel[i]);//注意每个保存
//					//}
//					//fprintf(fpa, "\n");
//					//保存关节点数据融合结果
//					std::unique_lock<std::mutex> locker_r(Flagrangle1to1r);
//					for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] += joints_Angel[i];//进行结果的累加
//					if (flagangle1to1r != 0 || i == master_num)//在拍照记录抵消（即主设备和从设备都拍完一张，flagangle1to1r == 0）时进行关节角平均（else操作）。前提是这不是主设备。
//					{
//						if (i == master_num)
//						{
//							flagangle1to1r += num - 1;
//							flagmasterr += 1;//计数master个数，如果超过2个代表从设备未捕捉到
//						}
//						else
//							flagangle1to1r -= 1;
//						locker_r.unlock();
//
//					}
//					else
//					{
//						locker_r.unlock();
//						if (flagmasterr == 1 || num == 1)//master个数，如果超过2个代表从设备未捕捉到（但是单摄像机时不一样）
//						{
//							for (int i = 0; i < ANGLE_NUM; i++) joints_AngelALL[i] = joints_Angeluse[i] / ANGLE_NUM;
//							for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] = 0;
//						}
//						else
//						{
//							//以下两句在下一个主设备拍到前都会进入这个else
//							flagmasterr = 0;
//							flagangle1to1r = 0;
//							//以下一句保证初始化累加数组
//							for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] = 0;
//						}
//					}
//					//延时函数一秒
//
//				}
//
//
//			}
//
//
//
//
//			//transformation = k4a_transformation_create(&calibration);//获取特定相机彩色和深度相机的转换信息
//
//			//// 获取点云
//			//depth_image = k4a_capture_get_depth_image(capture);
//			//if (depth_image == 0)
//			//{
//			//	printf("Failed to get depth image from capture\n");
//			//	goto Exit;
//			//}
//
//			//color_image = k4a_capture_get_color_image(capture);
//			//if (color_image == 0)
//			//{
//			//	printf("Failed to get color image from capture\n");
//			//	goto Exit;
//			//}
//
//			/////////////////////////////////
//			//// Convert color frame from mjpeg to bgra
//			//k4a_image_format_t format;
//			//format = k4a_image_get_format(color_image);
//			//if (format != K4A_IMAGE_FORMAT_COLOR_MJPG)
//			//{
//			//	printf("Color format not supported. Please use MJPEG\n");
//			//	goto Exit;
//			//}
//
//			//int color_width, color_height;
//			//color_width = k4a_image_get_width_pixels(color_image);
//			//color_height = k4a_image_get_height_pixels(color_image);
//
//			//if (K4A_RESULT_SUCCEEDED != k4a_image_create(K4A_IMAGE_FORMAT_COLOR_BGRA32,
//			//	color_width,
//			//	color_height,
//			//	color_width * 4 * (int)sizeof(uint8_t),
//			//	&uncompressed_color_image))
//			//{
//			//	printf("Failed to create image buffer\n");
//			//	goto Exit;
//			//}
//
//			//tjhandle tjHandle;
//			//tjHandle = tjInitDecompress();
//			//if (tjDecompress2(tjHandle,
//			//	k4a_image_get_buffer(color_image),
//			//	static_cast<unsigned long>(k4a_image_get_size(color_image)),
//			//	k4a_image_get_buffer(uncompressed_color_image),
//			//	color_width,
//			//	0, // pitch
//			//	color_height,
//			//	TJPF_BGRA,
//			//	TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE) != 0)
//			//{
//			//	printf("Failed to decompress color frame\n");
//			//	if (tjDestroy(tjHandle))
//			//	{
//			//		printf("Failed to destroy turboJPEG handle\n");
//			//	}
//			//	goto Exit;
//			//}
//			//if (tjDestroy(tjHandle))
//			//{
//			//	printf("Failed to destroy turboJPEG handle\n");
//			//}
//			/////////////////////////////////
//
//			//// Compute color point cloud by warping depth image into color camera geometry
//			//if (point_cloud_depth_to_color(transformation, depth_image, uncompressed_color_image, output_filename, deviceId) == false)
//			//{
//			//	printf("Failed to transform depth to color\n");
//			//	goto Exit;
//			//}
//
//			//returnCode = 0;
//
//		Exit://释放内存
//			if (playback != NULL)
//			{
//				k4a_playback_close(playback);
//			}
//			if (depth_image != NULL)
//			{
//				k4a_image_release(depth_image);
//			}
//			if (color_image != NULL)
//			{
//				k4a_image_release(color_image);
//			}
//			if (uncompressed_color_image != NULL)
//			{
//				k4a_image_release(uncompressed_color_image);
//			}
//			if (capture != NULL)
//			{
//				k4a_capture_release(capture);
//			}
//			if (transformation != NULL)
//			{
//				k4a_transformation_destroy(transformation);
//			}
//			return returnCode;
//		}
//	}//结束
//	
//}
//
//
//
//int read_record_main(char* input_path,float(&joints_AngelALL)[ANGLE_NUM],uint32_t num)
//{
//	int dev[3] = { 0, 1, 2 };
//	//新建一累加的关节点数据
//	float joints_Angeluse[ANGLE_NUM];
//	for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] = NULL;
//	//主函数所用变量
//
//	cv::Mat* colorframe = NULL;
//	int master_num = -1;
//
//
//	tidsr = new thread[num];
//	colorframe = new cv::Mat[num];
//
//
//	//tid_ss = thread(keyboards);
//	for (int i = 0; i < num; ++i)
//	{
//		tidsr[i] = thread(playback, input_path,i,ref(joints_Angeluse), ref(joints_AngelALL), num,"output.ply", 1000);
//
//	}
//	for (int i = 0; i < num; ++i)
//	{
//		cout << "thread' id is " << tidsr[i].get_id() << endl;
//	}
//
//	//保证子线程停止
//	for (int i = 0; i < num; ++i)
//	{
//		tidsr[i].join();
//	}
//	//tid_ss.join();
//	//or use detach保证子线程稳定
//	//th1.detach();
//	//th2.detach();
//
//	//释放内存,new后最好做，虽然进程结束后都会回收
//	delete[] tidsr;
//	delete[] colorframe;
//
//
//	return 0;
//}