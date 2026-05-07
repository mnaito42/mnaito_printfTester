# mnaito_printfTester

A simple tester for the 42 ft_printf project.

This tester compares:
- output string
- return value

between:
- printf
- ft_printf

## Supported conversions

- `%c`
- `%s`
- `%p`
- `%d`
- `%i`
- `%u`
- `%x`
- `%X`
- `%%`

## Tested features

- width
- precision
- flag combinations
- edge cases
- NULL handling
- INT_MIN / INT_MAX
- UINT_MAX
- pointer formatting

## Installation

Clone this repository next to your `ft_printf` repository.

Example:

```
.
├── ft_printf/
└── mnaito_printfTester/
```

## Usage

The tester automatically builds `libftprintf.a` if needed.

Build:

```
make
```

Run tests:

```
make run
```

Test results are saved to `result.txt`.

Show full result:

```
make show
```

Run with valgrind (Linux only):

```
make valgrind
```

Clean:

```
make clean
```

Remove binaries:

```
make fclean
```

## Notes

Some '%' flag tests may behave differently depending on the libc implementation because they are undefined behavior in the C standard.
