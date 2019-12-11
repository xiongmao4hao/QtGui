#include "kinect_record.h"


using namespace cv;
using namespace std;

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8001) ? 1:0)//自对GetAsyncKeyState函数的上一次调用以来，如键已被按过，则位0设为1；否则设为0。如键目前处于按下状态，则位15设为1；如抬起，则为0。详见有道云
//定义的报错显示程序
#define VERIFY(result, error)                                                                            \
    if(result != K4A_RESULT_SUCCEEDED)                                                                   \
    {                                                                                                    \
        printf("%s \n - (File: %s, Function: %s, Line: %d)\n", error, __FILE__, __FUNCTION__, __LINE__); \
        exit(1);                                                                                         \
    }       

thread* tids;
thread* tids_s;
k4a_device_t* dev;
k4a_record_t* record = NULL;
k4a_calibration_t* sensor_calibration;
k4a_device_configuration_t* config;
mutex Flag1to1, Flagr1to1;
int flag1to1 = 0, flagr1to1 = 0;
//bool esc = false;



void save(cv::Mat colorFrame, int i, int frame_count)//每次保存时所创建的线程
{
	imwrite(".\\photos" + std::to_string(i) + "\\alpha" + std::to_string(frame_count) + ".jpg", colorFrame);
	cout << "You can take another picture1!" << endl;
}

