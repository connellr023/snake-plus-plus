#ifdef _WIN32
#include "./windows/windows_framebuffer.hpp"

typedef WindowsFrameBuffer FrameBufferImpl;

#elif __linux__
#include "./linux/linux_framebuffer.hpp"

typedef LinuxFrameBuffer FrameBufferImpl;

#endif
