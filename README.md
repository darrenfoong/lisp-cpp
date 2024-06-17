# Lisp interpreter in C++

## Configuring

```
cmake --preset=dev
```

## Formatting

```
cmake --build --preset=dev -t format-fix
```

## Building

```
cmake --build --preset=dev
```

## Testing

```
ctest --preset=dev
```

## Running

```
cmake --build --preset=dev -t run-exe
```

## Notes

Project structure adapted from https://github.com/friendlyanon/cmake-init-executable

Code adapted from http://norvig.com/lispy.html
