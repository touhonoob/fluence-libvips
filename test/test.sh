#!/bin/bash
BIN=fluence_libvips_test

rm -rf build && mkdir build && cd build && cmake .. && make && cp $BIN .. && cd ..

for input_file in data/*.in.json; do
  input_file_basename=$(basename $input_file)
  test_name=${input_file_basename%%.*}
  output_file="data/${test_name}.out.json"
  ./$BIN $input_file $output_file
done