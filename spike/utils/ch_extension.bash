# Rename all ./*.cpp to ./*.cc
find . -name "*.cpp" -exec bash -c 'mv "$1" "${1%.cpp}".cc' - '{}' \;