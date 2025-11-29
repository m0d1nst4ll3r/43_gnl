# 43_gnl

Done like last time with chained list for fds. Tested extensively with multiple fd's at once, should be good. Valgrind tests are good.

Handles binaries by replacing any 0's it encounters with 1's (ASCII 1 meaning SOH (start of heading)). This loses information (cannot know if a 1 was a 1 or a 0 before) but at least it outputs something that looks like cat.

PS: This approach takes a tad too much time in case of very big lines. Since buffer is copied into a realloc'd string after every read, extremely long lines will take increasingly more and more time after each read (more and more bytes to copy). The way to fix this is to memorize the buffers without copying anything after each read, in, for example, a chained list. E.g chained list node #1 has 1st buffer sent to read, node #2 has 2nd buffer, etc...

Could redo this one more clean with this approach. Read in a loop, each new read follows an add_back, there is 0 copying. A tot_index gives you how much you should malloc at the end, you only copy at the end. Depending on whether you reached the end of the file/of the buffer, you might have to clear all of your list, or you might clear all but the last (reached a \n which is not the last byte).