void cap(k4a_device_t& dev_d, cv::Mat& colorFrame, k4a_record_t& record_d, int i, int master_num, int num, float(&joints_Angel)[ANGLE_NUM])  //普通的函数，用来执行线程
{
	//为save建立的变量
	int flag = -1, flag_r = -1;
	int frame_count = 0;
	FILE* fp = NULL;
	FILE* fpa = NULL;
	//主要功能实现所用变量
	//k4a::image depthImage;
	k4a_image_t colorImage;
	//k4a::image irImage;
	k4a_float3_t tf_source_depth;
	k4a_float3_t tf_target_color;
	//cv::Mat depthFrame;
	//cv::Mat irFrame;

	//std::vector<Pixel> depthTextureBuffer;
	//std::vector<Pixel> irTextureBuffer;
	uint8_t* colorTextureBuffer;

	k4a_capture_t sensor_capture;//捕获用变量
	//关节点追踪变量tracker的建立
	k4abt_tracker_t tracker = NULL;
	k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
	VERIFY(k4abt_tracker_create(&sensor_calibration[i], tracker_config, &tracker), "Body tracker initialization failed!");
	//查看capture地址
	//cout << "capture adress" << &sensor_capture << endl;

	//普通视频声明
	//cv:VideoWriter colorwriter;
	//为了创建普通视频的读取一帧的函数
	//while (1)
	//{
	//	if (k4a_device_get_capture(dev_d, &sensor_capture, K4A_WAIT_INFINITE) == K4A_WAIT_RESULT_SUCCEEDED)
	//	{			
	//		//depthImage = capture.get_depth_image();
	//		colorImage = k4a_capture_get_color_image(sensor_capture);
	//		k4a_capture_release(sensor_capture);
	//		//irImage = capture.get_ir_image();


	//		//ColorizeDepthImage(depthImage, DepthPixelColorizer::ColorizeBlueToRed, GetDepthModeRange(config.depth_mode), &depthTextureBuffer);
	//		//ColorizeDepthImage(irImage, DepthPixelColorizer::ColorizeGreyscale, GetIrLevels(K4A_DEPTH_MODE_PASSIVE_IR), &irTextureBuffer);

	//		colorTextureBuffer = k4a_image_get_buffer(colorImage);

	//		//depthFrame = cv::Mat(depthImage.get_height_pixels(), depthImage.get_width_pixels(), CV_8UC4, depthTextureBuffer.data());
	//		colorFrame = cv::Mat(1, k4a_image_get_height_pixels(colorImage) * k4a_image_get_width_pixels(colorImage), CV_8UC1, colorTextureBuffer);
	//		colorFrame = imdecode(colorFrame, IMREAD_COLOR);
	//		k4a_image_release(colorImage);
	//		/*cvtColor(colorFrame, colorFrame, COLOR_BGRA2BGR);*/
	//		break;
	//	}
	//}
	//创建普通视频
	//colorwriter.open(".\\output-" + std::to_string(i) + ".mkv", VideoWriter::fourcc('M', 'J', 'P', 'G'), 15, colorFrame.size(), true);
	//if (!colorwriter.isOpened())
	//{
	//	cout << "初始化VideoWriter失败！" << endl;
	//}

	while (1)
	{
		if (k4a_device_get_capture(dev_d, &sensor_capture, K4A_WAIT_INFINITE) == K4A_WAIT_RESULT_SUCCEEDED)
		{
			//depthImage = capture.get_depth_image();
			colorImage = k4a_capture_get_color_image(sensor_capture);//从捕获中获取图像
			if (colorImage != NULL)
			{
				/*printf(" | Depth16 res:%4dx%4d stride:%5d time:%lld\n",
					k4a_image_get_height_pixels(colorImage),
					k4a_image_get_width_pixels(colorImage),
					k4a_image_get_stride_bytes(colorImage),
					k4a_image_get_system_timestamp_nsec(colorImage));*/
			}
			if ((GetKeyState('R') & 0x8001) ? 1 : 0)
			{
				if (record_d != NULL)
				{
					//写视频
					k4a_record_write_capture(record_d, sensor_capture);
					cout << "have recorded\n";
					//捕获并写入人体骨架
					k4a_wait_result_t queue_capture_result = \
						k4abt_tracker_enqueue_capture(tracker, sensor_capture, K4A_WAIT_INFINITE);//异步提取骨骼信息
					if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)// && queue_capture_result1 == K4A_WAIT_RESULT_TIMEOUT)
					{
						// It should never hit timeout when K4A_WAIT_INFINITE is set.
						printf("Error! Add capture to tracker process queue timeout!\n");
					}
					else if (queue_capture_result == K4A_WAIT_RESULT_FAILED)// && queue_capture_result1 == K4A_WAIT_RESULT_FAILED)
					{
						printf("Error! Add capture to tracker process queue failed!\n");
					}
					else
					{
						k4abt_frame_t body_frame = NULL;
						k4a_wait_result_t pop_frame_result = \
							k4abt_tracker_pop_result(tracker, &body_frame, K4A_WAIT_INFINITE);
						if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
						{
							//Get the number of detecied human bodies
							//size_t num_bodies = k4abt_frame_get_num_bodies(body_frame0);
							//Get access to every idex of human bodies
							k4abt_skeleton_t skeleton;
							k4a_result_t get_body_skeleton = \
								k4abt_frame_get_body_skeleton(body_frame, 0, &skeleton);

							if (get_body_skeleton == K4A_RESULT_SUCCEEDED)
							{
								////***************求角度*******************
								//float (*joints_Angel)[ANGLE_NUM] ;
								//for (int i = 0; i < 12; i++) (*joints_Angel)[i] = NULL;//错误：不能创建空的数组初始化
								
								JointsPositionToAngel(skeleton, &joints_Angel);//必须传入地址&，joints_Angel虽然值相同但是数据类型有问题
								for (int i = 0; i < ANGLE_NUM; i++)
								{
									//printf("%f", joints_Angel[i]);
									//printf("   ");
									fprintf(fpa,"%f,", joints_Angel[i]);//注意每个保存
								}
								fprintf(fpa, "\n");
								//****************************************
								uint64_t timestamp;
								timestamp = k4abt_frame_get_device_timestamp_usec(body_frame);
								fprintf(fp, "%llu,", timestamp);
								for (int i = 0; i < BODU_POINT_NUM; i++)
								{
									// write the raw cordinates into the txt file
									/*tf_source_depth.xyz.x = skeleton.joints[i].position.xyz.x;
									tf_source_depth.xyz.y = skeleton.joints[i].position.xyz.y;
									tf_source_depth.xyz.z = skeleton.joints[i].position.xyz.z;*/
									tf_target_color.xyz.x = skeleton.joints[i].position.xyz.x;
									tf_target_color.xyz.y = skeleton.joints[i].position.xyz.y;
									tf_target_color.xyz.z = skeleton.joints[i].position.xyz.z;
									/* Doing cordinate translation there */
								/*	k4a_result_t tf_result = \
										k4a_calibration_3d_to_3d\
										(&sensor_calibration[i], &tf_source_depth, K4A_CALIBRATION_TYPE_DEPTH, K4A_CALIBRATION_TYPE_COLOR, &tf_target_color);*/
										// write the transfered cordinates into the txt file
									if (1)// tf_result == K4A_RESULT_SUCCEEDED)//为了彩色，深度相机旋转变换是否成功的预留变量tf_result
									{
										fprintf(fp, "%f,%f,%f,", tf_target_color.xyz.x, tf_target_color.xyz.y, tf_target_color.xyz.z);
									}
									else {
										printf("Cordinates transfered failed!\n");
									}

									//fprintf(fp0, "%f,%f,%f,", skeleton0.joints[i].position.xyz.x, skeleton0.joints[i].position.xyz.y, skeleton0.joints[i].position.xyz.z);
								}
								fprintf(fp, "\n");
							}
							else if (get_body_skeleton == K4A_RESULT_FAILED)
							{
								printf("Get body skeleton failed!!\n");
							}
							uint32_t id = k4abt_frame_get_body_id(body_frame, 1);
							//printf("Body ID is %u\n", id);

							//}
							//printf("%zu bodies are detected!\n", num_bodies);

							k4abt_frame_release(body_frame);
						}
						else if (pop_frame_result == K4A_WAIT_RESULT_TIMEOUT)
						{
							//  It should never hit timeout when K4A_WAIT_INFINITE is set.
							printf("Error! Pop body frame result timeout!\n");
							break;
						}
						else
						{
							printf("Pop body frame result failed!\n");
							break;
						}
					}
				}

			}
			//互斥量Flagr1to1，保证拍照数量
			std::unique_lock<std::mutex> locker_r(Flagr1to1);
			if (KEY_DOWN('N') && flag_r == -1 || flagr1to1 != 0)
			{
				if (i == master_num)
					flagr1to1 += num - 1;
				else
					flagr1to1 -= 1;
				locker_r.unlock();
				flag_r = 0;
				char name[220], namet[220], tmp[20], buffer[80];
				// 基于当前系统的当前日期/时间
				time_t now = time(0);
				// 把 now 转换为字符串形式
				char* dt = ctime(&now);
				cout << dt << "ready" << endl;
				strftime(buffer, sizeof(buffer), "%m_%d_%H_%M_%S_", localtime(&now));
				_itoa_s(i, tmp, 10);//序号deviceindex变，文本tmp
				strcpy(name, ".\\fulloutput-"); //前面的filename_
				strcat(name, buffer); //时间
				strcat(name, tmp); //文件序号
				strcat(name, ".mkv"); //文件后缀名

				if (fp != NULL)
				{
					k4a_record_close(record_d);//关闭录像句柄
					fclose(fp);//关闭关节点文件
					fclose(fpa);//关闭关节角文件
				}
				VERIFY(k4a_record_create(name, dev[i], config[i], &record_d), "create record failed!");
				VERIFY(k4a_record_write_header(record_d), "write record header failed!");


				//创建关节点文件
				strcpy(namet, ".\\fulloutput-"); //前面的filename_
				strcat(namet, buffer); //时间
				strcat(namet, tmp); //文件序号
				strcat(namet, ".csv"); //文件后缀名
				fp = fopen(namet, "w");//创建关节点文件
				//创建关节角度文件
				strcpy(namet, ".\\angleoutput-"); //前面的filename_
				strcat(namet, buffer); //时间
				strcat(namet, tmp); //文件序号
				strcat(namet, ".csv"); //文件后缀名
				fpa = fopen(namet, "w");//创建关节点文件
			}
			else
				locker_r.unlock();

			if (!KEY_DOWN('S'))
			{
				flag_r = -1;
			}
			k4a_capture_release(sensor_capture);
			//irImage = capture.get_ir_image();
			//ColorizeDepthImage(depthImage, DepthPixelColorizer::ColorizeBlueToRed, GetDepthModeRange(config.depth_mode), &depthTextureBuffer);
			//ColorizeDepthImage(irImage, DepthPixelColorizer::ColorizeGreyscale, GetIrLevels(K4A_DEPTH_MODE_PASSIVE_IR), &irTextureBuffer);

			colorTextureBuffer = k4a_image_get_buffer(colorImage);

			//depthFrame = cv::Mat(depthImage.get_height_pixels(), depthImage.get_width_pixels(), CV_8UC4, depthTextureBuffer.data());
			colorFrame = cv::Mat(1, k4a_image_get_height_pixels(colorImage) * k4a_image_get_width_pixels(colorImage), CV_8UC1, colorTextureBuffer);
			colorFrame = imdecode(colorFrame, IMREAD_COLOR);
			k4a_image_release(colorImage);
			cvtColor(colorFrame, colorFrame, COLOR_BGRA2BGR);
			if (colorFrame.data == NULL)
			{
				cout << "colorframe imdecode erro" << endl;
			}
			//写入普通视频
			/*colorwriter << colorFrame;*/
			//short hh = GetAsyncKeyState('S');
			//bool h = KEY_DOWN('S');
			//short hhh = GetAsyncKeyState('S') & 0x8001;
			std::unique_lock<std::mutex> locker(Flag1to1);
			if (KEY_DOWN('S') && flag == -1 || flag1to1 != 0)
			{
				if (i == master_num)
					flag1to1 += num - 1;
				else
					flag1to1 -= 1;
				locker.unlock();
				frame_count++;
				flag = 0;
				tids_s[i] = thread(save, colorFrame, i, frame_count);//保证第一时间开启存储程序
				tids_s[i].detach();
			}
			else
				locker.unlock();

			if (!KEY_DOWN('S'))
			{
				flag = -1;
			}


			//colorFrame = cv::imdecode(colorTextureBuffer,cv::IMREAD_UNCHANGED);
			//irFrame = cv::Mat(irImage.get_height_pixels(), irImage.get_width_pixels(), CV_8UC4, irTextureBuffer.data());
			//imshow("Kinect depth map", depthFrame);
			//imshow("Kinect ir frame", irFrame);



			imshow("Kinect color frame" + std::to_string(i), colorFrame);
			waitKey(1);//窗口的要等待时间，当显示图片时，窗口不用实时更新，所以imshow之前不加waitKey也是可以的，但若显示实时的视频，就必须加waitKey

		}
		//if (esc)
		//{
		//	k4a_record_close(record_d);//关闭录像句柄
		//	k4a_device_stop_cameras(dev_d);//停止流
		//	k4a_device_close(dev_d);
		//	break;
		//}
		if (KEY_DOWN('Q'))
		{
			k4a_device_stop_cameras(dev_d);//停止流
			k4abt_tracker_shutdown(tracker);//关闭捕捉
			k4abt_tracker_destroy(tracker);

			if (fp != NULL)
			{
				k4a_record_close(record_d);//关闭录像句柄
				fclose(fp);//关闭关节点文件
				fclose(fpa);//关闭关节角文件
			}
			k4a_device_close(dev_d);
			break;
		}
	}
}



