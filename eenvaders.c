#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <Evas.h>
#include    <Ecore.h>
#include    <Ecore_Evas.h>
#include    <math.h>

Ecore_Evas  * ee;
Evas        * evas;
Evas_Object * o_bg;
Evas_Coord    x, y, w, h;

int
main(void)
{
    Evas_Object * o;

    if (!ecore_evas_init())
        return -1;

    w = 500;
    h = 300;

    ee = ecore_evas_software_x11_new(
            NULL, /* const char * disp_name */
            0,    /*  Ecore_X_Window parent */
            0, 0, w, h);

    ecore_evas_title_set(ee, "EEnavders.fractal");
    ecore_evas_borderless_set(ee, 0);
    ecore_evas_show(ee);
    evas = ecore_evas_get(ee);

    o_bg = evas_object_image_add(evas);

    evas_object_resize(o_bg, w, h);
    evas_object_layer_set(o_bg, -999);
    evas_object_image_file_set(
            o_bg,
            "data/bg.png",
            NULL);
    evas_object_image_fill_set(o_bg, 0, 0, 128, 128);
    /*evas_object_pass_events_set(o_bg, 1);*/
    evas_object_show(o_bg);


    o = evas_object_image_add(evas);


    evas_object_show(o);

    ecore_main_loop_begin();

    ecore_evas_shutdown();
    ecore_shutdown();
    return 0;
}
