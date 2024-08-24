
#include"Framework.h"

int main(int argc, char* argv[])
{
    bool isRunning = true;
    Framework fw;

    // main loop
    while (isRunning)
    {
        fw.run(isRunning);
    }
    return 0;
}

