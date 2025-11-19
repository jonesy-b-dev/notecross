rm ./Makefile

premake5 --file=../premake5.lua gmake

# first build so (with prebuild commands doest work fsr in premake)
(cd ../notecross-shared/; ./buildshared.sh)

#make clean

if ! bear -- make config=debug; then
    echo "Compilation failed stopping...."
    exit 1
fi

./bin/Debug/notecross-daemon
