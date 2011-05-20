#include    <stdlib.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <Evas.h>
#include    <Ecore.h>
#include    <Ecore_Evas.h>

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


static Evas_Object*
new_eenvader(void)
{
    Evas_Object *o = NULL;
    uint16_t u = lrand48();
    int *data = malloc(7 * 7 * sizeof(int));

    if (!data) {
        perror(NULL);
        exit(1);
    }
    for (int i = 0; i < 7 * 7; i++) {
        data[i] = BG;
    }

    for (int i = 0; i < 15; i++) {
        if (u & (1 << i)) {
            data[7 + 7*(i/3) + 1 + i%3] = FG;
            data[7 + 7*(i/3) + 5 - i%3] = FG;
        }
    }

    o = evas_object_image_filled_add(_G.evas);
    evas_object_image_fill_set(o, 0, 0, 7, 7);
    evas_object_image_smooth_scale_set(o, EINA_FALSE);
    evas_object_image_size_set (o, 7, 7);
    evas_object_image_data_set(o, (void *) data);

    return o;
}

static int
square_ceil_7(int n)
{
    /* XXX: considering n >= 7 */
    int r = 1;

    n /= 7;

    while (n >>= 1) {
        r <<= 1;
    }
    return r * 7;
}

void draw_eenvaders(int x, int y, int w, int h)
{
    Evas_Object *o;
    int d;

    if (w < 7 || h < 7)
        return;

    d = square_ceil_7(MIN(w,h));

    o = new_eenvader();
    evas_object_resize(o, d, d);

    switch(lrand48() & 3) {
      case 0:
        /* top-left */
        evas_object_move(o, x, y);
        evas_object_show(o);

        draw_eenvaders(x+d, y, w-d, h); /* right */
        draw_eenvaders(x, y+d, d, h-d); /* bottom */
        break;
      case 1:
        /* top-right */
        evas_object_move(o, x+w-d, y);
        evas_object_show(o);

        draw_eenvaders(x, y+d, w, h-d); /* bottom */
        draw_eenvaders(x, y, w-d, d); /* left */
        break;
      case 2:
        /* bottom-right */
        evas_object_move(o, x+w-d, y+h-d);
        evas_object_show(o);

        draw_eenvaders(x, y, w-d, h); /* left */
        draw_eenvaders(x+w-d, y, d, h-d); /* top */
        break;
      case 3:
        /* bottom-left */
        evas_object_move(o, x, y+h-d);
        evas_object_show(o);

        draw_eenvaders(x, y, w, h-d); /* top */
        draw_eenvaders(x+d, y+h-d, w-d, d); /* right */
        break;
    }
}

int
main(void)
{
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

    draw_eenvaders(0, 0, _G.w, _G.h);

    ecore_main_loop_begin();

    ecore_evas_shutdown();
    ecore_shutdown();
    return 0;
}
