#ifndef FRAMEBUFFER_IMPL_H
#define FRAMEBUFFER_IMPL_H

#ifdef _WIN32
#include "./windows/windows_framebuffer.hpp"

typedef WindowsFrameBuffer FrameBufferImpl;

#elif __linux__
#include "./linux/linux_framebuffer.hpp"

typedef LinuxFrameBuffer FrameBufferImpl;

#endif // _WIN32

#endif // FRAMEBUFFER_IMPL_H
