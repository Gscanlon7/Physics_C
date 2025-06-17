#pragma once

typedef struct Window Window;

// Event callbacks
typedef void(*WindowResizeCallback)(Window*, int w, int h);
typedef void(*WindowCloseCallback)(Window*);

typedef void(*MouseButtonCallback)(Window*, int button, int action, int mods);
typedef void(*MouseMoveCallback)(Window*, int x, int y);
typedef void(*MouseScrollCallback)(Window*, int xdelta, int ydelta);

typedef void(*KeyCallback)(Window*, int key, int action, int mods);