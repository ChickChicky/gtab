#include "gtab.h"
#include "threads.h"

void msleep(long ms) {
    struct timespec ts = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000) * 1000000,
    };
    nanosleep(&ts, &ts);
}

int GTParseAddress(const char *addr, struct in6_addr *result) {
    if (addr == NULL || strlen(addr) == 0) {
        if (result)
            *result = in6addr_any;
        return 0;
    }
    return 1;
}

int GTRunClient(GTEnv env) {
    if (env.addr == NULL) {
        printf("Error: Missing address argument\n");
        printf(HELP_A);
        return 1;
    }

    return 0;
}

int GTRunServer(GTEnv env) {
    if (env.addr != NULL)
        printf("Warning: bind address might not work as expected\n");

    struct in6_addr bind_addr;
    if (GTParseAddress(env.addr, &bind_addr) < 0) {
        printf("Invalid address '%s'\n", env.addr);
        return 1;
    }

    int server_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));
    struct sockaddr_in6 addr = {
        .sin6_family = AF_INET6,
        .sin6_addr = bind_addr,
        .sin6_port = htons(env.port)
    };
    socklen_t addr_len = sizeof(addr);
    struct sockaddr *sockaddr = (struct sockaddr*)&addr;
    bind(server_fd, sockaddr, addr_len);
    listen(server_fd, 16);

    int client_fd;
    struct sockaddr_in6 client_addr;
    threads_thread client_thread;
    GTHandleData client_data;
    for (;;) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr,  (socklen_t*)sizeof(client_addr));
        client_data.started = 0;
        client_data.fd = client_fd;
        threads_create(&client_thread, GTHandleClient, &client_data);
        for (size_t i = 0; i < 5 && !client_data.started; ++i)
            msleep(100);
        if (!client_data.started) {
            threads_cancel(&client_thread);
            printf("Client handler took too long to detach\n");
        }
    }

    return 0;
}

void *GTHandleClient(void *d) {
    GTHandleData data = *(GTHandleData*)d;
    ((GTHandleData*)d)->started = 1;

    GTStylus stylus;
    GTStylusCreate(&stylus);

    return NULL;
}


#define SHIFT(argc, argv) (assert(*(argc) > 0),(*(argc))--,*((*(argv))++))

void GTTestStylus(int screen) {
    if (screen < 0) {
        printf("Screen number (%d) must be >=0\n", screen);
        return;
    }
    
    GTStylus stylus;
    GTStylusCreate(&stylus);
    
    GTScreenList screens = GTScreenFetch();
    if (screen >= (int)screens.count) {
        printf("Screen number (%d) must be <%d\n", screen, (int)screens.count);
        return;
    }

    GTRect box = screens.screens[screen].location;
    GTStylusBind(&stylus, (GTPoint){box.width, box.height}, &screens, screen);
    
    int x = 100;
    int y = 200;
    int vx = 10;
    int vy = -10;

    for (int t=0; t<1000; ++t) {
        msleep(10);

        int nx = x + vx;
        int ny = y + vy;

        if (nx > box.width || nx < 0)
            vx*= -1;
        else if (ny > box.height || ny < 0)
            vy*= -1;
        else {
            x = nx;
            y = ny;
        }

        GTPoint pt = GTStylusMapCoord(&stylus, (GTPoint){x, y});
        GTStylusUpdate(&stylus, pt.x, pt.y, 0);
    }
}

int main(int argc, const char **argv) {
    const char *program_name = SHIFT(&argc, &argv);

    GTEnv env = {
        .port = DEFAULT_PORT,
        .mode = 0,
        .addr = NULL,
    };

    while (argc) {
        const char *arg = SHIFT(&argc, &argv);
        if (strcmp(arg, "-p") == 0) {
            const char *portv = SHIFT(&argc, &argv);
            env.port = strtol(portv, NULL, 0);
        }
        else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--client") == 0) {
            if (env.mode != 0)
                printf("Warning: duplicate -c/-s\n");
            env.mode = MODE_CLIENT;
        }
        else if (strcmp(arg, "-s") == 0 || strcmp(arg, "--server") == 0) {
            if (env.mode != 0)
                printf("Warning: duplicate -c/-s\n");
            env.mode = MODE_SERVER;
        }
        else if (strcmp(arg, "-a") == 0 || strcmp(arg, "--addr") == 0) {
            env.addr = SHIFT(&argc, &argv);
        }
        else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--test") == 0) {
            int screen = 0;
            if (argc)
                screen = strtol(SHIFT(&argc, &argv), NULL, 0);
            GTTestStylus(screen);
            return 0;
        }
        else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            printf(HELP_USAGE, program_name);
            return 0;
        }
        else {
            printf("Invalid argument '%s'\n\n", arg);
            printf(HELP_USAGE, program_name);
            return 1;
        }
    }

    if (env.mode == MODE_CLIENT)
        return GTRunClient(env);

    if (env.mode == MODE_SERVER)
        return GTRunServer(env);

    printf("Error: Missing mode argument\n");
    printf(HELP_MODE);
    return 1;
}