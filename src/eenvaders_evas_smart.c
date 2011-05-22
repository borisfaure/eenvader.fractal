#include <stdlib.h>

#include "eenvaders_evas_smart.h"

#define BG 0xff002b36
#define FG 0xff839496

#define MIN(a,b) (((a)<(b))?(a):(b))

/* Prototypes -{{{-*/
typedef struct Eenvaders_Object {
    Evas_Object *clip;
    Evas_Coord x, y, w, h;
    Evas_Object *outer;
    Evas_Object *inner;
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
        .show = _eenvaders_object_show,
        .hide = _eenvaders_object_hide,
        .color_set = _eenvaders_object_color_set,
        .clip_set = _eenvaders_object_clip_set,
        .clip_unset = _eenvaders_object_clip_unset,
        .calculate = NULL,
        .member_add = NULL,
        .member_del = NULL,
        .parent = NULL,
        .callbacks = NULL,
        .interfaces = NULL,
        .data = NULL,
    }
#define _G eenvaders_evas_smart_g
};

/* }}} */

Evas_Object *
eenvaders_new(Evas *e)
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
    data = (Eenvaders_Object *)malloc(sizeof(Eenvaders_Object));
    memset(data, 0, sizeof(Eenvaders_Object));

    data->clip = evas_object_rectangle_add(evas);
    data->outer = evas_object_rectangle_add(evas);
    evas_object_color_set(data->outer, 0, 0, 0, 255);
    evas_object_clip_set(data->outer, data->clip);
    evas_object_show(data->outer);
    data->inner = evas_object_rectangle_add(evas);
    evas_object_color_set(data->inner, 255, 255, 255, 126);
    evas_object_clip_set(data->inner, data->clip);
    evas_object_show(data->inner);
    data->x = 0;
    data->y = 0;
    data->w = 0;
    data->h = 0;
    evas_object_smart_data_set(o, data);
}

static void
_eenvaders_object_del(Evas_Object *o)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o))) {
        evas_object_del(data->clip);
        evas_object_del(data->outer);
        evas_object_del(data->inner);
        free(data);
    }
}

static void
_eenvaders_object_move(Evas_Object *o, Evas_Coord x, Evas_Coord y)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o))) {
        float ix, iy;

        ix = (data->w - (data->w * 0.8)) / 2;
        iy = (data->h - (data->h * 0.8)) / 2;
        evas_object_move(data->clip, x, y);
        evas_object_move(data->outer, x, y);
        evas_object_move(data->inner, x + ix, y + iy);
        data->x = x;
        data->y = y;
    }
}


static void
_eenvaders_object_resize(Evas_Object *o, Evas_Coord w, Evas_Coord h)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o))) {
        float ix, iy, iw, ih;

        iw = w * 0.8;
        ih = h * 0.8;
        ix = (w - iw) / 2;
        iy = (h - iw) / 2;
        evas_object_resize(data->clip, w, h);
        evas_object_resize(data->outer, w, h);
        evas_object_move(data->inner, data->x + ix, data->y + iy);
        evas_object_resize(data->inner, iw, ih);
        data->w = w;
        data->h = h;
    }
}

static void
_eenvaders_object_show(Evas_Object *o)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o)))
        evas_object_show(data->clip);
}

static void
_eenvaders_object_hide(Evas_Object *o)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o)))
        evas_object_hide(data->clip);
}


static void
_eenvaders_object_color_set(Evas_Object *o, int r, int g, int b, int a)
{
}

static void
_eenvaders_object_clip_set(Evas_Object *o, Evas_Object *clip)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o)))
        evas_object_clip_set(data->clip, clip);
}

static void
_eenvaders_object_clip_unset(Evas_Object *o)
{
    Eenvaders_Object *data;

    if ((data = evas_object_smart_data_get(o)))
        evas_object_clip_unset(data->clip);
}



