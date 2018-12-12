#include "backtrace.h"
#include "printf.h"
#include "strings.h"

int backtrace(frame_t f[], int max_frames)
{
		void *cur_fp;
		__asm__("mov %0, fp" : "=r" (cur_fp));

		uintptr_t saved_fpAddress; // or uintptr_t *saved_pc = *(uintptr_t*)cur_fp;
		uintptr_t lrAddress;
		uintptr_t ffTextAddress;
		int counter = 0;

		uintptr_t saved_pcAddress = (uintptr_t)cur_fp; // the address of cur_fp - address.


		while (*(uintptr_t *)(saved_pcAddress - 12) != 0x00000000 && counter < max_frames) { // watch the edge case here. maybe just check.
			saved_fpAddress = saved_pcAddress - 12; // set them up
			lrAddress = saved_pcAddress - 4; // set them up.

			f[counter].resume_addr = *(uintptr_t *)lrAddress;
			saved_pcAddress = *(uintptr_t *)saved_fpAddress; // does the move from fp to pc.

			//test
			//f[counter].resume_offset = *(uintptr_t *)saved_pc;
			// funky.
			f[counter].resume_offset = f[counter].resume_addr - (*(uintptr_t *)saved_pcAddress - 12);
			//if statement here. -> bitmask.

			ffTextAddress = *(uintptr_t *)saved_pcAddress - 0x0010;
			uintptr_t textContents = *(uintptr_t *)ffTextAddress;
			uintptr_t mask = 0xff000000;

			if ((textContents & mask) == mask) {
				uintptr_t length = textContents - mask; // 0xff000008 - 0xff000000
				f[counter].name = (char *)(ffTextAddress - length);
			} else {
				f[counter].name = "???";
			}

			counter++;
		}
		return counter;
}




void print_frames (frame_t f[], int n)
{
    for (int i = 0; i < n; i++)
        printf("#%d 0x%x at %s+%d\n", i, f[i].resume_addr, f[i].name, f[i].resume_offset);
}

void print_backtrace (void)
{
    int max = 50;
    frame_t arr[max];

    int n = backtrace(arr, max);
    print_frames(arr+1, n-1);   // print frames starting at this function's caller
}
