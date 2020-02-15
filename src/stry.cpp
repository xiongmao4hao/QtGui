////#include <stdio.h>
////#include <stdlib.h>
////int main()
////{
////	int i = 150;
////	int j = -100;
////	double k = 3.14159;
////	fprintf(stdout, "%d %f %x \n", j, k, i);
////	fprintf(stdout, "%2d %*d\n", i, 1, i);
////}
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////*Problem B:请写一个程序，对于一个m行m列的（1<m<10）的方阵，求其每一行，每一列及主对角线元素之和，最后按照从大到小的顺序依次输出。
//////输入说明：共一组数据，输入的第一行为一个正整数，表示m，接下来的m行，每行m个整数表示方阵元素。
//////输出说明：从大到小排列的一行整数，每个整数后跟一个空格，最后换行。
//////输入样本：
//////4 15 8 -26
//////31 24 18 71
//////-3-9 27 13
//////17 21 38 69
//////输出样本：
//////159 145 144 13581 60 44 32 2827*/
///////*
//////time 7.11 problem
//////*/
////#include "stdio.h"
////#include "malloc.h"
////#include "stdlib.h"
////int m, * array = 0, *result = 0;
////void aInput() 
////{
////	int i = 0, j = 0, num = 0;
////
////	scanf("%d", &m);
////	//分配空间
////	array = new int[m * m];// (int*)malloc(sizeof(int) * m * m);
////	result = (int*)malloc(sizeof(int) * (2 * m + 2));
////	if (array == 0)
////	{
////		printf("quit!------");
////		exit(0);
////	}
////	//动态输入数组
////	for (i = 0; i < m; i++) {
////		for (j = 0; j < m; j++) 
////		{
////			scanf("%d", &array[i * m + j]);
////		}
////	}
////}
////void aCount() {
////	int i = 0, j = 0, xsum = 0, ysum = 0;
////	for (i = 0; i < m; i++) {
////		for (j = 0; j < m; j++) {
////			xsum += array[i * m + j];
////			ysum += array[i + j * m];
////		}
////		result[i * 2] = xsum;
////		xsum = 0;
////		result[i * 2 + 1] = ysum;
////		ysum = 0;
////	}
////
////	for (i = 0; i < m; i++) {
////		xsum += array[i * m + i];
////		ysum += array[i * m + (m - i - 1)];
////	}
////	result[2 * m] = xsum;
////	result[2 * m + 1] = ysum;
////}
////void aOutput() {
////	int i, j, temp;
////
////	for (i = 0; i < 2 * m + 2; i++) {
////
////		for (j = i + 1; j < 2 * m + 2; j++) {
////			if (result[i] < result[j]) {
////				temp = result[i];
////				result[i] = result[j];
////				result[j] = temp;
////			}
////		}
////	}
////}
////int main() 
////{
////	int i;
////	aInput();//输入数据
////	aCount();//数据计算
////	aOutput();//数据排列
////	for (i = 0; i < 2 * m + 2; i++) 
////	{
////		printf("%d ", result[i]);
////	}
////	free(array);
////	free(result);
////
////}
//
//
//
//
////////////////////////////////////////////////////////////////////
//////计算夹角
////#include <stdio.h>
////#include <stdlib.h>
////#include <math.h>
////int main()
////{
////	double r11 = 0.538, r12 = -0.0305, r13 = 0.8422, r21 = 0.0351, r22 = 0.9993, r23 = 0.0137, \
////		r31 = -0.8420, r32 = 0.0222, r33 = 0.5390;
////	const double PI = 3.141592653;
////	double thetaz = atan2(r21, r11) / PI * 180;
////	double thetay = atan2(-1 * r31, sqrt(r32 * r32 + r33 * r33)) / PI * 180;
////	double thetax = atan2(r32, r33) / PI * 180;
////	printf("%f\n%f\n%f\n", thetaz, thetay, thetax);
////}
//
////////////////////////////////////////////////
////#include <iostream>
////
////#include <thread>
////
////std::thread::id main_thread_id = std::this_thread::get_id();
////
////void hello()
////{
////	std::cout << "Hello Concurrent World\n";
////	if (main_thread_id == std::this_thread::get_id())
////		std::cout << "This is the main thread.\n";
////	else
////		std::cout << "This is not the main thread.\n";
////}
////
////void pause_thread(int n) {
////	std::this_thread::sleep_for(std::chrono::seconds(n));
////	std::cout << "pause of " << n << " seconds ended\n";
////}
////
////int main() {
////	std::thread t(hello);
////	std::cout << t.hardware_concurrency() << std::endl;//可以并发执行多少个(不准确)
////	std::cout << "native_handle " << t.native_handle() << std::endl;//可以并发执行多少个(不准确)
////	t.join();
////	std::thread a(hello);
////	a.detach();
////	std::thread threads[5];                         // 默认构造线程
////
////	std::cout << "Spawning 5 threads...\n";
////	for (int i = 0; i < 5; ++i)
////		threads[i] = std::thread(pause_thread, i + 1);   // move-assign threads
////	std::cout << "Done spawning threads. Now waiting for them to join:\n";
////	for (auto& thread : threads)
////		thread.join();
////	std::cout << "All threads joined!\n";
////}
//
/////////////////////////////////////////////////////////
////#include <iostream>
////#include <thread>
////#include<windows.h>
////
////using namespace std;
////void sayHello();
////void show();
////void method(int& a, int& b)//ref
////{
////	while (a<5000)
////	{
////		a += 5;
////		cout << a << endl;
////		cout << b << endl;
////	}
////	
////}
////int main(int argc, char const* argv[]) {
////
////	int a = 0;
////	thread th1(method, ref(a),ref(a));//ref !!
////	//thread th2(method, a);
////	th1.join();
////	//thread th1_move = thread(move(th1));//move constructor,th1被转移到th1_move后th1被销毁，类似于剪切
////	thread t11(&sayHello);
////	t11.join();
////	cout << "dd" << a<<endl;
////	cout << "world" << endl;
////	cout << "------------" << endl;
////	//栈上 
////	//thread t1(show);   //根据函数初始化执行
////	//thread t2(show);
////	//thread t3(show);
////	//t1.join();
////	//t2.join();
////	//t3.join();
////
////	////线程数组
////	thread th[3]{ thread(show), thread(move(th[0])), thread(show) };
////	th[1].join();
////	//th[0].join();
////	th[2].join();
////	////堆上
////	thread* pt1(new thread(show));
////	thread* pt2(new thread(show));
////	thread* pt3(new thread(show));
////	(*pt1).join();
////	pt2->join();
////	pt3->join();
////	////线程指针数组
////	//thread* pth(new thread[3]{ thread(show), thread(show), thread(show) });
////	return 0;
////}
////void sayHello()
////{
////	cout << "hello" << endl;
////}
////
////void show()
////{
////	cout << "hello cplusplus!\n" << endl;
////}
//
///////////////////////////////////
////#include<iostream>
////#include<thread>
////#include<array>
////using namespace std;
////void show()
////{
////	cout << "hello cplusplus!" << endl;
////}
////int main()
////{
////	thread th = thread(show);
////	th.detach();//脱离主线程的绑定，主线程挂了，子线程不报错，子线程执行完自动退出。
////	//detach以后，子线程会成为孤儿线程，线程之间将无法通信。
////	cout << th.joinable() << endl;
////	//th.join(); //error
////	array<thread, 3>  threads = { thread(show), thread(show), thread(show) };
////	for (int i = 0; i < 3; i++)
////	{
////		cout << threads[i].joinable() << endl;//判断线程是否可以join
////		threads[i].join();//主线程等待当前线程执行完成再退出
////	}
////	return 0;
////}
//
/////////////////////////////////////////
////#include <iostream>
////#include <thread>
////
////using namespace std;
////int main()
////{
////	thread t1([]()
////		{
////			cout << "thread1" << endl;
////		});
////	cout << "thread1' id is " << t1.get_id() << endl;
////	thread t2 = move(t1);
////
////	cout << "thread2' id is " << t2.get_id() << endl;
////	cout << "thread1' id is " << t1.get_id() << endl;
////	t2.join();
////	return 0;
////}
//
////////////////////////////////////////////////
////#include <iostream>
////#include <utility>
////#include <thread>
////#include <chrono>
////#include <functional>
////#include <atomic>
////
////void f1(int n)
////{
////	for (int i = 0; i < 5; ++i) {
////		std::cout << "Thread " << n << " executing\n";
////		std::this_thread::sleep_for(std::chrono::milliseconds(10));
////	}
////}
////
////void f2(int& n)
////{
////	for (int i = 0; i < 5; ++i) {
////		std::cout << "Thread 2 executing\n";
////		++n;
////		std::this_thread::sleep_for(std::chrono::milliseconds(10));
////	}
////}
////
////int main()
////{
////	int n = 0;
////	std::thread t1; // t1 is not a thread
////	std::thread t2(f1, n + 1); // pass by value
////	std::thread t3(f2, std::ref(n)); // pass by reference
////	std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
////	t2.join();
////	t4.join();
////	std::cout << "Final value of n is " << n << '\n';
////}
//
///////////////////////////////////
////#include <opencv2/opencv.hpp>
////#include <iostream>
////
////
////using namespace cv;  //不要忘记这句代码！！！
////using namespace std;
////
////int main(int argc, char** argv)
////{
////	VideoWriter::fourcc('P', 'I', 'M', '1');
////	//VideoWriter writer("D:\\Azure Kinect DK\\try example\\output-1.mkv", , 30, Size(480, 360));
////	if (!writer.isOpened())
////	{
////		cout << "初始化VideoWriter失败！" << endl;
////	}
////}
//
////////////////////////////////////////
////#include <opencv2/opencv.hpp>
////#include <iostream>
////
////using namespace cv;  //不要忘记这句代码！！！
////using namespace std;
////
////int main()
////{
////	VideoCapture a("D:\\Azure Kinect DK\\try example\\output-1.mkv");
////	if (!a.isOpened())
////	{
////				cout << "初始化VideoWriter失败！" << endl;
////	}
////
////	bool ret = false;
////	Mat M;
////	ret = a.read(M);
////
////	imshow("img", M);
////	waitKey(0);
////
////	cin.get();
////	return 0;
////}
//
//
//
/////////////////////
//
//#include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <stdio.h>
//using namespace cv;
//using namespace std;
//
//int main(int, char**)
//{
//	Mat src;
//	// use default camera as video source
//	VideoCapture cap(0);
//	// check if we succeeded
//	if (!cap.isOpened()) {
//		cerr << "ERROR! Unable to open camera\n";
//		return -1;
//	}
//	// get one frame from camera to know frame size and type
//	cap >> src;
//	// check if we succeeded
//	if (src.empty()) {
//		cerr << "ERROR! blank frame grabbed\n";
//		return -1;
//	}
//	bool isColor = (src.type() == CV_8UC3);
//	//--- INITIALIZE VIDEOWRITER
//	VideoWriter writer;
//	int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
//	double fps = 25.0;                          // framerate of the created video stream
//	string filename = "./live.mp4";             // name of the output video file
//	writer.open(filename, codec, fps, src.size(), isColor);
//	// check if we succeeded
//	if (!writer.isOpened()) {
//		cerr << "Could not open the output video file for write\n";
//		return -1;
//	}
//	//--- GRAB AND WRITE LOOP
//	cout << "Writing videofile: " << filename << endl
//		<< "Press any key to terminate" << endl;
//	for (;;)
//	{
//		// check if we succeeded
//		if (!cap.read(src)) {
//			cerr << "ERROR! blank frame grabbed\n";
//			break;
//		}
//		// encode the frame into the videofile stream
//		writer.write(src);
//		// show live and wait for a key with timeout long enough to show images
//		imshow("Live", src);
//		if (waitKey(5) >= 0)
//			break;
//	}
//	// the videofile will be closed and released automatically in VideoWriter destructor
//	return 0;
//}
//
/////////////////////////////////////////////////////////////////
//#include <iostream>
//
//using namespace std;
//
//int main()
//
//{
//
//	cout << "cout" << endl;
//
//	cerr << "cerr" << endl;
//
//	return 0;
//
//}
///////////////////////////////////////////////////测试错误输出
//#include <iostream>
//#include <stdexcept>
//#include <vector>
//#include <string>
//using namespace std;
//
//class Test {
//public:
//	int a;
//	int b;
//	Test(int a, int b) {
//		this->a = a;
//		this->b = b;
//	}
//
//	double division() {
//		if (b == 0) {
//			throw "Division by zero condition!";  //自定义抛出的异常
//		}
//		return (a / b);
//	}
//	~Test() {  //此函数为析构函数，异常发生后，该函数会被执行，相当于java中的finally
//		cout << "this func will be exec finally" << endl;
//	}
//};
//
//
//int main()
//{
//	int x = 50;
//	int y = 0;
//	double z = 0;
//	Test test(x, y);
//
//	try {
//		z = test.division();
//	}
//	catch (const char* msg) {  //捕获异常
//		cerr << msg << endl;
//	}
//
//	try {
//		z = test.division();
//	}
//	catch (...) {  //三个点表示可以捕获任意类型的异常
//		cerr << "there is an error" << endl;
//	}
//
//
//	vector<int> v(10);
//	try {
//		v.at(100) = 100;  //拋出 out_of_range 异常
//	}
//	catch (out_of_range & e) {
//		cerr << e.what() << endl;
//	}
//	string s = "hello";
//	try {
//		char c = s.at(100);  //拋出 out_of_range 异常
//	}
//	catch (out_of_range & e) {
//		cerr << e.what() << endl;
//	}
//
//	return 0;
//}