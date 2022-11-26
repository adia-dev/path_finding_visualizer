#include "./Window/Window.h"

using namespace se;

int main(int argc, char **argv)
{

#ifdef __PLAYGROUND_MODE__
    Window::Get().Playground();
#else
    Window::Get().Play();
#endif

    return 0;
}
