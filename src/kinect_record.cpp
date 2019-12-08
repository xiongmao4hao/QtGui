#include "kinect_record.h"


using namespace cv;
using namespace std;

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8001) ? 1:0)//�Զ�GetAsyncKeyState��������һ�ε�������������ѱ���������λ0��Ϊ1��������Ϊ0�����Ŀǰ���ڰ���״̬����λ15��Ϊ1����̧����Ϊ0������е���
//����ı�����ʾ����
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
k4a_device_configuration_t* config;
mutex Flag1to1, Flagr1to1;
int flag1to1 = 0, flagr1to1 = 0;
//bool esc = false;



void save(cv::Mat colorFrame, int i, int frame_count)//ÿ�α���ʱ���������߳�
{
	imwrite(".\\photos" + std::to_string(i) + "\\alpha" + std::to_string(frame_count) + ".jpg", colorFrame);
	cout << "You can take another picture1!" << endl;
}

void cap(k4a_device_t& dev_d, cv::Mat& colorFrame, k4a_record_t& record_d, int i, int master_num, int num)  //��ͨ�ĺ���������ִ���߳�
{
	//Ϊsave�����ı���
	int flag = -1, flag_r = -1;
	int frame_count = 0;


	//��Ҫ����ʵ�����ñ���
	//k4a::image depthImage;
	k4a_image_t colorImage;
	//k4a::image irImage;

	//cv::Mat depthFrame;
	//cv::Mat irFrame;

	//std::vector<Pixel> depthTextureBuffer;
	//std::vector<Pixel> irTextureBuffer;
	uint8_t* colorTextureBuffer;

	k4a_capture_t sensor_capture;//�����ñ���
	//�鿴capture��ַ
	//cout << "capture adress" << &sensor_capture << endl;

	//��ͨ��Ƶ����
	//cv:VideoWriter colorwriter;
	//Ϊ�˴�����ͨ��Ƶ�Ķ�ȡһ֡�ĺ���
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
	//������ͨ��Ƶ
	//colorwriter.open(".\\output-" + std::to_string(i) + ".mkv", VideoWriter::fourcc('M', 'J', 'P', 'G'), 15, colorFrame.size(), true);
	//if (!colorwriter.isOpened())
	//{
	//	cout << "��ʼ��VideoWriterʧ�ܣ�" << endl;
	//}

	while (1)
	{
		if (k4a_device_get_capture(dev_d, &sensor_capture, K4A_WAIT_INFINITE) == K4A_WAIT_RESULT_SUCCEEDED)
		{
			//depthImage = capture.get_depth_image();
			colorImage = k4a_capture_get_color_image(sensor_capture);//�Ӳ����л�ȡͼ��
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
					k4a_record_write_capture(record_d, sensor_capture);
					cout << "record\n";
				}

			}
			std::unique_lock<std::mutex> locker_r(Flagr1to1);//��֤����ͻ�Ļ������
			if (KEY_DOWN('N') && flag_r == -1 || flagr1to1 != 0)
			{
				if (i == master_num)
					flagr1to1 += num - 1;
				else
					flagr1to1 -= 1;
				locker_r.unlock();
				flag_r = 0;
				char name[220], tmp[20], buffer[80];
				// ���ڵ�ǰϵͳ�ĵ�ǰ����/ʱ��
				time_t now = time(0);
				// �� now ת��Ϊ�ַ�����ʽ
				char* dt = ctime(&now);
				cout << dt << "ready" << endl;
				strftime(buffer, sizeof(buffer), "%m_%d_%H_%M_%S_", localtime(&now));
				_itoa_s(i, tmp, 10);//���deviceindex�䣬�ı�tmp
				strcpy(name, ".\\fulloutput-"); //ǰ���filename_
				strcat(name, buffer); //ʱ��
				strcat(name, tmp); //�ļ����
				strcat(name, ".mkv"); //�ļ���׺��
				k4a_record_close(record_d);//�ر�¼����
				VERIFY(k4a_record_create(name, dev[i], config[i], &record_d), "create record failed!");
				VERIFY(k4a_record_write_header(record_d), "write record header failed!");
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
			//д����ͨ��Ƶ
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
				tids_s[i] = thread(save, colorFrame, i, frame_count);//��֤��һʱ�俪���洢����
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
			waitKey(1);//���ڵ�Ҫ�ȴ�ʱ�䣬����ʾͼƬʱ�����ڲ���ʵʱ���£�����imshow֮ǰ����waitKeyҲ�ǿ��Եģ�������ʾʵʱ����Ƶ���ͱ����waitKey

		}
		//if (esc)
		//{
		//	k4a_record_close(record_d);//�ر�¼����
		//	k4a_device_stop_cameras(dev_d);//ֹͣ��
		//	k4a_device_close(dev_d);
		//	break;
		//}
		if (KEY_DOWN('Q'))
		{
			k4a_record_close(record_d);//�ر�¼����
			k4a_device_stop_cameras(dev_d);//ֹͣ��
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

	record = new k4a_record_t[devicecount]{ NULL };//��ʼ��ΪNULL
	dev = new k4a_device_t[devicecount];
	config = new k4a_device_configuration_t[devicecount];

	char str[5];

	for (uint8_t deviceindex = 0; deviceindex < devicecount; deviceindex++)
	{
		if (K4A_RESULT_SUCCEEDED != k4a_device_open(deviceindex, &dev[deviceindex]))
		{
			printf("%d: Failed to open device\n", deviceindex);
			continue;
		}

		//ʵ�鸳ֵ
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
		////�㶨�Զ�����
		//char name[220],tmp[20], buffer[80];
		//// ���ڵ�ǰϵͳ�ĵ�ǰ����/ʱ��
		//time_t now = time(0);
		//// �� now ת��Ϊ�ַ�����ʽ
		//char* dt = ctime(&now);
		//cout << dt << endl;
		//strftime(buffer, sizeof(buffer), "%m_%d_%H_%M_%S_", localtime(&now));
		//_itoa_s(deviceindex, tmp, 10);//���deviceindex�䣬�ı�tmp
		//strcpy(name, ".\\fulloutput-"); //ǰ���filename_
		//strcat(name, buffer); //ʱ��
		//strcat(name, tmp); //�ļ����
		//strcat(name, ".mkv"); //�ļ���׺��
		//VERIFY(k4a_record_create(name, dev[deviceindex], config[deviceindex], &record[deviceindex]), "create record failed!");
		//VERIFY(k4a_record_write_header(record[deviceindex]), "write record header failed!");
		cout << "started opening k4a device..." << endl;
		VERIFY(k4a_device_start_cameras(dev[deviceindex], &config[deviceindex]), "Start K4A cameras failed!");//����
		//У׼�豸
		k4a_calibration_t sensor_calibration;
		VERIFY(k4a_device_get_calibration(dev[deviceindex], config[deviceindex].depth_mode, config[deviceindex].color_resolution, &sensor_calibration),
			"Get depth camera calibration failed!")
			VERIFY(k4a_device_set_color_control(dev[deviceindex], K4A_COLOR_CONTROL_BRIGHTNESS, K4A_COLOR_CONTROL_MODE_MANUAL, 150), "color brightness control failed");//�ֶ������ع�
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

int record_main()
{
	//���������ñ���

	cv::Mat* colorframe = NULL;
	int master_num;


	uint32_t num = init_start(&master_num);//��ʼ��,�������
	tids = new thread[num];
	tids_s = new thread[num];
	colorframe = new cv::Mat[num];


	//tid_ss = thread(keyboards);
	for (int i = 0; i < num; ++i)
	{
		tids[i] = thread(cap, ref(dev[i]), ref(colorframe[i]), ref(record[i]), i, master_num, num);

	}
	for (int i = 0; i < num; ++i)
	{
		cout << "thread' id is " << tids[i].get_id() << endl;
		cout << "threads' id is " << tids_s[i].get_id() << endl;
	}


	//��֤���߳�ֹͣ
	for (int i = 0; i < num; ++i)
	{
		tids[i].join();
	}
	//tid_ss.join();
	//or use detach��֤���߳��ȶ�
	//th1.detach();
	//th2.detach();

	//�ͷ��ڴ�,new�����������Ȼ���̽����󶼻����
	delete[] tids;
	delete[] tids_s;
	delete[] colorframe;


	return 0;
}


