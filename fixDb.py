f = open("compile_commands.json", "r")
contents = f.read()
f.close()

contents = contents.replace("-mlongcalls ", "")

f = open("compile_commands.json", "w")
f.write(contents)
f.close()

