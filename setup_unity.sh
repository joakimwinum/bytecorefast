version="2.6.0"

url="https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v$version.zip" # The Unity repository is MIT-licensed, separate from this project. See its docs for details.
zip_file="v$version.zip"
extract_dir="Unity-$version"
destination_dir="tests"
unity_h="src/unity.h"
unity_c="src/unity.c"
unity_internals_h="src/unity_internals.h"

wget -O "$zip_file" "$url"
unzip -jo "$zip_file" "$extract_dir/$unity_h" "$extract_dir/$unity_c" "$extract_dir/$unity_internals_h" -d "$destination_dir"
rm "$zip_file"
