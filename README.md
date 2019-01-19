
# Smart Pointers for Cinnamon

_in development_

## Todo
- remove all couts
- constnesses and optionalities
- make registry entries unions
- proper includes

## Open Points

### Syntax

To indicate the pointer type, I would like to use * for unique pointers, & for shared pointers, ~ for weak pointers and another symbol for point-at-able stack objects ($? @? #? We will work with @ for now.). Unmarked objects can only be passed by value.

C++ users will be confused by the & meaning shared pointer but & is just a beautiful symbol for a shared pointer.

Should handle type only be indicated during declaration?
```
var myObj := myClass&(5)
var myObj := &myClass(5)
```
Or should the variable have to carry it around?
```
var &myObj := myClass(5)
var myObj& := myClass(5)
```
Or both?
```
var &myObj := &myClass(5)
var myObj& := myClass&(5)
```
