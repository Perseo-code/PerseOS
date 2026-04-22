# Welcome to the docs directory!
In here, I will documentate everything I can to explain how to create an OS if you're interested. \
You will also find information about how this OS works, meaning you can easily configure, execute and more things.

# Directories.
For now, I'll talk about the directory structure and how it works. \
├── LICENSE → The public license \
├── Makefile → A file that we use to write a program which compiles a program easily \
├── README.md → What appears on github when you find this directory \
├── build → A directory where you can find compiled files. \
├── docs → The documentation directory \
├── include → Where the header files reside
├── kernel → The important files that compose your kernel
│   ├── arch → Architecture (Depends on the cpu)
│   │   └── x86_64 → The directory of that specific architecture of your PC (64-bit only)
│   ├── core → Where you can find the file that makes your computer do something.
│   ├── drivers → The drivers (keyboards, vga...) Without this, you'll see nothing.
│   ├── libs → Libraries (Not the book one)
│   └── memory → Here goes the allocator files and more.
└── linker → Where the linker file resides.