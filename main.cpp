#include <iostream>
#include "button.h"

using namespace std;

int main()
{

    button btn;
    btn.registerLongReleased([](int delay)
    {
        std::cout<<" Button long released"<<delay<<std::endl;
    });

    btn.registerButtonPressed([]()
    {
        std::cout<<" Button Pressed"<<std::endl;
    });

    btn.registerButtonReleased([]()
    {
        std::cout<<" Button Released"<<std::endl;
    });

    getchar();
    return 0;
}
