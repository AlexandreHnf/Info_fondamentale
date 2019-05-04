#!/usr/bin/env bash
for filename in $(ls exemples-bonus/); do
  echo "$filename"
  cat "exemples-bonus/$filename" | ./lightup
done