SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
FILE(GLOB_RECURSE source_files
  adv.c
  console.c
  events.c
  globals.c
  hardware.c
  init.c
  interrupts.c
  main.c
  nb_instr.c
  opcodes.c
  parse.c
  plugin.c
  timing.c
  values.c
  )
ADD_EXECUTABLE(dddcpu16 ${source_files})
TARGET_LINK_LIBRARIES(dddcpu16 dl rt)
