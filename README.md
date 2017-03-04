# What is ShikuDB
ShikuDB is a naive NoSQL database designed for finishing my C++ course.

`shiku` can either be pronouced as シク(詩句, poem) or xīkù(奚库, big-belly database).

# Environment
I use OS X 10.11.

ShikuDB currently runs well on Windows 7/8/10, Fedora 25, OS X 10.11. 

# Driver usage
- Ruby
	- Run an `irb`
	- `require_relative` the `Driver/Ruby/shiku.rb`
	- Use it

# Dependencies
- Included in this repo
	- [nlohmann/json](https://github.com/nlohmann/json)
	- [cesanta/mongoose](https://github.com/cesanta/mongoose)
- Not included in this repo
	- *null*

# Build Instructions
1. Run following in terminal (use MinGW on Windows):
```plain
scons
```