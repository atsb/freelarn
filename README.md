I am the official maintainer of FreeLarn.

# Compiling from Source #
Compiling Larn from source will require a C++11 compiler as a minimum plus ncurses/pdcurses.  See the 'build' folder for makefiles.
Compilation should be without a single warning.  I treat all warnings as errors (and therefore bugs).

# History #
FreeLarn is a direct descendent of the original Larn, that has been mostly (approx 91%) rewritten into C++11 with many additions and features that many now consider a variant.

Initially Development stopped in March 2018 due to the negativity expressed by some people about my changes.  I genuinely missed working on it and enjoy playing it, so I picked it back up.

# Code Style #
I'm generally following the Linux kernel coding style, going function by function and rewriting them to classes and / or templating them to modern equivalents but still be almost drop-in replacements for the old stuff.  Usually each function rewritten gets about 1 hour of testing and fine-tuning.

# Contributing # 
Feel free to raise Issues or PR's if you have something to add/fix.
