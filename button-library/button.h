#ifndef BUTTON_H
#define BUTTON_H

//linux includes
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

//C++ includes
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <atomic>

//defines
#define BUTTON_FILE "/sys/devices/button/values"
#define READ_TIMEOUT 100

//LOGICAL VALUES
#define HIGH '1'
#define LOW '0'

//offset values
#define PRESS_DELAY 500


//error messages
#define OPEN_FAILED "Failed to open device file"
#define CLOSE_FAILED "Failed to close device file"
#define POLL_ERROR "Polling error"
#define READ_FAILED "Failed to read device file"



class button
{

public:
    button();
    ~button();
    void registerButtonPressed( std::function<void()> fun);
    void registerButtonReleased( std::function<void()> fun);
    void registerLongReleased( std::function<void(int)> fun);

private:
    std::function<void()> buttonPressedFun;
    std::function<void()> buttonReleasedFun;
    std::function<void(int)> buttonLongReleasedFun;
    void readThreadHandler();
    int m_fd;
    std::thread *m_readThread;
    std::mutex m_readMutex;
    std::condition_variable m_readCV;
    char m_data;
};

#endif // BUTTON_H
