# 43_gnl

Done like last time with chained list for fds. Tested extensively with multiple fd's at once, should be good. Valgrind tests are good.
Handles binaries by replacing any 0's it encounters with 1's (ASCII 1 meaning SOH (start of heading)). This loses information (cannot know if a 1 was a 1 or a 0 before) but at least it outputs something that looks like cat.
