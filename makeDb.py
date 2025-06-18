import os
Import("env")

env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

print("makeDb ran successfully")

