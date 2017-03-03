# What is shiku
`shiku` is a naive NoSQL database designed for finishing my C++ course.

`shiku` can either be pronouced as シク(詩句, poem) or xīkù(奚库, big-belly database).

# Environment
I use OS X 10.11.

`shiku` currently runs well on Windows 7/8/10, Fedora 25, OS X 10.11. 

# Dependencies
- [nlohmann/json](https://github.com/nlohmann/json)
- [Libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/)

# Build Instructions
1. Install `Libmicrohttpd`
    - macOS / OS X: `brew install libmicrohttpd`
    - Linux (Fedora): `sudo dnf install libmicrohttpd`
    - Windows: Manually download binary from [Libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) and copy it to your MinGW environment.
    
2. Run following in terminal (use MinGW on Windows):
    ```plain
    scons
    ```