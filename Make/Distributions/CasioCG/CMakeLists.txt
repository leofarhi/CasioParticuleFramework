# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)

#/*<NAME_APP_FIELD>*/
set(NAME_APP "Engine")
#/*</NAME_APP_FIELD>*/

#/*<ASSETS_DIR_FIELD>*/
set(ASSETS_DIR "../../../Assets/assets-cg")
#/*</ASSETS_DIR_FIELD>*/

project(${NAME_APP})

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.1 REQUIRED)

set(SOURCES
  #/*<SRC>*/
    ../../../Build/build/extra/Resources.c
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineInput.h
    ../../../Build/build/extra/Resources.h
    ../../../Libs/DistributionCases/CASIO/MyKeyboard.h
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineFont.h
    ../../../Libs/Essential/List/List.c
    ../../../Libs/ParticuleEngine/All/src/essential/ParticuleEngineFunction.c
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineType.h
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineFunction.h
    ../../../Libs/Essential/Tuple/Tuple.h
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineTexture.h
    ../../../Libs/Essential/Dict/Dict.c
    ../../../Libs/DistributionCases/CASIO/MyKeyboard.c
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineGraphic.h
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineEssential.h
    ../../../Libs/ParticuleEngine/CasioCg/essential/ParticuleEngineFont.c
    ../../../Sources/src/main.c
    ../../../Libs/ParticuleEngine/CasioCg/essential/ParticuleEngineScreen.c
    ../../../Libs/ParticuleEngine/CasioCg/essential/ParticuleEngineDraw.c
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineDraw.h
    ../../../Libs/Essential/List/List.h
    ../../../Libs/ParticuleEngine/CasioCg/essential/ParticuleEngineOverwrited.h
    ../../../Libs/Essential/Tuple/Tuple.c
    ../../../Libs/Essential/Dict/Dict.h
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineAsset.h
    ../../../Libs/ParticuleEngine/All/include/essential/ParticuleEngineScreen.h
    ../../../Libs/ParticuleEngine/All/src/essential/ParticuleEngineAsset.c
    ../../../Libs/ParticuleEngine/CasioCg/essential/ParticuleEngineTexture.c
    ../../../Libs/ParticuleEngine/CasioCg/essential/ParticuleEngineInput.c
    ../../../Libs/ParticuleEngine/All/src/essential/ParticuleEngineType.c
#/*</SRC>*/
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
#/*</IMG_CG>*/
)

#/*<LIBS_DIR_FIELD>*/
set(LIBS_DIR "../../../Libs")
#/*</LIBS_DIR_FIELD>*/

#/*<LIBS>*/
include_directories("../../../Libs/Essential/Dict")
link_directories("../../../Libs/Essential/Dict")
include_directories("../../../Libs/Essential/List")
link_directories("../../../Libs/Essential/List")
include_directories("../../../Libs/Essential/Tuple")
link_directories("../../../Libs/Essential/Tuple")
include_directories("../../../Libs/ParticuleEngine/CasioCg/essential")
link_directories("../../../Libs/ParticuleEngine/CasioCg/essential")
include_directories("../../../Libs/ParticuleEngine/All/include/essential")
link_directories("../../../Libs/ParticuleEngine/All/include/essential")
include_directories("../../../Libs/ParticuleEngine/All/src/essential")
link_directories("../../../Libs/ParticuleEngine/All/src/essential")
#/*</LIBS>*/

include_directories("${LIBS_DIR}/DistributionCases/CASIO")
link_directories("${LIBS_DIR}/DistributionCases/CASIO")

#/*<DIR_OUT_FIELD>*/
set(DIR_OUT "../../../Build/bin")
#/*</DIR_OUT_FIELD>*/

#/*<DEFINE_FIELD>*/
add_definitions(-DDIST_CASIO_CG -DOVERWRITE_INIT_ASSET -DPROJECT_NAME=Engine -DDEBUG_MODE)
#/*</DEFINE_FIELD>*/

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os)
target_link_libraries(myaddin Gint::Gint "${CMAKE_CURRENT_SOURCE_DIR}/libsupc++.a")

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET myaddin OUTPUT "${DIR_OUT}/${NAME_APP}.g1a"
    NAME "${NAME_APP}" ICON assets/assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "${DIR_OUT}/${NAME_APP}.g3a"
    NAME "${NAME_APP}" ICONS "${ASSETS_DIR}/icon-uns.png" "${ASSETS_DIR}/icon-sel.png")
endif()
