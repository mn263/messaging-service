all: client server

client:
		g++ echo-client.cc inet-client.cc client.cc -o msg
		
server:
		g++ echo-server.cc server.cc users.cpp queue.cpp worker.cpp -o msgd -pthread
		
		
clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) msg msgd




# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
		