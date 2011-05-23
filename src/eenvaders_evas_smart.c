#include <stdlib.h>

#include "eenvaders_evas_smart.h"

#define BG 0xff002b36
#define FG 0xff839496

#define MIN(a,b) (((a)<(b))?(a):(b))

/* Prototypes -{{{-*/
typedef struct Eenvaders_Object {
    Eina_List *datas;
} Eenvaders_Object;

static Evas_Smart *_eenvaders_object_smart_get(void);
static Evas_Object *eenvaders_object_new(Evas *evas);
static void _eenvaders_object_add(Evas_Object *o);
static void _eenvaders_object_del(Evas_Object *o);
static void _eenvaders_object_move(Evas_Object *o, Evas_Coord x, Evas_Coord y);
static void _eenvaders_object_resize(Evas_Object *o, Evas_Coord w, Evas_Coord h);
static void _eenvaders_object_show(Evas_Object *o);
static void _eenvaders_object_hide(Evas_Object *o);
static void _eenvaders_object_color_set(Evas_Object *o, int r, int g, int b, int a);
static void _eenvaders_object_clip_set(Evas_Object *o, Evas_Object *clip);
static void _eenvaders_object_clip_unset(Evas_Object *o);

/* }}} */
/* Globals -{{{-*/

static struct {
    Evas_Smart_Class klass;
} eenvaders_evas_smart_g = {
    .klass = {
        .name = "eenvaders_object",
        .version = EVAS_SMART_CLASS_VERSION,
        .add =  _eenvaders_object_add,
        .del = _eenvaders_object_del,
        .move = _eenvaders_object_move,
        .resize = _eenvaders_object_resize,
        .show = NULL,
        .hide = NULL,
        .color_set = NULL,
        .clip_set = NULL,
        .clip_unset = NULL,
        .calculate = NULL,
        .member_add = NULL,
        .member_del = NULL,
        .parent = NULL,
        .callbacks = NULL,
        .interfaces = NULL,
        .data = NULL,
    },
#define _G eenvaders_evas_smart_g
};

/* }}} */

Evas_Object *
eenvaders_smart_new(Evas *e)
{
    Evas_Object *result = NULL;

    if ((result = eenvaders_object_new(e))) {
        Eenvaders_Object *data = NULL;

        if ((data = evas_object_smart_data_get(result)))
            return result;
        else
            evas_object_del(result);
    }
    return NULL;
}

static Evas_Object *
eenvaders_object_new(Evas *evas)
{
    Evas_Object *eenvaders_object;

    eenvaders_object = evas_object_smart_add(evas,
                                _eenvaders_object_smart_get());
    return eenvaders_object;
}

static Evas_Smart *
_eenvaders_object_smart_get(void)
{
    static Evas_Smart *smart = NULL;

    if (smart)
        return smart;

    smart = evas_smart_class_new(&_G.klass);
    return smart;
}

static void
_eenvaders_object_add(Evas_Object *o)
{
    Eenvaders_Object *data = NULL;
    Evas *evas = NULL;

    evas = evas_object_evas_get(o);
    data = (Eenvaders_Object *)calloc(1, sizeof(Eenvaders_Object));

    evas_object_smart_data_set(o, data);
}

static void
_eenvaders_object_del(Evas_Object *o)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o))) {
        void *mem;

        EINA_LIST_FREE(data->datas, mem)
            free(mem);
        free(data);
    }
}

static void
_eenvaders_object_move(Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o))) {
        /* TODO */
    }
}

static Evas_Object*
new_eenvader(Evas *evas, Eenvaders_Object *eo)
{
    Evas_Object *o = NULL;
    uint16_t u = lrand48();
    int *mem = malloc(7 * 7 * sizeof(int));

    if (!mem) {
        perror(NULL);
        exit(1);
    }
    for (int i = 0; i < 7 * 7; i++) {
        mem[i] = BG;
    }

    for (int i = 0; i < 15; i++) {
        if (u & (1 << i)) {
            mem[7 + 7*(i/3) + 1 + i%3] = FG;
            mem[7 + 7*(i/3) + 5 - i%3] = FG;
        }
    }

    o = evas_object_image_filled_add(evas);
    evas_object_image_fill_set(o, 0, 0, 7, 7);
    evas_object_image_smooth_scale_set(o, EINA_FALSE);
    evas_object_image_size_set (o, 7, 7);
    evas_object_image_data_set(o, (void *) mem);

    eo->datas = eina_list_append(eo->datas, mem);

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

void
draw_eenvaders(Evas_Object *smart_obj, Eenvaders_Object *eo,
               int x, int y, int w, int h)
{
    Evas_Object *o;
    int d;

    if (w < 7 || h < 7)
        return;

    d = square_ceil_7(MIN(w,h));

    o = new_eenvader(evas_object_evas_get(smart_obj), eo);
    evas_object_resize(o, d, d);
    evas_object_smart_member_add(o, smart_obj);

    switch(lrand48() & 3) {
      case 0:
        /* top-left */
        evas_object_move(o, x, y);
        evas_object_show(o);

        draw_eenvaders(smart_obj, eo, x+d, y, w-d, h); /* right */
        draw_eenvaders(smart_obj, eo, x, y+d, d, h-d); /* bottom */
        break;
      case 1:
        /* top-right */
        evas_object_move(o, x+w-d, y);
        evas_object_show(o);

        draw_eenvaders(smart_obj, eo, x, y+d, w, h-d); /* bottom */
        draw_eenvaders(smart_obj, eo, x, y, w-d, d); /* left */
        break;
      case 2:
        /* bottom-right */
        evas_object_move(o, x+w-d, y+h-d);
        evas_object_show(o);

        draw_eenvaders(smart_obj, eo, x, y, w-d, h); /* left */
        draw_eenvaders(smart_obj, eo, x+w-d, y, d, h-d); /* top */
        break;
      case 3:
        /* bottom-left */
        evas_object_move(o, x, y+h-d);
        evas_object_show(o);

        draw_eenvaders(smart_obj, eo, x, y, w, h-d); /* top */
        draw_eenvaders(smart_obj, eo, x+d, y+h-d, w-d, d); /* right */
        break;
    }
}

static void
_eenvaders_object_resize(Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o))) {
        Evas_Coord x, y;

        evas_object_geometry_get(o, &x, &y, NULL, NULL);
        draw_eenvaders(o, data, x, y, w, h);
    }
}


