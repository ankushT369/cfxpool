#!/bin/bash

FILE="ABC.txt"

# Create file if it doesn't exist

if [ ! -f "$FILE" ]; then
touch "$FILE"
fi

# Append a single character

echo "a" >> "$FILE"

# Git operations

git add .
git commit -m "Auto update $(date)"
git push origin main

