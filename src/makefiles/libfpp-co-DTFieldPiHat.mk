
OBJECTS_fpp_co_DTFieldPiHat_so += channeloutput/DTFieldPiHat.o
LIBS_fpp_co_DTFieldPiHat_so += -L. -lfpp

TARGETS += libfpp-co-DTFieldPiHat.so
OBJECTS_ALL+=$(OBJECTS_fpp_co_DTFieldPiHat_so)

libfpp-co-DTFieldPiHat.so: $(OBJECTS_fpp_co_DTFieldPiHat_so) libfpp.so
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_co_DTFieldPiHat_so) $(LIBS_fpp_co_DTFieldPiHat_so) $(LDFLAGS) $(LDFLAGS_fpp_co_DTFieldPiHat_so) -o $@

