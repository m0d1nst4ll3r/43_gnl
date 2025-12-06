# 43\_gnl

V2 with everything redone:

- No more realloc, just storing buffers in a chained list
- No writing over and over, just writing once at the end
- Still not using "stash" strategy, just leaving buffer as-is and updating index
- Removed binary handling due to norm (technically doable but would make the code even more of a headache than it is)
	- By the way, norm just makes my code uglier. It could be way more readable. But nope: norm.

### Improvements

- Do with a "stash" to optimize memory (but require extra instructions)
- Optimize code even more (probably a LOT to optimize, but annoying with the norm)
- Handle binaries
- Add libmalloc
