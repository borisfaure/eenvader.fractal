#include    <stdlib.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <Evas.h>
#include    <Ecore.h>
#include    <Ecore_Evas.h>
#include    <math.h>
static struct {
    Ecore_Evas  *ee;
    Evas        *evas;
    Evas_Coord   x, y, w, h;
} eenvaders_g;
#define _G eenvaders_g



int
main(void)
{
    long int seedval;
    int fd;

    if (!ecore_evas_init())
        return -1;

    _G.w = 500;
    _G.h = 300;


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


    ecore_main_loop_begin();

    ecore_evas_shutdown();
    ecore_shutdown();
    return 0;
}