uint32_t init_start(int* master_num)
{
	const uint32_t devicecount = k4a::device::get_installed_count();
	if (devicecount == 0)
	{
		cout << "no azure kinect dk devices detected!" << endl;
	}

	//初始化为NULL但是会造成程序在k4a_record_close时报错
	record = new k4a_record_t[devicecount]{ NULL };
	
	record = new k4a_record_t[devicecount];
	dev = new k4a_device_t[devicecount];
	config = new k4a_device_configuration_t[devicecount];
	sensor_calibration = new k4a_calibration_t[devicecount];

	for (uint8_t deviceindex = 0; deviceindex < devicecount; deviceindex++)
	{
		if (K4A_RESULT_SUCCEEDED != k4a_device_open(deviceindex, &dev[deviceindex]))
		{
			printf("%d: Failed to open device\n", deviceindex);
			continue;
		}

		//实验赋值
		//k4a::device& dd = dev[deviceindex];
		//cout << "dd is" << dd << "while d is" << dev[deviceindex] << endl;
		config[deviceindex] = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
		config[deviceindex].camera_fps = K4A_FRAMES_PER_SECOND_30;
		config[deviceindex].depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
		config[deviceindex].color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
		config[deviceindex].color_resolution = K4A_COLOR_RESOLUTION_720P;
		config[deviceindex].synchronized_images_only = true;
		bool sync_in, sync_out;
		VERIFY(k4a_device_get_sync_jack(dev[deviceindex], &sync_in, &sync_out), "get sync jack failed");
		if (sync_in == true)
		{
			cout << "subordinate device detected!" << endl;
			config[deviceindex].wired_sync_mode = K4A_WIRED_SYNC_MODE_SUBORDINATE;
		}
		else if (sync_out == true)
		{
			cout << "master device detected!" << endl;
			*master_num = (int)deviceindex;
			config[deviceindex].wired_sync_mode = K4A_WIRED_SYNC_MODE_MASTER;
		}
		else
		{
			cout << "standalone device detected!" << endl;
			config[deviceindex].wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;
		}
		/*record[deviceindex] = NULL;*/
		////搞定自动命名
		//char name[220],tmp[20], buffer[80];
		//// 基于当前系统的当前日期/时间
		//time_t now = time(0);
		//// 把 now 转换为字符串形式
		//char* dt = ctime(&now);
		//cout << dt << endl;
		//strftime(buffer, sizeof(buffer), "%m_%d_%H_%M_%S_", localtime(&now));
		//_itoa_s(deviceindex, tmp, 10);//序号deviceindex变，文本tmp
		//strcpy(name, ".\\fulloutput-"); //前面的filename_
		//strcat(name, buffer); //时间
		//strcat(name, tmp); //文件序号
		//strcat(name, ".mkv"); //文件后缀名
		//VERIFY(k4a_record_create(name, dev[deviceindex], config[deviceindex], &record[deviceindex]), "create record failed!");
		//VERIFY(k4a_record_write_header(record[deviceindex]), "write record header failed!");
		cout << "started opening k4a device..." << endl;
		VERIFY(k4a_device_start_cameras(dev[deviceindex], &config[deviceindex]), "Start K4A cameras failed!");//启动
		//校准设备
		VERIFY(k4a_device_get_calibration(dev[deviceindex], config[deviceindex].depth_mode, config[deviceindex].color_resolution, &sensor_calibration[deviceindex]),
			"Get depth camera calibration failed!")
			VERIFY(k4a_device_set_color_control(dev[deviceindex], K4A_COLOR_CONTROL_BRIGHTNESS, K4A_COLOR_CONTROL_MODE_MANUAL, 150), "color brightness control failed");//手动设置曝光
		cout << "finished opening k4a device!\n" << endl;
	}
	return devicecount;

}

