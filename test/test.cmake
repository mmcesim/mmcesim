include(CTest)
enable_testing()
add_test(NAME astyle    COMMAND astyle -h)
add_test(NAME help      COMMAND mmcesim -h)
add_test(NAME version   COMMAND mmcesim --version)
add_test(NAME null1     COMMAND mmcesim) # [will fail]
add_test(NAME null2     COMMAND mmcesim sim) # [will fail]
add_test(NAME sim       COMMAND mmcesim sim ../test/MIMO.sim -f)
add_test(NAME exp       COMMAND mmcesim exp ../test/MIMO.sim -f)
add_test(NAME in_no_ext COMMAND mmcesim exp ../test/MIMO -f)
add_test(NAME not_exist COMMAND mmcesim sim input_not_exists) # [will fail]
add_test(NAME yaml_err  COMMAND mmcesim sim ../test/syntax_error.sim) # [will fail]
set_tests_properties(null1 null2 not_exist yaml_err PROPERTIES WILL_FAIL TRUE)
