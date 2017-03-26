# Suffix Tree Algorithms

This is a collection of [suffix tree](https://en.wikipedia.org/wiki/Suffix_tree)
algorithms, implemented in C. The featured algorithms are:

- The longest to shortest suffix order naive algorithm
- [McCreight's algorithm](https://en.wikipedia.org/wiki/Ukkonen%27s_algorithm)
- Ukkonen's algorithm

The naive algorithm has a worst-case performance of roughly O(n^2), while both
McCreight's algorithm and Ukkonen's algorithm are O(n). But, while McCreight's
algorithm always requires access to the whole string from the start, Ukkonen's
algorithm is online (it reads the string one character at a time).

The algorithms compile to one executable each. When run without arguments they
start a benchmark that prints a [gnuplot](http://www.gnuplot.info) script that
plots the performance results. When run with an argument, they produce the
suffix tree of the argument and they output it as a
[graphviz](http://www.graphviz.org) file. You can pipe the output directly into
gnuplot/graphviz if you're in a hurry.

## Compiling

These algorithms can be compiled with [gcc](https://gcc.gnu.org/) or 
[clang](http://clang.llvm.org/) on Linux and OS X, or with 
[mingw-w64](http://mingw-w64.org/doku.php) on Windows. Just open a shell
and run `make`. At this moment they have been tested on OS X only, but
except for the benchmark stuff they should already be portable enough.

If you want to compile with Visual Studio on Windows, you'll have to make
the Visual Studio project yourself. It should work without modifications but
I have not tested it yet.

## References

For understanding both McCreight's algorithm and Ukkonen's algorithm, I found
[these lecture 
slides](http://users-birc.au.dk/cstorm/courses/StrAlg_f12/slides/suffix-tree-construction.pdf)
by [Thomas Mailund](http://users-birc.au.dk/mailund/new/index.html) very useful.
They use a more visual approach than most other material I've read; if you like
visualizing things in your head to understand them, they will most definitively
be useful.

Despite their very good description, the implementation of McCreight's algorithm
found in Mailund's slides is a bit chaotic because it does not properly
segregate suffix link creation and it does not handle special cases in a smooth
way. [These
slides](https://www.cs.helsinki.fi/u/tpkarkka/teach/15-16/SPA/lecture08.pdf) by
[Juha Kärkkäinen](https://www.cs.helsinki.fi/u/tpkarkka/) have a very scant
description of the algorithm, but they also present a very streamlined
implementation in pseudocode. I reccommend reading at them after reading
Mailund's slides.

If you find formal notations more natural, [this
paper](http://europa.zbh.uni-hamburg.de/pubs/pdf/GieKur1997.pdf) by R. Giegerich
and S. Kurtz contains the most succint descriptions and pseudocode of McCreight
and Ukkonen's algorithms that I've seen. The goal of the paper is proving that
Ukkonen's algorithm is none other than McCreight's algorithm in disguise (and it
turns out that it is).

These [slides](http://www.cs.cmu.edu/~guyb/realworld/slidesF07/suffix.ppt) by
Guy Blelloch explain Ukkonen's algorithm as it was popularized in "Algorithms on
Strings, Trees and Sequences" by D. Gusfield. Not as clear as Mailund's slides
in my opinion, but pretty decent nontheless. Notable is the fact that, while it
is not obvious at first glance, a complete pseudocode explanation of the
algorithm is present in slide 19 (most explanations of this algorithm have no
pseudocode at all for some reason).

Last (and least), many people recommend reading [this
explanation](https://stackoverflow.com/a/9513423/414272) of Ukkonen's algorithm
on stackoverflow, but I do **not**. It focuses a lot on the specifics of the
implementation, and on specific special cases, and not on the general idea
behind the algorithm; and it is even wrong on how suffix links should be handled
(the second-rated answer clears it up but the damage is done; the thing is even
linked on Wikipedia). I do not say this for direspect of the author, but because
this is the first thing I came across while researching suffix links, and it
confused me for quite some time.

I am recommending (or not) this material based on personal experience; the fact
that it worked (or not) for me does not mean it will also work (or not) for you.
If you do not manage to understand these algorithms with these resources and my
implementation, do your own research.

### Primary References

Weiner, P. (1973), "Linear pattern matching algorithms", 14th Annual IEEE
Symposium on Switching and Automata Theory, pp. 1–11

McCreight, Edward M. (1976), "A Space-Economical Suffix Tree Construction
Algorithm", Journal of the ACM, 23 (2): 262–272

Weiner, P. (1973), "Linear pattern matching algorithms", 14th Annual IEEE
Symposium on Switching and Automata Theory, pp. 1–11


