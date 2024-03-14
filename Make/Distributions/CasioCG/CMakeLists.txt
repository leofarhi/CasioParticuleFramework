# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)

#/*<NAME_APP_FIELD>*/
set(NAME_APP "Engine")
#/*</NAME_APP_FIELD>*/

project(${NAME_APP})

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.1 REQUIRED)

set(SOURCES
  #/*<SRC>*/
    src/main.c
    src/testMain.c
    src/testMain.h
    src/InterfaceSystem/EventDataSystem.c
    src/InterfaceSystem/EventDataSystem.h
    src/InterfaceSystem/InterfaceSystem.c
    src/InterfaceSystem/InterfaceSystem.h
    src/ParticuleEngine/ParticuleEngine.c
    src/ParticuleEngine/ParticuleEngine.h
    src/ParticuleEngine/ParticuleEngineDraw.c
    src/ParticuleEngine/ParticuleEngineDraw.h
    src/ParticuleEngine/ParticuleEngineFont.c
    src/ParticuleEngine/ParticuleEngineFont.h
    src/ParticuleEngine/ParticuleEngineInput.c
    src/ParticuleEngine/ParticuleEngineInput.h
    src/ParticuleEngine/ParticuleEngineTexture.c
    src/ParticuleEngine/ParticuleEngineTexture.h
    src/ParticuleEngine/Resources.c
    src/ParticuleEngine/Resources.h
    src/Rpg/Camera.c
    src/Rpg/Camera.h
    src/Rpg/Chunk.c
    src/Rpg/Chunk.h
    src/Rpg/Entity.c
    src/Rpg/Entity.h
    src/Rpg/RpgMain.c
    src/Rpg/RpgMain.h
    src/Rpg/RpgResources.c
    src/Rpg/RpgResources.h
    src/Rpg/SpriteRpg.c
    src/Rpg/SpriteRpg.h
    src/Rpg/World.c
    src/Rpg/World.h
    src/ScriptEditor/GenericType.c
    src/ScriptEditor/GenericType.h
    src/ScriptEditor/Interpreter.c
    src/ScriptEditor/Interpreter.h
    src/ScriptEditor/ScriptEditor.c
    src/ScriptEditor/ScriptEditor.h
    Libs/List/List.c
    Libs/List/List.h
    Libs/CASIO/MyKeyboard.c
    Libs/CASIO/MyKeyboard.h
#/*</SRC>*/
  # ...
)
# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
  # ...
)
set(ASSETS_fx
  #/*<IMG_FX>*/
#/*</IMG_FX>*/
)
set(ASSETS_cg
  #/*<IMG_CG>*/
assets/assets-cg/Images/test.png
assets/assets-cg/Images/Tiles/Outside_A1.png
assets/assets-cg/Images/Tiles/Outside_A2.png
assets/assets-cg/Images/Tiles/Outside_A3.png
assets/assets-cg/Images/Tiles/Outside_A4.png
assets/assets-cg/Images/Tiles/Outside_A5.png
assets/assets-cg/Images/Tiles/Outside_B.png
assets/assets-cg/Images/Tiles/Outside_C.png
#/*</IMG_CG>*/
  # ...
)

include_directories(Libs/List)
link_directories(Libs/List)

include_directories(Libs/CASIO)
link_directories(Libs/CASIO)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os)
target_link_libraries(myaddin Gint::Gint "${CMAKE_CURRENT_SOURCE_DIR}/libsupc++.a")



#/*<DIR_OUT_FIELD>*/
set(DIR_OUT "bin")
#/*</DIR_OUT_FIELD>*/

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET myaddin OUTPUT "${DIR_OUT}/${NAME_APP}.g1a"
    NAME "${NAME_APP}" ICON assets/assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "${DIR_OUT}/${NAME_APP}.g3a"
    NAME "${NAME_APP}" ICONS assets/assets-cg/icon-uns.png assets/assets-cg/icon-sel.png)
endif()
