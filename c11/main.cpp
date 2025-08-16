#include <iostream>
#include <string>
#include <chrono>
#include <utility>
#include <thread>
#include "myThreadPool.hpp"
using namespace std;


int main()
{
    std::mutex mtx;
    try
    {
        ox::ThreadPool tp;
        //存放返回值
        std::vector<std::future<int>> v;
        std::vector<std::future<void>> v1;
        for (int i = 0; i <= 100; ++i)
        {
            auto ans = tp.add([](int answer) { 
                //延时操作模拟线程处理时间
                std::this_thread::sleep_for(std::chrono::seconds(1));
                return answer; }, i);
            v.push_back(std::move(ans));//移动构造vector对象
        }
        for (int i = 0; i <= 500; ++i)
        {
            auto ans = tp.add([&mtx](const std::string& str1, const std::string& str2)
            {
                //锁 cout避免打印混乱
                std::lock_guard<std::mutex> lg(mtx);
                std::cout << (str1 + str2) << std::endl;
                std::cout<<std::thread::hardware_concurrency()<<endl;;
                return;
            }, "hello ", "world");
            v1.push_back(std::move(ans));
        }
        for (size_t i = 0; i < v.size(); ++i)
        {
            std::lock_guard<std::mutex> lg(mtx);
            cout << v[i].get() << endl;
        }
        for (size_t i = 0; i < v1.size(); ++i)
        {
            v1[i].get();
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}