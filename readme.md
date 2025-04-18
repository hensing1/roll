# Roll

Roll dice in the command line.

## Installation

On Linux:

```bash
$ make
cc main.c -o roll
$ make install
cp roll ~/.local/bin/
```

On others: see [here](https://fedoraproject.org/workstation/)

## How it work

The default roll is a d6.

```bash
$ roll
4	(d6)
```

Specify other dice with `d[num_faces]`:

```bash
$ roll d20
15	(d20)
```

Multiple dice: `[num_dice]d[num_faces]`

```bash
$ roll 4d8
3	(d8)
1	(d8)
8	(d8)
8	(d8)
Sum: 20
```

Mulitple different dice can be separated by `+` or `<space>`:

```bash
$ roll 2d8+2d4
7	(d8)
6	(d8)
4	(d4)
4	(d4)
Sum: 21

$ roll d20 d100 d592
19	(d20)
97	(d100)
584	(d592)
Sum: 700
```

Happy rolling, may the odds be ever in your favor!
