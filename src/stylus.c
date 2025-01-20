#include "stylus.h"

#if GT_GENV == GT_GENV_X11

#define GT_MAXV 65535

void GTStylusSetupAbs(int fd, int code, int minimum, int maximum, int resolution) {
    ioctl(fd, UI_SET_ABSBIT, code);
    struct uinput_abs_setup abs_setup = {
        .code = code,
        .absinfo = {
            .minimum = minimum,
            .maximum = maximum,
            .resolution = resolution,
            .fuzz = 0,
            .flat = 0,
        },
    };
    ioctl(fd, UI_ABS_SETUP, &abs_setup);
}

void setup(int fd, const char *name) {
    struct uinput_setup setup = {
        .name = {0},
        .id = {
            .bustype = BUS_VIRTUAL,
            .vendor  = 0x1701,
            .product = 0x1701,
            .version = 0x0001,
        },
        .ff_effects_max = 0,
    };
    strncpy(setup.name, name, UINPUT_MAX_NAME_SIZE-1);
    ioctl(fd, UI_DEV_SETUP, &setup);
    ioctl(fd, UI_DEV_CREATE);
}

void GTStylusSendEventUtil(int fd, int type, int code, int value) {
    struct timeval time;
    gettimeofday(&time, NULL);
    struct input_event event = {
        .type = type,
        .code = code,
        .value = value,
        .time = {
            .tv_usec = 0,
            .tv_sec = 0,
        },
    };
    write(fd, &event, sizeof(event));
}

GTPoint GTStylusMapCoord(GTStylus *stylus, GTPoint pt) {
    return (GTPoint){
        .x = (int)((((long)pt.x*(long)stylus->window.width)/(long)stylus->client.x+(long)stylus->window.x)*(long)GT_MAXV/(long)stylus->display.x),
        .y = (int)((((long)pt.y*(long)stylus->window.height)/(long)stylus->client.y+(long)stylus->window.y)*(long)GT_MAXV/(long)stylus->display.y),
    };
}

void GTStylusCreate(GTStylus *stylus) {
    int fd = open("/dev/uinput", O_WRONLY|O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    ioctl(fd, UI_SET_PROPBIT, INPUT_PROP_POINTER);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOOL_PEN);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOOL_RUBBER);
    ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(fd, UI_SET_KEYBIT, BTN_STYLUS);
    ioctl(fd, UI_SET_KEYBIT, BTN_STYLUS2);
    ioctl(fd, UI_SET_KEYBIT, BTN_STYLUS3);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(fd, UI_SET_EVBIT, EV_ABS);
    GTStylusSetupAbs(fd, ABS_X, 0, GT_MAXV, 1);
    GTStylusSetupAbs(fd, ABS_Y, 0, GT_MAXV, 1);
    GTStylusSetupAbs(fd, ABS_PRESSURE, 0, GT_MAXV, 1);

    setup(fd, "GTab Virtual Stylus");

    stylus->fd = fd;
}

void GTStylusUpdate(GTStylus *stylus, int x, int y, int pressure) {
    GTStylusSendEventUtil(stylus->fd, EV_ABS, ABS_X, x);
    GTStylusSendEventUtil(stylus->fd, EV_ABS, ABS_Y, y);
    GTStylusSendEventUtil(stylus->fd, EV_ABS, ABS_PRESSURE, pressure);
    GTStylusSendEventUtil(stylus->fd, EV_SYN, SYN_REPORT, 0);
}

void GTStylusBind(GTStylus *stylus, GTPoint client, GTScreenList *screens, int screen) {
    stylus->client = client;
    stylus->display = screens->size;
    stylus->window = screens->screens[screen].location;
}

#else

int gt_stylus_unsupported_warned = 0;

#define GTUNSUPPORTEDSTYLUS() (gt_stylus_unsupported_warned ? (void)0 : (void)printf("WARNING: Platform not supported for stylus\n"), gt_stylus_unsupported_warned=1)

void GTStylusCreate(GTStylus *stylus) {
    (void)stylus;
    printf("WARNING: Platform not supported for stylus\n");
}

void GTStylusBind(GTStylus *stylus, GTPoint client, GTScreenList *screens, int screen) {
    (void)stylus;
    (void)client;
    (void)screens;
    (void)screen;
    GTUNSUPPORTEDSTYLUS();
}

void GTStylusSendEvent(GTStylus *stylus, GTStylusEvent event) {
    (void)stylus;
    (void)event;
    GTUNSUPPORTEDSTYLUS();
}

void GTStylusUpdate(GTStylus *stylus, int x, int y, int pressure) {
    (void)stylus;
    (void)x;
    (void)y;
    (void)pressure;
    GTUNSUPPORTEDSTYLUS();
}

GTPoint GTStylusMapCoord(GTStylus *stylus, GTPoint pt) { 
    (void)stylus;
    (void)pt;
    GTUNSUPPORTEDSTYLUS();
    return (GTPoint){0,0}; 
}

#endif
