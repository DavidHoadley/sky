The Sky routines
================

Routines for astronomical tracking, particularly solar tracking. They are
designed to be included in a controller that will drive a device (like a
telescope, or a solar power heliostat, or a high accuracy solar tracker) that
needs to track a celestial object.

For documentation, see https://davidhoadley.github.io/sky/html/index.html

The routines provide an efficient implementation of the various celestial
coordinate transformations. They are not designed for astrometric reduction
of coordinates - if you want that, go to the International Astronomical Union's
set of SOFA routines. But they do provide better than arcsecond accuracy for
objects outside the solar system, and one arcsecond accuracy for the Sun. (The
routines for the Moon and other planets in the Solar System are not as accurate
as that, but they are good enough for most purposes.)

Why this set of routines?
-------------------------

There are other routines out there, including those written in Python. This set
is written in C, for speed. But because I have seen so much bad C code out
there, it is also meant as an example of how I think C code should be written.
Mind you, I don't claim that the code is perfect, but a bit of work has gone
into making it as good and as clear as possible. But any code can be improved.

The code has also been written around the use of the Doxygen tool for generating
documentation. Once again, I have seen so much crap documentation written using
Doxygen, that I have put this here as an example of how I think this should be
done. Too many programmers seem to think that a \brief one-liner is enough to
document any function. It isn't.

If you have suggestions to improve the code or documentation, do get in touch.

Doxygen
-------
There are two guiding principles to my use of Doxygen. They are:
1. Comments, including the ones to be read by Doxygen, are to be as close as
    possible to the code they describe. So function descriptions are always in
    the .c file with each function, __never__ in the .h file. Having
    documentation and implementation in separate files is a great way to make
    sure they evolve away from each other.
2. Comments should be readable in the code. So there should be an absolute
    minimum of markup commands polluting the descriptions. They are basically
    a distraction to someone working on the code. So I make use of the Markdown
    support provided by Doxygen as much as possible, with only a few explicit
    Doxygen commands. For example, \file is needed at the top of each header
    file, and \param is needed to document function parameters. I also use some
    \note and \par commands, as they are not too visually distracting.

### Doxygen settings file

The Doxyfile settings file is changed from the default as follows:
```
OUTPUT_DIRECTORY      = docs
FULL_PATH_NAMES       = NO
JAVADOC_AUTOBRIEF     = YES
JAVADOC_BANNER        = YES
QT_AUTOBRIEF          = YES
OPTIMIZE_OUTPUT_FOR_C = YES
EXTRACT_STATIC        = YES     (see note)
EXTRACT_LOCAL_CLASSES = NO
HIDE_SCOPE_NAMES      = YES
SHOW_INCLUDE_FILES    = NO
SORT_MEMBER_DOCS      = NO
INPUT                 = src
EXAMPLE_PATH          = examples
SOURCE_BROWSER        = YES
STRIP_CODE_COMMENTS   = NO
ALPHABETICAL_INDEX    = NO
GENERATE_TREEVIEW     = YES
GENERATE_LATEX        = NO
PREDEFINED            = PREDEF_STANDARD_C_1999
```

Note. EXTRACT_STATIC is set to yes in order to document static inline
functions. But I don't want to document all other static functions. That is
achieved by __not__ having a \file command in any .c file; \file appears only
in .h files.

