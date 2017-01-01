INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_KS1Q ks1q)

FIND_PATH(
    KS1Q_INCLUDE_DIRS
    NAMES ks1q/api.h
    HINTS $ENV{KS1Q_DIR}/include
        ${PC_KS1Q_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    KS1Q_LIBRARIES
    NAMES gnuradio-ks1q
    HINTS $ENV{KS1Q_DIR}/lib
        ${PC_KS1Q_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(KS1Q DEFAULT_MSG KS1Q_LIBRARIES KS1Q_INCLUDE_DIRS)
MARK_AS_ADVANCED(KS1Q_LIBRARIES KS1Q_INCLUDE_DIRS)

