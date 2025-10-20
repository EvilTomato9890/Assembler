APP      := assembler
SRCDIR   := ../source/compiler
INCDIR   := ../include
BUILDDIR := ../build/compiler
BINDIR   := ../bin
COMMON_BUILDDIR := ../build/common

CXX      ?= g++
CXXSTD    = -std=c++17
WARNINGS  = -Wall -Wextra -Wpedantic
OPT       = -O2
INCLUDES  = -I$(INCDIR)
DEPFLAGS  = -MMD -MP

\
REQ_WARN := -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 \
-Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd \
-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
-Wstrict-null-sentinel -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
-Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef \
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros \
-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -fstack-protector-strong \
-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 \
-pie -fPIE -Werror=vla -Wall -DCANARY_DEBUG -DHASH_DEBUG -DLOGGER_ALL
LDFLAGS += -fsanitize=address,undefined,float-divide-by-zero,float-cast-overflow \
-fsanitize=null,bounds,alignment,object-size,return,vptr,leak \
-fsanitize-address-use-after-scope -no-pie

CXXFLAGS += $(CXXSTD) $(WARNINGS) $(OPT) $(INCLUDES) $(DEPFLAGS) $(REQ_WARN)

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS:.cpp=.o))
DEPS := $(OBJS:.o=.d)

COMMON_OBJS := $(wildcard $(COMMON_BUILDDIR)/*.o)

.PHONY: all clean common run
all: common $(BINDIR)/$(APP)

common:
	$(MAKE) -C ../common all

$(BINDIR)/$(APP): $(OBJS) $(COMMON_OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) $(OBJS) $(COMMON_OBJS)  -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	@cd .. && ./bin/$(APP)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)/$(APP)

-include $(DEPS)

\
