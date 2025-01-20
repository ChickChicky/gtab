#include "screen.h"

#if GT_GENV == GT_GENV_X11

void GTScreenFetchUtil(int depth, Display *display, Window w, GTScreenList *screens, GTRect location) {
    Window *children;
    unsigned int n;
    XWindowAttributes attributes;
    char *name;
    Window window;

    XQueryTree(display, w, &w, &window, &children, &n);
    for (unsigned int i = 0; i < n; ++i) {
        window = children[i];
        XGetWindowAttributes(display, window, &attributes);
        XFetchName(display, window, &name);
        // for (int j = 0; j < depth; ++j)
        //     printf("|        ");
        // printf(
        //     "%04x %04x %08lx %04x %04x %08lx %4d %4d %4dx%-4d %s\n", 
        //         attributes.map_installed, 
        //         attributes.map_state, 
        //         attributes.all_event_masks, 
        //         attributes.class,
        //         attributes.visual->class, 
        //         attributes.visual->visualid, 
        //         attributes.x, attributes.y, 
        //         attributes.width, attributes.height, 
        //         name
        // );
        if (attributes.visual->visualid == 0x7A && name != NULL && strlen(name) != 0) {
            // printf("Found screen %d @%d,%d %dx%d \"%s\"", (screens == NULL && screens->count >= MAX_SCREENS) ? -1 : (int)screens->count, location.x, location.y, location.width, location.height, name);
            // if (screens != NULL) {
            //     if (screens->count < MAX_SCREENS) {
            //         screens->screens[screens->count++] = (GTScreenInfo){
            //             .window = window,
            //             .location = location,
            //         };
            //     }
            //     else {
            //         printf(" ignoring");
            //     }
            // }
            // printf("\n");
            if (screens != NULL) {
                if (screens->count >= MAX_SCREENS)
                    return;
                screens->screens[screens->count++] = (GTScreenInfo){
                    .window = window,
                    .location = location,
                };
            }
        }
        GTScreenFetchUtil(
            depth+1, 
            display, 
            window, 
            screens, 
            depth == 0 ? 
                (GTRect){ 
                    .x = attributes.x, 
                    .y = attributes.y, 
                    .width = attributes.width, 
                    .height = attributes.height 
                }
                : location
        );
    }
}

GTScreenList GTScreenFetch(void) {
    GTScreenList list = { .count = 0 };
    Display *display = XOpenDisplay(NULL);
    int screen = XDefaultScreen(display);
    list.size.x = XDisplayWidth(display, screen);
    list.size.y = XDisplayHeight(display, screen);
    GTScreenFetchUtil(0, display, XDefaultRootWindow(display), &list, (GTRect){0});
    return list;
}

GTScreenList GTScreenFetchDisplay(Display *display) {
    GTScreenList list = { .count = 0 };
    int screen = XDefaultScreen(display);
    list.size.x = XDisplayWidth(display, screen);
    list.size.y = XDisplayHeight(display, screen);
    GTScreenFetchUtil(0, display, XDefaultRootWindow(display), &list, (GTRect){0});
    return list;
}

#else

int gt_screen_unsupported_warned = 0;

#define GTUNSUPPORTEDSCREEN() (gt_screen_unsupported_warned ? (void)0 : (void)printf("WARNING: Platform not supported for screen\n"), gt_screen_unsupported_warned=1)

GTScreenList GTScreenFetch(void) {
    GTUNSUPPORTEDSCREEN();
    return (GTScreenList){ .count = 0 };
}

#endif