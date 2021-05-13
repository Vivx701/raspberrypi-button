#include "button.h"

button::button(): m_fd(-1)
{

    m_fd = open(BUTTON_FILE, O_RDONLY);
    if (m_fd < 0)
    {
        std::cerr<<OPEN_FAILED<<std::endl;
    }
    else
    {
        m_readThread = new std::thread(&button::readThreadHandler, this);
        m_readThread->detach();
    }
}

button::~button()
{
    m_readCV.notify_all();
    if(close(m_fd) < 0)
    {
        std::cerr<<CLOSE_FAILED<<std::endl;
    }
}

void button::registerButtonPressed(std::function<void()> fun)
{
    buttonPressedFun = fun;
}

void button::registerButtonReleased(std::function<void()> fun)
{
    buttonReleasedFun = fun;
}

void button::registerLongReleased(std::function<void(int)> fun)
{
    buttonLongReleasedFun = fun;
}

void button::readThreadHandler()
{
    char buf;
    std::chrono::time_point<std::chrono::system_clock> pressed_time, released_time;

    for(;;)
    {
        lseek (m_fd, 0, SEEK_SET);
        ssize_t count = read( m_fd, &buf, 1);
        if(count < 0)
        {
            std::cerr<<READ_FAILED<<std::endl;
            continue;
        }
        std::unique_lock<std::mutex> lock(m_readMutex);
        std::cv_status status = m_readCV.wait_for(lock, std::chrono::milliseconds(1));
        if(status == std::cv_status::no_timeout)
        {
            break;
        }
        if(buf != m_data)
        {
            if(m_data == LOW && buf == HIGH)
            {
                //button pressed
               pressed_time = std::chrono::system_clock::now();
               if(buttonPressedFun)
               {
                    buttonPressedFun();
               }
            }
            else if(m_data == HIGH && buf == LOW)
            {
                //button released
                released_time = std::chrono::system_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(released_time - pressed_time);
                if(diff.count() < PRESS_DELAY)
                {
                    if(buttonReleasedFun)
                    {
                         buttonReleasedFun();
                    }
                }
                else
                {
                    if(buttonLongReleasedFun)
                    {
                         buttonLongReleasedFun(diff.count());
                    }
                }
            }
            m_data = buf;
        }
    }
}