//void keyboards()
//{
//	while (1)
//	{
//		Sleep(1000);
//		if (KEY_DOWN('Q'))
//		{
//			/*Sleep(3000);*/
//			esc = true;
//			break;
//		}
//		else
//		{
//			esc = false;
//		}
//	}
//
//}

int record_main(float (&joints_Angel)[18])
{
	//主函数所用变量

	cv::Mat* colorframe = NULL;
	int master_num;


	uint32_t num = init_start(&master_num);//初始化,启动相机
	tids = new thread[num];
	tids_s = new thread[num];
	colorframe = new cv::Mat[num];


	//tid_ss = thread(keyboards);
	for (int i = 0; i < num; ++i)
	{
		tids[i] = thread(cap, ref(dev[i]), ref(colorframe[i]), ref(record[i]), i, master_num, num, ref(joints_Angel));

	}
	for (int i = 0; i < num; ++i)
	{
		cout << "thread' id is " << tids[i].get_id() << endl;
		cout << "threads' id is " << tids_s[i].get_id() << endl;
	}

	//保证子线程停止
	for (int i = 0; i < num; ++i)
	{
		tids[i].join();
	}
	//tid_ss.join();
	//or use detach保证子线程稳定
	//th1.detach();
	//th2.detach();

	//释放内存,new后最好做，虽然进程结束后都会回收
	delete[] tids;
	delete[] tids_s;
	delete[] colorframe;


	return 0;
}


