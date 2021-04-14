# CMakeLists for source directory
PROJECT(MiniSQL)

aux_source_directory(. MINISQL_SRCS)
ADD_LIBRARY(minisql ${MINISQL_SRCS})
