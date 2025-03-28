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

```@FOR item IN ${y}@```
```@END FOR@```

Iterate over the items in the variable y. If used on an object, this will iterate over
their values and not display their keys.

```@FOR key, value IN ${y}@```
```@END FOR@```

Iterate over the items in the variable y. If used on an array, the key will be the
index.

```@FOR i IN start..end@```
```@END FOR@```

Iterate over a range of numbers, begining at `start` and ending at the element before
`end`. Thus, `0..3` will iterate over `0 1 2`.

### Selection

```@IF condition@```
```@END IF@```

Evaluate the contents between IF and END IF if the condition evaluates to true.

### Set output filename

```@OUTPUT filename@```
```@END OUTPUT@```

Until END OUTPUT is reached, render to the given filename instead of the current
file.

### Render a variable

```${variable}```

Render the contents of a variable. This is only valid for literal objects.

```${JSON variable}```

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

NOOUT_COMMAND ::= "@NOOUT@"
NOOUT_COMMAND ::= "@INCLUDE " <EXPRESSION> "@"

DEFINE_COMMAND ::= "@" <DEFINITION> "@"
DEFINITION ::= "DEFINE " <VARIABLE_EXPANSION>  " = " <EXPRESSION>

FOR_BLOCK ::= <BEGIN_FOR> | <VALID_VOLE> | <END_FOR>
BEGIN_FOR ::= "@FOR" FOR_DEFINITION "@"
END_FOR ::= "@END FOR@"
FOR_DEFINITION ::= <FOR_EACH> | <FOR_EACH_KEY> | <FOR_RANGE>
FOR_EACH := <IDENTIFIER> " IN " <VARIABLE_EXPANSION>
FOR_EACH_KEY ::= <IDENTIFIER> ", " <IDENTIFIER> " IN " <VARIABLE_EXPANSION>
FOR_RANGE ::= <IDENTIFIER> " IN " <EXPRESSION> ".." <EXPRESSION>

IF_BLOCK ::= <IF_DEFINITION> | <VALID_VOLE> | <END_IF>
IF_DEFINITION ::= "@IF" <EXPRESSION> "@"
END_IF ::= "@END IF@"

IF_BLOCK ::= <IF_DEFINITION> | <VALID_VOLE> | <END_IF>
IF_DEFINITION ::= "@IF" <EXPRESSION> "@"
END_IF ::= "@END IF@"

OUTPUT_BLOCK ::= <OUTPUT_DEFINITION> | <VALID_VOLE> | <END_OUTPUT>
OUTPUT_DEFINITION ::= "@OUTPUT" <EXPRESSION> "@"
END_OUTPUT ::= "@END OUTPUT@"
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
@NOOUT@
@DEFINE notice="/** COPYRIGHT Michael Volling 2025"@

@FOR enum IN emums@
    @OUTPUT "enums/${enum.name}.hpp"@
        ${notice}
        
        #include <string>
        
        enum class ${enum.name} {
            @FOR name, value IN ${enum.items}@
            ${name} = ${value}
            @END FOR@
        }
        
        std::string stringify(${enum.name} value);
        
        ${enum.name} parse_${enum.name}(const std::string &value);
    @END OUTPUT@
@END FOR@

@FOR enum IN emums@
    @OUTPUT "enums/${enum.name}.cpp"@
        ${notice}
        
        #include "enums/${enum.name}"
        #include <exception>
        
        std::string stringify(${enum.name} value) {
            switch (value) {
                @FOR name, _value IN ${enum.items}@
                case ${name}: return "${name}";
                @END FOR@
                default: return "INVALID";
            }
        }
        
        ${enum.name} parse_${enum.name}(const std::string &value) {
            @FOR name, _ IN ${enum.items}@
                if (value == "${name}") return ${name};
            @END FOR@
            throw std::runtime_error("Invalid Value: " + value);
    @END OUTPUT@
@END FOR@
```
