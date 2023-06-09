TARGET_EXEC ?= libthread.a

BUILD_DIR ?= ./dist
SRC_DIRS ?= ./source

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
VCPKG_INC_DIR ?= ./build/sanitize/vcpkg_installed/x64-osx/

INC_DIRS := $(shell find $(VCPKG_INC_DIR) or $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I./, $(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) #-MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# $(info    INC_DIRS is $(INC_DIRS))
$(info    INC_FLAGS is $(INC_FLAGS))

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	@echo "var is $(CPPFLAGS)"


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
