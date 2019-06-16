#!/usr/bin/env bash

for file in source/*
do
  filename=$(basename "$file")
  ./transform_to_cnf.py "$file" cnf > cnfs_sat/$filename.cnf
    ./transform_to_cnf.py "$file" sym > syms_sat/$filename.symin
done
