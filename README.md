
# Sudoku with C++

Sudoku is a favourite puzzle game of mine. When I started programming in my first year of university, I wrote a solution generator in C. Looking back, I can say the code was terrible, but I still remember the feelings of excitement and gratification that I felt. I [reworked it after learning Java](https://github.com/david-fong/Sudoku-J), then later started this repo, and next, I look to write [a hardware implementation using SystemVerilog](https://github.com/david-fong/Sudoku-SV).

I'm quite proud of this version. It can dance circles around my earlier attempts written in C and Java (because I figured out some new tricks). You can find [sample benchmark statistics in the sample data](/sample_data) folder, or clone, build, and run it for yourself (I've set up a handy [REPL](https://wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop)). I was especially and delightfully surprised to find that it can sometimes generate 25x25 solutions within a matter of seconds, which I previously thought I would never have the pleasure of being able to say.

I could talk a lot about this, but I would actually rather you try it for yourself first rather than read anything I have to say about it. I think you would enjoy that more anyway- by orders of magnitude. For my friends outside the field of computer engineering and science, I will prepare a demo / teaching video and link it here when it is finished.

<img src="images/25x25_example.png" width="44%" align="left" />

<img src="images/measured_average_num_operations.png" width="51%" align="right" />

## Table of Contents

- [Sudoku in a Nutshell](#sudoku-in-a-nutshell)
- [Wikipedia Links](#wikipedia-links)
- [Solution Space](#solution-space)

## Sudoku in a Nutshell

Sudoku is a common puzzle game often found in newspapers where the goal is to re-construct a square grid of symbols according to simple conflict rules. The grid is defined by a size argument `S` which is typically three, and is composed of `S^4` atomic containers that can carry one of certain symbols, of which there are the same `S^2` for each container in the grid (typically the numbers one through `S^2`). The only rule is that no row, column, or block can contain duplicate symbols. In other words, each row, column, and block must contain one of each symbol. It's a lot easier said than done.

Puzzles are computer-generated in such a way that there is at least enough starting information for there to be only one solution (ie. There are no two ways to complete the grid that produce the same arrangements of symbols). This can be done by brute-forcing the creation of a full solution, and then incrementally erasing symbols at random until any further erasure would allow for multiple valid completions of the grid.

The systematic processes of solving a puzzle and of generating a solution using backtracing (brute force) are actually the same. The defining differences are in how they start and end. When generating a solution, there is no starting information, and a very large (but computable and finite) number of possible completions. When solving a puzzle, there is at least enough starting information such that there is only one possible completion.

## Wikipedia Links

- [Sudoku (Topic Homepage)](https://wikipedia.org/wiki/Sudoku)
- [Mathematics of Sudoku](https://wikipedia.org/wiki/Mathematics_of_Sudoku)
- [Sudoku Solving Algorithms](https://wikipedia.org/wiki/Sudoku_solving_algorithms)

- [Number of Sudoku Solutions](http://www.afjarvis.staff.shef.ac.uk/sudoku/)

## Solution Space

We can compute some easier ceilings on the number of unique solutions (including those that are symbolically and/or relationally equivalent) first to help get an idea of the scale of things. We know it must be equal to or less than the number of symbol arrangements that do not need to follow the rules, which is `(S^2)^(S^4)`. We know that each group of the same kind cannot contain duplicate elements, so it must be equal to or less than `((S^2)!)^(S^2)`, which is the number of symbol arrangements where the conflict rule is only applied to groups of one chosen type.

The true number of all solutions can be calculated by taking the product of the number of symbol options for each container as they are procedurally filled-in according to a conflict-heavy traversal order for solution generation. Symbol option counts should be taken pessimistically: Use the complement of the maximum occupancy count across the three related group types. That is, if the next container to fill in the theoretical generation traversal for a size-3 grid is in a row occupied by 3 other symbols, a column by 5, and a block by 2, then the pessimistic number of symbol options for that container is `9 - max(3,5,2) = 4`.

The equation described above would be quite complex. As a first step, we can take a slightly simplified version that is still a provable ceiling for the size of the solution space (but is still not the lowest provable ceiling). The traversal order travels over all blocks in row-major order, and at each block, traverses it at once internally- also in row-major order.

`TODO: create diagrams illustrating the logic and insert them here.`

The above diagram shows that for a block `B_i,j`, the number of combinations of symbols according to this traversal order is no more than (graph's vertical axis is in log scale):

<img src="images/solution_space_graphs.png" width="55%" align="left" />

<img src="images/solution_space_approx.png" width="40%" align="right" />

[**`open graph in desmos`** 👈](https://www.desmos.com/calculator/bwq2njp2ru)

## Time Complexity
