make:
	pio run

upload:
	pio run --target upload

serial:
	pio device monitor --baud 115200

uploadserial:
	make upload
	sleep 1.5
	make serial

compiledb:
	pio run --target compiledb

