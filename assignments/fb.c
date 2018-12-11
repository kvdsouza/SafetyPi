#include "mailbox.h"
#include "fb.h"
#include "printf.h"
#include "font.h"

// This prevents the GPU and CPU from caching mailbox messages
#define GPU_NOCACHE 0x40000000

typedef struct {
  unsigned int width;       // width of the display
  unsigned int height;      // height of the display
  unsigned int virtual_width;  // width of the virtual framebuffer
  unsigned int virtual_height; // height of the virtual framebuffer
  unsigned int pitch;       // number of bytes per row
  unsigned int depth;       // number of bits per pixel
  unsigned int x_offset;    // x of the upper left corner of the virtual fb
  unsigned int y_offset;    // y of the upper left corner of the virtual fb
  unsigned int framebuffer; // pointer to the start of the framebuffer
  unsigned int size;        // number of bytes in the framebuffer
} fb_config_t;

// fb is volatile because the GPU will write to it
static volatile fb_config_t fb __attribute__ ((aligned(16)));

void fb_init(unsigned int width, unsigned int height, unsigned int depth, unsigned int mode)
{
  // TODO: add code to handle double buffering depending on mode
  // should i just have an if statement that sets virtual height = 2*height;
  fb.width = width;
  fb.virtual_width = width;
  fb.height = height;
  if (mode == 1) {
      fb.virtual_height = 2*height; // in 2 times case.
  } else {
      fb.virtual_height = height;
  }
  fb.depth = depth * 8; // convert number of bytes to number of bits
  fb.x_offset = 0;
  fb.y_offset = 0;

  // the manual requires we to set these value to 0
  // the GPU will return new values
  fb.pitch = 0;
  fb.framebuffer = 0;
  fb.size = 0;

  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
  (void) mailbox_read(MAILBOX_FRAMEBUFFER);
}

/*
 * Swap the front and back buffers. The draw buffer is moved to the
 * front (displayed) and the front buffer is moved to the back
 * (becomes the draw buffer).
 *
 * If not in double buffering mode, this function has no effect.
 */

void fb_swap_buffer(void)
{
    // set up a drawbuffer and a front buffer.
    // display front, draw into back, swap buffer to update display.
    // move the two. (if not in double buffering mode, do nothing)
    if (fb.height != fb.virtual_height) {
        if (fb.y_offset == 0) {
            fb.y_offset = fb.height;
        } else if (fb.y_offset == fb.height) {
            fb.y_offset = 0;
        }
        mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
        mailbox_read(MAILBOX_FRAMEBUFFER);
    }
    // is this how i set it up?
    // swap buffer - double case.
    //writing the fb struct to the mailbox with the new x_offset and y_offset
    // y_offset in double case is equal to height. mailbox_write and then mailbox_read
}


/*
 * Get the start address of the portion of the framebuffer currently
 * being drawn into. This address is the start of an array of size
 * pitch*height bytes.  If in single buffering mode, the address
 * returned will not change as there is only one buffer in use.
 * In double buffering mode, the returned address differs based on
 * which buffer is currently being used as the draw buffer.
 *
 * @return    the address of the current draw buffer
 */
unsigned char* fb_get_draw_buffer(void)
{
    // check y_offset
    if (fb.height != fb.virtual_height) {
        if (fb.y_offset == 0) {
            return (unsigned char *)(fb.framebuffer + fb.height*fb.pitch);
        } else if (fb.y_offset == fb.height) {
            return (unsigned char *)fb.framebuffer;
        }
    }
    return (unsigned char *)fb.framebuffer;
}
    // and then based on what it is (height or 0) I'll return the opposite.
    // in the single case.

    // in double case.



unsigned int fb_get_width(void)
{
    // single case.
    return fb.width;
}

unsigned int fb_get_height(void)
{
    // TODO: implement this function
    return fb.height;
}


// bits to bytes.
unsigned int fb_get_depth(void)
{
    // TODO: implement this function
    return (fb.depth / 8);
}


unsigned int fb_get_pitch(void)
{
    // single case
    return fb.pitch;
}
