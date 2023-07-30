f = open("compile_commands.json", "r")
contents = f.read()
f.close()

contents = contents.replace("-mlongcalls ", "")
contents = contents.replace("-fstrict-volatile-bitfields ", "")
contents = contents.replace("-fno-tree-switch-conversion ", "")

f = open("compile_commands.json", "w")
f.write(contents)
f.close()

