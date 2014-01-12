EXPORT_INCLUDES=$(shell echo *.h)
SOURCES=$(shell echo *.cpp)

MODE=CPP
TAGS_FILE=math_fit_c++.tags

LINKED_LIBS=libUtils \
            libIOInterface 


# REQUIRED FOR tokenizer

USE_BOOST=YES
USE_GSL=YES

# Preventing intel due to lack of csv support
NO_INTEL=YES

include ../makefile.libMath.include
