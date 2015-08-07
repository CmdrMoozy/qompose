# The following are set by this file:
#   LEVELDB_FOUND
#   LEVELDB_INCLUDE_DIR
#   LEVELDB_LIBRARY

find_path(LEVELDB_INCLUDE_DIR leveldb/db.h)
find_library(LEVELDB_LIBRARIES leveldb)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LEVELDB DEFAULT_MSG
	LEVELDB_INCLUDE_DIR LEVELDB_LIBRARIES)
mark_as_advanced(LEVELDB_INCLUDE_DIR LEVELDB_LIBRARIES)
