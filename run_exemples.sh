#!/usr/bin/env bash
for filename in $(ls exemples/); do
  echo "$filename"
  cat "exemples/$filename" | ./lightup
done