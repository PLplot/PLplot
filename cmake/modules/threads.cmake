# cmake/modules/threads.cmake
#
# Copyright (C) 2019 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Configure threads for PLplot.

# Use the recommended approach (-pthread preferred) for accessing the threads library.
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
if(Threads_FOUND)
  if(CMAKE_USE_PTHREADS_INIT)
    set(PL_HAVE_PTHREADS ON)
  else(CMAKE_USE_PTHREADS_INIT)
    set(PL_HAVE_PTHREADS OFF)
  endif(CMAKE_USE_PTHREADS_INIT)
  if(CMAKE_THREAD_LIBS_INIT STREQUAL "-pthread")
    set(PL_HAVE_PTHREAD_FLAG ON)
  else(CMAKE_THREAD_LIBS_INIT STREQUAL "-pthread")
    set(PL_HAVE_PTHREAD_FLAG OFF)
  endif(CMAKE_THREAD_LIBS_INIT STREQUAL "-pthread")
else(Threads_FOUND)
  set(PL_HAVE_PTHREADS OFF)
  set(PL_HAVE_PTHREAD_FLAG OFF)
endif(Threads_FOUND)
