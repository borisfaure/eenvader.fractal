#include    <stdlib.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <Evas.h>
#include    <Ecore.h>
#include    <Ecore_Evas.h>

#include "eenvaders_evas_smart.h"

#define BG 0xff002b36
#define FG 0xff839496

#define MIN(a,b) (((a)<(b))?(a):(b))

static struct {
    Ecore_Evas  *ee;
    Evas        *evas;
    Evas_Coord   w, h;
    Evas_Object *bg;
} eenvaders_g;
#define _G eenvaders_g


int
main(void)
{
    Evas_Object *o;
    long int seedval;
    int fd;

    if (!ecore_evas_init())
        return -1;

    _G.w = 600;
    _G.h = 350;


    /*open file */
    if ((fd = open("/dev/urandom", O_RDONLY)) < 0) {
       perror(NULL);
       exit(1);
    }
    if (read(fd, &seedval, sizeof(seedval)) != sizeof(seedval)) {
       perror(NULL);
       close(fd);
       exit(1);
    }
    close(fd);
    srand48(seedval);

    _G.ee = ecore_evas_software_x11_new(
            NULL, /* const char * disp_name */
            0,    /*  Ecore_X_Window parent */
            0, 0, _G.w, _G.h);

    ecore_evas_title_set(_G.ee, "EEnavders.fractal");
    ecore_evas_borderless_set(_G.ee, 0);
    ecore_evas_show(_G.ee);
    _G.evas = ecore_evas_get(_G.ee);

    _G.bg  = evas_object_rectangle_add(_G.evas);
    evas_object_color_set(_G.bg,
                          (BG >> 16) & 0xff,
                          (BG >> 8) & 0xff,
                          BG & 0xff,
                          BG >> 24);
    evas_object_move(_G.bg, 0, 0);
    evas_object_resize(_G.bg, _G.w, _G.h);
    evas_object_show(_G.bg);

    o = eenvaders_smart_new(_G.evas);
    evas_object_resize(o, _G.w, _G.h);
    evas_object_show(o);

    ecore_main_loop_begin();

    ecore_evas_shutdown();
    ecore_shutdown();
    return 0;
}
