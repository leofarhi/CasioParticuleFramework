# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)

#/*<NAME_APP_FIELD>*/
set(NAME_APP "Enigne")
#/*</NAME_APP_FIELD>*/

project(${NAME_APP})

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.1 REQUIRED)

set(SOURCES
  #/*<SRC>*/
    ../../../Libs/DistributionCases/CASIO/MyKeyboard.c
    ../../../Libs/Essential/Dict/Dict.h
    ../../../Libs/Essential/List/List.c
    ../../../Sources/src/main.c
    ../../../Libs/DistributionCases/CASIO/MyKeyboard.h
    ../../../Libs/Essential/Tuple/Tuple.c
    ../../../Libs/Essential/Dict/Dict.c
    ../../../Libs/Essential/Tuple/Tuple.h
    ../../../Libs/Essential/List/List.h
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
#/*</LIBS>*/

include_directories("${LIBS_DIR}/CASIO")
link_directories("${LIBS_DIR}/CASIO")

#/*<DIR_OUT_FIELD>*/
set(DIR_OUT "../../../Build/bin")
#/*</DIR_OUT_FIELD>*/

#/*<DEFINE_FIELD>*/
add_definitions(-DDIST_CASIO_CG -DDEBUG_MODE)
#/*</DEFINE_FIELD>*/

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os)
target_link_libraries(myaddin Gint::Gint "${CMAKE_CURRENT_SOURCE_DIR}/libsupc++.a")


#/*<ASSETS_DIR_FIELD>*/
set(ASSETS_DIR "../../../Assets/assets-cg")
#/*</ASSETS_DIR_FIELD>*/

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET myaddin OUTPUT "${DIR_OUT}/${NAME_APP}.g1a"
    NAME "${NAME_APP}" ICON assets/assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "${DIR_OUT}/${NAME_APP}.g3a"
    NAME "${NAME_APP}" ICONS "${ASSETS_DIR}/icon-uns.png" "${ASSETS_DIR}/icon-sel.png")
endif()
