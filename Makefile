CXX      = g++
CPPFLAGS = -g -Wall
RM       = rm -f
SRCS     = connection.cpp contact.cpp date.cpp misc.cpp network.cpp run_network.cpp
OBJS     = $(SRCS:.cpp=.o)
EXEC     = social_network

all: $(EXEC)

social_network: $(OBJS)
		@$(CXX) $(CPPFLAGS) -o social_network $(OBJS)

clean:
		@$(RM) $(OBJS) social_network
