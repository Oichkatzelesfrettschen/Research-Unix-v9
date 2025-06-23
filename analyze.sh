#!/bin/sh
# Generate code analysis artifacts
set -e

# Build cscope database
cscope -R -b

# Count lines of code
cloc --quiet --csv --out=cloc.csv .

# Run lizard for complexity metrics on C files; ignore read errors
lizard -l c v9 > lizard.txt || true

# Attempt tree-sitter tagging if grammar is available
if command -v tree-sitter >/dev/null 2>&1; then
    tree-sitter tags v9 > TAGS 2>/dev/null || true
fi
