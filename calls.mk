# Компиляция .c файла
$(OBJS_DIR)/%.o:: %.c $(TOOLCHAIN)
	$(CC) $(CFLAGS) $(COMMON_FLAGS) $(OPTFLAGS) $(DEFINES) $(ALL_INCLUDES) -c -MMD -MP $< -o $@

# Компиляция .S файла
$(OBJS_DIR)/%.o:: %.S $(TOOLCHAIN)
	$(AS) $(ASMFLAGS) --MD $(@:.o=.d) $< -o $@

# Компиляция .cpp файла
$(OBJS_DIR)/%.o:: %.cpp $(TOOLCHAIN)
	$(CXX) $(CXXFLAGS) $(COMMON_FLAGS) $(OPTFLAGS) $(DEFINES) $(ALL_INCLUDES) -c -MMD -MP $< -o $@
