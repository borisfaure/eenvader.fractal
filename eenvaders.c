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


static Evas_Object*
new_eenvader(void)
{
    Evas_Object *o = NULL;
    uint16_t u = lrand48();
    int *data = calloc(7 * 7, sizeof(int));

    if (!data) {
        perror(NULL);
        exit(1);
    }

    for (int i = 0; i < 15; i++) {
        if (u & (1 << i)) {
            data[7 + 7*(i/3) + 1 + i%3] = 0xffffffff;
            data[7 + 7*(i/3) + 5 - i%3] = 0xffffffff;
        }
    }

    o = evas_object_image_filled_add(_G.evas);
    evas_object_resize(o, 7, 7);
    evas_object_image_fill_set(o, 0, 0, 7, 7);
    evas_object_image_colorspace_set (o, EVAS_COLORSPACE_ARGB8888);
    evas_object_image_alpha_set (o, 1);
    evas_object_image_smooth_scale_set(o, 0);
    evas_object_image_size_set (o, 7, 7);
    evas_object_image_data_set(o, (void *) data);

    return o;
}

int
main(void)
{
    Evas_Object *o;
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

    o = new_eenvader();
    evas_object_resize(o, 280, 280);
    evas_object_show(o);

    ecore_main_loop_begin();

    ecore_evas_shutdown();
    ecore_shutdown();
    return 0;
}
