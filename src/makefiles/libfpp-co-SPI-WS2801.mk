ifeq '$(ARCH)' 'Raspberry Pi'

OBJECTS_fpp_co_SPI_WS2801_so += channeloutput/SPIws2801.o
LIBS_fpp_co_SPI_WS2801_so=-L. -lfpp

TARGETS += libfpp-co-SPI-WS2801.so
OBJECTS_ALL+=$(OBJECTS_fpp_co_SPI_WS2801_so)

libfpp-co-SPI-WS2801.so: $(OBJECTS_fpp_co_SPI_WS2801_so) libfpp.so
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_co_SPI_WS2801_so) $(LIBS_fpp_co_SPI_WS2801_so) $(LDFLAGS) $(LDFLAGS_fpp_co_SPI_WS2801_so) -o $@

endif
