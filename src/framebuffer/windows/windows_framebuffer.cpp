#ifdef __WIN32
#include <cassert>
#include "windows_framebuffer.hpp"

LRESULT CALLBACK WindowsFrameBuffer::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            break;
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

void WindowsFrameBuffer::create_window_impl() {
    const wchar_t *class_name = L"WindowsFrameBuffer";

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = class_name;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    
    if (!RegisterClassW(&wc)) {
        MessageBoxW(nullptr, L"Failed to register window class", L"Error", MB_ICONEXCLAMATION | MB_ICONERROR);
        return;
    }

    RECT window_rect = { 0, 0, this->width, this->height };
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

    this->hwnd = CreateWindowExW(
        0,
        class_name,
        L"Snake++",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    if (!this->hwnd) {
        MessageBoxW(nullptr, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    this->hdc = GetDC(this->hwnd);

    // Create bitmap to hold framebuffer
    BITMAPINFO bmi = { { 0 } };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = this->width;
    bmi.bmiHeader.biHeight = -this->height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    this->bitmap = CreateDIBSection(this->hdc, &bmi, DIB_RGB_COLORS, (void **) &this->buffer, nullptr, 0);

    if (!this->bitmap) {
        MessageBoxW(nullptr, L"Failed to create DIB section", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);

    this->set_running(true);
}

void WindowsFrameBuffer::render_impl() {
    HDC hdc_mem = CreateCompatibleDC(this->hdc);
    assert(hdc_mem);

    HBITMAP old_bitmap = (HBITMAP) SelectObject(hdc_mem, this->bitmap);

    if (!old_bitmap) {
        DeleteDC(hdc_mem);

        this->set_running(false);
        return;
    }

    BitBlt(this->hdc, 0, 0, this->width, this->height, hdc_mem, 0, 0, SRCCOPY);

    // Cleanup
    SelectObject(hdc_mem, old_bitmap);
    DeleteDC(hdc_mem);
}

void WindowsFrameBuffer::handle_events_impl() {
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) && this->should_run()) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        switch (msg.message) {
            case WM_KEYDOWN: {
                const uint64_t keycode = msg.wParam;
                
                this->notify_keypress(keycode);
                break;
            }
            case WM_QUIT: {
                this->set_running(false);
                break;
            }
        }
    }
}

void WindowsFrameBuffer::write_pixel_impl(int x, int y, uint32_t color) {
    assert(x >= 0 && x < this->width && y >= 0 && y < this->height);
    this->buffer[y * this->width + x] = color;
}

WindowsFrameBuffer::~WindowsFrameBuffer() {
    if (this->bitmap) DeleteObject(this->bitmap);
    if (this->hwnd && this->hdc) ReleaseDC(this->hwnd, this->hdc);
    if (this->hwnd) DestroyWindow(this->hwnd);
    if (this->buffer) delete[] this->buffer;
}

#endif // __WIN32