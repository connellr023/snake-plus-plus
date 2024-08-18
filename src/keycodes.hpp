#ifndef KEYCODES_H
#define KEYCODES_H

#ifdef __linux__
#define KEY_ESC     65307
#define KEY_UP      65362
#define KEY_LEFT    65361
#define KEY_DOWN    65364
#define KEY_RIGHT   65363
#define KEY_ENTER   65293
#elif __WIN32
#define KEY_ESC     27
#define KEY_UP      38
#define KEY_LEFT    37
#define KEY_DOWN    40
#define KEY_RIGHT   39
#define KEY_ENTER   13
#endif

#endif // KEYCODES_H
