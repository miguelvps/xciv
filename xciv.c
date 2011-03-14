#include <stdlib.h>

#include <xcb/xcb.h>


int main(int argc, const char *argv[])
{
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_window_t win;
    xcb_gcontext_t gc;
    uint32_t mask = 0;
    uint32_t values[2];
    uint8_t *image;
    int i;

    /* Open the connection to the X server */
    c = xcb_connect(NULL, NULL);

    /* Get the first screen */
    screen = xcb_setup_roots_iterator(xcb_get_setup (c)).data;

    /* Ask for our window's Id */
    win = xcb_generate_id(c);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->black_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE;

    /* Create the window */
    xcb_create_window(c,                             /* Connection    */
                      XCB_COPY_FROM_PARENT,          /* depth         */
                      win,                           /* window Id     */
                      screen->root,                  /* parent window */
                      0, 0,                          /* x, y          */
                      150, 150,                      /* width, height */
                      10,                            /* border_width  */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class         */
                      screen->root_visual,           /* visual        */
                      mask, values);                 /* masks         */

    /* create gc */
    mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    values[0] = screen->white_pixel;
    values[1] = screen->black_pixel;

    gc = xcb_generate_id (c);
    xcb_create_gc (c, gc, win, mask, values);

    /* Map the window on the screen */
    xcb_map_window(c, win);

    /* Make sure commands are sent before we pause, so window is shown */
    xcb_flush(c);

    image = (uint8_t *)malloc(150 * 150 * 4);
    for (i=0; i<150*150*4;) {
        image[i++] = 0;
        image[i++] = 0;
        image[i++] = 255;
        image[i++] = 0;
    }

    while (xcb_wait_for_event (c)) {
        xcb_put_image(c, XCB_IMAGE_FORMAT_Z_PIXMAP, win, gc, 150, 150, 0, 0, 0, 24, 150*150*4, image);
    }

    return 0;
}
