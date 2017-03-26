# ShikuDB
Build Status: [![Build Status](https://travis-ci.org/Bokjan/ShikuDB.svg?branch=master)](https://travis-ci.org/Bokjan/ShikuDB) (Travis-CI)

ShikuDB is a naive NoSQL database designed for finishing my C++ course.

`shiku` can either be pronouced as シク(詩句, poem) or xīkù(奚库, big-belly database).

# Environment
I use OS X 10.11. Excluding Travis-CI, physical machines used to test: Fedora 25(amd64/ARMv7), Windows 7(x64).

ShikuDB currently runs well on Fedora 25, OS X 10.11, and is partly compatible with Windows. There're some [known bugs](https://github.com/Bokjan/ShikuDB/issues/3).

# Driver usage
- Ruby
	- Run an `irb`
	- `require_relative` the `Driver/Ruby/shiku.rb`
	- Use it

# Dependencies
- Included in this repo
	- [nlohmann/json](https://github.com/nlohmann/json)
	- [cesanta/mongoose](https://github.com/cesanta/mongoose)
	- [witwall/mman-win32](https://github.com/witwall/mman-win32)
- Not included in this repo
	- [Boost C++ Libraries](http://www.boost.org)

# Build Instructions
1. Run following in terminal (use MinGW on Windows):
```plain
scons
```