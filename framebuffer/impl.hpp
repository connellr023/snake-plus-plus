#ifdef _WIN32
#include "WindowsFrameBuffer.h"

#include "windows_framebuffer.hpp"
typedef WindowsFrameBuffer FrameBufferImpl;

#elif __linux__

#include "linux_framebuffer.hpp"
typedef LinuxFrameBuffer FrameBufferImpl;

#endif
