## Vole Template Language Reference

A vole template will render the text that appears verbatim with the following
caveats:

Blocks can appear over multiple lines.

| Block       | Name            | Meaning                                                                      |
|-------------|-----------------|------------------------------------------------------------------------------|
| `@# ... #@` | Comment Section | The wrapped text will be interpreted as a comment block and is not rendered. |
| `@ ... @`   | Command Section | The wrapped text will be interpreted as vole instructions.                   |
| `@@`        | @ Literal       | Vole will render an @ symbol into the produced output.                       |
| `$$`        | $ Literal       | Vole will render an $ symbol into the produced output.                       |
| `${var}`    | Render Section  | Vole will render the variable represented by the var name.                   |

## Vole Commands

```@DEFINE var=value@```

Define a new variable `var` with value `value`.

```@IMPORT filename@```

Import the contents of filename into the current render scope.

### Looping

```@for item in ${y}@```
```@end for@```

Iterate over the items in the variable y. If used on an object, this will iterate over
their values and not display their keys.

```@for key, value in ${y}@```
```@end for@```

Iterate over the items in the variable y. If used on an array, the key will be the
index.

```@for i in start..end@```
```@end for@```

Iterate over a range of numbers, begining at `start` and ending at the element before
`end`. Thus, `0..3` will iterate over `0 1 2`.

### Selection

```@if condition@```
```@end if@```

Evaluate the contents between IF and END IF if the condition evaluates to true.

### Set output filename

```@output filename@```
```@end output@```

Until `end output` is reached, render to the given filename instead of the current
file.

### Render a variable

```${variable}```

Render the contents of a variable. This is only valid for literal objects.

```${variable | dump}```

Render the model of a variable. This is valid for any object type.

## BNF Form

```BNF
VALID_VOLE ::= <BLOCK> | <BLOCK> <BLOCK>
BLOCK ::= <TEMPLATE_BLOCK> | <COMMENT_BLOCK> | <COMMAND_BLOCK>

COMMENT_BLOCK ::= "@#" <CHAR_SET> "#@"

TEMPLATE_BLOCK ::= <RENDERABLE> | <RENDERABLE> <RENDER_BLOCK>
RENDERABLE ::= <TEXT> | <RENDER_BLOCK>
RENDER_BLOCK ::= "${" <EXPRESSION> "}"
TEXT ::= <LETTERS> | <NUMBERS> | " " | <NON_SPECIAL_SYMBOLS> | <TEXT> <TEXT>

EXPRESSION ::= <VARIABLE_EXPANSION> | <LITERAL>

VARIABLE_EXPANSION ::= <IDENTIFIER> | <IDENTIFIER> <ACCESSOR>

ACCESSOR ::= <PATH_ACCESSOR> | <ARRAY_ACCESSOR>
PATH_ACCESSOR ::= "." <VARIABLE_EXPANSION>
ARRAY_ACCESSOR ::= "[" <EXPRESSION> "]" | "[" <EXPRESSION> "]" <ACCESSOR>

COMMAND_BLOCK ::= <NOOUT_COMMAND> | <INCLUDE_COMMAND> | <DEFINE_COMMAND> | <FOR_BLOCK> | <IF_BLOCK> | <OUTPUT_BLOCK>

NOOUT_COMMAND ::= "@noout@"
INCLUDE_COMMAND ::= "@include " <EXPRESSION> "@"

DEFINE_COMMAND ::= "@" <DEFINITION> "@"
DEFINITION ::= "define " <VARIABLE_EXPANSION>  " = " <EXPRESSION>

FOR_BLOCK ::= <BEGIN_FOR> | <VALID_VOLE> | <END_FOR>
BEGIN_FOR ::= "@for" FOR_DEFINITION "@"
END_FOR ::= "@end for@"
FOR_DEFINITION ::= <FOR_EACH> | <FOR_EACH_KEY> | <FOR_RANGE>
FOR_EACH := <IDENTIFIER> " in " <VARIABLE_EXPANSION>
FOR_EACH_KEY ::= <IDENTIFIER> ", " <IDENTIFIER> " in " <VARIABLE_EXPANSION>
FOR_RANGE ::= <IDENTIFIER> " in " <EXPRESSION> ".." <EXPRESSION>

IF_BLOCK ::= <IF_DEFINITION> | <VALID_VOLE> | <END_IF>
IF_DEFINITION ::= "@if" <EXPRESSION> "@"
END_IF ::= "@end if@"

IF_BLOCK ::= <IF_DEFINITION> | <VALID_VOLE> | <END_IF>
IF_DEFINITION ::= "@if" <EXPRESSION> "@"
END_IF ::= "@end if@"

OUTPUT_BLOCK ::= <OUTPUT_DEFINITION> | <VALID_VOLE> | <END_OUTPUT>
OUTPUT_DEFINITION ::= "@output" <EXPRESSION> "@"
END_OUTPUT ::= "@end output@"
```

## Motivating Example: Enum Stringification

Input:
```json
{
  "enums": [
    {
      "name": "Operations",
      "items": {
        "ADD": 1,
        "SUB": 2,
        "MULT": 3,
        "DIVIDE": 4
      }
    },
    {
      "name": "Directions",
      "items": {
        "NORTH": 1,
        "SOUTH": 2,
        "EAST": 3,
        "WEST": 4
      }
    }
  ]
}
```

```
@# Disable outputting files outside of the OUTPUT statements #@
@noout@
@define notice="/** COPYRIGHT Michael Volling 2025"@

@for enum in emums@
    @output "enums/${enum.name}.hpp"@
        ${notice}
        
        #include <string>
        
        enum class ${enum.name} {
            @for name, value in ${enum.items}@
            ${name} = ${value}
            @end for@
        }
        
        std::string stringify(${enum.name} value);
        
        ${enum.name} parse_${enum.name}(const std::string &value);
    @end output@
@end for@

@for enum in emums@
    @output "enums/${enum.name}.cpp"@
        ${notice}
        
        #include "enums/${enum.name}"
        #include <exception>
        
        std::string stringify(${enum.name} value) {
            switch (value) {
                @for name, _value in ${enum.items}@
                case ${name}: return "${name}";
                @end for@
                default: return "INVALID";
            }
        }
        
        ${enum.name} parse_${enum.name}(const std::string &value) {
            @for name, _ in ${enum.items}@
                if (value == "${name}") return ${name};
            @end for@
            throw std::runtime_error("Invalid Value: " + value);
        }
    @end output@
@end for@
```
