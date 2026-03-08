savedcmd_test_offset.mod := printf '%s\n'   test_offset.o | awk '!x[$$0]++ { print("./"$$0) }' > test_offset.mod
