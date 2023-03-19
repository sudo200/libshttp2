include config.mk
include lib/libs.mk
all: out/libshttp2.so

out/libshttp2.so: out 
	if [ -n '$(wildcard obj/*.cpp.o)' ]; then $(CXX) -shared $(LDFLAGS) -o'out/libshttp2.so' $(wildcard obj/*.o) $(wildcard lib/bin/*.a); else $(CC) -shared $(LDFLAGS) -o'out/libshttp2.so' $(wildcard obj/*.o) $(wildcard lib/bin/*.a); fi
	$(OBJCOPY) --only-keep-debug 'out/libshttp2.so' 'out/libshttp2.so.dbg'
	chmod -x out/libshttp2.so*
	$(OBJCOPY) --strip-unneeded 'out/libshttp2.so'
	$(OBJCOPY) --add-gnu-debuglink='out/libshttp2.so.dbg' 'out/libshttp2.so.dbg'

obj/%.cpp.o: obj src/%.cpp
	$(CXX) -c -o'$@' 'src/$(patsubst obj/%.cpp.o,%,$@).cpp' $(CXXFLAGS)

obj/%.c.o: obj src/%.c
	$(CC) -c -o'$@' 'src/$(patsubst obj/%.c.o,%,$@).c' $(CFLAGS)

clean:
	$(RM) -r out
	$(RM) -r obj

out:
	$(MKDIR) out

obj:
	$(MKDIR) obj

compiledb: clean
	bear -- $(MAKE)

.PHONY: clean all compiledb
