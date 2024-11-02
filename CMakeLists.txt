# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(VAMeter)

# chinese font
if(0)
    #   设置 font 分区数据的文件
    set(FONT_IMAGE ${build_dir}/font.bin)
    #   设置 字库文件路径
    set(FONT_FONT ${project_dir}/lv_font_siyuan_chinese_bold_16.bin)
    #   获取 font 分区偏移值
    partition_table_get_partition_info(FONT_PARTITION_OFFSET "--partition-name font" "offset")
    #   自定义命令: 从 拷贝 字库文件数据 到 ${build_dir}/font.bin (build/font.bin)
    add_custom_command(
        OUTPUT ${FONT_IMAGE}
        COMMAND ${CMAKE_COMMAND} -E copy ${FONT_FONT} ${FONT_IMAGE}
    )
    #   添加依赖文件 ${build_dir}/font.bin 到 font
    add_custom_target(font ALL DEPENDS ${FONT_IMAGE})
    #   添加依赖 font
    add_dependencies(flash font)
    #   将参数写入 flasher_args.json
    esptool_py_flash_target_image(flash font
        "${FONT_PARTITION_OFFSET}"
        "${FONT_IMAGE}")
endif()