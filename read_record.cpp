//#include "read_record.h"
//
//
//
//using namespace cv;
//using namespace std;
//
//
////��ȡͬ���ñ���
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
////	tranformation_helpers_write_point_cloud(point_cloud_image, color_image, file_name.c_str(), deviceId);//.c_str��ʾ��C++�ַ�����c�ַ�������ʽ����
////
////	k4a_image_release(transformed_depth_image);
////	k4a_image_release(point_cloud_image);
////
////	return true;
////}
//static int playback(char* input_path, uint8_t i, \
//	float(&joints_Angeluse)[ANGLE_NUM], float(&joints_AngelALL)[ANGLE_NUM],\
//	int num,\
//	std::string output_filename = "output.ply", int timestamp = 1000)//timestamp�������ʼ����Ƶ�޷��ֱ����������
//{
//	//Ϊ�˱��浥��һ������ĹؽڽǶȽ���������
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
//	VERIFY(k4a_playback_get_record_configuration(playback, &deviceConfig), "Get record0 info failed!");//��ȡ��Ƶ��Ϣ�����������
//
//
//	get_record_configint(deviceConfig);//����¼��Ĳ�����Ϣ��ӡ���ն���
//
//
//	result = k4a_playback_seek_timestamp(playback, timestamp * 1000, K4A_PLAYBACK_SEEK_BEGIN);//������ͷ������
//
//	if (result != K4A_RESULT_SUCCEEDED)
//	{
//		printf("Failed to seek timestamp %d\n", timestamp);
//		goto Exit;
//	}
//	printf("Seeking to timestamp: %d/%d (ms)\n",
//		timestamp,
//		(int)(k4a_playback_get_recording_length_usec(playback) / 1000));//��ȡ������Ƶ�ж�����
//	//��ȡ�������
//	if (K4A_RESULT_SUCCEEDED != k4a_playback_get_calibration(playback, &calibration))
//	{
//		printf("Failed to get calibration\n");
//		goto Exit;
//	}
//	//�ؽڵ�׷�ٱ���tracker�Ľ���
//	k4abt_tracker_t tracker = NULL;
//	k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
//	VERIFY(k4abt_tracker_create(&calibration, tracker_config, &tracker), "Body tracker initialization failed!");
//	//����������������Ƶ�İ�����ȡ
//	for (int progress = 1; progress < (int)(k4a_playback_get_recording_length_usec(playback) / 1000); progress++)
//	{//��ʼ
//		stream_result = k4a_playback_get_next_capture(playback, &capture);
//		if (stream_result != K4A_STREAM_RESULT_SUCCEEDED || capture == NULL)
//		{
//			printf("Failed to fetch frame\n");
//			goto Exit;
//		}
//		result = k4a_playback_seek_timestamp(playback, (timestamp + progress) * 1000, K4A_PLAYBACK_SEEK_BEGIN);//����Ƶ����ǰ��һ��
//
//		if (result != K4A_RESULT_SUCCEEDED)
//		{
//			printf("Failed to seek timestamp %d\n", timestamp);
//			goto Exit;
//		}
//		//����д������Ǽ�
//		k4a_wait_result_t queue_capture_result = \
//			k4abt_tracker_enqueue_capture(tracker, capture, K4A_WAIT_INFINITE);//�첽��ȡ������Ϣ
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
//					////***************��Ƕ�*******************
//					//float (*joints_Angel)[ANGLE_NUM] ;
//					//for (int i = 0; i < 12; i++) (*joints_Angel)[i] = NULL;//���󣺲��ܴ����յ������ʼ��
//
//					JointsPositionToAngel(skeleton, &joints_Angel);//���봫���ַ&��joints_Angel��Ȼֵ��ͬ������������������
//					//for (int i = 0; i < ANGLE_NUM; i++)
//					//{
//					//	//printf("%f", joints_Angel[i]);
//					//	//printf("   ");
//					//	fprintf(fpa, "%f,", joints_Angel[i]);//ע��ÿ������
//					//}
//					//fprintf(fpa, "\n");
//					//����ؽڵ������ںϽ��
//					std::unique_lock<std::mutex> locker_r(Flagrangle1to1r);
//					for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] += joints_Angel[i];//���н�����ۼ�
//					if (flagangle1to1r != 0 || i == master_num)//�����ռ�¼�����������豸�ʹ��豸������һ�ţ�flagangle1to1r == 0��ʱ���йؽڽ�ƽ����else��������ǰ�����ⲻ�����豸��
//					{
//						if (i == master_num)
//						{
//							flagangle1to1r += num - 1;
//							flagmasterr += 1;//����master�������������2��������豸δ��׽��
//						}
//						else
//							flagangle1to1r -= 1;
//						locker_r.unlock();
//
//					}
//					else
//					{
//						locker_r.unlock();
//						if (flagmasterr == 1 || num == 1)//master�������������2��������豸δ��׽�������ǵ������ʱ��һ����
//						{
//							for (int i = 0; i < ANGLE_NUM; i++) joints_AngelALL[i] = joints_Angeluse[i] / ANGLE_NUM;
//							for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] = 0;
//						}
//						else
//						{
//							//������������һ�����豸�ĵ�ǰ����������else
//							flagmasterr = 0;
//							flagangle1to1r = 0;
//							//����һ�䱣֤��ʼ���ۼ�����
//							for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] = 0;
//						}
//					}
//					//��ʱ����һ��
//
//				}
//
//
//			}
//
//
//
//
//			//transformation = k4a_transformation_create(&calibration);//��ȡ�ض������ɫ����������ת����Ϣ
//
//			//// ��ȡ����
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
//		Exit://�ͷ��ڴ�
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
//	}//����
//	
//}
//
//
//
//int read_record_main(char* input_path,float(&joints_AngelALL)[ANGLE_NUM],uint32_t num)
//{
//	int dev[3] = { 0, 1, 2 };
//	//�½�һ�ۼӵĹؽڵ�����
//	float joints_Angeluse[ANGLE_NUM];
//	for (int i = 0; i < ANGLE_NUM; i++) joints_Angeluse[i] = NULL;
//	//���������ñ���
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
//	//��֤���߳�ֹͣ
//	for (int i = 0; i < num; ++i)
//	{
//		tidsr[i].join();
//	}
//	//tid_ss.join();
//	//or use detach��֤���߳��ȶ�
//	//th1.detach();
//	//th2.detach();
//
//	//�ͷ��ڴ�,new�����������Ȼ���̽����󶼻����
//	delete[] tidsr;
//	delete[] colorframe;
//
//
//	return 0;
//}