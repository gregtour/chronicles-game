# Chronicles Makefile

BINARY_TARGET = ../../bin/Chronicles/Chronicles

GAME_SRC = 	main.cpp \
		entity/bat.cpp entity/cameraController.cpp entity/character.cpp \
		entity/gremling.cpp entity/item.cpp entity/level.cpp \
		entity/monster.cpp entity/player.cpp \
		game/dungeonGenerator.cpp game/game.cpp game/gameInput.cpp \
		game/options.cpp

ENGINE_SRC =	../engine/engine.cpp ../engine/update.cpp \
		../engine/common/log.cpp ../engine/common/resource.cpp \
		../engine/common/timer.cpp \
		../engine/entity/camera.cpp ../engine/entity/entity.cpp \
		../engine/entity/graphicalObj.cpp ../engine/entity/inputtingObj.cpp \
		../engine/entity/loggableObj.cpp ../engine/entity/physicalObj.cpp \
		../engine/graphics/graphics.cpp ../engine/graphics/texture.cpp \
		../engine/input/input.cpp ../engine/input/inputData.cpp \
		../engine/physics/collision.cpp ../engine/physics/common.cpp \
		../engine/physics/object.cpp ../engine/physics/physics.cpp \
		../engine/sound/sound.cpp \
		../engine/tinyxml/tinystr.cpp ../engine/tinyxml/tinyxml.cpp \
		../engine/tinyxml/tinyxmlparser.cpp ../engine/tinyxml/tinyxmlerror.cpp
		
#../engine/game.cpp ../engine/update.cpp ../engine/graphics.cpp \
		../engine/physics/physics.cpp ../engine/sound.cpp ../engine/input.cpp \
		../engine/common/log.cpp ../engine/common/timer.cpp \
		../engine/entity/entity.cpp ../engine/entity/camera.cpp \
		../engine/entity/inputtingObj.cpp ../engine/entity/loggableObj.cpp

MAKE_OPTIONS =	-Wall -ansi

LIBS = 		-lGL -lGLU -lSDL

OPTIONS = 	$(LIBS) `sdl-config --cflags --libs`



all:
	g++ $(MAKE_OPTIONS) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS)

clean:
	@echo Cleaning up...
	@echo Done.
